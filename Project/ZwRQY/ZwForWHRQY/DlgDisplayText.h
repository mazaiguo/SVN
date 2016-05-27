//-----------------------------------------------------------------------------
//----- DlgDisplayText.h : Declaration of the CDlgDisplayText
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgDisplayText : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgDisplayText)

public:
	CDlgDisplayText (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGDISPLAYTEXT} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCurText;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	void SetText(const CString);
	CString GetText();
} ;
