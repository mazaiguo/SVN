
//-----------------------------------------------------------------------------
//----- DlgCreateNewBlk.h : Declaration of the CDlgCreateNewBlk
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CDlgCreateNewBlk : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgCreateNewBlk)

public:
	CDlgCreateNewBlk (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGCREATENEWBLK} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComBType;
	CString m_strBlkName;
	// -----------------------------------------------------------------------------
	virtual BOOL OnInitDialog(void);
	afx_msg void OnCbnSelchangeComboName();

	CString m_strType;
	afx_msg void OnBnClickedOk();

private:
	CString m_strFileName;
} ;
