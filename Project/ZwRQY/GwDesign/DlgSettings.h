//-----------------------------------------------------------------------------
//----- DlgSettings.h : Declaration of the CDlgSettings
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxcmn.h"
#include "DlgTextSettings.h"
#include "DlgBasicSettings.h"
//-----------------------------------------------------------------------------
class CDlgSettings : public CAcUiTabMainDialog {
	DECLARE_DYNAMIC (CDlgSettings)

public:
	CDlgSettings (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgSettings();

	enum { IDD = IDD_DLGSETTING} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CAdUiTab m_tab;
	virtual BOOL OnInitDialog();

private:
	CDlgBasicSettings* m_pBasicSettings;
	CDlgTextSettings* m_pTextSettings;
	//CDialog* pDialog[2];  //用来保存对话框对象指针
	int m_CurSelTab;
public:
	afx_msg void OnTcnSelchangeTabSettings(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual void OnCancel();
} ;
