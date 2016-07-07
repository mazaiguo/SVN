//-----------------------------------------------------------------------------
//----- DlgBasicSettings.h : Declaration of the CDlgBasicSettings
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "editlist.h"

//-----------------------------------------------------------------------------
class CDlgBasicSettings : public CAcUiTabChildDialog {
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
	
	bool CreateLayerId();

	void iniData();
	void ReWriteFile();
	void SetInfoToListCtrl(int nItem);
	void GetInfoFromListCtrl(int i);
	CEditList m_ListCtrl;
	CString m_strDescription;//说明
	CString m_strLayerColor;//颜色
	CString m_strLineType;//线型
	CString m_strLineweight;//线宽	
	CString m_strScale;
} ;
