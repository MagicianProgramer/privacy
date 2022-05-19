// MessageWnd1.cpp : implementation of the CMessageWnd class
//

#include "stdafx.h"
#include "MessageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMessageWnd

BEGIN_MESSAGE_MAP(CMessageWnd, CUIWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_YES, OnBtnYes)
	ON_BN_CLICKED(IDC_BTN_NO, OnBtnNo)
END_MESSAGE_MAP()

// CMessageWnd construction/destruction

CMessageWnd::CMessageWnd()
{
	m_pBtnClose = NULL;
	m_pStaTitle = NULL;
	m_pBtnYes = NULL;
	m_pBtnNo = NULL;
	m_pStaMsg = NULL;

	m_nMeassageWndMode = MW_OK;
	m_strTitle = "";
	m_strMsg = "";
}

CMessageWnd::~CMessageWnd()
{
}

void CMessageWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

}


//////////////////////////////////////////////////////////////////////////
// protected

int CMessageWnd::InitControls()
{
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pStaTitle = (CSkinStatic *)GetDlgItem(IDC_STA_TITLE);
	ASSERT(m_pStaTitle->GetSafeHwnd() != NULL);
	m_pBtnYes = (CSkinButton *)GetDlgItem(IDC_BTN_YES);
	ASSERT(m_pBtnYes->GetSafeHwnd() != NULL);
	m_pBtnNo = (CSkinButton *)GetDlgItem(IDC_BTN_NO);
	ASSERT(m_pBtnNo->GetSafeHwnd() != NULL);
	m_pStaMsg = (CSkinStatic *)GetDlgItem(IDC_STA_MSG);
	ASSERT(m_pStaMsg->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMessageWnd message handlers

int CMessageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;

	m_pStaTitle->SetWindowText(m_strTitle);
	m_pStaMsg->SetWindowText(m_strMsg);

	switch (m_nMeassageWndMode)
	{
	case MW_OK:
		m_pBtnYes->ShowWindow(SW_HIDE);
		m_pBtnNo->SetWindowText(_T("OK"));
		break;
	case MW_YESNO:
		m_pStaMsg->SetTextAlignHor(TAH_CENTER, TAH_CENTER);
		break;
	case MW_BUYOK:
		m_pBtnYes->SetWindowText("BUY NOW");
		m_pBtnNo->SetWindowText("OK");
		break;
	}

	return 0;
}

void CMessageWnd::OnBtnClose()
{
	
}

void CMessageWnd::OnBtnYes()
{
	CUIWnd::OnOK();
}

void CMessageWnd::OnBtnNo()
{
	CUIWnd::OnCancel();
}

void CMessageWnd::SetMessageWndInfo(int wndType, CString strTitle, CString strMsg)
{
	m_nMeassageWndMode = wndType;
	m_strTitle = strTitle;
	m_strMsg = strMsg;
}