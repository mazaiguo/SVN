
//-----------------------------------------------------------------------------
//----- DlgSQLDbSet.h : Declaration of the CDlgSQLDbSet
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "DBAdo.h"
#include <vector>
using namespace std;
//-----------------------------------------------------------------------------
class CDlgSQLDbSet : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgSQLDbSet)

public:
	CDlgSQLDbSet (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgSQLDbSet();

	enum { IDD = IDD_DLGSQLDBSET} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	void SaveInfoToReg();
	void GetInfoFromReg();
	CString GetStringFromReg(LPCTSTR strKey);

	CString m_strAddress;
	CString m_strUserName;
	CString m_strPassWord;
	CString m_strSQLName;
	bool m_bSuccess;
} ;
