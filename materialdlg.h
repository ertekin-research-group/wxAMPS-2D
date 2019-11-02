/////////////////////////////////////////////////////////////////////////////
// Name:        materialdlg.h
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     20/02/2010 13:49:21
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MATERIALDLG_H_
#define _MATERIALDLG_H_


/*!
 * Includes
 */
#include "ambient.h"
#include "tinyxml.h"
#include <fstream>
#include <vector>
using namespace std;
////@begin includes
#include "wx/grid.h"
#include "wx/notebook.h"
#include "wx/valtext.h"
#include "wx/valgen.h"
#include "wx/statline.h"
////@end includes
struct defect
{
	int type,dist;
	wxString den, el, width, sigN, sigP;
	wxString name;
};
WX_DECLARE_LIST(defect, DefectList);
class layer{
public:
	wxString thickness,Eg,Epson,affi,Nc,Nv,un,up,n,p;
	wxString name;

	DefectList defList;
	bool BgShape, IsBT; wxString EgOpt;
	wxString CBTEnergy, CBTGo,CBTSigN,CBTSigP,VBTEnergy, VBTGo,VBTSigN,VBTSigP;
	wxString MGEnergy,GMGA,SigNMGA,SigPMGA,GMGD,SigNMGD,SigPMGD;
	wxString hEdge, hCenter;

	wxString absA,absB;
	vector<double> absWave;
	vector<double> alpha;
	layer();
	layer(layer& matPar);
	~layer();
	void copy(layer& matPar);
};

WX_DECLARE_LIST(layer, MyList);

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGrid;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MATERIALDLG 10014
#define ID_GRID 10016
#define ID_ADD 10021
#define ID_DELETE 10022
#define ID_BUTTON4 10009
#define ID_BUTTON5 10010
#define ID_SAVELYR 10000
#define ID_LOADLYR 10001
#define ID_MATERIALOK 10023
#define ID_MATERIALCANCEL 10024
#define ID_TEXTCTRL11 10026
#define ID_NOTEBOOK 10015
#define ID_PANEL 10017
#define ID_TEXTCTRL 10018
#define ID_TEXTCTRL2 10002
#define ID_TEXTCTRL3 10004
#define ID_TEXTCTRL4 10003
#define ID_TEXTCTRL5 10005
#define ID_TEXTCTRL6 10006
#define ID_TEXTCTRL7 10007
#define ID_TEXTCTRL8 10008
#define ID_TEXTCTRL9 10008
#define ID_PANEL1 10019
#define ID_LISTBOX1 10012
#define ID_BUTTON9 10044
#define ID_BUTTON10 10045
#define ID_CHOICE1 10054
#define ID_CHOICE 10046
#define ID_TEXTCTRL12 10047
#define ID_TEXTCTRL10 10025
#define ID_TEXTCTRL13 10027
#define ID_TEXTCTRL14 10028
#define ID_CHECKBOX1 10055
#define ID_TEXTCTRL23 10056
#define ID_BTTEXTCTRL1 10013
#define ID_TEXTCTRL16 10049
#define ID_TEXTCTRL15 10029
#define ID_TEXTCTRL17 10050
#define ID_TEXTCTRL18 10030
#define ID_TEXTCTRL19 10031
#define ID_TEXTCTRL20 10032
#define ID_TEXTCTRL21 10033
#define ID_CHECKBOX 10048
#define ID_TEXTCTRL22 10034
#define ID_TEXTCTRL24 10036
#define ID_TEXTCTRL25 10037
#define ID_TEXTCTRL26 10038
#define ID_TEXTCTRL27 10051
#define ID_TEXTCTRL28 10052
#define ID_TEXTCTRL29 10053
#define ID_PANEL2 10020
#define ID_TEXTCTRLA 10011
#define ID_TEXTCTRLB 10040
#define ID_BUTTON3 10043
#define ID_GRID1 10041
#define ID_BUTTON8 10042
#define ID_BUTTON7 10039
#define ID_PANEL4 10071
#define ID_TEXTCTRL41 10072
#define ID_TEXTCTRL42 10073
#define SYMBOL_MATERIALDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_MATERIALDLG_TITLE _("MaterialDlg")
#define SYMBOL_MATERIALDLG_IDNAME ID_MATERIALDLG
#define SYMBOL_MATERIALDLG_SIZE wxSize(400, 300)
#define SYMBOL_MATERIALDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * MaterialDlg class declaration
 */

class MaterialDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( MaterialDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MaterialDlg();
    MaterialDlg( wxWindow* parent, wxWindowID id = SYMBOL_MATERIALDLG_IDNAME, const wxString& caption = SYMBOL_MATERIALDLG_TITLE, const wxPoint& pos = SYMBOL_MATERIALDLG_POSITION, const wxSize& size = SYMBOL_MATERIALDLG_SIZE, long style = SYMBOL_MATERIALDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MATERIALDLG_IDNAME, const wxString& caption = SYMBOL_MATERIALDLG_TITLE, const wxPoint& pos = SYMBOL_MATERIALDLG_POSITION, const wxSize& size = SYMBOL_MATERIALDLG_SIZE, long style = SYMBOL_MATERIALDLG_STYLE );

    /// Destructor
    ~MaterialDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MaterialDlg event handler declarations

    /// wxEVT_INIT_DIALOG event handler for ID_MATERIALDLG
    void OnInitDialog( wxInitDialogEvent& event );

    /// wxEVT_GRID_CMD_SELECT_CELL event handler for ID_GRID
    void OnGridSelectCell( wxGridEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD
    void OnAddClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE
    void OnDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON4
    void OnSavematClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON5
    void OnLoadmatClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVELYR
    void OnSavelyrClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_LOADLYR
    void OnLoadlyrClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MATERIALOK
    void OnMaterialOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MATERIALCANCEL
    void OnMaterialcancelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1
    void OnDefListboxSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON9
    void OnDefectAddClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON10
    void OnDefectDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
    void OnDefTypeChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
    void OnDefDistChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
    void OnBTCheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
    void OnMGCheckClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
    void OnFromABClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON8
    void OnToxmlClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON7
    void OnFromFileClick( wxCommandEvent& event );

////@end MaterialDlg event handler declarations

////@begin MaterialDlg member function declarations

    int GetoldItem() const { return m_oldItem ; }
    void SetoldItem(int value) { m_oldItem = value ; }

    MyList GetLayerList() const { return LayerList ; }
    void SetLayerList(MyList value) { LayerList = value ; }

    int GetItem() const { return m_item ; }
    void SetItem(int value) { m_item = value ; }

    wxString GetLayerTitle() const { return curLyrTitle ; }
    void SetLayerTitle(wxString value) { curLyrTitle = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MaterialDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MaterialDlg member variables
    wxGrid* LayerListCtrl;
    wxButton* OkBtn;
    wxTextCtrl* layerTitle;
    wxScrolledWindow* defectPanel;
    wxListBox* DefListCtrl;
    wxChoice* DefTypeCtrl;
    wxChoice* DefDistCtrl;
    wxStaticText* DefWidthText;
    wxTextCtrl* DefWidthCtrl;
    wxCheckBox* BTCheckCtrl;
    wxTextCtrl* EgOptCtrl;
    wxTextCtrl* CBTECtrl;
    wxTextCtrl* VBTECtrl;
    wxTextCtrl* CBTGCtrl;
    wxTextCtrl* VBTGCtrl;
    wxTextCtrl* CBTSigNCtrl;
    wxTextCtrl* VBTSigNCtrl;
    wxTextCtrl* CBTSigPCtrl;
    wxTextCtrl* VBTSigPCtrl;
    wxCheckBox* MGCheckCtrl;
    wxTextCtrl* DefMgEnergyCtrl;
    wxTextCtrl* DefMgGmgACtrl;
    wxTextCtrl* DefMgGmgDCtrl;
    wxTextCtrl* DefMgSigNACtrl;
    wxTextCtrl* DefMgSigNDCtrl;
    wxTextCtrl* DefMgSigPACtrl;
    wxTextCtrl* DefMgSigPDCtrl;
    wxGrid* absGridCtrl;
    int m_oldItem;
    MyList LayerList;
    int m_item;
    wxString curLyrTitle;
////@end MaterialDlg member variables
	
	int def_oldItem;
	int def_item;
private:
	void ExportX();
	void SaveLyr(TiXmlDocument *myDocument, layer* cur);
	void LoadLyr(TiXmlElement *LyrElement,layer* curLyr);
public:
	void UpdateWinPar(layer* curLyr);
	void UpdateLyrPar(int);
	layer matPar;
	defect winDef;
	void UpdateAbsGrid(void);
	/*ambientPar ambPar;
	void ExportG(void);*/
	/*vector<double> xCoor;
	vector<double> interval;*/
	
private:
	void UpdateDefPar(int defItem);
	void UpdateDefPage();
};

#endif
    // _MATERIALDLG_H_
