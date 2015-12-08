

//-----------------------------------------------------------------------------
//----- DlgTKManage.cpp : Implementation of CDlgTKManage
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgTKManage.h"
#include "Global.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgTKManage, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgTKManage, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgTKManage::OnTvnSelchangedTypeName)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgTKManage::CDlgTKManage (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgTKManage::IDD, pParent, hInstance) 
 ,m_pDb(NULL)
{
	ClearDatabase();
}


CDlgTKManage::~CDlgTKManage()
{
	ClearDatabase();
}
//-----------------------------------------------------------------------------
void CDlgTKManage::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgTKManage::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgTKManage::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_TreeCtrl.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);
	m_strParentPath = _T("F:\\SVN\\201406\\");
	m_TreeCtrl.SetParentPath(m_strParentPath);
	m_TreeCtrl.DisPlayTree(_T("ZWHG"), TRUE);
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();

	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
	m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
	m_TreeCtrl.Expand(hChild, TVE_EXPAND);

	HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
	m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".dwg");
	InitDatabase();
	InitGraphList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTKManage::ClearDatabase()
{
	m_gsView.DeleteAllItem();
	m_gsView.SetCurDatabase(NULL);

	if (m_pDb!=NULL)
	{
		delete m_pDb;
	}
}

void CDlgTKManage::InitDatabase()
{
	ClearDatabase();
	CAcModuleResourceOverride rs;
	m_pDb = new AcDbDatabase(false);
	m_pDb->readDwgFile(m_strFileName);
}

bool CDlgTKManage::InitGraphList()
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_BMP);
	if (NULL == pWnd)
	{
		return false;
	}

	DWORD dwStyle = pWnd->GetStyle();
	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	pWnd->DestroyWindow();
	if (!m_gsView.Create(rc, this, IDC_STATIC_BMP, dwStyle))
	{
		return false;
	}

	m_gsView.SetCurDatabase(m_pDb);
	m_gsView.ShowItemCaption(false);
	m_gsView.EnableDefault(false);
	m_gsView.EnableSel(true);
	m_gsView.SetLayer(1, 1);

	m_gsView.InsertItem(0, ACDB_MODEL_SPACE);
	return true;
}




void CDlgTKManage::OnTvnSelchangedTypeName(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hCurSel = m_TreeCtrl.GetSelectedItem();
	CString strGet = m_TreeCtrl.GetItemText(hCurSel);

	if (m_TreeCtrl.ItemHasChildren(hCurSel) && (!m_TreeCtrl.GetParentItem(hCurSel)))//父节点
	{

	}
	else if (m_TreeCtrl.ItemHasChildren(hCurSel) && m_TreeCtrl.GetParentItem(hCurSel))//所选为节点
	{
		
	}
	else if (!m_TreeCtrl.ItemHasChildren(hCurSel))
	{
		m_strFileName = m_TreeCtrl.GetFullPath(hCurSel) + _T(".dwg");
		InitDatabase();
		InitGraphList();
	}	
	*pResult = 0;
}
