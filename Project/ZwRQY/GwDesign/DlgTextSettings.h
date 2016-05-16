//-----------------------------------------------------------------------------
//----- DlgTextSettings.h : Declaration of the CDlgTextSettings
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "editlist.h"

//-----------------------------------------------------------------------------
class CDlgTextSettings : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgTextSettings)

public:
	CDlgTextSettings (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DLGTEXTSETTINGS} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CEditList m_ListCtrl;
	virtual BOOL OnInitDialog();


	void InsertEmpty();
	void GetAcadFont();
	void GetWindowFont();
	BOOL PdThenameIsHave(CString name, CStringArray& arr);


	CStringArray m_font, m_bigfont;
	CStringArray m_cadfont;

	CString m_strDescription;//˵��
	CString m_strFont;//����
	CString m_strBigFont;//������
	CString m_strHeight;//�߶�	
	CString m_strScale;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
} ;
