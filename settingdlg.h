/////////////////////////////////////////////////////////////////////////////
// Name:        settingdlg.h
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     17/03/2011 19:29:12
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SETTINGDLG_H_
#define _SETTINGDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
#include "wx/valtext.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SETTINGDLG 10063
#define ID_RADIOBOX 10064
#define ID_TEXTCTRL33 10078
#define ID_Precision 10065
#define ID_TEXTCTRL31 10066
#define ID_TEXTCTRL32 10077
#define ID_SettingOK 10075
#define ID_SettingCancel 10076
#define SYMBOL_SETTINGDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SETTINGDLG_TITLE _("Numeric Settings")
#define SYMBOL_SETTINGDLG_IDNAME ID_SETTINGDLG
#define SYMBOL_SETTINGDLG_SIZE wxSize(400, 300)
#define SYMBOL_SETTINGDLG_POSITION wxDefaultPosition
////@end control identifiers
struct settingPar{
	int model;
	wxString eps,clamp,F0,iterTimes;
};

/*!
 * SettingDlg class declaration
 */

class SettingDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SettingDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SettingDlg();
    SettingDlg( wxWindow* parent, wxWindowID id = SYMBOL_SETTINGDLG_IDNAME, const wxString& caption = SYMBOL_SETTINGDLG_TITLE, const wxPoint& pos = SYMBOL_SETTINGDLG_POSITION, const wxSize& size = SYMBOL_SETTINGDLG_SIZE, long style = SYMBOL_SETTINGDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SETTINGDLG_IDNAME, const wxString& caption = SYMBOL_SETTINGDLG_TITLE, const wxPoint& pos = SYMBOL_SETTINGDLG_POSITION, const wxSize& size = SYMBOL_SETTINGDLG_SIZE, long style = SYMBOL_SETTINGDLG_STYLE );

    /// Destructor
    ~SettingDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SettingDlg event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_SETTINGDLG
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SettingOK
    void OnSettingOKClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SettingCancel
    void OnSettingCancelClick( wxCommandEvent& event );

////@end SettingDlg event handler declarations

////@begin SettingDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SettingDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SettingDlg member variables
    wxRadioBox* ModelCtrl;
////@end SettingDlg member variables
public:
	settingPar setPar;
};

#endif
    // _SETTINGDLG_H_
