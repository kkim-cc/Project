
// GASA3.h : main header file for the GASA3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGASA3App:
// See GASA3.cpp for the implementation of this class
//

class CGASA3App : public CWinApp
{
public:
	CGASA3App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGASA3App theApp;
