/////////////////////////////////////////////////////////////////////////////
// Name:        ambient.h
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     18/02/2010 14:54:26
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _AMBIENT_H_
#define _AMBIENT_H_


/*!
 * Includes
 */
#include <vector>
using namespace std;
////@begin includes
#include "wx/valtext.h"
#include "wx/valgen.h"
#include "wx/statline.h"
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
#define ID_AMBIENT 10005
#define ID_AMBTEXTCTRL 10006
#define ID_RADIOOFF 10010
#define ID_RADIOON 10011
#define ID_TEXTCTRL1 10007
#define ID_CHECKBOX3 10074
#define ID_CHECKBOX2 10069
#define ID_TEXTCTRL40 10070
#define ID_AMBTEXTCTRL3 10002
#define ID_AMBTEXTCTRL2 10001
#define ID_AMBTEXTCTRL4 10003
#define ID_AMBTEXTCTRL5 10004
#define ID_AMBTEXTCTRL6 10008
#define ID_AMBTEXTCTRL7 10009
#define ID_VolFIlePath 10059
#define ID_BUTTON12 10057
#define ID_AMBIENTCANCLE 10013
#define SYMBOL_AMBIENT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_AMBIENT_TITLE _("ambientDlg")
#define SYMBOL_AMBIENT_IDNAME ID_AMBIENT
#define SYMBOL_AMBIENT_SIZE wxDefaultSize
#define SYMBOL_AMBIENT_POSITION wxDefaultPosition
////@end control identifiers
struct ambientPar
{
public:
	wxString m_temp;
    //wxString m_biasV;
    bool m_light,IsWF,IsQE;
	wxString m_specPath;
//	wxString VStart,VEnd,Step1,Step2,Step3,Step4,Switch1,Switch2,Switch3,VolStream;
	wxString TopWF,BtmWF,TopSn,TopSp,TopRF,BtmSn,BtmSp,BtmRF;
	vector<double> waveLength;//m
	vector<double> photonNum;
	vector<double> vSet;
};

/*!
 * ambient class declaration
 */

class ambient: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ambient )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ambient();
    ambient(wxWindow* parent, wxWindowID id = SYMBOL_AMBIENT_IDNAME, const wxString& caption = SYMBOL_AMBIENT_TITLE, const wxPoint& pos = SYMBOL_AMBIENT_POSITION, const wxSize& size = SYMBOL_AMBIENT_SIZE, long style = SYMBOL_AMBIENT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_AMBIENT_IDNAME, const wxString& caption = SYMBOL_AMBIENT_TITLE, const wxPoint& pos = SYMBOL_AMBIENT_POSITION, const wxSize& size = SYMBOL_AMBIENT_SIZE, long style = SYMBOL_AMBIENT_STYLE );

    /// Destructor
    ~ambient();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ambient event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_AMBIENT
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOOFF
    void OnRadiooffSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOON
    void OnRadioonSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX2
    void OnCheckbox2Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON12
    void OnVolLoadClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_AMBIENTCANCLE
    void OnAmbientcancleClick( wxCommandEvent& event );

////@end ambient event handler declarations

////@begin ambient member function declarations

    wxString GetVolPath() const { return m_volPath ; }
    void SetVolPath(wxString value) { m_volPath = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ambient member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ambient member variables
    wxRadioButton* RadioBtnOff;
    wxRadioButton* RadioBtnOn;
    wxTextCtrl* SpecPathCtrl;
    wxTextCtrl* TopWFCtrl;
    wxTextCtrl* BtmWFCtrl;
    wxButton* OkButton;
    wxString m_volPath;
////@end ambient member variables
	ambientPar ambPar;
	wxString darkVolPath,lightVolPath,darkName,lightName,volPath,volPathName;
	
};

#endif
    // _AMBIENT_H_
