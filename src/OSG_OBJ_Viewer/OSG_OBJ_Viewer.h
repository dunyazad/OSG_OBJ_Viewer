
// OSG_OBJ_Viewer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// OpenSceneGraph_OBJ_ViewerApp:
// See OSG_OBJ_Viewer.cpp for the implementation of this class
//

class OpenSceneGraph_OBJ_ViewerApp : public CWinApp
{
public:
	OpenSceneGraph_OBJ_ViewerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern OpenSceneGraph_OBJ_ViewerApp theApp;
