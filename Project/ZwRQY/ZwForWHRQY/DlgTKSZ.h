//-----------------------------------------------------------------------------
//----- DlgTKSZ.h : Declaration of the CDlgTKSZ
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"

//-----------------------------------------------------------------------------
class CDlgTKSZ : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgTKSZ)

public:
	CDlgTKSZ (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGTKSZ} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboFrametype();
	afx_msg void OnBnClickedOk();
	bool SaveToIniFile();

	CComboBox m_CombFrameType;
	CString m_strFrameType;
	CString m_strScale;
} ;
