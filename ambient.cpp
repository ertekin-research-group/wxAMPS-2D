/////////////////////////////////////////////////////////////////////////////
// Name:        ambient.cpp
// Purpose:
// Author:      Liu
// Modified by:
// Created:     18/02/2010 14:54:26
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
#include "Constant.h"
#include "ambient.h"
#include <fstream>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>

#include <wx/wfstream.h>
////@begin XPM images
////@end XPM images


/*
 * ambient type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ambient, wxDialog )


/*
 * ambient event table definition
 */

BEGIN_EVENT_TABLE( ambient, wxDialog )

////@begin ambient event table entries
    EVT_INIT_DIALOG( ambient::OnInitDialog )

    EVT_RADIOBUTTON( ID_RADIOOFF, ambient::OnRadiooffSelected )

    EVT_RADIOBUTTON( ID_RADIOON, ambient::OnRadioonSelected )

    EVT_CHECKBOX( ID_CHECKBOX2, ambient::OnCheckbox2Click )

    EVT_BUTTON( ID_BUTTON12, ambient::OnVolLoadClick )

    EVT_BUTTON( wxID_OK, ambient::OnOkClick )

    EVT_BUTTON( ID_AMBIENTCANCLE, ambient::OnAmbientcancleClick )

////@end ambient event table entries

END_EVENT_TABLE()


/*
 * ambient constructors
 */

ambient::ambient()
{
    Init();
}

ambient::ambient(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * ambient creator
 */

bool ambient::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ambient creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ambient creation
    return true;
}


/*
 * ambient destructor
 */

ambient::~ambient()
{
////@begin ambient destruction
////@end ambient destruction
}


/*
 * Member initialisation
 */

void ambient::Init()
{
////@begin ambient member initialisation
    RadioBtnOff = NULL;
    RadioBtnOn = NULL;
    SpecPathCtrl = NULL;
    TopWFCtrl = NULL;
    BtmWFCtrl = NULL;
    OkButton = NULL;
////@end ambient member initialisation

}


/*
 * Control creation for ambient
 */

void ambient::CreateControls()
{
////@begin ambient content construction
    ambient* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemTextCtrl5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("K"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Spectrum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    RadioBtnOff = new wxRadioButton( itemDialog1, ID_RADIOOFF, _("Off"), wxDefaultPosition, wxDefaultSize, 0 );
    RadioBtnOff->SetValue(false);
    itemBoxSizer7->Add(RadioBtnOff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    RadioBtnOn = new wxRadioButton( itemDialog1, ID_RADIOON, _("On"), wxDefaultPosition, wxDefaultSize, 0 );
    RadioBtnOn->SetValue(false);
    itemBoxSizer7->Add(RadioBtnOn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    SpecPathCtrl = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_MULTILINE|wxTE_READONLY );
    SpecPathCtrl->Enable(false);
    itemBoxSizer7->Add(SpecPathCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxFIXED_MINSIZE, 5);

    wxCheckBox* itemCheckBox12 = new wxCheckBox( itemDialog1, ID_CHECKBOX3, _("QE"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox12->SetValue(false);
    itemBoxSizer7->Add(itemCheckBox12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine13 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine13, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer14, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemDialog1, wxID_STATIC, _("Contacts"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemStaticText15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox16 = new wxCheckBox( itemDialog1, ID_CHECKBOX2, _("PHIB(ev)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox16->SetValue(false);
    if (ambient::ShowToolTips())
        itemCheckBox16->SetToolTip(_("Ec-Ef on the contact"));
    itemBoxSizer14->Add(itemCheckBox16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("Sn0(cm/s)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemDialog1, wxID_STATIC, _("Sp0(cm/s)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemStaticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemDialog1, wxID_STATIC, _("RF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemStaticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer20, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( itemDialog1, wxID_STATIC, _("Top"), wxDefaultPosition, wxSize(55, -1), 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    TopWFCtrl = new wxTextCtrl( itemDialog1, ID_TEXTCTRL40, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    TopWFCtrl->Enable(false);
    itemBoxSizer20->Add(TopWFCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl23 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL3, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer20->Add(itemTextCtrl23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl24 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer20->Add(itemTextCtrl24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl25 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer20->Add(itemTextCtrl25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer26, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText27 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bottom"), wxDefaultPosition, wxSize(55, -1), 0 );
    itemBoxSizer26->Add(itemStaticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    BtmWFCtrl = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL4, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    BtmWFCtrl->Enable(false);
    itemBoxSizer26->Add(BtmWFCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl29 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL5, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer26->Add(itemTextCtrl29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl30 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL6, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer26->Add(itemTextCtrl30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl31 = new wxTextCtrl( itemDialog1, ID_AMBTEXTCTRL7, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer26->Add(itemTextCtrl31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine32 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine32, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer33, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText34 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bias Voltages"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer33->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl35 = new wxTextCtrl( itemDialog1, ID_VolFIlePath, wxEmptyString, wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer33->Add(itemTextCtrl35, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton36 = new wxButton( itemDialog1, ID_BUTTON12, _("Load"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer33->Add(itemButton36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer37, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    OkButton = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    OkButton->SetDefault();
    itemBoxSizer37->Add(OkButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton39 = new wxButton( itemDialog1, ID_AMBIENTCANCLE, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer37->Add(itemButton39, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Set validators
    itemTextCtrl5->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.m_temp) );
    SpecPathCtrl->SetValidator( wxGenericValidator(& ambPar.m_specPath) );
    itemCheckBox12->SetValidator( wxGenericValidator(& ambPar.IsQE) );
    itemCheckBox16->SetValidator( wxGenericValidator(& ambPar.IsWF) );
    TopWFCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.TopWF) );
    itemTextCtrl23->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.TopSn) );
    itemTextCtrl24->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.TopSp) );
    itemTextCtrl25->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.TopRF) );
    BtmWFCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.BtmWF) );
    itemTextCtrl29->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.BtmSn) );
    itemTextCtrl30->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.BtmSp) );
    itemTextCtrl31->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ambPar.BtmRF) );
    itemTextCtrl35->SetValidator( wxGenericValidator(& volPathName) );
////@end ambient content construction
}


/*
 * Should we show tooltips?
 */

bool ambient::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap ambient::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ambient bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ambient bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon ambient::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ambient icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ambient icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OK
 */

void ambient::OnOkClick( wxCommandEvent& event )
{
	ambPar.photonNum.clear();ambPar.waveLength.clear();
	ambPar.vSet.clear();
	
	if(1==ambPar.m_light)
	{
		wxFileInputStream input(ambPar.m_specPath);
		if(input.IsOk())
		{
			wxTextInputStream file( input );
			double wave,energy,photon;
			wxString str;	wxArrayString arrStr;

			while(!input.Eof())//XML
			{
				str=file.ReadLine();
				if(str!=_("\0")&&str.Find('>')!=0&&str.Find('#')!=0&&str.Find('/')!=0)
				{
					arrStr=wxStringTokenize(str);
					arrStr[0].ToDouble(&wave);
					arrStr[1].ToDouble(&energy);
					photon=wave*1e-9*energy*1e-4/H/C;//cm-2
					ambPar.waveLength.push_back(wave*1e-9);
					ambPar.photonNum.push_back(photon);
				}
			}
		}
	}	

	ifstream file(volPath);
	if(file.is_open())
	{
		wxString str;	double v;
		while(!file.eof())
		{
			if(!(file>>v))//if v is not double
				break;
			ambPar.vSet.push_back(v);
		}
		file.close();

		TransferDataFromWindow();
		//output ambient parameters
		ofstream out(_("amb.tmp"));
		out<<ambPar.m_temp<<endl<<ambPar.IsQE<<endl;
		out<<ambPar.IsWF<<endl;
		if(1==ambPar.IsWF)out<<ambPar.TopWF<<" "<<ambPar.BtmWF<<endl;
		out<<ambPar.TopSn<<" "<<ambPar.TopSp<<" "<<ambPar.TopRF<<" "<<ambPar.BtmSn<<" "<<ambPar.BtmSp<<" "<<ambPar.BtmRF<<endl;
		out<<darkVolPath<<endl<<darkName<<endl;
		out<<lightVolPath<<endl<<lightName<<endl;
		out.close();
		EndModal(wxID_OK);
		event.Skip();
	}   
	else
		wxMessageBox(_("Please load a voltage file"));
	//event.Skip();
}


/*
* wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON1
*/

void ambient::OnRadioonSelected( wxCommandEvent& event )
{
	TransferDataFromWindow();
	wxFileDialog fileDlg(NULL,wxT("Choose a file"),_(""),_(""),
		_("Spectrum files (*.spe)|*.spe|XML files (*.spex)|*.spex|All files (*.*)|*.*"));

	if(wxID_OK==fileDlg.ShowModal())
	{
		ambPar.m_light=1;
		SpecPathCtrl->Enable(1);
		ambPar.m_specPath=fileDlg.GetPath();
		volPath=lightVolPath;volPathName=lightName;
		//	ambPar.Switch1=_("0.4");ambPar.Switch2=_("1");ambPar.Switch3=_("2");
		//	if(0==RsltionCtrl->GetSelection())
		//	{ambPar.Step1=_("0.05");ambPar.Step2=_("0.02");ambPar.Step3=_("0.1");ambPar.Step4=_("0.5");}
		//	else
		//	{ambPar.Step1=_("0.01");ambPar.Step2=_("0.01");ambPar.Step3=_("0.05");ambPar.Step4=_("0.2");}
		TransferDataToWindow();
	}
	RadioBtnOff->SetValue(!ambPar.m_light);
	RadioBtnOn->SetValue(ambPar.m_light);

	OkButton->SetFocus();
	event.Skip();

}


/*
* wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON
*/

void ambient::OnRadiooffSelected( wxCommandEvent& event )
{
	ambPar.m_light=0;
	SpecPathCtrl->Enable(0);
	volPath=darkVolPath;volPathName=darkName;
	//	ambPar.Switch1=_("1");ambPar.Switch2=_("2");ambPar.Switch3=_("5");
	//	ambPar.Step1=_("0.05");ambPar.Step2=_("0.1");ambPar.Step3=_("0.2");ambPar.Step4=_("0.5");
	TransferDataToWindow();
	event.Skip();
}


/*
* wxEVT_INIT_DIALOG event handler for ID_AMBIENT
*/

void ambient::OnInitDialog( wxInitDialogEvent& event )
{
	RadioBtnOff->SetValue(!ambPar.m_light);
	RadioBtnOn->SetValue(ambPar.m_light);
	SpecPathCtrl->Enable(ambPar.m_light);

	TopWFCtrl->Enable(ambPar.IsWF);
	BtmWFCtrl->Enable(ambPar.IsWF);

	if(wxFile::Exists(_("amb.tmp")))
	{
		wxFileInputStream input(_("amb.tmp"));
		if(input.IsOk())
		{
			wxTextInputStream file( input ); wxString str;			
			file>>ambPar.m_temp>>str;			
			if(_("1")==str)ambPar.IsQE=1;else ambPar.IsQE=0;
			file>>str;
			if(_("1")==str)ambPar.IsWF=1;else ambPar.IsWF=0; 			
			if(ambPar.IsWF)
			{
				file>>ambPar.TopWF>>ambPar.BtmWF;
				TopWFCtrl->Enable(ambPar.IsWF);	BtmWFCtrl->Enable(ambPar.IsWF);
			}
			file>>ambPar.TopSn>>ambPar.TopSp>>ambPar.TopRF>>ambPar.BtmSn>>ambPar.BtmSp>>ambPar.BtmRF;
			//file>>darkVolPath>>darkName;
			//file>>lightVolPath>>lightName;	
			darkVolPath=file.ReadLine(); darkName=file.ReadLine();
			lightVolPath=file.ReadLine();lightName=file.ReadLine();
			if(!wxFile::Exists(darkVolPath))
				darkName.Printf(_(" "));
			if(!wxFile::Exists(lightVolPath))
				lightName.Printf(_(" "));
		}
		if(0==ambPar.m_light)
		{volPath=darkVolPath;volPathName=darkName;}
		else
		{volPath=lightVolPath;volPathName=lightName;}
	}
	TransferDataToWindow();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_AMBIENTCANCLE
 */

void ambient::OnAmbientcancleClick( wxCommandEvent& event )
{
	EndModal(wxID_CANCEL);
    event.Skip();
}





/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX2
 */

void ambient::OnCheckbox2Click( wxCommandEvent& event )
{
	TransferDataFromWindow();

	TopWFCtrl->Enable(ambPar.IsWF);
	BtmWFCtrl->Enable(ambPar.IsWF);
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX2 in ambient.
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON12
 */

void ambient::OnVolLoadClick( wxCommandEvent& event )
{
	TransferDataFromWindow();
	wxFileDialog fileDlg(NULL,wxT("Choose a file"),_(""),_(""),
		_("Voltage files (*.vol)|*.vol|All files (*.*)|*.*"));

	if(wxID_OK==fileDlg.ShowModal())
	{
		volPath=fileDlg.GetPath();
		volPathName=fileDlg.GetFilename();
		if(ambPar.m_light==0)
		{	darkVolPath=volPath;darkName=volPathName;}
		else
		{	lightVolPath=volPath;lightName=volPathName;}
		TransferDataToWindow();
	}
	OkButton->SetFocus();
    event.Skip();

}

