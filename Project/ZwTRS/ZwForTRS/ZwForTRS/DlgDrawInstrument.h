//-----------------------------------------------------------------------------
//----- DlgDrawInstrument.h : Declaration of the CDlgDrawInstrument
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"
#include "GraphListCtrl.h"
#include <vector>
#include "DirTreeCtrl.h"
#include "BlkInfo.h"
#include "DataStore.h"
//-----------------------------------------------------------------------------
class CDlgDrawInstrument : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgDrawInstrument)

public:
	CDlgDrawInstrument (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgDrawInstrument();

	enum { IDD = IDD_DLGDRAWINSTRUMENT} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboName();
	afx_msg void OnBnClickedCheckFix();
	afx_msg void OnBnClickedRadioHengxiang();
	afx_msg void OnBnClickedButtonData();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckUp();
	afx_msg void OnBnClickedCheckLeft();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnBnClickedRadioZongxiang();
	afx_msg void OnBnClickedCheckDisplay();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnBnClickedRadioUp();
	afx_msg void OnBnClickedRadioLeft();
	afx_msg void OnBnClickedRadioDown();
	afx_msg void OnBnClickedRadioRight();

	HTREEITEM FindItem(HTREEITEM ht);

	BOOL m_bFixed;
	BOOL m_bZongxiang;
	/*BOOL m_bUp;
	BOOL m_bLeft;*/
	CComboBox m_CombName;
	////////////////////////////////////////////////////////////////////////
	CGraphListCtrl m_gsView;
	AcDbDatabase* m_pDb;
	void ClearDatabase();
	void InitDatabase();
	bool InitGraphList();
	//////////////////////////////////////////////////////////////////////////
	CGraphListCtrl m_listGraph;
	CDirTreeCtrl m_TreeCtrl;

	bool GetModified() const;
	void SetModified(bool bModified);
	AcGePoint3d GetInsertPt() const;
	void SetInsertPt(AcGePoint3d pt);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//处理插入文字
	void DrawText();
	//绘制拉线
	void DrawLine();


private:
	//读写ini文件
	void SaveInfoToFile();
	void GetInfoFromFile();
	void SetRegInfo();

	//获取距离
	double GetLength() const;

	CString m_strFileName;
	CString m_strParentPath;
	CDataBaseInfo m_baseInfo;
	CString m_strBlkName;

	bool m_bIsModified;//是否编辑
	AcGePoint3d m_InsertPt;
	CString m_strName;
	CString m_strPipe;
	CString m_strCount;
	CBlkRefCsv m_blkFile;//ini文件
	CString m_strIniFile;
	// 序号
	CString m_strNo;
	
	AcDbObjectId m_blkId;//块参照id
	AcDbExtents	m_Exts;//块的外框
	BOOL m_bIsDisplay;
	int m_nPosition;
	CComboBox m_ComboType;
	CString m_strType;	
public:
	CComboBox m_CombWeihao;
	CString m_strWeihao;
	afx_msg void OnCbnSelchangeComboWeihao();
	afx_msg void OnRBClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddBlock(); 
	afx_msg void OnDelBlock();
} ;
