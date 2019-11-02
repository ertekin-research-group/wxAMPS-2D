/////////////////////////////////////////////////////////////////////////////
// Name:        resdisplay.cpp
// Purpose:
// Author:      Liu
// Modified by:
// Created:     24/02/2010 16:23:35
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
#include "mathplot.h"
////@end includes
#include <fstream>
#include <vector>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/wfstream.h>
#include <wx/dir.h>
#include "resdisplay.h"
#include "mathplot.h"
#include "NonEqui.h"
#include "Calculation.h"
#include <wx/clipbrd.h>
#include "wx/busyinfo.h"
using namespace std;
////@begin XPM images
////@end XPM images

/*
 * ResDisplay type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ResDisplay, wxDialog )


/*
 * ResDisplay event table definition
 */

BEGIN_EVENT_TABLE( ResDisplay, wxDialog )

////@begin ResDisplay event table entries
    EVT_INIT_DIALOG( ResDisplay::OnInitDialog )

    EVT_LISTBOX( ID_LISTBOX, ResDisplay::OnListboxSelected )

    EVT_BUTTON( ID_BUTTON, ResDisplay::OnCopyButtonClick )

    EVT_BUTTON( ID_BUTTON1, ResDisplay::OnSaveButtonClick )

    EVT_BUTTON( ID_BUTTON6, ResDisplay::OnExportClick )

    EVT_BUTTON( ID_BUTTON11, ResDisplay::OnImportClick )

    EVT_BUTTON( ID_BUTTON2, ResDisplay::OnPrintButtonClick )

    EVT_SLIDER( ID_SLIDER, ResDisplay::OnSliderUpdated )

    EVT_SLIDER( ID_SLIDERSH, ResDisplay::OnSlidershUpdated )

    EVT_SLIDER( ID_SLIDERRS, ResDisplay::OnSliderrsUpdated )

////@end ResDisplay event table entries

END_EVENT_TABLE()


/*
 * ResDisplay constructors
 */

ResDisplay::ResDisplay()
{
    Init();
}

ResDisplay::ResDisplay( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * ResDisplay creator
 */

bool ResDisplay::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ResDisplay creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ResDisplay creation
    return true;
}


/*
 * ResDisplay destructor
 */

ResDisplay::~ResDisplay()
{
////@begin ResDisplay destruction
////@end ResDisplay destruction
}


/*
 * Member initialisation
 */

void ResDisplay::Init()
{
////@begin ResDisplay member initialisation
    resListCtrl = NULL;
    ExportBtn = NULL;
    VText = NULL;
    sliderCtrl = NULL;
    RshCtrl = NULL;
    RshMaxText = NULL;
    RshMinText = NULL;
    RsCtrl = NULL;
    RsMinText = NULL;
    RsMaxText = NULL;
    m_plot = NULL;
////@end ResDisplay member initialisation
	VocTextR=NULL;JscTextR=NULL;FFTextR=NULL;EffiTextR=NULL;RshText=NULL; RsText=NULL;
	RLayer=NULL;
	Rs=1e-3;Rsh=1e6;

}


/*
 * Control creation for ResDisplay
 */

void ResDisplay::CreateControls()
{
////@begin ResDisplay content construction
    ResDisplay* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_TOP|wxALL, 5);

    wxArrayString resListCtrlStrings;
    resListCtrl = new wxListBox( itemDialog1, ID_LISTBOX, wxDefaultPosition, wxSize(-1, 120), resListCtrlStrings, wxLB_SINGLE );
    itemBoxSizer3->Add(resListCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_BUTTON, _("Copy"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_BUTTON1, _("Save"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    ExportBtn = new wxButton( itemDialog1, ID_BUTTON6, _("Export"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer8->Add(ExportBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON11, _("Import"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer8->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_BUTTON2, _("Print"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer3->Add(itemButton11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Voltage:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    VText = new wxTextCtrl( itemDialog1, ID_RESTEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer12->Add(VText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    sliderCtrl = new wxSlider( itemDialog1, ID_SLIDER, 0, 0, 7, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer3->Add(sliderCtrl, 0, wxGROW|wxALL, 5);

    wxStaticLine* itemStaticLine16 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer3->Add(itemStaticLine16, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer17, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemDialog1, wxID_STATIC2, _("Rsh"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemStaticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    RshCtrl = new wxSlider( itemDialog1, ID_SLIDERSH, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer17->Add(RshCtrl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer20, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    RshMaxText = new wxTextCtrl( itemDialog1, ID_RshTEXTCTRL, _("1e6"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer20->Add(RshMaxText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("To"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    RshMinText = new wxTextCtrl( itemDialog1, ID_TEXTCTRL30, _("1"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer20->Add(RshMinText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer24, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( itemDialog1, ID_STATICTEXT, _("Rs"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    RsCtrl = new wxSlider( itemDialog1, ID_SLIDERRS, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer24->Add(RsCtrl, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer27, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    RsMinText = new wxTextCtrl( itemDialog1, ID_TEXTCTRLRs, _("1e-3"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer27->Add(RsMinText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText29 = new wxStaticText( itemDialog1, wxID_STATIC, _("To"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemStaticText29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    RsMaxText = new wxTextCtrl( itemDialog1, ID_TEXTCTRLRs2, _("1e3"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer27->Add(RsMaxText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxPanel* itemPanel31 = new wxPanel( itemDialog1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer2->Add(itemPanel31, 0, wxALIGN_TOP|wxALL, 5);

    m_plot = new mpWindow( itemPanel31, ID_MPWINDOW, wxDefaultPosition, wxSize(600, 500), wxSUNKEN_BORDER );

    // Connect events and objects
    VText->Connect(ID_RESTEXTCTRL1, wxEVT_KILL_FOCUS, wxFocusEventHandler(ResDisplay::OnVoltageKillFocus), NULL, this);
    RshMaxText->Connect(ID_RshTEXTCTRL, wxEVT_KILL_FOCUS, wxFocusEventHandler(ResDisplay::OnVoltageKillFocus), NULL, this);
    RsMinText->Connect(ID_TEXTCTRLRs, wxEVT_KILL_FOCUS, wxFocusEventHandler(ResDisplay::OnVoltageKillFocus), NULL, this);
////@end ResDisplay content construction
}


/*
 * Should we show tooltips?
 */

bool ResDisplay::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap ResDisplay::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ResDisplay bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ResDisplay bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon ResDisplay::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ResDisplay icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ResDisplay icon retrieval
}


/*
 * wxEVT_INIT_DIALOG event handler for ID_RESDISPLAY
 */

void ResDisplay::OnInitDialog( wxInitDialogEvent& event )
{
	VText->SetValue(_("0"));
	resListCtrl->Append(wxT("IV"));
	resListCtrl->Append(wxT("Energy Band"));
	resListCtrl->Append(wxT("Carriers"));
	resListCtrl->Append(_("Elec Field"));
	resListCtrl->Append(wxT("Current"));	
	resListCtrl->Append(wxT("R&G"));
	resListCtrl->Append(wxT("QE"));
//	resListCtrl->Append(wxT("CV"));
	resListCtrl->SetSelection(0);
    //resListCtrl->SetClientSize(200,100);
	PlotIV();
	wxRect rect=m_plot->GetClientRect();
	m_plot->AddLayer(new mpInfoCoords(wxRect(rect.width-180,0,180,20), wxTRANSPARENT_BRUSH));

	m_plot->EnableDoubleBuffer(true);
	m_plot->SetMPScrollbars(false);
	//m_plot->
	resIndex=0;//resArrayP->GetCount()-1;
	sliderCtrl->SetRange(0,resPar.V.size()-1);//resArrayP->GetCount()-1);
	if(resPar.V.size())
	{
		wxString vol; vol.Printf(_("%.2f"),resPar.V[0]);
		VText->SetValue(vol);
	}
	else
	//if(!resArrayP->GetCount())
		ExportBtn->Enable(0);
	event.Skip();
}


void ResDisplay::PlotEB()
{
	int PointNumber;

	result curRes=resArrayP->Item(resIndex);
	PointNumber=curRes.Ec.size();
	if(PointNumber!=0)
	{
		mpFXYVector* EcLayer = new mpFXYVector(_("Ec"));
		mpFXYVector* EfnLayer = new mpFXYVector(_("Efn" ));
		mpFXYVector* EfpLayer = new mpFXYVector(_("Efp"));
		mpFXYVector* EvLayer = new mpFXYVector(_("Ev"));
		
		EcLayer->SetData(curRes.x, curRes.Ec);EfnLayer->SetData(curRes.x,curRes.Efn);
		EfpLayer->SetData(curRes.x,curRes.Efp);EvLayer->SetData(curRes.x,curRes.Ev);
		EfnLayer->ShowName(false);EfpLayer->ShowName(false);EcLayer->ShowName(false);EvLayer->ShowName(false);
		EcLayer->SetContinuity(true);EfnLayer->SetContinuity(true);EfpLayer->SetContinuity(true);EvLayer->SetContinuity(true);
		wxPen BluePen(*wxBLUE, 2, wxSOLID);wxPen BLACKPEN(*wxBLACK, 2, wxSOLID);wxPen REDPEN(*wxRED, 2, wxSOLID);
		EcLayer->SetPen(BLACKPEN);EvLayer->SetPen(BLACKPEN);
		EfpLayer->SetPen(REDPEN);EfnLayer->SetPen(BluePen);
		EcLayer->SetDrawOutsideMargins(false);EvLayer->SetDrawOutsideMargins(false);
		EfnLayer->SetDrawOutsideMargins(false);EfpLayer->SetDrawOutsideMargins(false);

		wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		mpScaleX* xaxis = new mpScaleX(wxT("X(um)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
		mpScaleY* yaxis = new mpScaleY(wxT("Energy(ev)"), mpALIGN_LEFT, true);
		xaxis->SetFont(graphFont);
		yaxis->SetFont(graphFont);
		xaxis->SetDrawOutsideMargins(false);
		yaxis->SetDrawOutsideMargins(false);
		m_plot->SetMargins(50, 0, 50, 50);
		m_plot->AddLayer( xaxis );
		m_plot->AddLayer( yaxis );
		m_plot->AddLayer( EcLayer );m_plot->AddLayer( EvLayer );m_plot->AddLayer( EfnLayer );m_plot->AddLayer( EfpLayer );
		m_plot->UpdateAll();
		//m_plot->Fit();
	}
}


/*
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
 */

void ResDisplay::OnSliderUpdated( wxCommandEvent& event )
{
	wxString vol;double voltage;
	resIndex=sliderCtrl->GetValue();
	//voltage=resArrayP->Item(resIndex).v;
	if(resPar.V.size())
	{
		voltage=resPar.V[resIndex];
		vol.Printf(_("%.2f"),voltage);
		VText->SetValue(vol);
		if( resArrayP->GetCount()!=0)
		{
			int sel=resListCtrl->GetSelection();
			switch(sel)
			{
			case 0:
				break;
			case 1:
				m_plot->DelAllLayers(1,true);
				PlotEB();
				break;
			case 2:
				m_plot->DelAllLayers(1,true);
				PlotCarrier();
				break;
			case 3:
				m_plot->DelAllLayers(1,true);
				PlotEF();break;
			case 4:
				m_plot->DelAllLayers(1,true);
				PlotJ();
				break;
			case 5:
				m_plot->DelAllLayers(1,true);
				PlotR();
				break;
			case 6:
				break;
			default:
				break;
			}
			wxRect rect=m_plot->GetClientRect();
			m_plot->AddLayer(new mpInfoCoords(wxRect(rect.width-180,0,180,20), wxTRANSPARENT_BRUSH));
			m_plot->AddLayer( new mpInfoLegend(wxRect(300,0,50,50), wxTRANSPARENT_BRUSH));
			m_plot->EnableDoubleBuffer(true);
			m_plot->SetMPScrollbars(false);
		}
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
 */

void ResDisplay::OnListboxSelected( wxCommandEvent& event )
{
	int sel=resListCtrl->GetSelection();	m_plot->DelAllLayers(1,true);
	RshCtrl->Enable(0);RsCtrl->Enable(0);
	if( resArrayP->GetCount()!=0)
	{
		switch(sel)
		{
		case 0:
			PlotIV();
			RshCtrl->Enable(1);RsCtrl->Enable(1);
			break;
		case 1:
			PlotEB();
			break;
		case 2:
			PlotCarrier();
			break;
		case 3:
			PlotEF();
			break;
		case 4:
			PlotJ();
			break;
		case 5:
			PlotR();
			break;
		case 6:
			PlotQEffi();
			break;
		default:
			break;
		}
		wxRect rect=m_plot->GetClientRect();
		m_plot->AddLayer(new mpInfoCoords(wxRect(rect.width-180,0,180,20), wxTRANSPARENT_BRUSH));
		m_plot->AddLayer( new mpInfoLegend(wxRect(300,0,50,50), wxTRANSPARENT_BRUSH));
		m_plot->EnableDoubleBuffer(true);
		m_plot->SetMPScrollbars(false);
		m_plot->Fit();

	}
	event.Skip();
}

void ResDisplay::PlotIV(void)
{
	mpFXYVector* IVLayer = new mpFXYVector;
	mpFXYVector* yLayer = new mpFXYVector;
	IVLayer->SetData(resPar.V,resPar.I);
	vector<double> y0,x1,y1,x2,y2;int n=resPar.V.size();
	if(resPar.max>0)
	{
		for(int i=0;i<=resPar.max;i++)
		{	x1.push_back(resPar.V[i]);y1.push_back(resPar.I[resPar.max]);}
		for(int i=resPar.max;i<n-1;i++)
		{	x2.push_back(resPar.V[resPar.max]);y2.push_back(resPar.I[i]);}
		x2.push_back(resPar.V[resPar.max]);y2.push_back(0);

		mpFXYVector* xMax= new mpFXYVector;	mpFXYVector* yMax = new mpFXYVector;
		xMax->SetData(x1,y1);yMax->SetData(x2,y2);
		wxPen DashPen(*wxBLACK, 1, wxDOT );
		xMax->SetPen(DashPen);		yMax->SetPen(DashPen);
		xMax->SetContinuity(1);		yMax->SetContinuity(1);
		m_plot->AddLayer(xMax); 	m_plot->AddLayer(yMax);

		wxString str;
		str.Printf(_("Voc :%f V"),resPar.Voc);
		int strx=50,stry=5;
		VocText=new mpText(str,strx, stry);		m_plot->AddLayer( VocText );
		str.Printf(_("Jsc :%f mA/cm2"),resPar.Jsc);
		JscText=new mpText(str, strx, stry+5);	m_plot->AddLayer( JscText );
		str.Printf(_("FF  :%.4f %c"),resPar.FF,'%');
		FFText=new mpText(str, strx, stry+10);	m_plot->AddLayer( FFText);
		str.Printf(_("Effi:%f %c"),resPar.Effi,'%');
		EffiText=new mpText(str, strx, stry+15);m_plot->AddLayer( EffiText );
	}
	for(int i=0;i<n;i++)
	{
		y0.push_back(0);
	}
	IVLayer->SetContinuity(1);	IVLayer->SetPen(*wxRED_PEN);	IVLayer->SetDrawOutsideMargins(false);
	yLayer->SetData(resPar.V,y0);yLayer->SetContinuity(1);yLayer->SetDrawOutsideMargins(false);
	wxPen BluePen(*wxBLUE, 1, wxDOT );yLayer->SetPen(BluePen);
	
	wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleX* xaxis = new mpScaleX(wxT("V(v)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
	mpScaleY* yaxis = new mpScaleY(wxT("I(mA/cm2)"), mpALIGN_LEFT, true);
	xaxis->SetFont(graphFont);
	yaxis->SetFont(graphFont);
	xaxis->SetDrawOutsideMargins(false);
	yaxis->SetDrawOutsideMargins(false);
	m_plot->SetMargins(80, 0, 50, 80);
	m_plot->AddLayer( xaxis );
	m_plot->AddLayer( yaxis );
	m_plot->AddLayer(IVLayer);
	m_plot->AddLayer(yLayer);

	
	m_plot->Fit();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void ResDisplay::OnCopyButtonClick( wxCommandEvent& event )
{
	m_plot->CopyScreenshot();
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
 */

void ResDisplay::OnSaveButtonClick( wxCommandEvent& event )
{
	wxFileDialog fileDlg(NULL,_("Write graph to file"),_(""),_(""),
		_("BMP Files(*.bmp)|*.bmp|All Files(*.*)|*.*"),wxFD_SAVE);

	if(wxID_OK==fileDlg.ShowModal())
	{
		m_plot->SaveScreenshot(fileDlg.GetPath());
	}
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON2
 */

void ResDisplay::OnPrintButtonClick( wxCommandEvent& event )
{
    mpPrintout prt(m_plot);
	//wxPrinter printer;
    //printer.Print(this, &prt, true);
  wxPrintPreview *preview = new wxPrintPreview(new mpPrintout(m_plot), new mpPrintout(m_plot));
      wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _("Demo Print Preview"), wxPoint(100, 100), wxSize(600, 650));
      frame->Centre(wxBOTH);
      frame->Initialize();
      frame->Show(true);

	event.Skip();
}
void ResDisplay::PlotJ()
{
	mpFXYVector* JnLayer = new mpFXYVector(_("Jn") );
	mpFXYVector* JpLayer = new mpFXYVector(_("Jp") );
	mpFXYVector* JLayer = new mpFXYVector(_("J total") );
	result curRes=resArrayP->Item(resIndex);
	vector<double> J,Jn,Jp;
	int PointNumber=curRes.Ec.size();
	if(PointNumber!=0)
	{
		for(int i=0;i<PointNumber;i++)
		{
			Jn.push_back(curRes.Jn[i]*1e3);Jp.push_back(curRes.Jp[i]*1e3);
			J.push_back(Jn[i]+Jp[i]);
		}
		JnLayer->SetData(curRes.x,Jn);JpLayer->SetData(curRes.x,Jp);JLayer->SetData(curRes.x,J);
		JnLayer->SetContinuity(1);JpLayer->SetContinuity(1);JLayer->SetContinuity(1);
		JpLayer->SetPen(*wxRED_PEN);JpLayer->ShowName(0);
		wxPen BluePen(*wxBLUE, 1, wxSOLID);JnLayer->SetPen(BluePen);JnLayer->ShowName(0);
		JnLayer->SetDrawOutsideMargins(false);JpLayer->SetDrawOutsideMargins(false);JLayer->SetDrawOutsideMargins(false);
		wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		mpScaleX* xaxis = new mpScaleX(wxT("x(cm)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
		mpScaleY* yaxis = new mpScaleY(wxT("mA/cm2"), mpALIGN_LEFT, true);
		xaxis->SetFont(graphFont);	yaxis->SetFont(graphFont);
		xaxis->SetDrawOutsideMargins(false);	yaxis->SetDrawOutsideMargins(false);
		m_plot->AddLayer( xaxis );	m_plot->AddLayer(yaxis);
		m_plot->AddLayer(JnLayer);m_plot->AddLayer(JpLayer);m_plot->AddLayer(JLayer);
		m_plot->SetMargins(50, 0, 80, 80);
		m_plot->UpdateAll();
	}
}

void ResDisplay::PlotR()
{
	mpFXYVector* RLayer = new mpFXYVector(_("R") );
	mpFXYVector* GLayer = new mpFXYVector(_("G" ));
	result curRes=resArrayP->Item(resIndex);
	//file>>PointNumber;
	int PointNumber=curRes.Ec.size();
	for(int i=0;i<PointNumber;i++)
	{
		curRes.R[i]=log(std::max(curRes.R[i],1.))/log(10.);curRes.G[i]=log(std::max(curRes.G[i],1.))/log(10.);
	}
	if(PointNumber!=0)
	{
		RLayer->SetData(curRes.x,curRes.R);GLayer->SetData(curRes.x,curRes.G);
		RLayer->SetContinuity(1);GLayer->SetContinuity(1);
		RLayer->SetPen(*wxRED_PEN);GLayer->SetPen(*wxGREEN_PEN);
		RLayer->SetDrawOutsideMargins(false);GLayer->SetDrawOutsideMargins(false);
		wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		mpScaleX* xaxis = new mpScaleX(wxT("x(cm)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
		mpScaleY* yaxis = new mpScaleY(wxT("R&G(lg)"), mpALIGN_LEFT, true);
		xaxis->SetFont(graphFont);	yaxis->SetFont(graphFont);
		xaxis->SetDrawOutsideMargins(false);	yaxis->SetDrawOutsideMargins(false);
		m_plot->AddLayer( xaxis );	m_plot->AddLayer(yaxis);
		m_plot->AddLayer(RLayer);m_plot->AddLayer(GLayer);
		m_plot->SetMargins(50, 0, 80, 80);
		m_plot->Fit();
	}
}

void ResDisplay::PlotCarrier()
{
	result curRes=resArrayP->Item(resIndex);

	int PointNumber=curRes.Ec.size();
	if(PointNumber!=0)
	{
		for(int i=0;i<PointNumber;i++)
		{
			curRes.n[i]=log(std::max(curRes.n[i],1.))/log(10.);curRes.p[i]=log(std::max(curRes.p[i],1.))/log(10.);
			curRes.nt[i]=log(std::max(curRes.nt[i],1.))/log(10.);	curRes.pt[i]=log(std::max(curRes.pt[i],1.))/log(10.);
			curRes.Ntd[i]=log(std::max(curRes.Ntd[i],1.))/log(10.);curRes.Nta[i]=log(std::max(curRes.Nta[i],1.))/log(10.);
		}
		mpFXYVector* nLayer = new mpFXYVector(_("free electrons") );mpFXYVector* pLayer = new mpFXYVector(_("free holes" ));
		mpFXYVector* NtdLayer = new mpFXYVector(_("charge(Donor-like defect)") );
		mpFXYVector* NtaLayer = new mpFXYVector(_("charge(Acceptor-like defect)" ));
		mpFXYVector* ptLayer = new mpFXYVector(_("trapped holes(BT)") );mpFXYVector* ntLayer = new mpFXYVector(_("trapped electrons(BT)") );
		nLayer->SetData(curRes.x,curRes.n);pLayer->SetData(curRes.x,curRes.p);
		NtdLayer->SetData(curRes.x,curRes.Ntd);NtaLayer->SetData(curRes.x,curRes.Nta);
		ptLayer->SetData(curRes.x,curRes.pt);ntLayer->SetData(curRes.x,curRes.nt);

		nLayer->SetContinuity(1);pLayer->SetContinuity(1);ntLayer->SetContinuity(1);ptLayer->SetContinuity(1);
		NtdLayer->SetContinuity(1);NtaLayer->SetContinuity(1);
		nLayer->ShowName(0);pLayer->ShowName(0);ntLayer->ShowName(0);ptLayer->ShowName(0);
		NtdLayer->ShowName(0);NtaLayer->ShowName(0);
		wxPen BluePen(*wxBLUE, 2, wxSOLID);wxPen BLACKPEN(*wxBLACK, 2, wxSOLID);
		wxPen REDPEN(*wxRED, 2, wxSOLID);wxPen GREYPEN(*wxLIGHT_GREY,2,wxSOLID);
		NtdLayer->SetPen(BLACKPEN);NtaLayer->SetPen(GREYPEN);
		pLayer->SetPen(REDPEN);nLayer->SetPen(BluePen);
		ntLayer->SetPen(*wxGREEN_PEN);
		wxPen purplePen(wxColour(160,32,240));ptLayer->SetPen(purplePen);
		nLayer->SetDrawOutsideMargins(false);pLayer->SetDrawOutsideMargins(false);
		ntLayer->SetDrawOutsideMargins(false);ptLayer->SetDrawOutsideMargins(false);
		NtdLayer->SetDrawOutsideMargins(false);NtaLayer->SetDrawOutsideMargins(false);
		wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		mpScaleX* xaxis = new mpScaleX(wxT("X(um)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
		mpScaleY* yaxis = new mpScaleY(wxT("lg(cm-3)"), mpALIGN_LEFT, true);
		xaxis->SetFont(graphFont);		yaxis->SetFont(graphFont);
		xaxis->SetDrawOutsideMargins(false);yaxis->SetDrawOutsideMargins(false);
		m_plot->AddLayer( xaxis );	m_plot->AddLayer(yaxis);
		m_plot->AddLayer(pLayer);m_plot->AddLayer(nLayer);m_plot->AddLayer(ptLayer);m_plot->AddLayer(ntLayer);
		m_plot->AddLayer(NtdLayer);m_plot->AddLayer(NtaLayer);
		m_plot->SetMargins(80,0,80,80);
		m_plot->UpdateAll();

	}
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON11
 */

void ResDisplay::OnImportClick( wxCommandEvent& event )
{
	wxDirDialog dir(NULL,_("select data folder"),wxGetCwd());
	wxString fileName,condition,str;	wxArrayString arrStr;
	if (dir .ShowModal() == wxID_OK)
	{
		wxBusyInfo info(_("Importing files, please wait."),this);
		wxString path = dir.GetPath();long light;
		fileName=path+_("/IV.csv");
		if(wxFileExists(fileName))
		{
			double V,I;resPar.V.clear();resPar.I.clear();
			wxFileInputStream input(fileName);
			wxTextInputStream file(input);

			str=file.ReadLine();str=file.ReadLine();arrStr=wxStringTokenize(str,_(","));
			arrStr[0].ToDouble(&resPar.Voc);			arrStr[1].ToDouble(&resPar.Jsc);
			arrStr[2].ToDouble(&resPar.FF);			arrStr[3].ToDouble(&resPar.Effi);
			str=file.ReadLine();arrStr=wxStringTokenize(str,_(","));
			arrStr[1].ToLong(&light);
			str=file.ReadLine();arrStr=wxStringTokenize(str,_(","));
			long max;	arrStr[1].ToLong(&max);
			resPar.max=max;
			file.ReadLine();str=file.ReadLine();
			do
			{
				arrStr=wxStringTokenize(str,_(","));
				arrStr[0].ToDouble(&V);arrStr[1].ToDouble(&I);
				resPar.V.push_back(V);resPar.I.push_back(I);
				str=file.ReadLine();
			}while(!input.Eof());

		}
		int n=resPar.V.size();
		if(light)condition=_("light_");
		else condition=_("dark_");
		resArrayP->Clear();
		for(int i=0;i<n;i++)
		{
			double x,Ec,Ev,Efn,Efp,n,p,Nta,Ntd,nt,pt,Jn,Jp,R,G,EleF;
			fileName.Printf(_("%s\\%s%.6fv.csv"),path.c_str(),condition.c_str(),resPar.V[i]);
			if(wxFileExists(fileName))
			{
				wxFileInputStream input(fileName);
				wxTextInputStream file(input);

				file.ReadLine();
				str=file.ReadLine();
				result *curRes=new result;
				curRes->v=resPar.V[i];
				do
				{
					arrStr=wxStringTokenize(str,_(","));

					arrStr[0].ToDouble(&x);
					arrStr[1].ToDouble(&Ec);arrStr[2].ToDouble(&Ev);arrStr[3].ToDouble(&Efn);arrStr[4].ToDouble(&Efp);
					arrStr[5].ToDouble(&n);arrStr[6].ToDouble(&p);arrStr[7].ToDouble(&Nta);arrStr[8].ToDouble(&Ntd);
					arrStr[9].ToDouble(&nt);arrStr[10].ToDouble(&pt);
					arrStr[11].ToDouble(&Jn);arrStr[12].ToDouble(&Jp);
					arrStr[14].ToDouble(&R);arrStr[15].ToDouble(&G);
					arrStr[16].ToDouble(&EleF);
					curRes->x.push_back(x);
					curRes->Ec.push_back(Ec);curRes->Ev.push_back(Ev);
					curRes->Efn.push_back(Efn);curRes->Efp.push_back(Efp);
					curRes->n.push_back(n);curRes->p.push_back(p);//log
					curRes->nt.push_back(nt);curRes->pt.push_back(pt);
					curRes->Ntd.push_back(Ntd);curRes->Nta.push_back(Nta);
					curRes->R.push_back(R);curRes->G.push_back(G);
					curRes->Jn.push_back(Jn);curRes->Jp.push_back(Jp);
					curRes->EleF.push_back(EleF);

					str=file.ReadLine();
				}while(!input.Eof());
				
				resArrayP->Add(curRes);
			}
			fileName=path+_("/QE.csv");
			if(wxFileExists(fileName))
			{
				wxFileInputStream input(fileName);
				wxTextInputStream file(input);
				file.ReadLine();
				str=file.ReadLine();double QEffi;
				resPar.QEffi.clear();
				do{
					arrStr=wxStringTokenize(str,_(","));
					arrStr[1].ToDouble(&QEffi);
					resPar.QEffi.push_back(QEffi);
					str=file.ReadLine();
				}while(!input.Eof());

			}
		}

		sliderCtrl->SetRange(0,n-1);
		renew();
	}
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON11 in ResDisplay.
}


void ResDisplay::renew(void)
{
	int sel=resListCtrl->GetSelection();
	m_plot->DelAllLayers(1,true);RshCtrl->Enable(0);RsCtrl->Enable(0);
	if( resArrayP->GetCount()!=0)
	{
		switch(sel)
		{
		case 0:
			PlotIV();RshCtrl->Enable(1);RsCtrl->Enable(1);
			break;
		case 1:
			PlotEB();
			break;
		case 2:
			PlotCarrier();
			break;
		case 3:
			PlotEF();
			break;
		case 4:
			PlotJ();
			break;
		case 5:
			PlotR();
			break;
		case 6:
			PlotQEffi();
			break;
		default:
			break;
		}
		wxString vol;  vol.Printf(_("%.2f"),resPar.V[resIndex]);
		VText->SetValue(vol);
	}
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON6
 */

void ResDisplay::OnExportClick( wxCommandEvent& event )
{
	wxDirDialog dirDlg(NULL,_("select folder to export data files"),wxGetCwd());
	if (dirDlg .ShowModal() == wxID_OK)
	{
		wxString filename,path = dirDlg.GetPath();
		wxString f1,f2;
		wxDir dir(_(".\\temp"));
		 bool cont = dir.GetFirst(&filename);
		 while ( cont )
		 {
			 f1.Printf(_(".\\temp\\%s"),filename.c_str());f2.Printf(_("%s\\%s"),path.c_str(),filename.c_str());
			wxCopyFile(f1,f2);
			 cont = dir.GetNext(&filename);
		 }
	}
    event.Skip();
}


void ResDisplay::PlotQEffi(void)
{
	mpFXYVector* QELayer = new mpFXYVector;
	vector<double> wL;
	int n=resPar.QEffi.size();
	for(int i=0;i<n;i++)
		wL.push_back(300+i*20);
	QELayer->SetData(wL,resPar.QEffi);
	QELayer->SetContinuity(1);
	QELayer->SetPen(*wxRED_PEN);
	QELayer->SetDrawOutsideMargins(false);
	wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleX* xaxis = new mpScaleX(wxT("WaveLength(nm))"), mpALIGN_BOTTOM, true, mpX_NORMAL);
	mpScaleY* yaxis = new mpScaleY(wxT("QE"), mpALIGN_LEFT, true);
	xaxis->SetFont(graphFont);
	yaxis->SetFont(graphFont);
	xaxis->SetDrawOutsideMargins(false);
	yaxis->SetDrawOutsideMargins(false);
	m_plot->SetMargins(30, 0, 50, 80);
	m_plot->AddLayer(QELayer);
	m_plot->AddLayer(xaxis);m_plot->AddLayer(yaxis);
}


/*
 * wxEVT_KILL_FOCUS event handler for ID_RESTEXTCTRL1
 */

void ResDisplay::OnVoltageKillFocus( wxFocusEvent& event )
{
	double voltage;
	VText->GetValue().ToDouble(&voltage);
	int n=resPar.V.size();
	if(n)
	{
		for(int i=0;i<n;i++)
		{
			if(voltage<=resPar.V[i])
			{
				resIndex=i;
				break;
			}
			else
			{
				resIndex=n-1;
			}
		}
		sliderCtrl->SetValue(resIndex);
		wxString str;str.Printf(_("%.2f"),resPar.V[resIndex]);
		VText->SetValue(str);
		if( resArrayP->GetCount()!=0)
		{
			int sel=resListCtrl->GetSelection();
			switch(sel)
			{
			case 0:
				break;
			case 1:
				m_plot->DelAllLayers(1,true);
				PlotEB();
				break;
			case 2:
				m_plot->DelAllLayers(1,true);
				PlotCarrier();
				break;
			case 3:
				m_plot->DelAllLayers(1,true);
				PlotEF();
				break;
			case 4:
				m_plot->DelAllLayers(1,true);
				PlotJ();
				break;
			case 5:
				m_plot->DelAllLayers(1,true);
				PlotR();
				break;
			case 6:
				break;
			default:
				break;
			}
			wxRect rect=m_plot->GetClientRect();
			m_plot->AddLayer(new mpInfoCoords(wxRect(rect.width-180,0,180,20), wxTRANSPARENT_BRUSH));
			m_plot->AddLayer( new mpInfoLegend(wxRect(300,0,50,50), wxTRANSPARENT_BRUSH));
			m_plot->EnableDoubleBuffer(true);
			m_plot->SetMPScrollbars(false);
		}
	}
	else
	{
		VText->SetValue(_("0"));
	}
    event.Skip();
}


/*
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDERSH
 */

void ResDisplay::OnSlidershUpdated( wxCommandEvent& event )
{
	double RshMax,RshMin;wxString str; int index;
	str=RshMaxText->GetValue();str.ToDouble(&RshMax);str=RshMinText->GetValue();str.ToDouble(&RshMin);
	index=RshCtrl->GetValue();	
	if(RshMax<=0)
		RshMax=1;
	if(RshMin<1)
		RshMin=1;
	Rsh=RshMax*pow(RshMin/RshMax,index/100.0);
	int n=resPar.V.size();	RsPar.V.clear();RsPar.I.clear();
	for(int i=0;i<n;i++)
	{
		RsPar.I.push_back(resPar.I[i]-1e3*resPar.V[i]/Rsh);//Rsh: Ohm cm2
		RsPar.V.push_back(resPar.V[i]-Rs*RsPar.I[i]*1e-3);
	}

	if(RLayer!=NULL)
	{
		m_plot->DelLayer(RLayer,1,0);		
	}
	RLayer = new mpFXYVector;	RLayer->SetData(RsPar.V,RsPar.I);
	RLayer->SetContinuity(1);	wxPen BluePen(*wxBLUE, 1, wxDOT );RLayer->SetPen(BluePen);	RLayer->SetDrawOutsideMargins(false);
	CalEffi(RsPar);
	m_plot->AddLayer(RLayer,1);
    event.Skip();
////@end wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDERSH in ResDisplay. 
}
void ResDisplay::CalEffi(resultPar res)
{
	int n=res.I.size();int i0=0,v0=n-1;
	do
	{
		i0++;
	}while(res.I[i0]>0);
	do
	{
		v0--;
	}while(res.V[v0]>0);
	
	res.Voc=res.V[i0-1]+log(1+(exp(res.V[i0]-res.V[i0-1])-1)*res.I[i0-1]/(res.I[i0-1]-res.I[i0]));	
	res.Jsc=res.I[v0]+(1-exp(res.V[v0]))/(exp(res.V[v0+1])-exp(res.V[v0]))*(res.I[v0+1]-res.I[v0]);

	double pMax=0;
	for(int i=0;i<n;i++)
	{
		if(res.I[i]*res.V[i]>pMax)
		{
			pMax=res.I[i]*res.V[i];
			res.max=i;
		}
	}
	
	res.FF=pMax/res.Jsc/res.Voc*100;
	res.Effi=pMax;//%
	
	if(VocTextR!=NULL)
	{
		m_plot->DelLayer(VocTextR,1,0);m_plot->DelLayer(JscTextR,1,0);m_plot->DelLayer(FFTextR,1,0);
		m_plot->DelLayer(EffiTextR,1,0);m_plot->DelLayer(RshText,1,0);m_plot->DelLayer(RsText,1,0);
	}
	wxString str;int strx,stry;
	str.Printf(_("Voc :%f V"),res.Voc);	
	strx=20;	stry=10;
	VocTextR=new mpText(str,strx, stry);		m_plot->AddLayer( VocTextR );
	str.Printf(_("Jsc :%f mA/cm2"),res.Jsc);
	JscTextR=new mpText(str, strx, stry+5);	m_plot->AddLayer( JscTextR );
	str.Printf(_("FF  :%.4f %c"),res.FF,'%');
	FFTextR=new mpText(str, strx, stry+10);	m_plot->AddLayer( FFTextR);
	str.Printf(_("Effi:%f %c"),res.Effi,'%');
	EffiTextR=new mpText(str, strx, stry+15);m_plot->AddLayer( EffiTextR );
	str.Printf(_("Rsh:%.4e Ohm.cm2"),Rsh);
	RshText=new mpText(str, strx, stry-5);m_plot->AddLayer( RshText );
	str.Printf(_("Rs:%f Ohm.cm2"),Rs);
	RsText=new mpText(str, strx, stry-10);m_plot->AddLayer( RsText );
}


/*
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER1
 */

void ResDisplay::OnSliderrsUpdated( wxCommandEvent& event )
{
	double RsMax,RsMin;wxString str; int index;
	str=RsMaxText->GetValue();str.ToDouble(&RsMax);str=RsMinText->GetValue();str.ToDouble(&RsMin);
	index=RsCtrl->GetValue();
	if(RsMin<1e-3)RsMin=1e-3;
	if(RsMax<1)RsMax=1;
	Rs=RsMin*pow(RsMax/RsMin,index/100.0);
	
	int n=resPar.V.size();	RsPar.V.clear();RsPar.I.clear();
	for(int i=0;i<n;i++)
	{		
		RsPar.I.push_back(resPar.I[i]-1e3*resPar.V[i]/Rsh);//Rsh: Ohm cm2
		RsPar.V.push_back(resPar.V[i]-Rs*RsPar.I[i]*1e-3);
	}

	if(RLayer!=NULL)
	{
		m_plot->DelLayer(RLayer,1,0);		
	}
	RLayer = new mpFXYVector;	RLayer->SetData(RsPar.V,RsPar.I);
	RLayer->SetContinuity(1);	wxPen BluePen(*wxBLUE, 1, wxDOT );RLayer->SetPen(BluePen);	RLayer->SetDrawOutsideMargins(false);
	CalEffi(RsPar);
	m_plot->AddLayer(RLayer,1);
    event.Skip();
}


void ResDisplay::PlotEF(void)
{
	mpFXYVector* EleFLayer = new mpFXYVector;
	result curRes=resArrayP->Item(resIndex);
	EleFLayer->SetData(curRes.x,curRes.EleF);
	EleFLayer->SetContinuity(1);
	EleFLayer->SetPen(*wxRED_PEN);
	EleFLayer->SetDrawOutsideMargins(false);
	wxFont graphFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	mpScaleX* xaxis = new mpScaleX(wxT("x(um))"), mpALIGN_BOTTOM, true, mpX_NORMAL);
	mpScaleY* yaxis = new mpScaleY(wxT("Electric Field(v/cm)"), mpALIGN_LEFT, true);
	xaxis->SetFont(graphFont);
	yaxis->SetFont(graphFont);
	xaxis->SetDrawOutsideMargins(false);
	yaxis->SetDrawOutsideMargins(false);
	m_plot->SetMargins(30, 0, 50, 80);
	m_plot->AddLayer(EleFLayer);
	m_plot->AddLayer(xaxis);m_plot->AddLayer(yaxis);
}
