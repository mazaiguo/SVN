//-----------------------------------------------------------------------------
//----- DlgDrawJSD.h : Declaration of the CDlgDrawJSD
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"

//-----------------------------------------------------------------------------
class CDlgDrawJSD : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgDrawJSD)

public:
	CDlgDrawJSD (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGDRAWJSD} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboType();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CComboBox m_CombType;
	CString m_strType;
} ;
