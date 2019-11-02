/////////////////////////////////////////////////////////////////////////////
// Name:        mainamps.cpp
// Purpose:
// Author:      Liu
// Modified by:
// Created:     16/02/2010 12:58:36
// RCS-ID:
// Copyright:
// Licence:
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include <fstream>
#include "wx/wfstream.h"
#include "wx/txtstrm.h"
#include <iomanip>
#include <wx/dir.h>
#include "wx/busyinfo.h"
#include "mainamps.h"
#include "ambient.h"
#include "materialdlg.h"
#include "resdisplay.h"

#include "Calculation.h"
#include "NonEqui.h"
#include "Constant.h"
#include "Point.h"
#include "Point2.h"
#include "LU.h"
//#include "main.rc"
using namespace std;
////@begin XPM images
#include "sun.xpm"
#include "arrow.xpm"
#include "cell.xpm"
#include "Run.xpm"
#include "curve.xpm"
////@end XPM images
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ResultArray);
/*
 * MainAMPS type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MainAMPS, wxDialog )


/*
 * MainAMPS event table definition
 */

BEGIN_EVENT_TABLE( MainAMPS, wxDialog )

////@begin MainAMPS event table entries
    EVT_INIT_DIALOG( MainAMPS::OnInitDialog )
    EVT_WINDOW_DESTROY( MainAMPS::OnDestroy )

    EVT_BUTTON( ID_AMBIENTBTN, MainAMPS::OnAmbientbtnClick )

    EVT_BUTTON( ID_MATERIALBTN, MainAMPS::OnMaterialbtnClick )

    EVT_BUTTON( ID_RUNBUTTON, MainAMPS::OnRunbuttonClick )

    EVT_BUTTON( ID_Settings, MainAMPS::OnSettingsClick )

    EVT_BUTTON( ID_GRAPHSBTN, MainAMPS::OnGraphsbtnClick )

    EVT_BUTTON( ID_OK, MainAMPS::OnOkClick )

////@end MainAMPS event table entries

END_EVENT_TABLE()


/*
 * MainAMPS constructors
 */

MainAMPS::MainAMPS()
{
    Init();
}

MainAMPS::MainAMPS( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * MainAMPS creator
 */

bool MainAMPS::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MainAMPS creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end MainAMPS creation
    return true;
}


/*
 * MainAMPS destructor
 */

MainAMPS::~MainAMPS()
{
	LayerList.DeleteContents(true);
	LayerList.Clear();
	ResArray.Clear();
	if(pt!=NULL)
	{
		delete pt;pt=NULL;
	}
}


/*
 * Member initialisation
 */

void MainAMPS::Init()
{
////@begin MainAMPS member initialisation
    RunBtn = NULL;
    GraphBtn = NULL;
////@end MainAMPS member initialisation
	IsInit=0;
	prgCnt=0;vOld=0;
	ambPar.m_light=0;ambPar.m_temp=_("300");ambPar.IsWF=0;ambPar.IsQE=0;
	/*ambPar.VStart=_("0");ambPar.VEnd=_("0.8");
	ambPar.Switch1=_("1");ambPar.Switch2=_("2");ambPar.Switch3=_("5");
	ambPar.Step1=_("0.05");ambPar.Step2=_("0.1");ambPar.Step3=_("0.2");ambPar.Step4=_("0.5");*/
	ambPar.TopSn=_("1e7");ambPar.TopSp=_("1e7");ambPar.BtmSn=_("1e7");ambPar.BtmSp=_("1e7");
	ambPar.TopRF=_("0");ambPar.BtmRF=_("1");ambPar.TopWF=_("0");ambPar.BtmWF=_("0");
	resPar.Effi=0;resPar.FF=0;
	resPar.Jsc=0;resPar.Voc=0;resPar.max=0;
	pt=NULL;
	setPar.model=0;setPar.iterTimes=_("500");
	setPar.eps=_("1e-6");setPar.clamp=_("0.05");setPar.F0=_("1e5");
}

/*
 * Control creation for MainAMPS
 */

void MainAMPS::CreateControls()
{
////@begin MainAMPS content construction
    MainAMPS* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBitmapButton* itemBitmapButton5 = new wxBitmapButton( itemDialog1, ID_AMBIENTBTN, itemDialog1->GetBitmapResource(wxT("sun.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer4->Add(itemBitmapButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Ambient"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBitmap* itemStaticBitmap7 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("arrow.xpm")), wxDefaultPosition, wxSize(56, 47), 0 );
    itemBoxSizer3->Add(itemStaticBitmap7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBitmapButton* itemBitmapButton9 = new wxBitmapButton( itemDialog1, ID_MATERIALBTN, itemDialog1->GetBitmapResource(wxT("cell.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer8->Add(itemBitmapButton9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Material"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer11, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    RunBtn = new wxBitmapButton( itemDialog1, ID_RUNBUTTON, itemDialog1->GetBitmapResource(wxT("Run.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    itemBoxSizer11->Add(RunBtn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Run"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton14 = new wxButton( itemDialog1, ID_Settings, _("Settings"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemButton14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    GraphBtn = new wxBitmapButton( itemDialog1, ID_GRAPHSBTN, itemDialog1->GetBitmapResource(wxT("curve.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
    GraphBtn->Enable(false);
    itemBoxSizer15->Add(GraphBtn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("Results"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText17, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton19 = new wxButton( itemDialog1, ID_OK, _("Quit"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton19->SetDefault();
    itemBoxSizer18->Add(itemButton19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Connect events and objects
    itemDialog1->Connect(ID_MAINAMPS, wxEVT_DESTROY, wxWindowDestroyEventHandler(MainAMPS::OnDestroy), NULL, this);
////@end MainAMPS content construction
}


/*
 * Should we show tooltips?
 */

bool MainAMPS::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MainAMPS::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MainAMPS bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("sun.xpm"))
    {
        wxBitmap bitmap(sun_xpm);
        return bitmap;
    }
    else if (name == _T("arrow.xpm"))
    {
        wxBitmap bitmap(arrow_xpm);
        return bitmap;
    }
    else if (name == _T("cell.xpm"))
    {
        wxBitmap bitmap(cell_xpm);
        return bitmap;
    }
    else if (name == _T("Run.xpm"))
    {
        wxBitmap bitmap(Run_xpm);
        return bitmap;
    }
    else if (name == _T("curve.xpm"))
    {
        wxBitmap bitmap(curve_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end MainAMPS bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MainAMPS::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MainAMPS icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MainAMPS icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void MainAMPS::OnOkClick( wxCommandEvent& event )
{
	EndModal(wxID_OK);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_AMBIENTBTN
 */

void MainAMPS::OnAmbientbtnClick( wxCommandEvent& event )
{
	ambient dlg(NULL);

	dlg.ambPar=ambPar;//not compatible in  old C++ version
	if(wxID_OK==dlg.ShowModal())
	{
		ambPar=dlg.ambPar;
	}
	dlg.Destroy();
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MATERIALBTN
 */

void MainAMPS::OnMaterialbtnClick( wxCommandEvent& event )
{
	MaterialDlg dlg(NULL);
	int lyrCnt=LayerList.GetCount();//copy layers to dlg
	if(lyrCnt!=0)
	{
		MyList::compatibility_iterator node;
		for(int i=0;i<lyrCnt;i++)
		{
			node=LayerList.Item(i);
			layer *curLyr=node->GetData();
			layer *newLyr=new layer;
			//(*newLyr)=(*curLyr);//shallow copy
			newLyr->copy(*curLyr);
			dlg.LayerList.Append(newLyr);
		}
	}
	if(wxID_OK==dlg.ShowModal())
	{
		int lyrCnt=dlg.LayerList.GetCount();//copy layers from dlg
		LayerList.DeleteContents(true);
		LayerList.Clear();
		MyList::compatibility_iterator node;
		for(int i=0;i<lyrCnt;i++)
		{
			node=dlg.LayerList.Item(i);
			layer *curLyr=node->GetData();
			layer *newLyr=new layer;
			newLyr->copy(*curLyr);
			//(*newLyr)=(*curLyr);
			LayerList.Append(newLyr);
		}

		if(1==IsInit)
		{
			if(pt!=NULL)
			{
				delete pt;
				pt=NULL;
			}
			IsInit=0;
		}
	}

	dlg.Destroy();
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GRAPHSBTN
 */

void MainAMPS::OnGraphsbtnClick( wxCommandEvent& event )
{
	ResDisplay dlg(NULL);
	double temp;ambPar.m_temp.ToDouble(&temp);
	dlg.kT=KBEV*temp;
	//dlg.pt=pt;dlg.num=PointNumber;
	dlg.resPar=resPar;
	dlg.resArrayP=&ResArray;

	dlg.ShowModal();
	resPar=dlg.resPar;
	dlg.Destroy();

    event.Skip();
}


/*
 * wxEVT_INIT_DIALOG event handler for ID_MAINAMPS
 */

void MainAMPS::OnInitDialog( wxInitDialogEvent& event )
{
	wxString path=_("./temp");
	if(!wxDirExists(path))
	{
		wxMkdir(path);
	}
	GraphBtn->Enable(1);
	if(wxFile::Exists(_("setting.tmp")))
	{
		wxFileInputStream input(_("setting.tmp"));
		if(input.IsOk())
		{
			wxTextInputStream file( input );
			file>>setPar.model>>setPar.iterTimes>>setPar.eps>>setPar.clamp>>setPar.F0;
		}
	}
//set program's icon
//#ifdef __WXMSW__
//	SetIcon(wxICON(_("AMPS.ico")));
//#else
//	SetIcon(wxICON(WXICON_AAA));
//#endif // __WXMSW__
    event.Skip();
}


/*
 * wxEVT_DESTROY event handler for ID_MAINAMPS
 */

void MainAMPS::OnDestroy( wxWindowDestroyEvent& event )
{
	/*if(wxFileExists("temp/x.tmp"))
		wxRemoveFile("temp/x.tmp");*/
	//LayerList.DeleteContents(true);
	//LayerList.Clear();
    event.Skip();
////@end wxEVT_DESTROY event handler for ID_MAINAMPS in MainAMPS.
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNBUTTON
 */

void MainAMPS::OnRunbuttonClick( wxCommandEvent& event )
{
	if(LayerList.IsEmpty())
	{
		wxMessageBox(_("Input material information"));
		return;
	}
	if(wxYES==wxMessageBox(_("start calculation?"),_("confirm"),wxYES_NO))
	{
		double temp,TopSn,TopSp,TopRF,BtmSn,BtmSp,BtmRF,TopWF,BtmWF;
		ambPar.m_temp.ToDouble(&temp);
		ambPar.TopRF.ToDouble(&TopRF);ambPar.TopSn.ToDouble(&TopSn);ambPar.TopSp.ToDouble(&TopSp);ambPar.TopWF.ToDouble(&TopWF);
		ambPar.BtmRF.ToDouble(&BtmRF);ambPar.BtmSn.ToDouble(&BtmSn);ambPar.BtmSp.ToDouble(&BtmSp);ambPar.BtmWF.ToDouble(&BtmWF);

		double kT=temp*KBEV;double thickness;int PointNumber;
		vector<double>I,V;		result *curRes;
		//show process dialog
		prgCnt=0; wxString str,title;
		ResArray.Clear();	resPar.V.clear();resPar.I.clear();
		wxProgressDialog *dlg=new wxProgressDialog(_("Status"),_("Processing..."),100,NULL,
			wxPD_APP_MODAL|wxPD_ELAPSED_TIME|wxPD_CAN_ABORT); //&prgDlg;
		dlg->SetClientSize(230,140);
		dlg->Show();
		//if not initialized
		//if(!IsInit)
		{
			wxFileInputStream input(_("temp/x.tmp"));
			wxTextInputStream file(input);
			if(input.IsOk()!=true)
			{
				wxMessageBox(_("x data doesn't exist"));
				dlg->Destroy();return;
			}
			file>>PointNumber>>thickness;
			if(0==thickness)
			{
				wxMessageBox(_("thickness is 0!"));
				dlg->Destroy();
				return;
			}
			else//(PointNumber>2)
			{
				if(pt!=NULL)
				{
					delete pt;pt=NULL;
				}
				{
					//pt=new Point(LayerList,kT);
					pt=new Point2(LayerList,kT);
					IsInit=1;
					PointNumber=pt->PointNumber;
					setPar.eps.ToDouble(&pt->precision);setPar.clamp.ToDouble(&pt->limit);
					setPar.F0.ToDouble(&pt->F0);setPar.iterTimes.ToDouble(&pt->iterTimes);
				}
				
				{	//optimize the initial value
					pt->psiInit[0]=pt->affi[0];//-pt->affi[PointNumber-1];//-kT*log(pt->Nd[0]*pt->Nc[PointNumber-1]/pt->Nd[PointNumber-1]/pt->Nc[0]);
					if(pt->Nd[0]!=0)
						pt->psiInit[0]+=kT*log(pt->Nc[0]/pt->Nd[0]);
					if(pt->Na[0]!=0)
						pt->psiInit[0]+=pt->Eg[0]+kT*log(pt->Na[0]/pt->Nv[0]);//increase the barrier
					pt->psiInit[PointNumber-1]=pt->affi[PointNumber-1];
					if(pt->Nd[PointNumber-1]!=0)
						pt->psiInit[PointNumber-1]+=kT*log(pt->Nc[PointNumber-1]/pt->Nd[PointNumber-1]);
					if(pt->Na[PointNumber-1]!=0)
						pt->psiInit[PointNumber-1]+=pt->Eg[PointNumber-1]+kT*log(pt->Na[PointNumber-1]/pt->Nv[PointNumber-1]);
					//pt->Efn[i]=0;pt->Efp[i]=0;
					for(int i=1;i<PointNumber-1;i++)
					{
						pt->psiInit[i]=pt->psiInit[0]+(pt->psiInit[PointNumber-1]-pt->psiInit[0])*(pt->x[i]/thickness);
					}
				}
				//-----------Initialization-------------			
				curRes=pt->ThermalInit(ambPar.IsWF,TopWF,BtmWF,dlg,prgCnt);
				vOld=0;
			
				//--------------------------------------
				if(curRes!=NULL)
				{
					ResArray.Add(curRes);resPar.V.push_back(0);resPar.I.push_back(0);
				}
				else
				{					
					dlg->Destroy();return;
				}				
			}			
		}
		double current;
		resPar.Effi=0;resPar.FF=0;resPar.Jsc=0;resPar.Voc=0;
		resPar.max=0;
		if(!ambPar.m_light&&!ambPar.vSet.empty())//&&voltage!=0)//dark
		{
			int drtn=1;//test PN direction
			if((pt->n0L>pt->p0L)&&(pt->n0R<pt->p0R))drtn=-1;

			int vCnt=ambPar.vSet.size();
			for(int i=0;i<pt->PointNumber;i++)//clear G
				pt->G[i]=0;
			title=_("Function:Dark Condition\n");str=_("Initializing ");str=title+str;
			dlg->Update(++prgCnt%100,str);
			ResArray.Clear();resPar.V.clear();resPar.I.clear();
			Point ptGummel(LayerList,kT); result* GummelRes;setPar.iterTimes.ToDouble(&ptGummel.iterTimes);
			setPar.eps.ToDouble(&ptGummel.precision);setPar.clamp.ToDouble(&ptGummel.limit);
			vOld=0;
			for(int j=0;j<vCnt;j++)
			{
				str.Printf(_("V=%fv"),ambPar.vSet[j]);str=title+str;
				/*curRes=NonEquiTun(drtn*ambPar.vSet[j],pt,PointNumber,kT,
					ambPar.IsWF,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
					dlg,str,prgCnt);*/
				curRes=pt->NonEquiNew(vOld,drtn*ambPar.vSet[j],TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
					dlg,str,prgCnt);
				vOld=drtn*ambPar.vSet[j];
				if(0==setPar.model)
				{
					if(curRes!=NULL)
					{
						ResArray.Add(curRes);
						current=pt->Jn[0]+pt->Jp[0];
					}
					else
						break;

				}
				else if(1==setPar.model)
				{					
					if(curRes!=NULL)
					{
						delete curRes;					
						pt->ToPoint(ptGummel);
						GummelRes=NonEquiTun(drtn*ambPar.vSet[j],&ptGummel,ptGummel.PointNumber,kT,
							ambPar.IsWF,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
							dlg,str,prgCnt);
						if(GummelRes!=NULL)
						{
							ResArray.Add(GummelRes);
							current=ptGummel.Jn[0]+ptGummel.Jp[0];
							//pt->FromPoint(ptGummel);
						}
						else
						{
							str.Printf(_("Gummel's method failed in voltage=%fv"),ambPar.vSet[j]);
							wxMessageBox(str);
							dlg->Destroy();return;//break;
						}
					}
					else 
						break;
				}

				I.push_back(current*1e3);V.push_back(ambPar.vSet[j]);
				str.Printf(_("V=%fv, J=%.4emA/cm2"),ambPar.vSet[j],I[j]);str=title+str;
				if(!dlg->Update(++prgCnt%100,str))
				{
					wxMessageBox(_("user interrupt!"));
					break;
				}
			}
			resPar.I=I;
			resPar.V=V;
		}
		int size=ambPar.vSet.size();
		if(ambPar.m_light)//light
		{
			Point ptGummel(LayerList,kT);	setPar.iterTimes.ToDouble(&ptGummel.iterTimes);
			setPar.eps.ToDouble(&ptGummel.precision);setPar.clamp.ToDouble(&ptGummel.limit);
			if(!ExportG(&ptGummel,TopRF,BtmRF,ambPar.photonNum,ambPar.waveLength))
			{
				dlg->Destroy();return;
			}			
			pt->ExportG(TopRF,BtmRF,ambPar.photonNum,ambPar.waveLength);
			double Jsc;
			title=_("Function:Light Condition\n");
			str=_("Calculating Jsc ");str=title+str;dlg->Update(++prgCnt%100,str);
			ResArray.Clear();resPar.V.clear();resPar.I.clear();
			//calculate Jsc and initialize			
			{
				pt->ToPoint(ptGummel);
				curRes=NonEquiInit(0,&ptGummel,ptGummel.PointNumber,kT,ambPar.IsWF,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,dlg,str,prgCnt);
				delete curRes;
				pt->FromPoint(ptGummel);
				vOld=0; 
				curRes=pt->NonEquiNew(0,0,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,dlg,str,prgCnt);
				if(curRes!=NULL)
					delete curRes;
				else
				{
					dlg->Destroy();wxMessageBox(_("Failed in initialization!"));
					return;
				}
			}			
			Jsc=pt->Jn[0]+pt->Jp[0];
			//Jsc=ptGummel.Jn[0]+ptGummel.Jp[0];	
			
			int drtn0;
			if(Jsc>0)drtn0=1;else drtn0=-1;
			int drtn1=drtn0;

			str.Printf(_("Jsc=%fmA/cm2"),drtn0*Jsc*1e3);str=title+str;
			dlg->Update(++prgCnt%100,str);
	
			int i=0;			
			do
			{
				curRes=pt->NonEquiNew(vOld,-ambPar.vSet[i]*drtn0,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
					dlg,str,prgCnt);
				vOld=-ambPar.vSet[i]*drtn0;
				if(0==setPar.model)
				{
					if(curRes!=NULL)
					{					
						ResArray.Add(curRes);
						current=pt->Jn[0]+pt->Jp[0];							
					}
					else 
						break;
				}
				else if(1==setPar.model)
				{
					pt->ToPoint(ptGummel);			result * GummelRes;	
					GummelRes=NonEquiTun(-ambPar.vSet[i]*drtn0,&ptGummel,ptGummel.PointNumber,kT,
						ambPar.IsWF,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
						dlg,str,prgCnt);
					if(GummelRes!=NULL)
					{
						delete curRes;ResArray.Add(GummelRes);
						current=ptGummel.Jn[0]+ptGummel.Jp[0];			
						//pt->FromPoint(ptGummel);
					}
					else
					{
						str.Printf(_("Gummel's method failed in voltage=%fv"),ambPar.vSet[i]);
						ResArray.Add(curRes);
						current=pt->Jn[0]+pt->Jp[0];	
						wxMessageBox(str);
						break;
					}
				}
				
				drtn1=int(current/fabs(current));//? current==0
				if(vOld==0)Jsc=current;
				I.push_back(drtn0*current*1e3);V.push_back(ambPar.vSet[i]);
				str.Printf(_("V=%fv, J=%fmA/cm2"),ambPar.vSet[i],I[i]);str=title+str;
				if(!dlg->Update(++prgCnt%100,str))
				{
					wxMessageBox(_("user interrupt!"));
					dlg->Destroy();
					return;
				}
				i++;
			}while((drtn1==drtn0)&&i<size);			

			int n=I.size();
			if(n>=2)
				resPar.Voc=V[n-2]+log(1+(exp(V[n-1]-V[n-2])-1)*I[n-2]/(I[n-2]-I[n-1]));

			double pMax=0;
			for( i=0;i<n;i++)
			{
				if(I[i]*V[i]>pMax)
				{
					pMax=I[i]*V[i];
					resPar.max=i;
				}
			}
			Jsc=drtn0*Jsc;//turn Jsc to be positive
			resPar.Jsc=Jsc*1e3;
			resPar.FF=pMax/resPar.Jsc/resPar.Voc*100;
			resPar.Effi=pMax;//%
			resPar.I=I;
			resPar.V=V;
		}
		if(ambPar.IsQE)
		{
			double beam=1e15;
			resPar.QEffi.clear();
			title=_("Function: Quantum Efficiency\n");
			Point ptGummel(LayerList,kT);		setPar.iterTimes.ToDouble(&ptGummel.iterTimes);
			setPar.eps.ToDouble(&ptGummel.precision);setPar.clamp.ToDouble(&ptGummel.limit);
			bool reCalc=0;int cnt=0;
			for(int i=0;i<=55;i++)
			{
				str.Printf(_("wavelength: %d nm"),(300+20*i));
				str=title+str;
				if(!dlg->Update(++prgCnt%100,str))
				{
					wxMessageBox(_("user interrupt!"));
					break;
				}
				vector<double> phi,waveLength;
				phi.push_back(beam);
				waveLength.push_back((300+20*i)*1e-9);								
				pt->ExportG(TopRF,BtmRF,phi,waveLength);

				if(0==i||reCalc)//Gummel initialize
				{
					if(!ExportG(&ptGummel,TopRF,BtmRF,phi,waveLength))
					{
						dlg->Destroy();return;
					}	
					for(int j=0;j<pt->PointNumber;j++)//initialize the pt
					{
						pt->psi[j]=pt->psiInit[j];pt->Efn[j]=0;pt->Efp[j]=0;
					}
					pt->ToPoint(ptGummel);
					curRes=NonEquiInit(0,&ptGummel,ptGummel.PointNumber,kT,ambPar.IsWF,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,dlg,str,prgCnt);
					delete curRes;
					pt->FromPoint(ptGummel);
					reCalc=0;
				}				
				curRes=pt->NonEquiNew(0,0,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
					dlg,str,prgCnt);
				if(curRes!=NULL)
				{
					delete curRes;cnt=0;
					double Jsc;
					if(0==setPar.model)
						Jsc=pt->Jn[0]+pt->Jp[0];
					else if(1==setPar.model)
					{
						pt->ToPoint(ptGummel); result * GummelRes;
						ExportG(&ptGummel,TopRF,BtmRF,phi,waveLength);
						GummelRes=NonEquiTun(0,&ptGummel,ptGummel.PointNumber,kT,
							ambPar.IsWF,TopWF,TopSn,TopSp,BtmWF,BtmSn,BtmSp,
							dlg,str,prgCnt);
						if(GummelRes!=NULL)
						{
							ResArray.Add(GummelRes);
							Jsc=ptGummel.Jn[0]+ptGummel.Jp[0];			
							//pt->FromPoint(ptGummel);
						}
						else
						{
							str.Printf(_("Gummel's method failed in wavelength=%fnm"),waveLength[i]*1e9);
							wxMessageBox(str);							
							//break;
						}
						//Jsc=ptGummel.Jn[0]+ptGummel.Jp[0];
					}				
					resPar.QEffi.push_back(fabs(Jsc)/QE/beam);
					if(resPar.QEffi.back()>1)
					{
						resPar.QEffi.pop_back();i--;
						reCalc=1;
					}
				}
				else
				{
					reCalc=1; 	
					if((++cnt)>1){
						str.Printf(_("failed in wavelength=%.0fnm, skip to next wave length"),waveLength[0]*1e9); 
						wxMessageBox(str);			
						double value=(double)resPar.QEffi.back();
						resPar.QEffi.push_back(value);
						continue;
					}
					i--;					
				}
				
			}
		}
		wxBusyInfo info(_("Generating files, please wait."),dlg);

		//----------------postprocess----------------
		wxDir dir(_("./temp"));wxString filename,f1;
		bool cont = dir.GetFirst(&filename);
		while ( cont )
		{
			// f1.Printf(_("./temp/%s"),filename);
			f1=_(".\\temp\\")+filename;
			if(filename!=_("x.tmp"))
				wxRemoveFile(f1);
			cont = dir.GetNext(&filename);
		}//remove old temp files
		/*	wxFileOutputStream output(_("temp/IV.csv"));
		wxTextOutputStream fileIV(output);*/
		ofstream fileIV(_("temp/IV.csv"));
		fileIV<<_("Voc,Jsc,FF,Efficiency")<<endl;
		fileIV<<resPar.Voc<<','<<resPar.Jsc<<','<<resPar.FF<<','<<resPar.Effi<<endl;
		fileIV<<_("light,")<<ambPar.m_light<<endl;
		fileIV<<_("max_point,")<<resPar.max<<endl;
		int n=V.size();
		fileIV<<_("V(v),")<<_("I(mA/cm2)")<<endl;
		for(int i=0;i<n;i++)
		{
			fileIV<<setprecision(17)<<V[i]<<_(",")<<I[i]<<endl;
		}
		fileIV.close();

		for(int j=0;j<n;j++)
		{
			if(ambPar.m_light)
				str.Printf(_("%s_%f%s"),_("temp/light"),V[j],_("v.csv"));
			else
				str.Printf(_("%s_%f%s"),_("temp/dark"),V[j],_("v.csv"));
			/*wxFileOutputStream output(str);
			wxTextOutputStream file(output);*/
			ofstream file(str);
			file<<_("x,Ec,Ev,Efn,Efp,n,p,acceptor-like charges,donor-like charges,nt,pt,")
				<<_("Jn,Jp,J total,Recombination,Generation,Filed")<<endl;
			result res=ResArray.Item(j); 	int cnt=res.x.size();
			for(int i=0;i<cnt;i++)
			{

				file<<setprecision(17)<<res.x[i]<<','<<res.Ec[i]<<','<<res.Ev[i]<<','<<res.Efn[i]<<','<<res.Efp[i]<<','
					<<res.n[i]<<','<<res.p[i]<<','<<res.Nta[i]<<','<<res.Ntd[i]<<','<<res.nt[i]<<','<<res.pt[i]<<','
					<<res.Jn[i]<<','<<res.Jp[i]<<','<<res.Jn[i]+res.Jp[i]<<','<<res.R[i]<<','<<res.G[i]<<','<<res.EleF[i]
					<<endl;
			}
			file.close();
		}
		if(ambPar.IsQE)
		{
			wxFileOutputStream output(_("temp/QE.csv"));
			wxTextOutputStream file(output);
			int size=resPar.QEffi.size();
			file<<wxT("WaveLength(nm)")<<_("QE")<<endl;
			for(int i=0;i<size;i++)
				file<<300+20*i<<','<<resPar.QEffi[i]<<endl;
		}
		dlg->Destroy();

	}
	event.Skip();
}


bool MainAMPS::ExportG(Point *pt,double TopRF,double BtmRF,vector<double> photo0,vector<double> waveLength)
{
	int region=0,rgn=LayerList.GetCount();
	int size=waveLength.size();double absi,geni;
	double genSum;
	double x0=0;
	//vector<double> photo0=ambPar.photonNum;
	for(int i=0;i<size;i++)
		photo0[i]*=(1-TopRF);
	MyList::compatibility_iterator node;
	node=LayerList.Item(region);
	layer *curLyr=node->GetData();
	for(int i=0;i<rgn;i++)
	{
		layer* testLyr=LayerList.Item(i)->GetData();
		if(!testLyr->absWave.size())
		{
			wxString str=_("No absorption coefficient in material ");
			str+=testLyr->name;
			wxMessageBox(str);
			return 0;
		}
	}
	for(int i=0;i<pt->PointNumber;i++)//x[i]
	{
		{
			genSum=0;
			for(int j=0;j<size;j++)//waveLength[j]
			{
				//double wL=waveLength[j];
				absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[j]);
				geni=absi*photo0[j]*exp(-absi*(pt->x[i]-x0));
				genSum+=geni;
			}
			pt->G[i]=genSum;
		}
		if(pt->step[i]==0)// into another region
		{			
			int waveNum=photo0.size();
			for(int k=0;k<waveNum;k++)
			{
				absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[k]);
				photo0[k]=photo0[k]*exp(-absi*(pt->x[i]-x0));
			}
			x0=pt->x[i];
			region++;
			if(region<rgn)
			{
				node=LayerList.Item(region);
				curLyr=node->GetData();
			}

		}
	}
	
	for(int i=0;i<size;i++)//reverse
		photo0[i]*=BtmRF;
	for(int i=pt->PointNumber-1;i>=0;i--)
	{
		if(pt->step[i]==0)// into another region
		{			
			int waveNum=photo0.size();
			for(int k=0;k<waveNum;k++)
			{
				absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[k]);
				photo0[k]=photo0[k]*exp(-absi*(x0-pt->x[i]));
			}
			x0=pt->x[i];
			region--;
			if(region>=0)
			{
				node=LayerList.Item(region);
				curLyr=node->GetData();
			}
		}
		genSum=0;
		for(int j=0;j<size;j++)//waveLength[j]
		{
			absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[j]);
			geni=absi*photo0[j]*exp(-absi*(x0-pt->x[i]));
			genSum+=geni;
		}
		pt->G[i]+=genSum;
		

	}
	/*	ofstream file("temp/gen.tmp");
	for(int i=0;i<PointNumber;i++)
	{
		file<<pt->x[i]<<' '<<pt->G[i]<<endl;
	}
	file.close();*/
	return 1;
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_Settings
 */

void MainAMPS::OnSettingsClick( wxCommandEvent& event )
{
	SettingDlg dlg(NULL);

	dlg.setPar=setPar;
	if(wxID_OK==dlg.ShowModal())
	{
		setPar=dlg.setPar;
	}
	dlg.Destroy();
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_Settings in MainAMPS. 
}

