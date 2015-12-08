
//-----------------------------------------------------------------------------
//----- DlgSQLDB.h : Declaration of the CDlgSQLDB
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "BlkInfo.h"
#include "ColorList.h"
#include "resource.h"

//-----------------------------------------------------------------------------
class  CDlgSQLDB : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgSQLDB)

public:
	CDlgSQLDB (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgSQLDB();

	enum { IDD = IDD_DLGSQLDB} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAuto();
	//afx_msg void OnBnClickedButtonWork();
	afx_msg void OnBnClickedCheckUpdate();
	afx_msg void OnBnClickedCheckNotifyupdate();
	afx_msg void OnBnClickedButtonSetdb();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SaveInfoToReg();
	void GetInfoFromReg();
	CString GetStringFromReg(LPCTSTR strKey);
	int GetDwordFromReg(LPCTSTR strKey) const;

	
	CColorList m_ListCtrl;
	CDataBaseInfo GetData();
	void InsertEmpty();

	bool GetDataFromFile();
	
	bool KMP();//kmp匹配
	//给combo添加数据
	void AddDataToCombo();

private:
	CDataBaseInfo m_BlkInfo;	
	
	CString m_strAddress;
	CString m_strUserName;
	CString m_strPassWord;

	CString m_strName;//FName
	CString m_strMark;//FitemId
	CString m_strMaterial;//F105
	CString m_strStandard;//F106
	CString m_strLingjianType;//零件类型FErpCls
	CString m_strFMaund;//FMaund单位
	CString m_strFNumber;//Fnumber物料代码
	CString m_strF107;//F107单重
	

	CComboBox m_CombStandard;
	CComboBox m_CombMaterial;

	vector<CDataCell*> m_Data;
	vector<CDataCell*> m_TmpData;//临时的数据列
	/*vector<CString> m_F105Vec;
	vector<CString> m_F106Vec;*/
	map<CString, CString> m_F105Map;
	map<CString, CString> m_F106Map;
	int m_nDefaultItem;//默认item位置
public:
	afx_msg void OnCbnSelchangeComboStandard();
	afx_msg void OnCbnSelchangeComboMaterial();
	//afx_msg void OnBnClickedButtonDefault();
	BOOL m_bUpdate;
	BOOL m_bNoticeUpdate;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
} ;