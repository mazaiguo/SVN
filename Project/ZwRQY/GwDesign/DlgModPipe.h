//-----------------------------------------------------------------------------
//----- DlgModPipe.h : Declaration of the CDlgModPipe
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgModPipe : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgModPipe)

public:
	CDlgModPipe (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGMODPIPE} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDiameter;
	CString m_strTexture;
	CString m_strAmount;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
} ;
