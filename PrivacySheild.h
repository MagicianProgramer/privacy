// PrivacySheild.h : main header file for the PRIVACYSHEILD application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CMainFrame;
class CMainWnd;

/////////////////////////////////////////////////////////////////////////////
// CPrivacySheildApp:
// See PrivacySheild.cpp for the implementation of this class
//

class CPrivacySheildApp : public CWinApp
{
public:
	CPrivacySheildApp();

public:
	BOOL IsCallLibUIDKMsgMap();

// Overrides
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bCallLibUIDKMsgMap;
	CUIWnd *m_pMainFrame;
};

