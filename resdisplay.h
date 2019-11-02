/////////////////////////////////////////////////////////////////////////////
// Name:        resdisplay.h
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     24/02/2010 16:23:35
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _RESDISPLAY_H_
#define _RESDISPLAY_H_


/*!
 * Includes
 */
#include "Point.h"
#include "mathplot.h"
#include "wx/progdlg.h"
////@begin includes
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class mpWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_RESDISPLAY 10027
#define ID_LISTBOX 10028
#define ID_BUTTON 10030
#define ID_BUTTON1 10031
#define ID_BUTTON6 10067
#define ID_BUTTON11 10068
#define ID_BUTTON2 10032
#define ID_RESTEXTCTRL1 10038
#define ID_SLIDER 10037
#define wxID_STATIC2 10060
#define ID_SLIDERSH 10058
#define ID_RshTEXTCTRL 10000
#define ID_TEXTCTRL30 10061
#define ID_STATICTEXT 10001
#define ID_SLIDERRS 10002
#define ID_TEXTCTRLRs 10003
#define ID_TEXTCTRLRs2 10004
#define ID_PANEL3 10035
#define ID_MPWINDOW 10034
#define SYMBOL_RESDISPLAY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_RESDISPLAY_TITLE _("ResDisplay")
#define SYMBOL_RESDISPLAY_IDNAME ID_RESDISPLAY
#define SYMBOL_RESDISPLAY_SIZE wxSize(800, 600)
#define SYMBOL_RESDISPLAY_POSITION wxDefaultPosition
////@end control identifiers
struct resultPar
{
	double Voc, Jsc, FF, Effi;
	int max;
	vector<double> I;vector<double> V;	
	vector<double> QEffi;
};
struct result
{
	double v;
	vector<double> x;
	vector<double> Jn,Jp,J;
	vector<double> R,G;
	vector<double> Ec,Ev,Efn,Efp;
	vector<double> n,p,Ntd,Nta,nt,pt;
	vector<double> EleF;
};
WX_DECLARE_OBJARRAY(result, ResultArray);

/*!
 * ResDisplay class declaration
 */

class ResDisplay: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ResDisplay )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ResDisplay();
    ResDisplay( wxWindow* parent, wxWindowID id = SYMBOL_RESDISPLAY_IDNAME, const wxString& caption = SYMBOL_RESDISPLAY_TITLE, const wxPoint& pos = SYMBOL_RESDISPLAY_POSITION, const wxSize& size = SYMBOL_RESDISPLAY_SIZE, long style = SYMBOL_RESDISPLAY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_RESDISPLAY_IDNAME, const wxString& caption = SYMBOL_RESDISPLAY_TITLE, const wxPoint& pos = SYMBOL_RESDISPLAY_POSITION, const wxSize& size = SYMBOL_RESDISPLAY_SIZE, long style = SYMBOL_RESDISPLAY_STYLE );

    /// Destructor
    ~ResDisplay();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ResDisplay event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_RESDISPLAY
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
    void OnListboxSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
    void OnCopyButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
    void OnSaveButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON6
    void OnExportClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON11
    void OnImportClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON2
    void OnPrintButtonClick( wxCommandEvent& event );

    /// wxEVT_KILL_FOCUS event handler for ID_RESTEXTCTRL1
    void OnVoltageKillFocus( wxFocusEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
    void OnSliderUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDERSH
    void OnSlidershUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDERRS
    void OnSliderrsUpdated( wxCommandEvent& event );

////@end ResDisplay event handler declarations

////@begin ResDisplay member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ResDisplay member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ResDisplay member variables
    wxListBox* resListCtrl;
    wxButton* ExportBtn;
    wxTextCtrl* VText;
    wxSlider* sliderCtrl;
    wxSlider* RshCtrl;
    wxTextCtrl* RshMaxText;
    wxTextCtrl* RshMinText;
    wxSlider* RsCtrl;
    wxTextCtrl* RsMinText;
    wxTextCtrl* RsMaxText;
    mpWindow* m_plot;
////@end ResDisplay member variables
	resultPar resPar;
	resultPar RsPar;
	double Rsh,Rs;
	mpText * VocText,*JscText,*FFText,*EffiText;
	mpText * VocTextR,*JscTextR,*FFTextR,*EffiTextR, *RshText, * RsText;
	mpFXYVector* RLayer;

	void PlotEB();
	void PlotR();
	void PlotCarrier();
	void PlotJ();
	//Point *pt;
	ResultArray* resArrayP;
	int resIndex;
	double kT;int num; 
	/*vector<double> I;
	vector<double> V;*/
	void PlotIV(void);
	void renew(void);
	void PlotQEffi(void);
	void CalEffi(resultPar res);
	void PlotEF(void);
};

#endif
    // _RESDISPLAY_H_
