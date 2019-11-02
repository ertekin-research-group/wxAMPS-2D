/////////////////////////////////////////////////////////////////////////////
// Name:        mainamps.h
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     16/02/2010 12:58:36
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAINAMPS_H_
#define _MAINAMPS_H_


/*!
 * Includes
 */
#include "Point.h"
#include "Point2.h"
#include "ambient.h"
#include "materialdlg.h"
#include "resdisplay.h"
#include "settingdlg.h"
//using namespace std;
////@begin includes
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
#define ID_MAINAMPS 10000
#define ID_AMBIENTBTN 10004
#define ID_MATERIALBTN 10001
#define ID_RUNBUTTON 10025
#define ID_Settings 10062
#define ID_GRAPHSBTN 10002
#define ID_OK 10003
#define SYMBOL_MAINAMPS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_MAINAMPS_TITLE _("wxAMPS")
#define SYMBOL_MAINAMPS_IDNAME ID_MAINAMPS
#define SYMBOL_MAINAMPS_SIZE wxSize(400, 300)
#define SYMBOL_MAINAMPS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * MainAMPS class declaration
 */

class MainAMPS: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( MainAMPS )
    DECLARE_EVENT_TABLE()

public:	
	int prgCnt;
	bool IsInit;
	//Point pts;
	//Point *pt;
	Point2 *pt;
    /// Constructors
    MainAMPS();
    MainAMPS( wxWindow* parent, wxWindowID id = SYMBOL_MAINAMPS_IDNAME, const wxString& caption = SYMBOL_MAINAMPS_TITLE, const wxPoint& pos = SYMBOL_MAINAMPS_POSITION, const wxSize& size = SYMBOL_MAINAMPS_SIZE, long style = SYMBOL_MAINAMPS_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MAINAMPS_IDNAME, const wxString& caption = SYMBOL_MAINAMPS_TITLE, const wxPoint& pos = SYMBOL_MAINAMPS_POSITION, const wxSize& size = SYMBOL_MAINAMPS_SIZE, long style = SYMBOL_MAINAMPS_STYLE );

    /// Destructor
    ~MainAMPS();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MainAMPS event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_MAINAMPS
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_DESTROY event handler for ID_MAINAMPS
    void OnDestroy( wxWindowDestroyEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_AMBIENTBTN
    void OnAmbientbtnClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MATERIALBTN
    void OnMaterialbtnClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNBUTTON
    void OnRunbuttonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_Settings
    void OnSettingsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GRAPHSBTN
    void OnGraphsbtnClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OK
    void OnOkClick( wxCommandEvent& event );

////@end MainAMPS event handler declarations

////@begin MainAMPS member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MainAMPS member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MainAMPS member variables
    wxBitmapButton* RunBtn;
    wxBitmapButton* GraphBtn;
////@end MainAMPS member variables
public:
	ambientPar ambPar;
	settingPar setPar;
	MyList LayerList;
	ResultArray ResArray;

	bool ExportG(Point *pt,double TopRF,double BtmRF,vector<double> photo0,vector<double> waveLength);
	resultPar	resPar;
	double vOld;
	/*vector<double> I;
	vector<double> V;*/
};

#endif
    // _MAINAMPS_H_
