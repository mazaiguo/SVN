

//-----------------------------------------------------------------------------
//----- DlgTKManage.h : Declaration of the CDlgTKManage
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxcmn.h"
#include "GraphListCtrl.h"
#include <vector>
#include "DirTreeCtrl.h"
//-----------------------------------------------------------------------------
class CDlgTKManage : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgTKManage)

public:
	CDlgTKManage (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgTKManage();

	enum { IDD = IDD_DLGTKMANAGE} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	//HTREEITEM addOneTreeItem(LPCTSTR name, const AcDbObjectId& objId, HTREEITEM parent);

	DECLARE_MESSAGE_MAP()
public:
	CDirTreeCtrl m_TreeCtrl;
	CGraphListCtrl m_listGraph;
	virtual BOOL OnInitDialog();

/////////////////////////////////////////////////////////////////////////////////
	CGraphListCtrl m_gsView;
	AcDbDatabase* m_pDb;
	void ClearDatabase();
	void InitDatabase();
	bool InitGraphList();
////////////////////////////////////////////////////////////////////////////////
private:
	//std::multimap<CString, CString> m_strRootData;//第一个参数为根目录，第二个为
	CStringArray m_strRootArr;
	CStringArray m_strFileNameArr;
	std::vector<std::pair<CString, CString> > m_VecPair;
	CString m_strFileName;
	CString m_strParentPath;
public:
	afx_msg void OnTvnSelchangedTypeName(NMHDR *pNMHDR, LRESULT *pResult);
} ;
