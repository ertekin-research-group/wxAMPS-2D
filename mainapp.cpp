/////////////////////////////////////////////////////////////////////////////
// Name:        mainapp.cpp
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     16/02/2010 13:41:54
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

#include "mainapp.h"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( MainApp )
////@end implement app


/*
 * MainApp type definition
 */

IMPLEMENT_CLASS( MainApp, wxApp )


/*
 * MainApp event table definition
 */

BEGIN_EVENT_TABLE( MainApp, wxApp )

////@begin MainApp event table entries
////@end MainApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for MainApp
 */

MainApp::MainApp()
{
    Init();
}


/*
 * Member initialisation
 */

void MainApp::Init()
{
////@begin MainApp member initialisation
////@end MainApp member initialisation
}

/*
 * Initialisation for MainApp
 */

bool MainApp::OnInit()
{    
////@begin MainApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	MainAMPS* mainWindow = new MainAMPS(NULL);
	/* int returnValue = */ mainWindow->ShowModal();

	mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
////@end MainApp initialisation

    return true;
}


/*
 * Cleanup for MainApp
 */

int MainApp::OnExit()
{    
////@begin MainApp cleanup
	return wxApp::OnExit();
////@end MainApp cleanup
}

