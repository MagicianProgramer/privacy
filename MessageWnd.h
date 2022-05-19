// MessageWnd1.h : interface of the CMessageWnd class
//
#pragma once


class CMessageWnd : public CUIWnd
{
public:
	CMessageWnd();
	virtual ~CMessageWnd();
	void WindowID() { IDD = IDW_MESSAGEBOX; }

	enum { MW_OK=0, MW_YESNO=1, MW_BUYOK=2 };
	void SetMessageWndInfo(int wndType, CString strTitle, CString strMsg);
protected:
	int InitControls();

public:

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnClose();
	afx_msg void OnBtnYes();
	afx_msg void OnBtnNo();
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnClose;
	CSkinStatic *m_pStaTitle;
	CSkinButton *m_pBtnYes;
	CSkinButton *m_pBtnNo;
	CSkinStatic *m_pStaMsg;
	
	int m_nMeassageWndMode;
	CString m_strTitle;
	CString m_strMsg;
};
