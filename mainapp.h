/////////////////////////////////////////////////////////////////////////////
// Name:        mainapp.h
// Purpose:     
// Author:      Liu
// Modified by: 
// Created:     16/02/2010 13:41:54
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAINAPP_H_
#define _MAINAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "mainamps.h"
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
////@end control identifiers

/*!
 * MainApp class declaration
 */

class MainApp: public wxApp
{    
    DECLARE_CLASS( MainApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    MainApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin MainApp event handler declarations

////@end MainApp event handler declarations

////@begin MainApp member function declarations

////@end MainApp member function declarations

////@begin MainApp member variables
////@end MainApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(MainApp)
////@end declare app

#endif
    // _MAINAPP_H_
