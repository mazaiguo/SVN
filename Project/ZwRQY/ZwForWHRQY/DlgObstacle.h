//-----------------------------------------------------------------------------
//----- DlgObstacle.h : Declaration of the CDlgObstacle
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgObstacle : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgObstacle)

public:
	CDlgObstacle (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGOBSTACLE} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	int m_nRadio;//记录编号
	CString m_strCurName;//当前的名称

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio24();
	afx_msg void OnBnClickedOk();
} ;
