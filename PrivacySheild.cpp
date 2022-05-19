// PrivacySheild.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PrivacySheild.h"
#include "MainWnd.h"
#include "common.h"
#include "MessageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



/////////////////////////////////////////////////////////////////////////////
// CPrivacySheildApp

BEGIN_MESSAGE_MAP(CPrivacySheildApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrivacySheildApp construction

CPrivacySheildApp::CPrivacySheildApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bCallLibUIDKMsgMap = TRUE;
	m_pMainFrame = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrivacySheildApp object

CPrivacySheildApp theApp;

//////////////////////////////////////////////////////////////////////////
// public

BOOL CPrivacySheildApp::IsCallLibUIDKMsgMap()
{
	return m_bCallLibUIDKMsgMap;
}

/////////////////////////////////////////////////////////////////////////////
// CPrivacySheildApp initialization

BOOL CPrivacySheildApp::InitInstance()
{
	//get module path//
	GetModulePath();

	//get device id//
	CString hwinfo = "";
	CString strDeviceID = "";
	GetDeviceID(strDeviceID, hwinfo);
	g_strDeviceID = strDeviceID;

	//check registry//
	if(!CheckRegistry())
		return FALSE;

	//load db//
	if(OpenDB() < 0)
	{
		return FALSE;
	}	

	//set font//
	//AddFontResource(g_strModulePath + L"\\Roboto-Black.ttf");
	//AddFontResource(g_strModulePath + L"\\CerebriSansPro-Heavy.ttf");	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Change the path under which your skin files are stored.	
#ifdef DISTRIBUTE
	int nRet = CUIMgr::SetUIPath(_T("the.xui"), _T("Resource.h"), _T("LIBUIDKSKIN"));
#else
	int nRet = CUIMgr::SetUIPath(_T(".\\Skins\\PrivacySheild\\the.xui"));
#endif

#ifdef TEST
	nRet = CUIMgr::SetUIPath(_T("the.xui"), _T("Resource.h"), _T("LIBUIDKSKIN"));
#endif

	if (nRet != 0)
	{
		AfxMessageBox(_T("Failed to set the ui path or the ui path isn't exist!"));
		return FALSE;
	}

	if (!PathFileExists(g_strModulePath + "\\Action.dll"))
	{
		AfxMessageBox(_T("Failed to load the Action.dll!"));
		return FALSE;
	}

	//check expiry//
	CString strMsg;
	int nCheckedExpiry = CheckExpiryDays(strMsg);
	if(!strMsg.IsEmpty())
	{
		CMessageWnd wnd;
		wnd.SetMessageWndInfo(CMessageWnd::MW_BUYOK, "WARNING", strMsg);
		if (wnd.DoModal() == IDOK)
		{
			ShellExecute(NULL, "open", URL_FOR_LICENSE, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else if (strMsg.IsEmpty() && nCheckedExpiry < 0)
	{
		ExitInstance();
		return FALSE;
	}

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.
	m_pMainFrame = new CMainWnd;
	m_pMainWnd = m_pMainFrame;

	CoInitialize(NULL);
	BOOL bRet = m_pMainFrame->LoadFrame(IDR_MAINFRAME, WS_POPUP | WS_MINIMIZEBOX|WS_MAXIMIZEBOX | WS_VISIBLE, NULL);

	if (!bRet)
	{
		DWORD dwLastError = m_pMainFrame->GetLastCreateError();
		switch (dwLastError)
		{
		case E_LOADBMPB: // 1 failed to load the base image
		case E_LOADBMPN: // 2 failed to load the background image of normal status(end by _N.bmp)
		case E_LOADBMPH: // 3 failed to laod the background image of highlight status(end by _H.bmp)
		case E_LOADBMPD: // 4 failed to load the background image of disable status(end by _D.bmp)
		case E_LOADBMPS: // 5 failed to load the background image of selected status(end by _S.bmp)
		case E_LOADBMPC: // 6 failed to load the background image of checked status(end by _C.bmp)
			TRACE(_T("Failed to load background image"));
			break;
		case E_ATTACHBMPB: // 50 failed to attach the HBITMAP of base image to CBitmap object
		case E_ATTACHBMPN: // 51 failed to attach the HBITMAP of normal image to CBitmap object
		case E_ATTACHBMPH: // 52 failed to attach the HBITMAP of highlight image to CBitmap object
		case E_ATTACHBMPD: // 53 failed to attach the HBITMAP of disable image to CBitmap object
		case E_ATTACHBMPS: // 54 failed to attach the HBITMAP of selected image to CBitmap object
		case E_ATTACHBMPC: // 55 failed to attach the HBITMAP of checked image to CBitmap object
		case E_ADJUSTCHILD:// 56 failed to adjust the child controls while change the skin
			TRACE(_T("Failed to attach bitmap to CBitmao object"));
			break;
		case E_CREATECHILD:// 110 failed to create the child window
			TRACE(_T("Failed to create child windows"));
			break;
		default:
			break;
		}

		return FALSE;
	}

	m_pMainFrame->CenterWindow();

	// The one and only window has been initialized, so show and update it.

	m_pMainFrame->UpdateWindow();

	return TRUE;
}

int CPrivacySheildApp::ExitInstance()
{
	if (m_pMainFrame != NULL)
	{
		SafeDelete(m_pMainFrame);
	}

	CUIMgr::ReleaseSkin();

	// TODO: Add your specialized code here and/or call the base class
	//RemoveFontResource(g_strModulePath + L"\\Roboto-Black.ttf");
	//RemoveFontResource(g_strModulePath + L"\\CerebriSansPro-Heavy.ttf");

	CoUninitialize();

	//close db//
	CloseDB();

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CPrivacySheildApp message handlers

