/////////////////////////////////////////////////////////////////////////////
// Name:        settingdlg.cpp
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     17/03/2011 19:29:12
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

#include "settingdlg.h"
#include <fstream>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
using namespace std;
////@begin XPM images
////@end XPM images


/*
 * SettingDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SettingDlg, wxDialog )


/*
 * SettingDlg event table definition
 */

BEGIN_EVENT_TABLE( SettingDlg, wxDialog )

////@begin SettingDlg event table entries
    EVT_INIT_DIALOG( SettingDlg::OnInitDialog )

    EVT_BUTTON( ID_SettingOK, SettingDlg::OnSettingOKClick )

    EVT_BUTTON( ID_SettingCancel, SettingDlg::OnSettingCancelClick )

////@end SettingDlg event table entries

END_EVENT_TABLE()


/*
 * SettingDlg constructors
 */

SettingDlg::SettingDlg()
{
    Init();
}

SettingDlg::SettingDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * SettingDlg creator
 */

bool SettingDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SettingDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end SettingDlg creation
    return true;
}


/*
 * SettingDlg destructor
 */

SettingDlg::~SettingDlg()
{
////@begin SettingDlg destruction
////@end SettingDlg destruction
}


/*
 * Member initialisation
 */

void SettingDlg::Init()
{
	setPar.clamp=_("0.05");
	setPar.eps=_("1e-6");
	setPar.F0=_("1e5");
	setPar.model=0;
}


/*
 * Control creation for SettingDlg
 */

void SettingDlg::CreateControls()
{    
////@begin SettingDlg content construction
    SettingDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxArrayString ModelCtrlStrings;
    ModelCtrlStrings.Add(_("&Trap-assisted Tunneling"));
    ModelCtrlStrings.Add(_("&Intra-band Tunneling"));
    ModelCtrl = new wxRadioBox( itemDialog1, ID_RADIOBOX, _("Model"), wxDefaultPosition, wxDefaultSize, ModelCtrlStrings, 1, wxRA_SPECIFY_COLS );
    ModelCtrl->SetSelection(0);
    itemBoxSizer2->Add(ModelCtrl, 0, wxGROW|wxALL, 5);

    wxStaticLine* itemStaticLine4 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine4, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Numerical Parameters"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Iteration Times Limit"), wxDefaultPosition, wxSize(120, -1), 0 );
    itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL33, _("500"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemTextCtrl8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Converging Precision"), wxDefaultPosition, wxSize(120, -1), 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl11 = new wxTextCtrl( itemDialog1, ID_Precision, _("1e-6"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemTextCtrl11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Clamping Range      "), wxDefaultPosition, wxSize(120, -1), 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl14 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL31, _("0.05"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemTextCtrl14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemDialog1, wxID_STATIC, _("TAT Settings"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("Mobility Enhanced E0"), wxDefaultPosition, wxSize(120, -1), 0 );
    itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl18 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL32, _("1e5"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemTextCtrl18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton20 = new wxButton( itemDialog1, ID_SettingOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton21 = new wxButton( itemDialog1, ID_SettingCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Set validators
    itemTextCtrl8->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & setPar.iterTimes) );
    itemTextCtrl11->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & setPar.eps) );
    itemTextCtrl14->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & setPar.clamp) );
    itemTextCtrl18->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & setPar.F0) );
////@end SettingDlg content construction
}


/*
 * Should we show tooltips?
 */

bool SettingDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap SettingDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SettingDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SettingDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon SettingDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SettingDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SettingDlg icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SettingOK
 */

void SettingDlg::OnSettingOKClick( wxCommandEvent& event )
{
	TransferDataFromWindow();
	setPar.model=ModelCtrl->GetSelection();
	ofstream out(_("setting.tmp"));
	out<<setPar.model<<endl<<setPar.iterTimes<<endl<<setPar.eps<<endl<<setPar.clamp<<endl<<setPar.F0;
	EndModal(wxID_OK);
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SettingCancel
 */

void SettingDlg::OnSettingCancelClick( wxCommandEvent& event )
{
	EndModal(wxID_CANCEL);
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SettingCancel in SettingDlg. 
}


/*
 * wxEVT_INIT_DIALOG event handler for ID_SETTINGDLG
 */

void SettingDlg::OnInitDialog( wxInitDialogEvent& event )
{
	ModelCtrl->SetSelection(setPar.model);
	TransferDataToWindow();
    event.Skip();
}

