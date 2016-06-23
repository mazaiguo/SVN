
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


	CListCtrl m_ListCtrl;
	CDataBaseInfo GetData();
	void InsertEmpty(CDataCell pData);

	bool GetDataFromFile();
	
	bool KMP(bool bIsKMP = true);//kmpƥ��
	//��combo�������
	void AddDataToCombo();

	//���õ�ǰ��
	void SetCurCell();

	bool getListData();

	void clearData(vector<CDataCell> data);
private:
	CDataBaseInfo m_BlkInfo;	
	
	CString m_strAddress;
	CString m_strUserName;
	CString m_strPassWord;
	CString m_strSQLName;

	CString m_strName;//FName
	CString m_strMark;//FitemId
	CString m_strMaterial;//F105
	CString m_strStandard;//F106
	CString m_strLingjianType;//�������FErpCls
	CString m_strFMaund;//FMaund��λ
	CString m_strFNumber;//Fnumber���ϴ���
	CString m_strF107;//F107����
	
	double m_dTime;//����ʱ��
	CComboBox m_CombStandard;
	CComboBox m_CombMaterial;

	vector<CDataCell> m_Data;
	vector<CDataCell> m_TmpData;//��ʱ��������
	map<CString, CString> m_F105Map;
	map<CString, CString> m_F106Map;
	int m_nDefaultItem;//Ĭ��itemλ��
	int m_nCurMark;
public:
	afx_msg void OnCbnSelchangeComboStandard();
	afx_msg void OnCbnSelchangeComboMaterial();
	//afx_msg void OnBnClickedButtonDefault();
	BOOL m_bUpdate;
	BOOL m_bNoticeUpdate;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	// ��¼�ò��ϵ�����
	int m_nCount;
	// ��¼�������� ���������������˳���1000��Ϊ��ʵ�ı���ֵ 
	//������������Ϊ�գ���ô���������� 
	//��Ϊ�� ������������ƺ������ L=������ֵ
	CString m_strJiliang;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOdfinditemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdcachehintList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditMark();
	afx_msg void OnBnClickedCheckDefault();
	BOOL m_bIsDeFault;
} ;