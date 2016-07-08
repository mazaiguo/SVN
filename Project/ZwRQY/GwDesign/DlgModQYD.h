//-----------------------------------------------------------------------------
//----- DlgModQYD.h : Declaration of the CDlgModQYD
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgModQYD : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgModQYD)

public:
	CDlgModQYD (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGMODQYD} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strAmount;
	CString m_strPressure;
	CString m_strDiameter;
	CString m_strTexture;
	CString m_strStartAmount;
} ;
