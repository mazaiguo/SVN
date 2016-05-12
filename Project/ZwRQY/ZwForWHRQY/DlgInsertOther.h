

//-----------------------------------------------------------------------------
//----- DlgInsertOther.h : Declaration of the CDlgInsertOther
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "GraphBlockViewWnd.h"
#include "afxwin.h"

class CDlgInsertOther : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgInsertOther)

public:
	CDlgInsertOther (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgInsertOther();
	enum { IDD = IDD_DLGINSERTOTHER} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()

public:
	CGraphListCtrl m_gsView1;
	CGraphListCtrl m_gsView2;
	CGraphListCtrl m_gsView3;
	CGraphListCtrl m_gsView4;
	CGraphListCtrl m_gsView5;
	CGraphListCtrl m_gsView6;
	CGraphListCtrl m_gsView7;
	CGraphListCtrl m_gsView8;
	CGraphListCtrl m_gsView9;
	AcDbDatabase* m_pDb1;
	AcDbDatabase* m_pDb2;
	AcDbDatabase* m_pDb3;
	AcDbDatabase* m_pDb4;
	AcDbDatabase* m_pDb5;
	AcDbDatabase* m_pDb6;
	AcDbDatabase* m_pDb7;
	AcDbDatabase* m_pDb8;
	AcDbDatabase* m_pDb9;
	CString m_strFilePath;//记录文件路径名
	CStringArray m_strAllFileNameArr;//记录所有的strPath下面的dwg文件名
	CStringArray m_strFileNameArr;//在m_strAllFileNameArr中提取需要的文件名
	CStringArray m_strTitleArr;//
	int m_nCount;//记录strFileNameArr的数量
	int m_nFlag;//控制翻页次数
	int m_nEntity;//控制是线杆还是其它实体//[Added by mzg 2012/11/25
	void ClearDatabase();
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	BOOL CreateGsView(CGraphListCtrl& m_gsView, AcDbDatabase* m_PDb, int nDlgID);
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
	void InitDatabase();
	afx_msg void OnStnClickedStaticView1();
	afx_msg void OnStnClickedStaticView2();
	afx_msg void OnStnClickedStaticView3();
	afx_msg void OnStnClickedStaticView4();
	afx_msg void OnStnClickedStaticView5();
	afx_msg void OnStnClickedStaticView6();
	afx_msg void OnStnClickedStaticView7();
	afx_msg void OnStnClickedStaticView8();
	afx_msg void OnStnClickedStaticView9();
	void GetAllDirectoryByPath(CString strPath, CStringArray& strFileNameArr);
	void CreateGS();
	afx_msg void OnLbnSelchangeList1();
	CListBox m_ListBox;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
} ;
