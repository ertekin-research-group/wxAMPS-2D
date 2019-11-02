/////////////////////////////////////////////////////////////////////////////
// Name:        materialdlg.cpp
// Purpose:
// Author:      Liu
// Modified by:
// Created:     20/02/2010 13:49:21
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
#include "wx/imaglist.h"
////@end includes
#include "Constant.h"
#include "materialdlg.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <iomanip>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/wfstream.h>

#include "wx/listimpl.cpp"
WX_DEFINE_LIST(DefectList);
WX_DEFINE_LIST(MyList);
layer::layer()
{
	Epson=_("0");Eg=_("0");affi=_("0");Nc=_("0");Nv=_("0");un=_("0");up=_("0");n=_("0");p=_("0");
	name=_(" ");thickness=_("0");
	absA=_("0");absB=_("0");
	BgShape=0;IsBT=0;EgOpt=_("0");
	CBTEnergy=_("0.01");CBTGo=_("1e14");CBTSigN=_("1e-17");CBTSigP=_("1e-15");
	VBTEnergy=_("0.01");VBTGo=_("1e14");VBTSigN=_("1e-15");VBTSigP=_("1e-17");
	MGEnergy=_("0.56");GMGA=_("1e12");SigNMGA=_("1e-17");SigPMGA=_("1e-15");
	GMGD=_("1e12");SigNMGD=_("1e-15");SigPMGD=_("1e-17");
	hEdge=_("0.5");hCenter=_("20");

}

layer::layer(layer& matPar)//deep copy
{
	absA=matPar.absA; absB=matPar.absB;absWave=matPar.absWave;alpha=matPar.alpha;
	affi=matPar.affi;Eg=matPar.Eg;Epson=matPar.Epson;n=matPar.n;p=matPar.p;Nc=matPar.Nc;Nv=matPar.Nv;un=matPar.un;up=matPar.up;
	name=matPar.name;thickness=matPar.thickness;

	IsBT=matPar.IsBT; EgOpt=matPar.EgOpt;
	CBTEnergy=matPar.CBTEnergy;CBTGo=matPar.CBTGo;CBTSigN=matPar.CBTSigN;CBTSigP=matPar.CBTSigP;
	VBTEnergy=matPar.VBTEnergy;VBTGo=matPar.VBTGo;VBTSigN=matPar.VBTSigN;VBTSigP=matPar.VBTSigP;
	BgShape=matPar.BgShape;
	MGEnergy=matPar.MGEnergy;GMGA=matPar.GMGA;SigNMGA=matPar.SigNMGA;SigPMGA=matPar.SigPMGA;
	GMGD=matPar.GMGD;SigNMGD=matPar.SigNMGD;SigPMGD=matPar.SigPMGD;

	int defCnt=matPar.defList.GetCount();
	DefectList::compatibility_iterator node;
	for(int i=0;i<defCnt;i++)
	{
		node=matPar.defList.Item(i);
		defect *curDef=node->GetData();
		defect *newDef=new defect;
		*newDef=*curDef;
		defList.Append(newDef);
	}
}
layer::~layer()
{
	defList.DeleteContents(true);
	defList.Clear();
}
void layer::copy(layer& matPar)//deep copy
{
	absA=matPar.absA; absB=matPar.absB;absWave=matPar.absWave;alpha=matPar.alpha;
	affi=matPar.affi;Eg=matPar.Eg;Epson=matPar.Epson;n=matPar.n;p=matPar.p;Nc=matPar.Nc;Nv=matPar.Nv;un=matPar.un;up=matPar.up;
	name=matPar.name;thickness=matPar.thickness;

	BgShape=matPar.BgShape;IsBT=matPar.IsBT;EgOpt=matPar.EgOpt;
	CBTEnergy=matPar.CBTEnergy; CBTGo=matPar.CBTGo;CBTSigN=matPar.CBTSigN;CBTSigP=matPar.CBTSigP;
	VBTEnergy=matPar.VBTEnergy; VBTGo=matPar.VBTGo;VBTSigN=matPar.VBTSigN;VBTSigP=matPar.VBTSigP;
	MGEnergy=matPar.MGEnergy;
	GMGA=matPar.GMGA;SigNMGA=matPar.SigNMGA;SigPMGA=matPar.SigPMGA;
	GMGD=matPar.GMGD;SigNMGD=matPar.SigNMGD;SigPMGD=matPar.SigPMGD;

	hEdge=matPar.hEdge;hCenter=matPar.hCenter;

	int defCnt=matPar.defList.GetCount();
	DefectList::compatibility_iterator node;
	defList.DeleteContents(true);
	defList.Clear();
	for(int i=0;i<defCnt;i++)
	{
		node=matPar.defList.Item(i);
		defect *curDef=node->GetData();
		defect *newDef=new defect;
		*newDef=*curDef;
		defList.Append(newDef);
	}
}
////@begin XPM images
#include "absorption_equation.xpm"
////@end XPM images


/*
 * MaterialDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MaterialDlg, wxDialog )


/*
 * MaterialDlg event table definition
 */

BEGIN_EVENT_TABLE( MaterialDlg, wxDialog )

////@begin MaterialDlg event table entries
    EVT_INIT_DIALOG( MaterialDlg::OnInitDialog )

    EVT_GRID_CMD_SELECT_CELL( ID_GRID, MaterialDlg::OnGridSelectCell )

    EVT_BUTTON( ID_ADD, MaterialDlg::OnAddClick )

    EVT_BUTTON( ID_DELETE, MaterialDlg::OnDeleteClick )

    EVT_BUTTON( ID_BUTTON4, MaterialDlg::OnSavematClick )

    EVT_BUTTON( ID_BUTTON5, MaterialDlg::OnLoadmatClick )

    EVT_BUTTON( ID_SAVELYR, MaterialDlg::OnSavelyrClick )

    EVT_BUTTON( ID_LOADLYR, MaterialDlg::OnLoadlyrClick )

    EVT_BUTTON( ID_MATERIALOK, MaterialDlg::OnMaterialOkClick )

    EVT_BUTTON( ID_MATERIALCANCEL, MaterialDlg::OnMaterialcancelClick )

    EVT_LISTBOX( ID_LISTBOX1, MaterialDlg::OnDefListboxSelected )

    EVT_BUTTON( ID_BUTTON9, MaterialDlg::OnDefectAddClick )

    EVT_BUTTON( ID_BUTTON10, MaterialDlg::OnDefectDeleteClick )

    EVT_CHOICE( ID_CHOICE1, MaterialDlg::OnDefTypeChoiceSelected )

    EVT_CHOICE( ID_CHOICE, MaterialDlg::OnDefDistChoiceSelected )

    EVT_CHECKBOX( ID_CHECKBOX1, MaterialDlg::OnBTCheckboxClick )

    EVT_CHECKBOX( ID_CHECKBOX, MaterialDlg::OnMGCheckClick )

    EVT_BUTTON( ID_BUTTON3, MaterialDlg::OnFromABClick )

    EVT_BUTTON( ID_BUTTON8, MaterialDlg::OnToxmlClick )

    EVT_BUTTON( ID_BUTTON7, MaterialDlg::OnFromFileClick )

////@end MaterialDlg event table entries

END_EVENT_TABLE()


/*
 * MaterialDlg constructors
 */

MaterialDlg::MaterialDlg()
{
    Init();
}

MaterialDlg::MaterialDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * MaterialDlg creator
 */

bool MaterialDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MaterialDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end MaterialDlg creation
    return true;
}


/*
 * MaterialDlg destructor
 */

MaterialDlg::~MaterialDlg()
{
	LayerList.DeleteContents(true);
	LayerList.Clear();
}


/*
 * Member initialisation
 */

void MaterialDlg::Init()
{
////@begin MaterialDlg member initialisation
    m_oldItem = 0;
    m_item = 0;
    LayerListCtrl = NULL;
    OkBtn = NULL;
    layerTitle = NULL;
    defectPanel = NULL;
    DefListCtrl = NULL;
    DefTypeCtrl = NULL;
    DefDistCtrl = NULL;
    DefWidthText = NULL;
    DefWidthCtrl = NULL;
    BTCheckCtrl = NULL;
    EgOptCtrl = NULL;
    CBTECtrl = NULL;
    VBTECtrl = NULL;
    CBTGCtrl = NULL;
    VBTGCtrl = NULL;
    CBTSigNCtrl = NULL;
    VBTSigNCtrl = NULL;
    CBTSigPCtrl = NULL;
    VBTSigPCtrl = NULL;
    MGCheckCtrl = NULL;
    DefMgEnergyCtrl = NULL;
    DefMgGmgACtrl = NULL;
    DefMgGmgDCtrl = NULL;
    DefMgSigNACtrl = NULL;
    DefMgSigNDCtrl = NULL;
    DefMgSigPACtrl = NULL;
    DefMgSigPDCtrl = NULL;
    absGridCtrl = NULL;
////@end MaterialDlg member initialisation
	def_oldItem=0;def_item=0;
}


/*
 * Control creation for MaterialDlg
 */

void MaterialDlg::CreateControls()
{
////@begin MaterialDlg content construction
    MaterialDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_TOP|wxALL, 5);

    LayerListCtrl = new wxGrid( itemDialog1, ID_GRID, wxDefaultPosition, wxSize(200, 225), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    LayerListCtrl->SetDefaultColSize(80);
    LayerListCtrl->SetDefaultRowSize(25);
    LayerListCtrl->SetColLabelSize(25);
    LayerListCtrl->SetRowLabelSize(25);
    LayerListCtrl->CreateGrid(1, 2, wxGrid::wxGridSelectCells);
    itemBoxSizer3->Add(LayerListCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_ADD, _("ADD"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemDialog1, ID_BUTTON4, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON5, _("Load"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, ID_SAVELYR, _("Save Current Layer"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton12 = new wxButton( itemDialog1, ID_LOADLYR, _("Load Current Layer"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    OkBtn = new wxButton( itemDialog1, ID_MATERIALOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    OkBtn->SetDefault();
    itemBoxSizer3->Add(OkBtn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton14 = new wxButton( itemDialog1, ID_MATERIALCANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    layerTitle = new wxTextCtrl( itemDialog1, ID_TEXTCTRL11, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(layerTitle, 0, wxGROW|wxALL, 5);

    wxNotebook* itemNotebook17 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxSize(300, -1), wxBK_DEFAULT );

    wxPanel* itemPanel18 = new wxPanel( itemNotebook17, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemPanel18->SetSizer(itemBoxSizer19);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer20, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel18, wxID_STATIC, _("Permittivity"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl22 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemTextCtrl22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer23, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText24 = new wxStaticText( itemPanel18, wxID_STATIC, _("Eg"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer23->Add(itemStaticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl25 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemTextCtrl25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText26 = new wxStaticText( itemPanel18, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemStaticText26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer27, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText28 = new wxStaticText( itemPanel18, wxID_STATIC, _("Affinity"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer27->Add(itemStaticText28, 0, wxALIGN_TOP|wxALL, 5);

    wxTextCtrl* itemTextCtrl29 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemTextCtrl29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel18, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemStaticText30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer31, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel18, wxID_STATIC, _("Nc"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer31->Add(itemStaticText32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl33 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer31->Add(itemTextCtrl33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText34 = new wxStaticText( itemPanel18, wxID_STATIC, _("cm-3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer31->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer35, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel18, wxID_STATIC, _("Nv"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer35->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl37 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer35->Add(itemTextCtrl37, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel18, wxID_STATIC, _("cm-3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer35->Add(itemStaticText38, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer39, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel18, wxID_STATIC, _("un"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer39->Add(itemStaticText40, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl41 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer39->Add(itemTextCtrl41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText42 = new wxStaticText( itemPanel18, wxID_STATIC, _("cm2/v/s"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer39->Add(itemStaticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer43, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText44 = new wxStaticText( itemPanel18, wxID_STATIC, _("up"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer43->Add(itemStaticText44, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl45 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer43->Add(itemTextCtrl45, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText46 = new wxStaticText( itemPanel18, wxID_STATIC, _("cm2/v/s"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer43->Add(itemStaticText46, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer47, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText48 = new wxStaticText( itemPanel18, wxID_STATIC, _("Nd"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer47->Add(itemStaticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl49 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemTextCtrl49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText50 = new wxStaticText( itemPanel18, wxID_STATIC, _("cm-3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemStaticText50, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer51, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel18, wxID_STATIC, _("Na"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer51->Add(itemStaticText52, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl53 = new wxTextCtrl( itemPanel18, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer51->Add(itemTextCtrl53, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText54 = new wxStaticText( itemPanel18, wxID_STATIC, _("cm-3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer51->Add(itemStaticText54, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook17->AddPage(itemPanel18, _("Eletrical"));

    defectPanel = new wxScrolledWindow( itemNotebook17, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL|wxTAB_TRAVERSAL );
    defectPanel->SetScrollbars(1, 5, 0, 0);
    wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxVERTICAL);
    defectPanel->SetSizer(itemBoxSizer56);

    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer57, 0, wxALIGN_LEFT|wxALL, 5);
    wxArrayString DefListCtrlStrings;
    DefListCtrl = new wxListBox( defectPanel, ID_LISTBOX1, wxDefaultPosition, wxSize(120, -1), DefListCtrlStrings, wxLB_SINGLE );
    itemBoxSizer57->Add(DefListCtrl, 0, wxALIGN_BOTTOM|wxALL, 5);

    wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer57->Add(itemBoxSizer59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton60 = new wxButton( defectPanel, ID_BUTTON9, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer59->Add(itemButton60, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton61 = new wxButton( defectPanel, ID_BUTTON10, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer59->Add(itemButton61, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer62, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText63 = new wxStaticText( defectPanel, wxID_STATIC, _("Type"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemBoxSizer62->Add(itemStaticText63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString DefTypeCtrlStrings;
    DefTypeCtrlStrings.Add(_("Acceptor-like"));
    DefTypeCtrlStrings.Add(_("Donor-like"));
    DefTypeCtrl = new wxChoice( defectPanel, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, DefTypeCtrlStrings, 0 );
    DefTypeCtrl->SetStringSelection(_("Acceptor-like"));
    itemBoxSizer62->Add(DefTypeCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString DefDistCtrlStrings;
    DefDistCtrlStrings.Add(_("Discrete"));
    DefDistCtrlStrings.Add(_("Banded"));
    DefDistCtrlStrings.Add(_("Gaussian"));
    DefDistCtrl = new wxChoice( defectPanel, ID_CHOICE, wxDefaultPosition, wxDefaultSize, DefDistCtrlStrings, 0 );
    DefDistCtrl->SetStringSelection(_("Discrete"));
    itemBoxSizer62->Add(DefDistCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer66, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText67 = new wxStaticText( defectPanel, wxID_STATIC, _("Density"), wxDefaultPosition, wxSize(70, -1), 0 );
    itemBoxSizer66->Add(itemStaticText67, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl68 = new wxTextCtrl( defectPanel, ID_TEXTCTRL12, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemTextCtrl68, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText69 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-3"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemStaticText69, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer70 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer70, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText71 = new wxStaticText( defectPanel, wxID_STATIC, _("Energy Level"), wxDefaultPosition, wxSize(70, -1), 0 );
    itemBoxSizer70->Add(itemStaticText71, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl72 = new wxTextCtrl( defectPanel, ID_TEXTCTRL10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemTextCtrl72->SetHelpText(_("Positive down from Ec"));
    if (MaterialDlg::ShowToolTips())
        itemTextCtrl72->SetToolTip(_("Positive down from Ec"));
    itemBoxSizer70->Add(itemTextCtrl72, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText73 = new wxStaticText( defectPanel, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer70->Add(itemStaticText73, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer74, 0, wxALIGN_LEFT|wxALL, 5);
    DefWidthText = new wxStaticText( defectPanel, wxID_STATIC, _("Width"), wxDefaultPosition, wxSize(70, -1), 0 );
    itemBoxSizer74->Add(DefWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefWidthCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL13, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    DefWidthCtrl->Enable(false);
    itemBoxSizer74->Add(DefWidthCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText77 = new wxStaticText( defectPanel, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer74->Add(itemStaticText77, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer78, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText79 = new wxStaticText( defectPanel, wxID_STATIC, _("Capture N"), wxDefaultPosition, wxSize(70, -1), 0 );
    itemBoxSizer78->Add(itemStaticText79, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl80 = new wxTextCtrl( defectPanel, ID_TEXTCTRL11, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemTextCtrl80, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText81 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer78->Add(itemStaticText81, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer82, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText83 = new wxStaticText( defectPanel, wxID_STATIC, _("Capture P"), wxDefaultPosition, wxSize(70, -1), 0 );
    itemBoxSizer82->Add(itemStaticText83, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl84 = new wxTextCtrl( defectPanel, ID_TEXTCTRL14, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer82->Add(itemTextCtrl84, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText85 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer82->Add(itemStaticText85, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine86 = new wxStaticLine( defectPanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxBORDER_THEME );
    itemBoxSizer56->Add(itemStaticLine86, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer87, 0, wxALIGN_LEFT|wxALL, 5);
    BTCheckCtrl = new wxCheckBox( defectPanel, ID_CHECKBOX1, _("Band Tails"), wxDefaultPosition, wxDefaultSize, 0 );
    BTCheckCtrl->SetValue(false);
    itemBoxSizer87->Add(BTCheckCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText89 = new wxStaticText( defectPanel, wxID_STATIC, _("Eg optical"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText89->Show(false);
    itemBoxSizer87->Add(itemStaticText89, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    EgOptCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL23, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    EgOptCtrl->Show(false);
    itemBoxSizer87->Add(EgOptCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText91 = new wxStaticText( defectPanel, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText91->Show(false);
    itemBoxSizer87->Add(itemStaticText91, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer92, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText93 = new wxStaticText( defectPanel, wxID_STATIC, _("Conduction"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer92->Add(itemStaticText93, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText94 = new wxStaticText( defectPanel, wxID_STATIC, _("Valence"), wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer92->Add(itemStaticText94, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer95 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer95, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText96 = new wxStaticText( defectPanel, wxID_STATIC, _("E"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemStaticText96->SetHelpText(_("characteristic energy"));
    if (MaterialDlg::ShowToolTips())
        itemStaticText96->SetToolTip(_("characteristic energy"));
    itemBoxSizer95->Add(itemStaticText96, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    CBTECtrl = new wxTextCtrl( defectPanel, ID_BTTEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer95->Add(CBTECtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    VBTECtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL16, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer95->Add(VBTECtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText99 = new wxStaticText( defectPanel, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer95->Add(itemStaticText99, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer100, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText101 = new wxStaticText( defectPanel, wxID_STATIC, _("Go"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemBoxSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    CBTGCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL15, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer100->Add(CBTGCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    VBTGCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL17, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer100->Add(VBTGCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText104 = new wxStaticText( defectPanel, wxID_STATIC, _("1/cm3/ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemStaticText104, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer105, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText106 = new wxStaticText( defectPanel, wxID_STATIC, _("SigN"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemBoxSizer105->Add(itemStaticText106, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    CBTSigNCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL18, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer105->Add(CBTSigNCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    VBTSigNCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL19, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer105->Add(VBTSigNCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText109 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer105->Add(itemStaticText109, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer110, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText111 = new wxStaticText( defectPanel, wxID_STATIC, _("SigP"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemBoxSizer110->Add(itemStaticText111, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    CBTSigPCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL20, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer110->Add(CBTSigPCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    VBTSigPCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL21, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    itemBoxSizer110->Add(VBTSigPCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText114 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer110->Add(itemStaticText114, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer115, 0, wxALIGN_LEFT|wxALL, 5);
    MGCheckCtrl = new wxCheckBox( defectPanel, ID_CHECKBOX, _("Mid-gap States"), wxDefaultPosition, wxDefaultSize, 0 );
    MGCheckCtrl->SetValue(false);
    itemBoxSizer115->Add(MGCheckCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgEnergyCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL22, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgEnergyCtrl->Enable(false);
    itemBoxSizer115->Add(DefMgEnergyCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText118 = new wxStaticText( defectPanel, wxID_STATIC, _("ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer115->Add(itemStaticText118, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer119, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText120 = new wxStaticText( defectPanel, wxID_STATIC, _("Acceptor-like"), wxDefaultPosition, wxSize(70, -1), 0 );
    itemBoxSizer119->Add(itemStaticText120, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText121 = new wxStaticText( defectPanel, wxID_STATIC, _("donor-like"), wxDefaultPosition, wxSize(120, -1), 0 );
    itemBoxSizer119->Add(itemStaticText121, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer122 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer122, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText123 = new wxStaticText( defectPanel, wxID_STATIC, _("Gmg"), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer122->Add(itemStaticText123, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgGmgACtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL24, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgGmgACtrl->Enable(false);
    itemBoxSizer122->Add(DefMgGmgACtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgGmgDCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL25, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgGmgDCtrl->Enable(false);
    itemBoxSizer122->Add(DefMgGmgDCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText126 = new wxStaticText( defectPanel, wxID_STATIC, _("1/cm3/ev"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer122->Add(itemStaticText126, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer127, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText128 = new wxStaticText( defectPanel, wxID_STATIC, _("SigN"), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer127->Add(itemStaticText128, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgSigNACtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL26, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgSigNACtrl->Enable(false);
    itemBoxSizer127->Add(DefMgSigNACtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgSigNDCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL27, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgSigNDCtrl->Enable(false);
    itemBoxSizer127->Add(DefMgSigNDCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText131 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer127->Add(itemStaticText131, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer132 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer56->Add(itemBoxSizer132, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText133 = new wxStaticText( defectPanel, wxID_STATIC, _("SigP"), wxDefaultPosition, wxSize(25, -1), 0 );
    itemBoxSizer132->Add(itemStaticText133, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgSigPACtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL28, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgSigPACtrl->Enable(false);
    itemBoxSizer132->Add(DefMgSigPACtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DefMgSigPDCtrl = new wxTextCtrl( defectPanel, ID_TEXTCTRL29, wxEmptyString, wxDefaultPosition, wxSize(60, -1), 0 );
    DefMgSigPDCtrl->Enable(false);
    itemBoxSizer132->Add(DefMgSigPDCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText136 = new wxStaticText( defectPanel, wxID_STATIC, _("cm-2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer132->Add(itemStaticText136, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    defectPanel->FitInside();
    itemNotebook17->AddPage(defectPanel, _("Defect"));

    wxPanel* itemPanel137 = new wxPanel( itemNotebook17, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxVERTICAL);
    itemPanel137->SetSizer(itemBoxSizer138);

    wxStaticBitmap* itemStaticBitmap139 = new wxStaticBitmap( itemPanel137, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("absorption_equation.xpm")), wxDefaultPosition, wxSize(200, 55), 0 );
    itemBoxSizer138->Add(itemStaticBitmap139, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer138->Add(itemBoxSizer140, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText141 = new wxStaticText( itemPanel137, wxID_STATIC, _("A"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer140->Add(itemStaticText141, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl142 = new wxTextCtrl( itemPanel137, ID_TEXTCTRLA, wxEmptyString, wxDefaultPosition, wxSize(80, -1), 0 );
    itemBoxSizer140->Add(itemTextCtrl142, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText143 = new wxStaticText( itemPanel137, wxID_STATIC, _("cm-1  B"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer140->Add(itemStaticText143, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl144 = new wxTextCtrl( itemPanel137, ID_TEXTCTRLB, wxEmptyString, wxDefaultPosition, wxSize(80, -1), 0 );
    itemBoxSizer140->Add(itemTextCtrl144, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer145 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer138->Add(itemBoxSizer145, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText146 = new wxStaticText( itemPanel137, wxID_STATIC, _("Direct Bandgap"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer145->Add(itemStaticText146, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton147 = new wxButton( itemPanel137, ID_BUTTON3, _("From AB"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer145->Add(itemButton147, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    absGridCtrl = new wxGrid( itemPanel137, ID_GRID1, wxDefaultPosition, wxSize(240, 180), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    absGridCtrl->SetDefaultColSize(90);
    absGridCtrl->SetDefaultRowSize(25);
    absGridCtrl->SetColLabelSize(25);
    absGridCtrl->SetRowLabelSize(50);
    absGridCtrl->CreateGrid(1, 2, wxGrid::wxGridSelectCells);
    itemBoxSizer138->Add(absGridCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer138->Add(itemBoxSizer149, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton150 = new wxButton( itemPanel137, ID_BUTTON8, _("To XML"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemButton150, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton151 = new wxButton( itemPanel137, ID_BUTTON7, _("From File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemButton151, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook17->AddPage(itemPanel137, _("Optical"));

    wxPanel* itemPanel152 = new wxPanel( itemNotebook17, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxVERTICAL);
    itemPanel152->SetSizer(itemBoxSizer153);

    wxBoxSizer* itemBoxSizer154 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer153->Add(itemBoxSizer154, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText155 = new wxStaticText( itemPanel152, wxID_STATIC, _("Edge grid  "), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemStaticText155, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl156 = new wxTextCtrl( itemPanel152, ID_TEXTCTRL41, _(".5"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemTextCtrl156, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText157 = new wxStaticText( itemPanel152, wxID_STATIC, _("nm"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemStaticText157, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer158 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer153->Add(itemBoxSizer158, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText159 = new wxStaticText( itemPanel152, wxID_STATIC, _("Center grid"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer158->Add(itemStaticText159, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl160 = new wxTextCtrl( itemPanel152, ID_TEXTCTRL42, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer158->Add(itemTextCtrl160, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText161 = new wxStaticText( itemPanel152, wxID_STATIC, _("nm"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer158->Add(itemStaticText161, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook17->AddPage(itemPanel152, _("Advanced"));

    itemBoxSizer15->Add(itemNotebook17, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    // Set validators
    itemTextCtrl22->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.Epson) );
    itemTextCtrl25->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.Eg) );
    itemTextCtrl29->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.affi) );
    itemTextCtrl33->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.Nc) );
    itemTextCtrl37->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.Nv) );
    itemTextCtrl41->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.un) );
    itemTextCtrl45->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.up) );
    itemTextCtrl49->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.n) );
    itemTextCtrl53->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.p) );
    DefTypeCtrl->SetValidator( wxGenericValidator(& winDef.type) );
    DefDistCtrl->SetValidator( wxGenericValidator(& winDef.dist) );
    itemTextCtrl68->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & winDef.den) );
    itemTextCtrl72->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & winDef.el) );
    DefWidthCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & winDef.width) );
    itemTextCtrl80->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & winDef.sigN) );
    itemTextCtrl84->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & winDef.sigP) );
    BTCheckCtrl->SetValidator( wxGenericValidator(& matPar.IsBT) );
    EgOptCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.EgOpt) );
    CBTECtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.CBTEnergy) );
    VBTECtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.VBTEnergy) );
    CBTGCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.CBTGo) );
    VBTGCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.VBTGo) );
    CBTSigNCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.CBTSigN) );
    VBTSigNCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.VBTSigN) );
    CBTSigPCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.CBTSigP) );
    VBTSigPCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.VBTSigP) );
    MGCheckCtrl->SetValidator( wxGenericValidator(& matPar.BgShape) );
    DefMgEnergyCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.MGEnergy) );
    DefMgGmgACtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.GMGA) );
    DefMgGmgDCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.GMGD) );
    DefMgSigNACtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.SigNMGA) );
    DefMgSigNDCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.SigNMGD) );
    DefMgSigPACtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.SigPMGA) );
    DefMgSigPDCtrl->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.SigPMGD) );
    itemTextCtrl142->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.absA) );
    itemTextCtrl144->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.absB) );
    itemTextCtrl156->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.hEdge) );
    itemTextCtrl160->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & matPar.hCenter) );
////@end MaterialDlg content construction
}


/*
 * Should we show tooltips?
 */

bool MaterialDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MaterialDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MaterialDlg bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("absorption_equation.xpm"))
    {
        wxBitmap bitmap(absorption_equation_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end MaterialDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MaterialDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MaterialDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MaterialDlg icon retrieval
}


/*
 * wxEVT_INIT_DIALOG event handler for ID_MATERIALDLG
 */

void MaterialDlg::OnInitDialog( wxInitDialogEvent& event )
{
	int lyrCnt=LayerList.GetCount();
	LayerListCtrl->SetColLabelValue(0,_("name"));
	LayerListCtrl->SetColLabelValue(1,_("um"));
	if(0==lyrCnt)
	{
		layerTitle->SetValue(_("1-Add Layer"));
		LayerListCtrl->SetCellValue(0,0,_("Add layer"));
		LayerListCtrl->SetCellValue(0,1,_("0"));
		
		layer* curLyr=new layer;
		LayerList.Append(curLyr);
		UpdateWinPar(curLyr);
	}
	else//load layers
	{
		for(int i=0;i<lyrCnt;i++)
		{
			MyList::compatibility_iterator node;
			node=LayerList.Item(i);
			layer *curLyr=node->GetData();
			if(i==0)
			{	UpdateWinPar(curLyr);LayerListCtrl->SetGridCursor(0,0);}
			LayerListCtrl->AppendRows();
			LayerListCtrl->SetCellValue(i,0,curLyr->name);LayerListCtrl->SetCellValue(i,1,curLyr->thickness);
		}
		LayerListCtrl->DeleteRows(lyrCnt);

	}
	LayerListCtrl->DisableDragRowSize();//in case cursor is on the grid line, event.GetRow() will point to next but cursor still remains
	//-------defect-------------
	//defectPanel->SetScrollbars(10,10,100,100);
	//-------optical------------
	absGridCtrl->SetColLabelValue(0,_("nm"));
	absGridCtrl->SetColLabelValue(1,_("m-1"));

    event.Skip();
////@end wxEVT_INIT_DIALOG event handler for ID_MATERIALDLG in MaterialDlg.
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD
 */

void MaterialDlg::OnAddClick( wxCommandEvent& event )
{
	layer *newLyr=new layer;

	LayerList.Insert(m_item+1,newLyr);
	LayerListCtrl->InsertRows(m_item+1);
	LayerListCtrl->SetCellValue(m_item+1,1,_("0"));

	event.Skip();
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MATERIALAPPLY
 */

void MaterialDlg::OnMaterialOkClick( wxCommandEvent& event )
{
	UpdateLyrPar(LayerListCtrl->GetCursorRow());
	ExportX();
//	if(1==ambPar.m_light)
//		ExportG();
	EndModal(wxID_OK);

    event.Skip();

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE
 */

void MaterialDlg::OnDeleteClick( wxCommandEvent& event )
{
	if(wxYES==wxMessageBox(_("Delete this layer?"),_("AMPS"),wxYES_NO | wxCANCEL))
	{
		int cnt=LayerList.GetCount();
		MyList::compatibility_iterator node;
		node=LayerList.Item(m_item);
		layer *curLyr=node->GetData();
		if(cnt!=1)
		{
			//delete curLyr;
			LayerList.DeleteContents(true);
			LayerList.DeleteNode(node);
			LayerListCtrl->DeleteRows(m_item);
			m_item=0;m_oldItem=0;
			node=LayerList.Item(0);//return to beginning
			curLyr=node->GetData();			
			UpdateWinPar(curLyr);
			//LayerListCtrl->SetGridCursor(0,0);			
		}
		else
		{
			layer * newLyr=new layer;
			*curLyr=*newLyr;
			UpdateWinPar(curLyr);
			delete newLyr;
		}
	}
    event.Skip();
}




void MaterialDlg::UpdateLyrPar(int item)
{
	//TransferDataFromWindow(); included in UpdateDefPar();
	UpdateDefPar(def_oldItem);
	//--	update abs grid---
	matPar.absWave.clear(); matPar.alpha.clear();
	wxString str,str2; double value1,value2;
	int cnt=absGridCtrl->GetNumberRows();
	for(int i=0;i<cnt;i++)
	{
		str=absGridCtrl->GetCellValue(i,0);str2=absGridCtrl->GetCellValue(i,1);
		if(str.ToDouble(&value1)&&str2.ToDouble(&value2))
		{
			matPar.absWave.push_back(value1*1e-9);
			matPar.alpha.push_back(value2);
		}
	}
	//--------------------
	MyList::compatibility_iterator node;
	node=LayerList.Item(item);
	layer *curLyr=node->GetData();
	//(*curLyr)=matPar;
	curLyr->copy(matPar);
	curLyr->name=LayerListCtrl->GetCellValue(item,0);curLyr->thickness=LayerListCtrl->GetCellValue(item,1);
}


/*
 * wxEVT_GRID_CELL_LEFT_CLICK event handler for ID_GRID
 */

//void MaterialDlg::OnCellLeftClick( wxGridEvent& event )
//{
//
//	m_oldItem=LayerListCtrl->GetCursorRow();//return the old cursor position
//	UpdateLyrPar(m_oldItem);
//
//	{
//		m_item=event.GetRow();
//
//		MyList::compatibility_iterator node;//show the current layer parameters
//		node=LayerList.Item(m_item);
//		layer *curLyr=node->GetData();
//
//		UpdateWinPar(curLyr);
//	}
//    event.Skip();
//
//}


void MaterialDlg::ExportX()//unit nm
{
	wxFileOutputStream input(_("temp/x.csv")),input2(_("temp/x.tmp"));
	wxTextOutputStream file(input), file2(input2);
	if(input.IsOk()!=TRUE)
		wxMessageBox(_("error"));
	//file.precision(17);//17 digits for double
	double hEdge, hCenter;
	int lyrCnt=LayerList.GetCount();
	int number=1;double L,sum=0,thickness=0;
	//int *grids=new int[lyrCnt];double *q=new double[lyrCnt];
	vector<int>grids; vector<double> q;
	for(int num=0;num<lyrCnt;num++)
	{
		//layer * cur=LayerList.GetAt(m_layerList.FindIndex(num));
		MyList::compatibility_iterator node;//show the current layer parameters
		node=LayerList.Item(num);
		layer *curLyr=node->GetData();
		curLyr->hEdge.ToDouble(&hEdge);curLyr->hCenter.ToDouble(&hCenter);
		curLyr->thickness.ToDouble(&L);//unit um
		if(hEdge<hCenter)
		{
		q.push_back((L-2*hEdge*1e-3)/(L-2*hCenter*1e-3));
		grids.push_back(2*(int)(log(hCenter/hEdge)/log(q[num])+1));
		}
		else
		{
			q.push_back(1);
			grids.push_back(int(L*1e3/hCenter));
		}
		/*q[num]=(L-2*hEdge*1e-3)/(L-2*hCenter*1e-3);
		grids[num]=2*(int)(log(hCenter/hEdge)/log(q[num])+1);*/
		sum+=grids[num]+1;
		thickness+=L;
	}
	file<<_("total_points,")<<sum<<_(",total_thickness,")<<thickness<<endl;
	file2<<sum<<_(" ")<<thickness<<endl;
	file<<_("Number,X coordinates(um),X step(um),")
		<<_("Dielectri,Eg,Affinity,Nc,Nv,un,up,Nd,Na")<<endl;
	sum=0;
	for(int num=0;num<lyrCnt;num++)
	{
		MyList::compatibility_iterator node;//show the current layer parameters
		node=LayerList.Item(num);
		layer *curLyr=node->GetData();
		curLyr->thickness.ToDouble(&L);//unit um
		if(grids[num]>0)
		{
			//double *step=new double[grids[num]];
			//double *x=new double[grids[num]+1];
			vector<double>step;vector<double> x;
			double ratio;
			x.push_back(sum);//x[0]=sum;
			double LSum=0;
			for(int i=0;i<grids[num];i++)
			{
				if(i<grids[num]/2)
					step.push_back(hEdge*1e-3*pow(q[num],i));//step[i]=hEdge*1e-3*pow(q[num],i);
				else
					step.push_back(step[grids[num]-1-i]);//step[i]=step[grids[num]-1-i];
				LSum+=step[i];
			}
			ratio=L/LSum;
			LSum=0;
			for(int i=0;i<grids[num];i++)
			{
				step[i]=step[i]*ratio;
				LSum+=step[i];
				x.push_back(x[0]+LSum);//x[i+1]=x[0]+LSum;
				file<<number<<_(",")<<x[i]<<_(",")<<step[i]<<_(",")
					<<curLyr->Epson<<_(",")<<curLyr->Eg<<_(",")<<curLyr->affi<<_(",")
					<<curLyr->Nc<<_(",")<<curLyr->Nv<<_(",")<<curLyr->un<<_(",")
					<<curLyr->up<<_(",")<<curLyr->n<<_(",")<<curLyr->p<<endl;
				file2<<number++<<_(" ")<<x[i]*1e-4<<_(" ")<<step[i]*1e-4<<_(" ")//um to cm
					<<curLyr->Epson<<_(" ")<<curLyr->Eg<<_(" ")<<curLyr->affi<<_(" ")
					<<curLyr->Nc<<_(" ")<<curLyr->Nv<<_(" ")<<curLyr->un<<_(" ")
					<<curLyr->up<<_(" ")<<curLyr->n<<_(" ")<<curLyr->p<<endl;
			}
			file<<number<<_(",")<<x[grids[num]]<<_(",")<<0<<_(",")
				<<curLyr->Epson<<_(" ")<<curLyr->Eg<<_(" ")<<curLyr->affi<<_(" ")
				<<curLyr->Nc<<_(" ")<<curLyr->Nv<<_(" ")<<curLyr->un<<_(" ")
				<<curLyr->up<<_(" ")<<curLyr->n<<_(" ")<<curLyr->p<<endl;
			file2<<number++<<_(" ")<<x[grids[num]]*1e-4<<_(" ")<<0<<_(" ")
				<<curLyr->Epson<<_(" ")<<curLyr->Eg<<_(" ")<<curLyr->affi<<_(" ")
				<<curLyr->Nc<<_(" ")<<curLyr->Nv<<_(" ")<<curLyr->un<<_(" ")
				<<curLyr->up<<_(" ")<<curLyr->n<<_(" ")<<curLyr->p<<endl;
			sum+=L;

			//delete[] x,step;
		}
	}
	//delete[] grids,q;
//	file.close();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVELYR
 */

void MaterialDlg::OnSavelyrClick( wxCommandEvent& event )
{
    wxFileDialog fileDlg(NULL,_("Write data to file"),_(""),_(""),
		_("Layer Files(*.mat)|*.mat|All Files(*.*)|*.*"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT );

	if(wxID_OK==fileDlg.ShowModal())
	{
		UpdateLyrPar(m_item);
		MyList::compatibility_iterator node;
		node=LayerList.Item(m_item);
		layer *curLyr=node->GetData();

		TiXmlDocument *myDocument = new TiXmlDocument();
		TiXmlElement *LyrElement = new TiXmlElement("Material");
		myDocument->LinkEndChild(LyrElement);
		LyrElement->SetAttribute("name",curLyr->name.To8BitData());
		LyrElement->SetAttribute("thickness",curLyr->thickness.To8BitData());
		SaveLyr(myDocument, curLyr);
		myDocument->SaveFile(fileDlg.GetPath().To8BitData());
		delete myDocument;
	}
	OkBtn->SetFocus();
	event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SAVELYR in MaterialDlg.
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_LOADLYR
 */

void MaterialDlg::OnLoadlyrClick( wxCommandEvent& event )
{
    wxFileDialog fileDlg(NULL,_("Read data from file"),_(""),_(""),
		_("Layer Files(*.mat)|*.mat|All Files(*.*)|*.*"));

	if(wxID_OK==fileDlg.ShowModal())
	{
		MyList::compatibility_iterator node;
		node=LayerList.Item(m_item);
		layer *curLyr=node->GetData();
		TiXmlDocument *myDocument = new TiXmlDocument(fileDlg.GetPath().To8BitData());
		myDocument->LoadFile();

		TiXmlElement *RootElement = myDocument->RootElement();
		//TiXmlElement *LyrElement=RootElement->FirstChildElement();
		LoadLyr(RootElement,curLyr);

		UpdateWinPar(curLyr);

		delete myDocument;
	}
	event.Skip();
}


void MaterialDlg::SaveLyr(TiXmlDocument *myDocument, layer* curLyr)
{
	TiXmlElement *RootElement = myDocument->RootElement();
	wxString root(RootElement->Value(), wxConvUTF8);
	TiXmlElement *LyrElement;
	if(root!=_T("Device")	)	//save lyr or mat
		LyrElement=RootElement;
	else
	{
		LyrElement = new TiXmlElement("Material");
		RootElement->LinkEndChild(LyrElement);
		LyrElement->SetAttribute("name",curLyr->name.To8BitData());
		LyrElement->SetAttribute("thickness",curLyr->thickness.To8BitData());
	}
	TiXmlElement *ElecElement = new TiXmlElement("Electric");
	LyrElement->LinkEndChild(ElecElement);

	TiXmlElement* DielecElement=new TiXmlElement("Dielectric");			ElecElement->LinkEndChild(DielecElement);
	TiXmlElement* EgElement=new TiXmlElement("Eg");			ElecElement->LinkEndChild(EgElement);
	TiXmlElement *AffiElement=new TiXmlElement("Affinity");			ElecElement->LinkEndChild(AffiElement);
	TiXmlElement* NcElement=new TiXmlElement("Nc");			ElecElement->LinkEndChild(NcElement);
	TiXmlElement* NvElement=new TiXmlElement("Nv");			ElecElement->LinkEndChild(NvElement);
	TiXmlElement* UnElement=new TiXmlElement("mobility_n");			ElecElement->LinkEndChild(UnElement);
	TiXmlElement* UpElement=new TiXmlElement("mobility_p");			ElecElement->LinkEndChild(UpElement);
	TiXmlElement* NElement=new TiXmlElement("Nd");			ElecElement->LinkEndChild(NElement);
	TiXmlElement* PElement=new TiXmlElement("Na");			ElecElement->LinkEndChild(PElement);


	TiXmlText * DielecText=new TiXmlText(curLyr->Epson.To8BitData());			DielecElement->LinkEndChild(DielecText);
	TiXmlText * EgText=new TiXmlText(curLyr->Eg.To8BitData());	EgElement->LinkEndChild(EgText);
	TiXmlText * AffiText=new TiXmlText(curLyr->affi.To8BitData()); AffiElement->LinkEndChild(AffiText);
	TiXmlText * NcText=new TiXmlText(curLyr->Nc.To8BitData()); NcElement->LinkEndChild(NcText);
	TiXmlText * NvText=new TiXmlText(curLyr->Nv.To8BitData()); NvElement->LinkEndChild(NvText);
	TiXmlText * UnText=new TiXmlText(curLyr->un.To8BitData()); UnElement->LinkEndChild(UnText);
	TiXmlText * UpText=new TiXmlText(curLyr->up.To8BitData()); UpElement->LinkEndChild(UpText);
	TiXmlText * NText=new TiXmlText(curLyr->n.To8BitData()); NElement->LinkEndChild(NText);
	TiXmlText * PText=new TiXmlText(curLyr->p.To8BitData()); PElement->LinkEndChild(PText);
	/*---------Defect------------*/
	int defCnt=curLyr->defList.GetCount();
	wxString str;str.Printf(_("%d"),defCnt);
	TiXmlElement *DefElement = new TiXmlElement("Defect");
	DefElement->SetAttribute("number",str.To8BitData());	LyrElement->LinkEndChild(DefElement);
	DefectList::compatibility_iterator node;

	for(int i=0;i<defCnt;i++)
	{
		node=curLyr->defList.Item(i);
		defect * curDef=node->GetData();
		TiXmlElement *DElement = new TiXmlElement(curDef->name.To8BitData());DefElement->LinkEndChild(DElement);
		TiXmlElement *DTypeElement = new TiXmlElement("type");DElement->LinkEndChild(DTypeElement);
		TiXmlElement *DDistElement = new TiXmlElement("distribution");DElement->LinkEndChild(DDistElement);
		TiXmlElement *DDenElement = new TiXmlElement("density");DElement->LinkEndChild(DDenElement);
		TiXmlElement *DEnergyElement = new TiXmlElement("energy");DElement->LinkEndChild(DEnergyElement);
		TiXmlElement *DWidthElement = new TiXmlElement("width");DElement->LinkEndChild(DWidthElement);
		TiXmlElement *DSigNElement = new TiXmlElement("sigN");DElement->LinkEndChild(DSigNElement);
		TiXmlElement *DSigPElement = new TiXmlElement("sigP");DElement->LinkEndChild(DSigPElement);
		str.Printf(_("%d"),curDef->type);
		TiXmlText * TypeText=new TiXmlText(str.To8BitData());DTypeElement->LinkEndChild(TypeText);
		str.Printf(_("%d"),curDef->dist);
		TiXmlText * DistText=new TiXmlText(str.To8BitData());DDistElement->LinkEndChild(DistText);
		TiXmlText * DenText=new TiXmlText(curDef->den.To8BitData());DDenElement->LinkEndChild(DenText);
		TiXmlText * EnergyText=new TiXmlText(curDef->el.To8BitData());DEnergyElement->LinkEndChild(EnergyText);
		TiXmlText * WidthText=new TiXmlText(curDef->width.To8BitData());DWidthElement->LinkEndChild(WidthText);
		TiXmlText * SigNText=new TiXmlText(curDef->sigN.To8BitData());DSigNElement->LinkEndChild(SigNText);
		TiXmlText * SigPText=new TiXmlText(curDef->sigP.To8BitData());DSigPElement->LinkEndChild(SigPText);
	}

	TiXmlElement * BTElement=new TiXmlElement("Band_tail"); DefElement->LinkEndChild(BTElement);
	str.Printf(_("%d"),curLyr->IsBT);BTElement->SetAttribute("exist",str.To8BitData());
	if(curLyr->IsBT)
	{
		TiXmlElement * CBTElement=new TiXmlElement("Conduction"); BTElement->LinkEndChild(CBTElement);
		TiXmlElement * CEnergyElement=new TiXmlElement("Energy_level");CBTElement->LinkEndChild(CEnergyElement);
		TiXmlElement * CGElement=new TiXmlElement("G0");CBTElement->LinkEndChild(CGElement);
		TiXmlElement * CSigNElement=new TiXmlElement("Sigma_electron");CBTElement->LinkEndChild(CSigNElement);
		TiXmlElement * CSigPElement=new TiXmlElement("Sigma_hole");CBTElement->LinkEndChild(CSigPElement);
		TiXmlElement * VBTElement=new TiXmlElement("Valence"); BTElement->LinkEndChild(VBTElement);
		TiXmlElement * VEnergyElement=new TiXmlElement("Energy_level");VBTElement->LinkEndChild(VEnergyElement);
		TiXmlElement * VGElement=new TiXmlElement("G0");VBTElement->LinkEndChild(VGElement);
		TiXmlElement * VSigNElement=new TiXmlElement("Sigma_electron");VBTElement->LinkEndChild(VSigNElement);
		TiXmlElement * VSigPElement=new TiXmlElement("Sigma_hole");VBTElement->LinkEndChild(VSigPElement);
		TiXmlText * CEText=new TiXmlText(curLyr->CBTEnergy.To8BitData());CEnergyElement->LinkEndChild(CEText);
		TiXmlText * CGText=new TiXmlText(curLyr->CBTGo.To8BitData());CGElement->LinkEndChild(CGText);
		TiXmlText * CSigNText=new TiXmlText(curLyr->CBTSigN.To8BitData());CSigNElement->LinkEndChild(CSigNText);
		TiXmlText * CSigPText=new TiXmlText(curLyr->CBTSigP.To8BitData());CSigPElement->LinkEndChild(CSigPText);
		TiXmlText * VEText=new TiXmlText(curLyr->VBTEnergy.To8BitData());VEnergyElement->LinkEndChild(VEText);
		TiXmlText * VGText=new TiXmlText(curLyr->VBTGo.To8BitData());VGElement->LinkEndChild(VGText);
		TiXmlText * VSigNText=new TiXmlText(curLyr->VBTSigN.To8BitData());VSigNElement->LinkEndChild(VSigNText);
		TiXmlText * VSigPText=new TiXmlText(curLyr->VBTSigP.To8BitData());VSigPElement->LinkEndChild(VSigPText);

		TiXmlElement * MGElement=new TiXmlElement("Mid-gap_States"); BTElement->LinkEndChild(MGElement);
		str.Printf(_("%d"),curLyr->BgShape);MGElement->SetAttribute("exist",str.To8BitData());
		if(curLyr->BgShape)
		{
			TiXmlElement * MGEnergyElement=new TiXmlElement("Energy_level");MGElement->LinkEndChild(MGEnergyElement);
			TiXmlElement * MGGAElement=new TiXmlElement("GA0");MGElement->LinkEndChild(MGGAElement);
			TiXmlElement * MGSigNAElement=new TiXmlElement("Sigma_electronA");MGElement->LinkEndChild(MGSigNAElement);
			TiXmlElement * MGSigPAElement=new TiXmlElement("Sigma_holeA");MGElement->LinkEndChild(MGSigPAElement);
			TiXmlElement * MGGDElement=new TiXmlElement("GD0");MGElement->LinkEndChild(MGGDElement);
			TiXmlElement * MGSigNDElement=new TiXmlElement("Sigma_electronD");MGElement->LinkEndChild(MGSigNDElement);
			TiXmlElement * MGSigPDElement=new TiXmlElement("Sigma_holeD");MGElement->LinkEndChild(MGSigPDElement);
			TiXmlText * MGEText=new TiXmlText(curLyr->MGEnergy.To8BitData());MGEnergyElement->LinkEndChild(MGEText);
			TiXmlText * MGGAText=new TiXmlText(curLyr->GMGA.To8BitData());MGGAElement->LinkEndChild(MGGAText);
			TiXmlText * MGSigNAText=new TiXmlText(curLyr->SigNMGA.To8BitData());MGSigNAElement->LinkEndChild(MGSigNAText);
			TiXmlText * MGSigPAext=new TiXmlText(curLyr->SigPMGA.To8BitData());MGSigPAElement->LinkEndChild(MGSigPAext);
			TiXmlText * MGGDText=new TiXmlText(curLyr->GMGD.To8BitData());MGGDElement->LinkEndChild(MGGDText);
			TiXmlText * MGSigNDText=new TiXmlText(curLyr->SigNMGD.To8BitData());MGSigNDElement->LinkEndChild(MGSigNDText);
			TiXmlText * MGSigPDext=new TiXmlText(curLyr->SigPMGD.To8BitData());MGSigPDElement->LinkEndChild(MGSigPDext);
		}

	}
	/*---------Optical------------*/
	TiXmlElement *OptElement = new TiXmlElement("Optical");
	LyrElement->LinkEndChild(OptElement);
	TiXmlElement * AElement=new TiXmlElement("A");
	TiXmlText * AText=new TiXmlText(curLyr->absA.To8BitData());	AElement->LinkEndChild(AText);
	TiXmlElement * BElement=new TiXmlElement("B");
	TiXmlText * BText=new TiXmlText(curLyr->absB.To8BitData());	BElement->LinkEndChild(BText);
	TiXmlElement * AbsElement=new TiXmlElement("alpha");
	AbsElement->SetAttribute("unit","m-1");
	OptElement->LinkEndChild(AElement);OptElement->LinkEndChild(BElement);

	OptElement->LinkEndChild(AbsElement);
	int size=curLyr->alpha.size();

	for(int i=0;i<size;i++)
	{
		TiXmlElement *waveElement = new TiXmlElement("waveLength");
		str.Printf(_("%.0f"),curLyr->absWave[i]*1e9);
		waveElement->SetAttribute("nm",str.To8BitData());
		AbsElement->LinkEndChild(waveElement);
		str.Printf(_("%f"),curLyr->alpha[i]);
		TiXmlText * alphaText=new TiXmlText(str.To8BitData());
		waveElement->LinkEndChild(alphaText);
	}
	/*-----advance-------*/
	TiXmlElement *hEElement = new TiXmlElement("Edge");	LyrElement->LinkEndChild(hEElement);
	TiXmlText * hEText=new TiXmlText(curLyr->hEdge.To8BitData());	hEElement->LinkEndChild(hEText);
	TiXmlElement *hCElement = new TiXmlElement("Center");	LyrElement->LinkEndChild(hCElement);
	TiXmlText * hCText=new TiXmlText(curLyr->hCenter.To8BitData());	hCElement->LinkEndChild(hCText);

}

void MaterialDlg::LoadLyr(TiXmlElement *LyrElement,layer* curLyr)
{
	curLyr->name=wxString::FromAscii(LyrElement->FirstAttribute()->Value());
	curLyr->thickness=wxString::FromAscii(LyrElement->FirstAttribute()->Next()->Value());
	wxString temp=wxString::FromAscii(LyrElement->Value());
	if(temp!=_("Material"))
	{
		wxMessageBox(_("file format error!"));
		return;
	}
	TiXmlElement* ElecElement=LyrElement->FirstChildElement();
	TiXmlElement* DielecElement=ElecElement->FirstChildElement(); curLyr->Epson=wxString::FromAscii(DielecElement->FirstChild()->Value());
	TiXmlElement* EgElement=DielecElement->NextSiblingElement(); curLyr->Eg=wxString::FromAscii(EgElement->FirstChild()->Value());
	TiXmlElement *AffiElement=EgElement->NextSiblingElement(); curLyr->affi=wxString::FromAscii(AffiElement->FirstChild()->Value());
	TiXmlElement* NcElement=AffiElement->NextSiblingElement(); curLyr->Nc=wxString::FromAscii(NcElement->FirstChild()->Value());
	TiXmlElement* NvElement=NcElement->NextSiblingElement(); curLyr->Nv=wxString::FromAscii(NvElement->FirstChild()->Value());
	TiXmlElement* UnElement=NvElement->NextSiblingElement(); curLyr->un=wxString::FromAscii(UnElement->FirstChild()->Value());
	TiXmlElement* UpElement=UnElement->NextSiblingElement(); curLyr->up=wxString::FromAscii(UpElement->FirstChild()->Value());
	TiXmlElement* NElement=UpElement->NextSiblingElement(); curLyr->n=wxString::FromAscii(NElement->FirstChild()->Value());
	TiXmlElement* PElement=NElement->NextSiblingElement(); curLyr->p=wxString::FromAscii(PElement->FirstChild()->Value());
	/*---------Defect------------*/
	TiXmlElement *DefElement = ElecElement->NextSiblingElement();
	temp=wxString::FromAscii(DefElement->FirstAttribute()->Value());
	long defCnt; temp.ToLong(&defCnt);
	TiXmlElement *DElement=DefElement->FirstChildElement();
	curLyr->defList.DeleteContents(true);
	curLyr->defList.clear();
	for(int i=0;i<defCnt;i++)//ergodic
	{
		defect * curDef=new defect;		curDef->name=wxString::FromAscii(DElement->Value());
		TiXmlElement* Value=DElement->FirstChildElement(); long v;
		temp=wxString::FromAscii(Value->FirstChild()->Value());temp.ToLong(&v);curDef->type=(int)v;
		Value=Value->NextSiblingElement();temp=wxString::FromAscii(Value->FirstChild()->Value());temp.ToLong(&v);curDef->dist=(int)v;
		Value=Value->NextSiblingElement();curDef->den=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curDef->el=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curDef->width=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curDef->sigN=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curDef->sigP=wxString::FromAscii(Value->FirstChild()->Value());		
		curLyr->defList.Append(curDef);
		DElement=DElement->NextSiblingElement();
	}
	temp=wxString::FromAscii(DElement->FirstAttribute()->Value());//BTElement
	long IsBT;temp.ToLong(&IsBT);
	if(IsBT)
	{
		curLyr->IsBT=1;
		TiXmlElement* CBTElement=DElement->FirstChildElement();
		TiXmlElement* Value=CBTElement->FirstChildElement();curLyr->CBTEnergy=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curLyr->CBTGo=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curLyr->CBTSigN=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curLyr->CBTSigP=wxString::FromAscii(Value->FirstChild()->Value());
		TiXmlElement* VBTElement=CBTElement->NextSiblingElement();
		Value=VBTElement->FirstChildElement();curLyr->VBTEnergy=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curLyr->VBTGo=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curLyr->VBTSigN=wxString::FromAscii(Value->FirstChild()->Value());
		Value=Value->NextSiblingElement();curLyr->VBTSigP=wxString::FromAscii(Value->FirstChild()->Value());

		TiXmlElement* MGElement=VBTElement->NextSiblingElement();
		temp=wxString::FromAscii(MGElement->FirstAttribute()->Value());
		long BgShape;temp.ToLong(&BgShape);
		if(BgShape)
		{
			curLyr->BgShape=1;
			Value=MGElement->FirstChildElement();curLyr->MGEnergy=wxString::FromAscii(Value->FirstChild()->Value());
			Value=Value->NextSiblingElement();curLyr->GMGA=wxString::FromAscii(Value->FirstChild()->Value());
			Value=Value->NextSiblingElement();curLyr->SigNMGA=wxString::FromAscii(Value->FirstChild()->Value());
			Value=Value->NextSiblingElement();curLyr->SigPMGA=wxString::FromAscii(Value->FirstChild()->Value());
			Value=Value->NextSiblingElement();curLyr->GMGD=wxString::FromAscii(Value->FirstChild()->Value());
			Value=Value->NextSiblingElement();curLyr->SigNMGD=wxString::FromAscii(Value->FirstChild()->Value());
			Value=Value->NextSiblingElement();curLyr->SigPMGD=wxString::FromAscii(Value->FirstChild()->Value());
		}
	}
	//----------Optical-----------------
	double wave, abs;wxString str;
	TiXmlElement* OptElement=DefElement->NextSiblingElement();
	TiXmlElement * AElement=OptElement->FirstChildElement();
	curLyr->absA=wxString::FromAscii(AElement->FirstChild()->Value());
	TiXmlElement * BElement=AElement->NextSiblingElement();
	curLyr->absB=wxString::FromAscii(BElement->FirstChild()->Value());
	TiXmlElement * AbsElement=BElement->NextSiblingElement();
	curLyr->absWave.clear();curLyr->alpha.clear();
	for(TiXmlElement* waveElement=AbsElement->FirstChildElement();waveElement;waveElement=waveElement->NextSiblingElement())//ergodic
	{
		str=wxString::FromAscii(waveElement->FirstAttribute()->Value());
		str.ToDouble(&wave);
		curLyr->absWave.push_back(wave*1e-9);
		str=wxString::FromAscii(waveElement->FirstChild()->Value());
		str.ToDouble(&abs);
		curLyr->alpha.push_back(abs);
	}
	//-----------advance--------------
	TiXmlElement* h=OptElement->NextSiblingElement();
	curLyr->hEdge=wxString::FromAscii(h->FirstChild()->Value());
	TiXmlElement* hc=h->NextSiblingElement();
	curLyr->hCenter=wxString::FromAscii(hc->FirstChild()->Value());
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON5
 */
void MaterialDlg::OnSavematClick( wxCommandEvent& event )
{
	wxFileDialog fileDlg(NULL,_("Write data to file"),_(""),_(""),
		_("Material Files(*.dev)|*.dev|All Files(*.*)|*.*"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT );
	if(wxID_OK==fileDlg.ShowModal())
	{
		UpdateLyrPar(m_item);
		int lyrCnt=LayerList.GetCount();
		wxString cnt;cnt.Printf(_("%d"),lyrCnt);
		TiXmlDocument * myDocument=new TiXmlDocument;
		TiXmlElement * MatElement=new TiXmlElement("Device");
		myDocument->LinkEndChild(MatElement);
		MatElement->SetAttribute("layers",cnt.To8BitData());
		for(int i=0;i<lyrCnt;i++)
		{
			MyList::compatibility_iterator node;
			node=LayerList.Item(i);
			layer *curLyr=node->GetData();
			SaveLyr(myDocument,curLyr);
		}
		myDocument->SaveFile(fileDlg.GetPath().To8BitData());
		delete myDocument;
	}
    event.Skip();
}
void MaterialDlg::OnLoadmatClick( wxCommandEvent& event )
{
	wxFileDialog fileDlg(NULL,_("read data from file"),_(""),_(""),
		_("Device Files(*.dev)|*.dev|All Files(*.*)|*.*"));
	if(wxID_OK==fileDlg.ShowModal())
	{
		long lyrCnt=LayerList.GetCount();//delete old data

		LayerList.DeleteContents(true);
		LayerList.Clear();

		LayerListCtrl->DeleteRows(0,lyrCnt-1);

		TiXmlDocument *myDocument = new TiXmlDocument(fileDlg.GetPath().To8BitData());
		myDocument->LoadFile();
	//	if(!myDocument->Error())
	//	try
		{
			TiXmlElement *RootElement = myDocument->RootElement();
			wxString cnt=wxString::FromAscii(RootElement->FirstAttribute()->Value());
			TiXmlElement *LyrElement=RootElement->FirstChildElement();
			cnt.ToLong(&lyrCnt);
			for(int i=0;i<lyrCnt;i++)//load and add layer
			{
				layer *curLyr=new layer;
				LoadLyr(LyrElement,curLyr);
				LayerList.Append(curLyr);
				LayerListCtrl->AppendRows();
				LayerListCtrl->SetCellValue(i,0,curLyr->name);LayerListCtrl->SetCellValue(i,1,curLyr->thickness);
				LyrElement=LyrElement->NextSiblingElement();
			}
			LayerListCtrl->DeleteRows(lyrCnt);

			MyList::compatibility_iterator node;
			node=LayerList.Item(0);
			layer * initLyr=node->GetData();
			m_item=0;m_oldItem=0;
			UpdateWinPar(initLyr);LayerListCtrl->SetGridCursor(0,0);
		}
	/*	catch(char * str)
		{
			wxMessageBox(_(".*"));
		}*/

		delete myDocument;

	}
	OkBtn->SetFocus();
	event.Skip();
}


void MaterialDlg::UpdateWinPar(layer* curLyr)
{
	curLyrTitle.Printf(_("%d-"),m_item+1); curLyrTitle=curLyrTitle+curLyr->name;
	layerTitle->SetValue(curLyrTitle);
//	LayerListCtrl->SetCellValue(m_item,0,curLyr->name);LayerListCtrl->SetCellValue(m_item,1,curLyr->thickness);
	//matPar=*curLyr;//shallow copy
	matPar.copy(*curLyr);
	//----------------defect page display-------------
	int defCnt=curLyr->defList.GetCount();
	DefListCtrl->Clear();
	if(defCnt>0)
	{
		DefectList::compatibility_iterator node;
		for(int i=0;i<defCnt;i++)
		{
//			node=matPar.defList.Item(i);
//			defect *curDef=node->GetData();
			wxString str; str.Printf(_("%d-"),i+1);
			str=str+matPar.defList.Item(i)->GetData()->name;//curLyr->name;
			DefListCtrl->Append(str);
		}
		DefListCtrl->Select(0);
		DefTypeCtrl->Select(matPar.defList.GetFirst()->GetData()->type);
		DefDistCtrl->Select(matPar.defList.GetFirst()->GetData()->dist);
		winDef=*(matPar.defList.GetFirst()->GetData());
	}
	else
	{
		defect newDef;
		winDef=newDef;
	}
	def_oldItem=0;def_item=0;
	//---------------optical page---------------
	UpdateAbsGrid();
	TransferDataToWindow();
	//---------------post process-------
	UpdateDefPage();
	LayerListCtrl->SetCellValue(m_item,0,curLyr->name);LayerListCtrl->SetCellValue(m_item,1,curLyr->thickness);

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
 */

void MaterialDlg::OnFromABClick( wxCommandEvent& event )
{
	TransferDataFromWindow();
	wxString listData;double hv,Eg,A,B;
	matPar.Eg.ToDouble(&Eg);matPar.absA.ToDouble(&A);matPar.absB.ToDouble(&B);
	int cnt=absGridCtrl->GetNumberRows();//clear
	absGridCtrl->DeleteRows(0,cnt-1);
	matPar.absWave.clear();matPar.alpha.clear();

	for(int i=0;i<110;i++)
	{
		matPar.absWave.push_back((i*10+200)*1e-9);
		hv=H*C/matPar.absWave[i]/QE;
		listData.Printf(_("%d"),int(matPar.absWave[i]*1e9));
		absGridCtrl->SetCellValue(i,0,listData);
		if(hv<Eg)
			matPar.alpha.push_back(0);
		else
			matPar.alpha.push_back((1e2*A+B/hv)*sqrt(hv-Eg));
		listData.Printf(_("%f"),matPar.alpha[i]);
		absGridCtrl->SetCellValue(i,1,listData);
		absGridCtrl->InsertRows(i+1);

	}
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON7
 */

void MaterialDlg::OnFromFileClick( wxCommandEvent& event )
{
	wxFileDialog fileDlg(NULL,_("Read data from file"),_(""),_(""),
		_("XML Files(*.absx)|*.absx|Absorption Files(*.abs)|*.abs"));

	if(wxID_OK==fileDlg.ShowModal())
	{
		int cnt=absGridCtrl->GetNumberRows();//clear
		absGridCtrl->DeleteRows(0,cnt-1);
		matPar.absWave.clear();matPar.alpha.clear();
		wxString str,listData;	wxArrayString arrStr;

		if(fileDlg.GetFilterIndex()==1)//common abs file
		{
			wxFileInputStream input(fileDlg.GetPath());
			wxTextInputStream file(input);
			double wave,alpha;

			while(!input.Eof())
			{
				str=file.ReadLine();
				if(str.Find('>')!=0&&str.Find('#')!=0&&str.Find('/')!=0&&str.Find('l')!=0)
				{
					arrStr=wxStringTokenize(str);
					if(arrStr.GetCount()!=0)
					{
						arrStr[0].ToDouble(&wave);
						arrStr[1].ToDouble(&alpha);
						matPar.absWave.push_back(wave*1e-9);
						matPar.alpha.push_back(alpha);
					}
				}
			}
		}
		else//xml file
		{
			TiXmlDocument *myDocument = new TiXmlDocument(fileDlg.GetPath().To8BitData());
			myDocument->LoadFile();
			double wave, abs;wxString str;
			TiXmlElement* RootElement=myDocument->RootElement();

			for(TiXmlElement* waveElement=RootElement->FirstChildElement();waveElement;waveElement=waveElement->NextSiblingElement())//ergodic
			{
				str=wxString::FromAscii(waveElement->FirstAttribute()->Value());
				str.ToDouble(&wave);
				matPar.absWave.push_back(wave*1e-9);
				str=wxString::FromAscii(waveElement->FirstChild()->Value());
				str.ToDouble(&abs);
				matPar.alpha.push_back(abs);
			}
			delete myDocument;
		}

		int size=matPar.alpha.size();
		for(int i=0;i<size;i++)
		{
			listData.Printf(_("%d"),int(matPar.absWave[i]*1e9));
			absGridCtrl->SetCellValue(i,0,listData);
			listData.Printf(_("%f"),matPar.alpha[i]);
			absGridCtrl->SetCellValue(i,1,listData);
			absGridCtrl->InsertRows(i+1);
		}
	}
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON7 in MaterialDlg.
}


void MaterialDlg::UpdateAbsGrid(void)
{
	int cnt=absGridCtrl->GetNumberRows();//clear
	absGridCtrl->DeleteRows(0,cnt-1);

	int size=matPar.alpha.size();
	wxString listData;
	if(size>0)
	{
		for(int i=0;i<size;i++)
		{
			listData.Printf(_("%d"),int(matPar.absWave[i]*1e9));
			absGridCtrl->SetCellValue(i,0,listData);
			listData.Printf(_("%.4e"),matPar.alpha[i]);
			absGridCtrl->SetCellValue(i,1,listData);
			absGridCtrl->InsertRows(i+1);
		}	
		absGridCtrl->DeleteRows(size,1);
	}
	else
	{
		absGridCtrl->SetCellValue(0,0,_("0"));
		absGridCtrl->SetCellValue(0,1,_("0"));
	}
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON8
 */

void MaterialDlg::OnToxmlClick( wxCommandEvent& event )
{
	wxFileDialog fileDlg(NULL,_("Write data to file"),_(""),_(""),
		_("XML Files(*.absx)|*.absx"),wxFD_SAVE);

	if(wxID_OK==fileDlg.ShowModal())
	{
		TiXmlDocument *myDocument = new TiXmlDocument();
		TiXmlElement * AbsElement=new TiXmlElement("alpha");
		AbsElement->SetAttribute("unit","m-1");
		myDocument->LinkEndChild(AbsElement);
		int size=matPar.alpha.size();
		wxString str;
		for(int i=0;i<size;i++)
		{
			TiXmlElement *waveElement = new TiXmlElement("waveLength");
			str.Printf(_("%.0f"),matPar.absWave[i]*1e9);
			waveElement->SetAttribute("nm",str.To8BitData());
			AbsElement->LinkEndChild(waveElement);
			str.Printf(_("%f"),matPar.alpha[i]);
			TiXmlText * alphaText=new TiXmlText(str.To8BitData());
			waveElement->LinkEndChild(alphaText);
		}
		myDocument->SaveFile(fileDlg.GetPath().To8BitData());
		delete myDocument;
	}
    event.Skip();
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MATERIALCANCEL
 */

void MaterialDlg::OnMaterialcancelClick( wxCommandEvent& event )
{
	EndModal(wxID_CANCEL);
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON9
 */

void MaterialDlg::OnDefectAddClick( wxCommandEvent& event )
{
	UpdateDefPar(def_oldItem);
	defect *curDef=new defect;
	curDef->den=_("0");curDef->el=_("0");curDef->sigN=_("0");curDef->sigP=_("0");curDef->type=0;curDef->dist=0;curDef->width=_("0");
	curDef->name=_("AcceptorLike");
	winDef=*curDef;

	matPar.defList.Append(curDef);
	wxString str; str.Printf(_("%d-"),matPar.defList.GetCount());str=str+curDef->name;
	DefListCtrl->Append(str);
	def_item=matPar.defList.GetCount()-1;
	DefListCtrl->Select(def_item);//(matPar.defList.GetCount()-1);
	def_oldItem=def_item;
	TransferDataToWindow();
	UpdateDefPage();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON10
 */

void MaterialDlg::OnDefectDeleteClick( wxCommandEvent& event )
{
	if(wxYES==wxMessageBox(_("Delete this defect?"),_("Defect"),wxYES_NO | wxCANCEL))
	{
		DefectList::compatibility_iterator node;
		node=matPar.defList.Item(def_item);
		matPar.defList.DeleteContents(true);
		matPar.defList.DeleteNode(node);

		int defCnt=matPar.defList.GetCount();
		DefListCtrl->Clear();
		if(defCnt>0)
		{
			DefectList::compatibility_iterator node;
			wxString str;
			for(int i=0;i<defCnt;i++)
			{
				node=matPar.defList.Item(i);
				defect *curDef=node->GetData();
				str.Printf(_("%d-"),i+1);str=str+curDef->name;
				DefListCtrl->Append(str);
			}
			DefListCtrl->SetSelection(0);
			def_item=0;def_oldItem=0;
			winDef=*(matPar.defList.GetFirst()->GetData());
		}
		else
		{
			defect newDef;
			winDef=newDef;
		}

		TransferDataToWindow();
	}
	event.Skip();
}


/*
* wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1
*/

void MaterialDlg::OnDefListboxSelected( wxCommandEvent& event )
{
	UpdateDefPar(def_oldItem);
	def_item=DefListCtrl->GetSelection();
	//UpdateWinDef(curDef);
	DefectList::compatibility_iterator node;
	node=matPar.defList.Item(def_item);
	defect * curDef=node->GetData();
	winDef=*curDef;
	TransferDataToWindow();
	UpdateDefPage();

	def_oldItem=def_item;
	event.Skip();
}


void MaterialDlg::UpdateDefPar(int item)
{
	TransferDataFromWindow();
	if(matPar.defList.GetCount()>0)
	{
		DefectList::compatibility_iterator node;
		node=matPar.defList.Item(item);
		defect * curDef=node->GetData();
		*curDef=winDef;
	}
}

void MaterialDlg::UpdateDefPage()
{

	switch(DefDistCtrl->GetSelection())
	{
	case 0:
		DefWidthText->SetLabel(_("Width"));
		DefWidthCtrl->Enable(0);
		break;
	case 1:
		DefWidthText->SetLabel(_("Width"));
		DefWidthCtrl->Enable(1);
		break;
	case 2:
		DefWidthText->SetLabel(_("Deviation"));
		DefWidthCtrl->Enable(1);
		break;
	}
	bool DefBT=BTCheckCtrl->GetValue();MGCheckCtrl->Enable(DefBT);
	if(DefBT)
	{
		BTCheckCtrl->SetForegroundColour(wxColour(0,0,255));
		bool DefMg=matPar.BgShape;
		if(DefMg)
			MGCheckCtrl->SetForegroundColour(wxColour(0,0,255));
		else
			MGCheckCtrl->SetForegroundColour(wxColour(0,0,0));;
		DefMgEnergyCtrl->Enable(DefMg);
		DefMgGmgACtrl->Enable(DefMg);DefMgSigNACtrl->Enable(DefMg);DefMgSigPACtrl->Enable(DefMg);
		DefMgGmgDCtrl->Enable(DefMg);DefMgSigNDCtrl->Enable(DefMg);DefMgSigPDCtrl->Enable(DefMg);
	}
	else
		BTCheckCtrl->SetForegroundColour(wxColour(0,0,0));
	EgOptCtrl->Enable(DefBT);
	CBTECtrl->Enable(DefBT);VBTECtrl->Enable(DefBT);
	CBTGCtrl->Enable(DefBT);CBTSigNCtrl->Enable(DefBT);CBTSigPCtrl->Enable(DefBT);
	VBTGCtrl->Enable(DefBT);VBTSigNCtrl->Enable(DefBT);VBTSigPCtrl->Enable(DefBT);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
 */

void MaterialDlg::OnDefTypeChoiceSelected( wxCommandEvent& event )
{
	switch(DefTypeCtrl->GetSelection())
	{
	case 0:
		winDef.name=_("AcceptorLike");
		break;
	case 1:
		winDef.name=_("DonorLike");
		break;
	case 2:
		winDef.name=_("Amphoteric");
		break;
	}
	if(DefDistCtrl->GetSelection()==2)
	{
		winDef.name+=_("_G");
	}
	wxString str; str.Printf(_("%d-"),def_item+1);str=str+winDef.name;
	if(matPar.defList.GetCount()!=0)
		DefListCtrl->SetString(def_item,str);
    event.Skip();
}

/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void MaterialDlg::OnDefDistChoiceSelected( wxCommandEvent& event )
{
	switch(DefTypeCtrl->GetSelection())
	{
	case 0:
		winDef.name=_("AcceptorLike");
		break;
	case 1:
		winDef.name=_("DonorLike");
		break;
	case 2:
		winDef.name=_("Amphoteric");
		break;
	}
	switch(DefDistCtrl->GetSelection())
	{
	case 0:
		DefWidthText->SetLabel(_("Width"));
		DefWidthCtrl->Enable(0);
		break;
	case 1:
		DefWidthText->SetLabel(_("Width"));
		DefWidthCtrl->Enable(1);
		break;
	case 2:
		DefWidthText->SetLabel(_("Deviation"));
		DefWidthCtrl->Enable(1);
		winDef.name+=_("_G");
		break;
	}
	wxString str; str.Printf(_("%d-"),def_item+1);str=str+winDef.name;
	if(matPar.defList.GetCount()!=0)
		DefListCtrl->SetString(def_item,str);
    event.Skip();
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void MaterialDlg::OnMGCheckClick( wxCommandEvent& event )
{
	bool DefMg=MGCheckCtrl->GetValue();
	if(DefMg)
		MGCheckCtrl->SetForegroundColour(wxColour(0,0,255));
	else
		MGCheckCtrl->SetForegroundColour(wxColour(0,0,0));;
	DefMgEnergyCtrl->Enable(DefMg);
	DefMgGmgACtrl->Enable(DefMg);DefMgSigNACtrl->Enable(DefMg);DefMgSigPACtrl->Enable(DefMg);
	DefMgGmgDCtrl->Enable(DefMg);DefMgSigNDCtrl->Enable(DefMg);DefMgSigPDCtrl->Enable(DefMg);

	matPar.BgShape=DefMg;//index;
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX in MaterialDlg.
}



/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
 */

void MaterialDlg::OnBTCheckboxClick( wxCommandEvent& event )
{
	bool DefBT=BTCheckCtrl->GetValue();
	if(DefBT)
		BTCheckCtrl->SetForegroundColour(wxColour(0,0,255));
	else
		BTCheckCtrl->SetForegroundColour(wxColour(0,0,0));
	EgOptCtrl->Enable(DefBT);
	CBTECtrl->Enable(DefBT);VBTECtrl->Enable(DefBT);
	CBTGCtrl->Enable(DefBT);CBTSigNCtrl->Enable(DefBT);CBTSigPCtrl->Enable(DefBT);
	VBTGCtrl->Enable(DefBT);VBTSigNCtrl->Enable(DefBT);VBTSigPCtrl->Enable(DefBT);
	MGCheckCtrl->Enable(DefBT);

	matPar.IsBT=DefBT;
    event.Skip();
}


/*
 * wxEVT_GRID_CMD_SELECT_CELL event handler for ID_GRID
 */

void MaterialDlg::OnGridSelectCell( wxGridEvent& event )
{
	//m_oldItem=LayerListCtrl->GetGridCursorRow();//->GetCursorRow();//return the old cursor position
	//if(m_oldItem!=-1)
	{
		UpdateLyrPar(m_oldItem);

		{
			m_item=event.GetRow();

			MyList::compatibility_iterator node;//show the current layer parameters
			node=LayerList.Item(m_item);
			layer *curLyr=node->GetData();

			UpdateWinPar(curLyr);
		}
		m_oldItem=m_item;
	}
    event.Skip();
////@end wxEVT_GRID_CMD_SELECT_CELL event handler for ID_GRID in MaterialDlg. 
}

