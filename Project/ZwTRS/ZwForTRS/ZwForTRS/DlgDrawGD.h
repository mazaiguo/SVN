

//-----------------------------------------------------------------------------
//----- DlgDrawGD.h : Declaration of the CDlgDrawGD
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxcmn.h"
#include "EditList.h"
//-----------------------------------------------------------------------------
class CDlgDrawGD : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgDrawGD)

public:
	CDlgDrawGD (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGDRAWGD} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CEditList m_ListCtrl;
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void InsertEmpty();
	afx_msg void OnBnClickedButtonAdd();

	bool DrawLine();
	void SetGDData();
	bool GetGDData();
	bool ReadFile();
	void ReWriteFile();

private:
	CString m_strDescription;//说明
	CString m_strLayerColor;//颜色
	CString m_strLineType;//线型
	CString m_strLineweight;//线宽
	bool	m_bDefault;
	int m_nDefaultItem;
} ;
