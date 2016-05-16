//-----------------------------------------------------------------------------
//----- DlgBasicSettings.h : Declaration of the CDlgBasicSettings
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "editlist.h"

//-----------------------------------------------------------------------------
class CDlgBasicSettings : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgBasicSettings)

public:
	CDlgBasicSettings (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGBASICSETTINGS} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	void InsertEmpty();
	afx_msg void OnBnClickedButtonDel();


	CEditList m_ListCtrl;
	CString m_strDescription;//˵��
	CString m_strLayerColor;//��ɫ
	CString m_strLineType;//����
	CString m_strLineweight;//�߿�	
	CString m_strScale;
} ;