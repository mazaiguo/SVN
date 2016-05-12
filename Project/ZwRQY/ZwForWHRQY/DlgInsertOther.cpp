//-----------------------------------------------------------------------------
//----- DlgInsertOther.cpp : Implementation of CDlgInsertOther
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgInsertOther.h"
#include "Global.h"
#include "BlkInfo.h"
//#include "ArxDbgUtils.h"
//#include "BlkInfo.h"

IMPLEMENT_DYNAMIC (CDlgInsertOther, CAcUiDialog)
BEGIN_MESSAGE_MAP(CDlgInsertOther, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CDlgInsertOther::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CDlgInsertOther::OnBnClickedButtonNext)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW1, &CDlgInsertOther::OnStnClickedStaticView1)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW2, &CDlgInsertOther::OnStnClickedStaticView2)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW3, &CDlgInsertOther::OnStnClickedStaticView3)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW4, &CDlgInsertOther::OnStnClickedStaticView4)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW5, &CDlgInsertOther::OnStnClickedStaticView5)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW6, &CDlgInsertOther::OnStnClickedStaticView6)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW7, &CDlgInsertOther::OnStnClickedStaticView7)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW8, &CDlgInsertOther::OnStnClickedStaticView8)
	ON_LBN_SELCHANGE(IDC_STATIC_VIEW9, &CDlgInsertOther::OnStnClickedStaticView9)
	//ON_CONTROL(BN_DOUBLECLICKED, IDC_STATIC_VIEW1,  &CDlgInsertOther::OnStnDblclickStaticView1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgInsertOther::OnLbnSelchangeList1)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
CDlgInsertOther::CDlgInsertOther (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgInsertOther::IDD, pParent, hInstance) 
, m_pDb1(NULL)
, m_pDb2(NULL)
, m_pDb3(NULL)
, m_pDb4(NULL)
, m_pDb5(NULL)
, m_pDb6(NULL)
, m_pDb7(NULL)
, m_pDb8(NULL)
, m_pDb9(NULL)
{
	m_nFlag = 0;
	m_strAllFileNameArr.RemoveAll();
	m_strFileNameArr.RemoveAll();
	m_strTitleArr.RemoveAll();
}
CDlgInsertOther::~CDlgInsertOther()
{
	ClearDatabase();
	m_strAllFileNameArr.RemoveAll();
	m_strFileNameArr.RemoveAll();	
	m_strTitleArr.RemoveAll();
}
//-----------------------------------------------------------------------------
void CDlgInsertOther::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgInsertOther::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgInsertOther::ClearDatabase()
{
	m_gsView1.DeleteAllItem();
	m_gsView1.SetCurDatabase(NULL);

	m_gsView2.DeleteAllItem();
	m_gsView2.SetCurDatabase(NULL);

	m_gsView3.DeleteAllItem();
	m_gsView3.SetCurDatabase(NULL);

	m_gsView4.DeleteAllItem();
	m_gsView4.SetCurDatabase(NULL);

	m_gsView5.DeleteAllItem();
	m_gsView5.SetCurDatabase(NULL);

	m_gsView6.DeleteAllItem();
	m_gsView6.SetCurDatabase(NULL);

	m_gsView7.DeleteAllItem();
	m_gsView7.SetCurDatabase(NULL);

	m_gsView8.DeleteAllItem();
	m_gsView8.SetCurDatabase(NULL);

	m_gsView9.DeleteAllItem();
	m_gsView9.SetCurDatabase(NULL);
	if (m_pDb1!=NULL)
	{
		delete m_pDb1;
		m_pDb1 = NULL;
	}
	if (m_pDb2!=NULL)
	{
		delete m_pDb2;
		m_pDb2 = NULL;
	}
	if (m_pDb3!=NULL)
	{
		delete m_pDb3;
		m_pDb3 = NULL;
	}
	if (m_pDb4!=NULL)
	{
		delete m_pDb4;
		m_pDb4 = NULL;
	}
	if (m_pDb5!=NULL)
	{
		delete m_pDb5;
		m_pDb5 = NULL;
	}
	if (m_pDb6!=NULL)
	{
		delete m_pDb6;
		m_pDb6 = NULL;
	}
	if (m_pDb7!=NULL)
	{
		delete m_pDb7;
		m_pDb7 = NULL;
	}
	if (m_pDb8!=NULL)
	{
		delete m_pDb8;
		m_pDb8 = NULL;
	}
	if (m_pDb9!=NULL)
	{
		delete m_pDb9;
		m_pDb9 = NULL;
	}
}
void CDlgInsertOther::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strFileName;
	this->GetWindowText(strFileName);

	CString strtemp1;
	strtemp1 = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\')-1);
	int nCur = m_ListBox.GetCurSel();
	CString strBlkName;
	m_ListBox.GetText(nCur, strBlkName);
	BeginEditorCommand();
	AcGePoint3d insertPt;

	/*int nRet = acedGetPoint(NULL, _T("\n请选择一点"), asDblArray(insertPt));
	if (nRet != RTNORM)
	{
		CancelEditorCommand();
		return;
	}*/
	CBlkInfo blkInfo;
	blkInfo.SetInsertPt(insertPt);
	blkInfo.SetBlkName(strBlkName);
	blkInfo.SetFilePath(strFileName);
	blkInfo.SetBlkType(_T("其它"));

	//CDataBaseInfo baseInfo;
	CBlkInsert blkInsert(blkInfo/*, baseInfo*/);
	//blkInsert.SetDrawWP(false);
	blkInsert.Insert();

	CompleteEditorCommand();
	gGlobal.WriteIniFile();
	CAcUiDialog::OnOK();
}

BOOL CDlgInsertOther::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_strFileNameArr.RemoveAll();
	CString strAppPath = MyBaseUtils::GetAppPath();
	CString strPath = strAppPath + _T("附属配件\\*.*");
	MyOperateFile::GetFileNameFromThePath(strPath, m_strAllFileNameArr, _T(".DWG"), true);
	m_nCount = m_strAllFileNameArr.GetSize();
	CString strTitle;
	if (m_nCount>9)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		for (int i=0; i<9; i++)
		{
			m_strFileNameArr.Add(m_strAllFileNameArr.GetAt(i));
			strTitle = MyOperateFile::GetFileNameByPath(m_strFileNameArr.GetAt(i));
			m_strTitleArr.Add(strTitle);
		}
	}
	else
	{
		for (int i=0; i<m_nCount; i++)
		{
			m_strFileNameArr.Add(m_strAllFileNameArr.GetAt(i));
			strTitle = MyOperateFile::GetFileNameByPath(m_strFileNameArr.GetAt(i));
			m_strTitleArr.Add(strTitle);
		}
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}
	// TODO:  在此添加额外的初始化
	InitDatabase();
	CreateGS();
	m_ListBox.ResetContent();
	for (int i=0; i<m_strTitleArr.GetSize(); i++)
	{
		m_ListBox.InsertString(-1, m_strTitleArr.GetAt(i));
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgInsertOther::CreateGsView(CGraphListCtrl& m_gsView, AcDbDatabase* m_PDb, int nDlgID)
{
	CWnd* pWnd = GetDlgItem(nDlgID);
	if (NULL == pWnd)
	{
		return false;
	}

	DWORD dwStyle = pWnd->GetStyle();
	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	pWnd->DestroyWindow();
	if (!m_gsView.Create(rc, this, nDlgID, dwStyle))
	{
		return false;
	}
	m_gsView.SetCurDatabase(m_PDb);
	m_gsView.ShowItemCaption(false);
	m_gsView.EnableDefault(false);
	m_gsView.EnableSel(true);
	m_gsView.SetLayer(1, 1);
	m_gsView.InsertItem(0, ACDB_MODEL_SPACE);

	return TRUE;
}
void CDlgInsertOther::OnBnClickedButtonPre()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strFileNameArr.RemoveAll();
	--m_nFlag;
	if (m_nFlag==0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	}

	int nTmpCount = m_nCount - m_nFlag*9;
	CString strTitle;
	m_strTitleArr.RemoveAll();
	m_strFileNameArr.RemoveAll();
	if (nTmpCount>9)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		for (int i=0; i<9; i++)
		{
			m_strFileNameArr.Add(m_strAllFileNameArr.GetAt(i+m_nFlag*9));
			strTitle = MyOperateFile::GetFileNameByPath(m_strFileNameArr.GetAt(i));
			m_strTitleArr.Add(strTitle);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		for (int i=0; i<nTmpCount; i++)
		{
			m_strFileNameArr.Add(m_strAllFileNameArr.GetAt(i+m_nFlag*9));
			strTitle = MyOperateFile::GetFileNameByPath(m_strFileNameArr.GetAt(i));
			m_strTitleArr.Add(strTitle);
		}
	}

	m_ListBox.ResetContent();
	for (int i=0; i<m_strTitleArr.GetSize(); i++)
	{
		m_ListBox.InsertString(-1, m_strTitleArr.GetAt(i));
	}

	InitDatabase();
	CreateGS();
	Invalidate(TRUE);
}

void CDlgInsertOther::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strFileNameArr.RemoveAll();
	++m_nFlag;
	if (m_nFlag==0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	}

	int nTmpCount = m_nCount - m_nFlag*9;
	CString strTitle;
	m_strTitleArr.RemoveAll();
	m_strFileNameArr.RemoveAll();
	if (nTmpCount>9)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		for (int i=0; i<9; i++)
		{
			m_strFileNameArr.Add(m_strAllFileNameArr.GetAt(i+m_nFlag*9));
			strTitle = MyOperateFile::GetFileNameByPath(m_strFileNameArr.GetAt(i));
			m_strTitleArr.Add(strTitle);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		for (int i=0; i<nTmpCount; i++)
		{
			m_strFileNameArr.Add(m_strAllFileNameArr.GetAt(i+m_nFlag*9));
			strTitle = MyOperateFile::GetFileNameByPath(m_strFileNameArr.GetAt(i));
			m_strTitleArr.Add(strTitle);
		}
	}
	m_ListBox.ResetContent();
	for (int i=0; i<m_strTitleArr.GetSize(); i++)
	{
		m_ListBox.InsertString(-1, m_strTitleArr.GetAt(i));
	}
	InitDatabase();
	CreateGS();
	Invalidate(TRUE);
}

void CDlgInsertOther::InitDatabase()
{
	ClearDatabase();

	CAcModuleResourceOverride rs;
	for (int i=0; i<m_strFileNameArr.GetSize(); i++)
	{
		switch (i)
		{
		case 0:
			m_pDb1 = new AcDbDatabase(false);
			m_pDb1->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 1:
			m_pDb2 = new AcDbDatabase(false);
			m_pDb2->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 2:
			m_pDb3 = new AcDbDatabase(false);
			m_pDb3->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 3:
			m_pDb4 = new AcDbDatabase(false);
			m_pDb4->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 4:
			m_pDb5 = new AcDbDatabase(false);
			m_pDb5->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 5:
			m_pDb6 = new AcDbDatabase(false);
			m_pDb6->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 6:
			m_pDb7 = new AcDbDatabase(false);
			m_pDb7->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 7:
			m_pDb8 = new AcDbDatabase(false);
			m_pDb8->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		case 8:
			m_pDb9 = new AcDbDatabase(false);
			m_pDb9->readDwgFile(m_strFileNameArr.GetAt(i));
			break;
		default:
			break;
		}
	}
}
void CDlgInsertOther::OnStnClickedStaticView1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView6.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize() <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(0);
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize()-1 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(1);
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+1);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	CString strFileName;
	if (m_strAllFileNameArr.GetSize()-2 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(2);
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+2);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	CString strFileName;
	if (m_strAllFileNameArr.GetSize()-3 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(3);
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+3);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize()-4 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(4);
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+4);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize()-5 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(5);
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+5);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize()-6 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(6);
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+6);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}
void CDlgInsertOther::OnStnClickedStaticView8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	m_gsView9.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize()-7 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	m_ListBox.SetCurSel(7);
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+7);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::OnStnClickedStaticView9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gsView1.SetCurSel(-1);
	m_gsView2.SetCurSel(-1);
	m_gsView3.SetCurSel(-1);
	m_gsView4.SetCurSel(-1);
	m_gsView5.SetCurSel(-1);
	m_gsView7.SetCurSel(-1);
	m_gsView8.SetCurSel(-1);
	m_gsView6.SetCurSel(-1);
	if (m_strAllFileNameArr.GetSize()-8 <= 9*m_nFlag)
	{
		this->SetWindowText(_T(""));
		return;
	}
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+8);
	this->SetWindowText(strFileName);
	m_ListBox.SetCurSel(8);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CDlgInsertOther::GetAllDirectoryByPath(CString strPath, CStringArray& strFileNameArr)
{
	strFileNameArr.RemoveAll();

	CFileFind ff;
	BOOL b = FALSE;
	b = ff.FindFile(strPath);

	BOOL bFlgLast = FALSE; //标志着遍历的最后一个文件
	if(b)
	{
		bFlgLast = FALSE;
	}
	else
	{
		bFlgLast = TRUE;
	}

	while(b || !bFlgLast)
	{
		if (!b)
		{
			bFlgLast = TRUE;
		}	
		if(!ff.IsDots())
		{
			if (ff.IsDirectory())
			{
				CString strFilename;
				strFilename = ff.GetFilePath();
				int nCur = strFilename.ReverseFind(_T('\\'));
				strFilename = strFilename.Right(strFilename.GetLength() - nCur - 1);
				strFileNameArr.Add(strFilename);
			}
		}		
		if(!bFlgLast)
		{
			b = ff.FindNextFile();
		}
	}
	ff.Close();
	return;
}

void CDlgInsertOther::CreateGS()
{
	if (!CreateGsView(m_gsView1, m_pDb1, IDC_STATIC_VIEW1))
	{
		return;
	}
	if (!CreateGsView(m_gsView2, m_pDb2, IDC_STATIC_VIEW2))
	{
		return;
	}
	if (!CreateGsView(m_gsView3, m_pDb3, IDC_STATIC_VIEW3))
	{
		return;
	}
	if (!CreateGsView(m_gsView4, m_pDb4, IDC_STATIC_VIEW4))
	{
		return;
	}
	if (!CreateGsView(m_gsView5, m_pDb5, IDC_STATIC_VIEW5))
	{
		return;
	}
	if (!CreateGsView(m_gsView6, m_pDb6, IDC_STATIC_VIEW6))
	{
		return;
	}
	if (!CreateGsView(m_gsView7, m_pDb7, IDC_STATIC_VIEW7))
	{
		return;
	}
	if (!CreateGsView(m_gsView8, m_pDb8, IDC_STATIC_VIEW8))
	{
		return;
	}
	if (!CreateGsView(m_gsView9, m_pDb9, IDC_STATIC_VIEW9))
	{
		return;
	}
}

void CDlgInsertOther::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int m_nIndex = m_ListBox.GetCurSel();
	if (m_nIndex == -1)
	{
		m_nIndex = 0;
	}
	m_ListBox.SetCurSel(m_nIndex);
	switch (m_nIndex)
	{
	case 0:
		m_gsView1.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView1();
		break;
	case 1:
		m_gsView2.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView2();
		break;
	case 2:
		m_gsView3.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView3();
		break;
	case 3:
		m_gsView4.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView4();
		break;
	case 4:
		m_gsView5.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView5();
		break;
	case 5:
		m_gsView6.SetCurSel(0);
		m_gsView1.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView6();
		break;
	case 6:
		m_gsView7.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView7();
		break;
	case 7:
		m_gsView8.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		m_gsView9.SetCurSel(-1);
		//OnStnClickedStaticView8();
		break;
	case 8:
		m_gsView9.SetCurSel(0);
		m_gsView6.SetCurSel(-1);
		m_gsView2.SetCurSel(-1);
		m_gsView3.SetCurSel(-1);
		m_gsView4.SetCurSel(-1);
		m_gsView5.SetCurSel(-1);
		m_gsView7.SetCurSel(-1);
		m_gsView8.SetCurSel(-1);
		m_gsView1.SetCurSel(-1);
		//OnStnClickedStaticView9();
		break;
	default:
		break;
	}
	CString strFileName;
	strFileName = m_strAllFileNameArr.GetAt(9*m_nFlag+m_nIndex);
	this->SetWindowText(strFileName);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}


BOOL CDlgInsertOther::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message== WM_LBUTTONDBLCLK)
	{
		CWnd *pWnd = GetFocus();
		if (pWnd != NULL)
		{
			UINT nID = pWnd->GetDlgCtrlID();
			if (nID == IDC_STATIC_VIEW1)
			{
				OnStnClickedStaticView1();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW2)
			{
				OnStnClickedStaticView2();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW3)
			{
				OnStnClickedStaticView3();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW4)
			{
				OnStnClickedStaticView4();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW5)
			{
				OnStnClickedStaticView5();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW6)
			{
				OnStnClickedStaticView6();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW7)
			{	
				OnStnClickedStaticView7();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW8)
			{
				OnStnClickedStaticView8();
				OnOK();
			}
			else if (nID == IDC_STATIC_VIEW9)
			{
				OnStnClickedStaticView9();
				OnOK();
			}
		}

	}
	return CAcUiDialog::PreTranslateMessage(pMsg);
}
