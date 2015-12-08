

//-----------------------------------------------------------------------------
//----- DlgDrawEquipment.h : Declaration of the CDlgDrawEquipment
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "GraphListCtrl.h"
#include <vector>
#include "DirTreeCtrl.h"
#include "afxcmn.h"
#include "BlkInfo.h"
#include "DataStore.h"
#include "afxwin.h"
//-----------------------------------------------------------------------------
class CDlgDrawEquipment : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgDrawEquipment)

public:
	CDlgDrawEquipment (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgDrawEquipment();

	enum { IDD = IDD_DLGDRAWEQUIPMENT} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bZongxiang;
	BOOL m_bFixed;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonData();
	afx_msg void OnBnClickedRadioHengxiang();
	afx_msg void OnBnClickedRadioZongxiang();
	afx_msg void OnBnClickedCheckFix();	
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	HTREEITEM FindItem(HTREEITEM ht);

	////////////////////////////////////////////////////////////////////////
	CGraphListCtrl m_gsView;
	AcDbDatabase* m_pDb;
	void ClearDatabase();
	void InitDatabase();
	bool InitGraphList();
	//////////////////////////////////////////////////////////////////////////

	bool GetModified() const;
	void SetModified(bool bModified);
	AcGePoint3d GetInsertPt() const;
	void SetInsertPt(AcGePoint3d pt);

	////////////////////////////////////////////////////////////////////////
	//读写ini文件
	bool ReadIniFile();
	bool WriteIniFile();

	//为注册表填写数据
	void SetRegInfo();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//绘制文字
	void DrawText();
private:
	CGraphListCtrl m_listGraph;
	CDirTreeCtrl m_TreeCtrl;
	CString m_strFileName;
	CString m_strParentPath;
	CDataBaseInfo m_baseInfo;
	CString m_strBlkName;
	bool m_bIsModified;//是否编辑
	AcGePoint3d m_InsertPt;
	CBlkRefCsv m_blkFile;//ini文件
	CString m_strIniFile;
	CString m_strFName;
	CString m_strStandard;
	CString m_strPipe;//管道名
	CString m_strCount;//位号
	AcDbExtents m_Exts;
public:
	BOOL m_bIsDisplay;
	CString m_strDefine;
	// 文字位置
	int m_nPosition;
	afx_msg void OnBnClickedRadioDown();
	afx_msg void OnBnClickedRadioRight();
	afx_msg void OnBnClickedCheckDisplay();
	afx_msg void OnBnClickedRadioUp();
	afx_msg void OnBnClickedRadioLeft();
	CComboBox m_CombWeihao;
	CString m_strWeihao;
	afx_msg void OnCbnSelchangeComboWeihao();
	afx_msg void OnRBClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddBlock(); 
	afx_msg void OnDelBlock();
} ;
