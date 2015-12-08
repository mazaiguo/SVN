//-----------------------------------------------------------------------------
//----- DlgDrawEquipment.cpp : Implementation of CDlgDrawEquipment
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDrawEquipment.h"
#include "Global.h"
#include "BlkInfo.h"
#include "DlgSQLDB.h"
#include "OperateReg.h"
#include "ArxDbgUtils.h"
#include "MyDrawEntity.h"
#include "MyEditEntity.h"
#include "MakeBlkFile.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDrawEquipment, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDrawEquipment, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDCANCEL, &CDlgDrawEquipment::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgDrawEquipment::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DATA, &CDlgDrawEquipment::OnBnClickedButtonData)
	ON_BN_CLICKED(IDC_RADIO_HENGXIANG, &CDlgDrawEquipment::OnBnClickedRadioHengxiang)
	ON_BN_CLICKED(IDC_RADIO_ZONGXIANG, &CDlgDrawEquipment::OnBnClickedRadioZongxiang)
	ON_BN_CLICKED(IDC_CHECK_FIX, &CDlgDrawEquipment::OnBnClickedCheckFix)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgDrawEquipment::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_RADIO_DOWN, &CDlgDrawEquipment::OnBnClickedRadioDown)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, &CDlgDrawEquipment::OnBnClickedRadioRight)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY, &CDlgDrawEquipment::OnBnClickedCheckDisplay)
	ON_BN_CLICKED(IDC_RADIO_UP, &CDlgDrawEquipment::OnBnClickedRadioUp)
	ON_BN_CLICKED(IDC_RADIO_LEFT, &CDlgDrawEquipment::OnBnClickedRadioLeft)
	ON_CBN_SELCHANGE(IDC_COMBO_WEIHAO, &CDlgDrawEquipment::OnCbnSelchangeComboWeihao)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CDlgDrawEquipment::OnRBClick)
	ON_COMMAND(ID_32771, &CDlgDrawEquipment::OnAddBlock)
	ON_COMMAND(ID_32772, &CDlgDrawEquipment::OnDelBlock)
END_MESSAGE_MAP()


bool addBlockToPath(CString strPath)
{
	TCHAR val[512];
	CString strValue;
	int nRet = acedGetString(0, _T("\n请输入要新建的块名"), val);
	if (nRet != RTNORM)
	{
		return false;
	}
	strValue = val;
	CString strFileName = strPath + strValue + _T(".dwg");
	BOOL bRet = gGlobal.IsFileExist(strFileName);
	if (bRet)
	{
		AfxMessageBox(_T("该块名已存在，请重命名"));
		return false;
	}
	CMakeBlkFile mFile;
	mFile.SetFileName(strFileName);
	if (!mFile.Save())
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
CDlgDrawEquipment::CDlgDrawEquipment (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDrawEquipment::IDD, pParent, hInstance) , m_bZongxiang(FALSE)
, m_bFixed(FALSE)
, m_pDb(NULL)
, m_strFName(_T(""))
, m_strStandard(_T(""))
, m_strPipe(_T(""))
, m_strCount(_T(""))
, m_strDefine(_T(""))
, m_nPosition(0)
, m_strWeihao(_T(""))
{
	m_bIsModified = false;
	m_InsertPt.set(0, 0, 0);
	m_strWeihao = gGlobal.GetIniValue(_T("插入设备"), _T("位数"));
	m_strBlkName = gGlobal.GetIniValue(_T("插入设备"), _T("块名"));
	m_strPipe = gGlobal.GetIniValue(_T("插入设备"), _T("管道名"));
	m_strCount = gGlobal.GetIniValue(_T("插入设备"), _T("位号"));
	m_bZongxiang = (BOOL)gGlobal.GetIniValue(_T("插入设备"), _T("横向"), 0);
	m_bFixed = (BOOL)gGlobal.GetIniValue(_T("插入设备"), _T("固定"), 0);
	m_strDefine = gGlobal.GetIniValue(_T("插入设备"), _T("自定义"));
	m_nPosition = (int)gGlobal.GetIniValue(_T("插入设备"), _T("文字位置"), 0);
	m_bIsDisplay = (BOOL)gGlobal.GetIniValue(_T("插入设备"), _T("显示"), 0);
	m_strIniFile = gGlobal.GetAppPath() + _T("设备\\") + m_strBlkName + _T(".txt");
	m_blkFile.SetFileName(m_strIniFile);
	bool bRet = m_blkFile.Read();
	if (bRet)
	{
		CDataCell pCell = m_blkFile.GetData();
		m_baseInfo = m_blkFile.SetDataCell(pCell);
		m_strFName = m_baseInfo.GetFName();
		m_strStandard = m_baseInfo.GetBeizhu();
		SetRegInfo();
	}
	else
	{
		m_strFName = _T("");
		m_strStandard = _T("");
		m_baseInfo.SetFName(_T(""));
		m_baseInfo.SetFNumber(_T(""));
		m_baseInfo.SetBeizhu(_T(""));
		m_baseInfo.SetCailiao(_T(""));
		COperateReg Reg;
		CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
		Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NAME"), _T(""));
		Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MARK"), _T(""));
		Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("STANDARD"), _T(""));
		Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MATERIAL"), _T(""));
	}
	ClearDatabase();
}

CDlgDrawEquipment::~CDlgDrawEquipment()
{
	ClearDatabase();
}
//-----------------------------------------------------------------------------
void CDlgDrawEquipment::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Radio(pDX, IDC_RADIO_HENGXIANG, m_bZongxiang);
	DDX_Check(pDX, IDC_CHECK_FIX, m_bFixed);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	//DDX_Text(pDX, IDC_EDIT_NAME, m_strFName);
	//DDX_Text(pDX, IDC_EDIT_TYPE, m_strStandard);
	DDX_Text(pDX, IDC_EDIT_PREWH, m_strPipe);
	DDX_Text(pDX, IDC_EDIT_NUM, m_strCount);
	DDX_Check(pDX, IDC_CHECK_DISPLAY, m_bIsDisplay);
	DDX_Text(pDX, IDC_EDIT_DEFINE, m_strDefine);
	DDX_Radio(pDX, IDC_RADIO_UP, m_nPosition);
	DDX_Control(pDX, IDC_COMBO_WEIHAO, m_CombWeihao);
	DDX_CBString(pDX, IDC_COMBO_WEIHAO, m_strWeihao);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDrawEquipment::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgDrawEquipment::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());
	// TODO:  在此添加额外的初始化
	m_TreeCtrl.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);
	m_strParentPath = gGlobal.GetAppPath();
	m_TreeCtrl.SetParentPath(m_strParentPath);
	m_TreeCtrl.DisPlayTree(_T("设备"), TRUE);
	if (!m_strBlkName.IsEmpty())
	{
		HTREEITEM hItem = m_TreeCtrl.GetRootItem();
		hItem = FindItem(hItem);
		if (hItem == NULL)
		{
			HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
			HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
			m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
			m_TreeCtrl.Expand(hChild, TVE_EXPAND);

			HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
			m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".dwg");
		}
		else
		{
			m_strBlkName = m_TreeCtrl.GetItemText(hItem);	
			m_TreeCtrl.SelectItem(hItem); //设置选中结点
			m_strFileName = m_TreeCtrl.GetFullPath(hItem) + _T(".dwg");
		}
	}
	else
	{
		HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
		HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
		m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
		m_TreeCtrl.Expand(hChild, TVE_EXPAND);

		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
		m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".dwg");
	}
	
	InitDatabase();
	InitGraphList();
	OnBnClickedCheckFix();
	
	CStringArray tmpArr;
	tmpArr.RemoveAll();
	CString strGanNature = gGlobal.GetIniValue(_T("插入设备"), _T("位数序列"));
	ArxDbgUtils::GetCommaFromString(strGanNature, tmpArr);
	for (int i=0; i<tmpArr.GetSize(); i++)
	{
		m_CombWeihao.AddString(tmpArr.GetAt(i));
	}
	int nCur = m_CombWeihao.FindString(-1, m_strWeihao);
	if (nCur = -1)
	{
		nCur = 0;
	}
	m_CombWeihao.SetCurSel(nCur);

	m_strCount = ArxDbgUtils::AddZeroForNumber(m_strCount, m_strWeihao);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDrawEquipment::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnCancel();
}

void CDlgDrawEquipment::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_baseInfo.GetFNumber().IsEmpty())//判定没有Fitemid的块都用空数据状态
	{
		AfxMessageBox(_T("请使用导入数据将数据导入"));
		return;
	}
	BeginEditorCommand();
	/*if (!m_bIsModified)
	{
		int nRet = acedGetPoint(NULL, _T("\n请选择一点"), asDblArray(m_InsertPt));
		if (nRet != RTNORM)
		{
			CancelEditorCommand();
			return;
		}
	}*/

	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(m_strCount);
	GetDlgItem(IDC_EDIT_PREWH)->GetWindowText(m_strPipe);
	//GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strFName);
	//GetDlgItem(IDC_EDIT_TYPE)->GetWindowText(m_strStandard);
	GetDlgItem(IDC_EDIT_DEFINE)->GetWindowText(m_strDefine);

	HTREEITEM hCurSel = m_TreeCtrl.GetSelectedItem();
	m_strBlkName = m_TreeCtrl.GetItemText(hCurSel);	
	CBlkInfo blkInfo;
	blkInfo.SetInsertPt(m_InsertPt);
	blkInfo.SetBlkName(m_strBlkName);
	blkInfo.SetFilePath(m_strFileName);
	blkInfo.SetBlkType(_T("设备"));

	CString strText;
	strText = m_strPipe + m_strCount;
	m_strIniFile = gGlobal.GetAppPath() + _T("设备\\") + m_strBlkName + _T(".txt");
	//m_baseInfo.SetChartNum(strText);
	CBlkInsert blkInsert(blkInfo, m_baseInfo);
	blkInsert.SetText(strText);
	blkInsert.SetZongxiang(m_bZongxiang);
	blkInsert.SetModified(m_bIsModified);
	//////////////////////////////////////////////////////////////////////////
	blkInsert.SetPosition(m_nPosition);
	blkInsert.SetDrawText(m_bIsDisplay);
	CString strFName;
	CString strStandard = ArxDbgUtils::GetNameAndStandard(m_strFName, strFName);
	blkInsert.SetStandard(strStandard);
	blkInsert.SetDefine(m_strDefine);
	//////////////////////////////////////////////////////////////////////////
	if (blkInsert.Insert())
	{
		m_Exts = blkInsert.GetObjExts();
		CDataCell pCell;
		/*pCell.SetFName(m_baseInfo.GetFName());
		pCell.SetFErpCls(m_baseInfo.GetFErpCls());
		pCell.SetF107(m_baseInfo.GetDanzhong());
		pCell.SetF105(m_baseInfo.GetCailiao());
		pCell.SetFItemId(m_baseInfo.GetFItemId());
		pCell.SetF106(m_baseInfo.GetBeizhu());
		pCell.SetFMaund(m_baseInfo.GetDanwei());
		pCell.SetFNumber(m_baseInfo.GetFNumber());*/
		pCell = m_blkFile.GetDataCellByDB(m_baseInfo);
		m_blkFile.SetFileName(m_strIniFile);
		m_blkFile.SetData(pCell);
		m_blkFile.Write();

		//DrawText();
		//写入ini文件
		gGlobal.SetIniValue(_T("插入设备"), _T("块名"), m_strBlkName);
		gGlobal.SetIniValue(_T("插入设备"), _T("管道名"), m_strPipe);
		if (!m_bFixed)
		{
			//对编号做递增处理
			CString strPrefix, strNext, strNum;
			int nlen = ArxDbgUtils::GetPileLengthEx(m_strCount, strPrefix, strNext);
			nlen+=1;
			strNum.Format(_T("%d"), nlen);
			m_strCount = strPrefix+ strNum + strNext;
		}
		m_strCount = ArxDbgUtils::AddZeroForNumber(m_strCount, m_strWeihao);
		gGlobal.SetIniValue(_T("插入设备"), _T("位号"), m_strCount);
		gGlobal.SetIniValue(_T("插入设备"), _T("横向"), m_bZongxiang);
		gGlobal.SetIniValue(_T("插入设备"), _T("固定"), m_bFixed);
		gGlobal.SetIniValue(_T("插入设备"), _T("自定义"), m_strDefine);
		gGlobal.SetIniValue(_T("插入设备"), _T("文字位置"), m_nPosition);
		gGlobal.SetIniValue(_T("插入设备"), _T("显示"), m_bIsDisplay);
		gGlobal.SetIniValue(_T("插入设备"), _T("位数"), m_strWeihao);
		gGlobal.WriteIniFile();
	}
	else	
	{

	}
	
	CompleteEditorCommand();
	CAcUiDialog::OnOK();
}

void CDlgDrawEquipment::OnBnClickedButtonData()//导入数据
{
	// TODO: 在此添加控件通知处理程序代码
	SetRegInfo();

	CAcModuleResourceOverride rs;
	
	CDlgSQLDB dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_baseInfo = dlg.GetData();
		if (m_baseInfo.GetFNumber().IsEmpty())
		{
			AfxMessageBox(_T("没有取到数据"));
			return;
		}
		else
		{
			m_strFName = m_baseInfo.GetFName();
			m_strStandard = m_baseInfo.GetBeizhu();
			CString strFName;
			CString strStandard = ArxDbgUtils::GetNameAndStandard(m_strFName, strFName);
			GetDlgItem(IDC_EDIT_NAME)->SetWindowText(strFName);
			GetDlgItem(IDC_EDIT_TYPE)->SetWindowText(strStandard);
			//GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_strFName);
			//GetDlgItem(IDC_EDIT_TYPE)->SetWindowText(m_strStandard);
		}
	}
	//CAcUiDialog::OnOK();
}

void CDlgDrawEquipment::OnBnClickedRadioHengxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bZongxiang = FALSE;
}

void CDlgDrawEquipment::OnBnClickedRadioZongxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bZongxiang = TRUE;
}

void CDlgDrawEquipment::OnBnClickedCheckFix()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_FIX);
	m_bFixed = (BOOL)pBtn->GetCheck();
}

void CDlgDrawEquipment::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hCurSel = m_TreeCtrl.GetSelectedItem();

	if (m_TreeCtrl.ItemHasChildren(hCurSel) && (!m_TreeCtrl.GetParentItem(hCurSel)))//父节点
	{

	}
	else if (m_TreeCtrl.ItemHasChildren(hCurSel) && m_TreeCtrl.GetParentItem(hCurSel))//所选为节点
	{

	}
	else if (!m_TreeCtrl.ItemHasChildren(hCurSel))
	{
		m_strBlkName = m_TreeCtrl.GetItemText(hCurSel);
		m_strFileName = m_TreeCtrl.GetFullPath(hCurSel) + _T(".dwg");
		InitDatabase();
		InitGraphList();
		m_strIniFile = gGlobal.GetAppPath() + _T("设备\\") + m_strBlkName + _T(".txt");
		m_blkFile.SetFileName(m_strIniFile);
		bool bRet = m_blkFile.Read();
		if (bRet)
		{
			CDataCell pCell = m_blkFile.GetData();
			m_baseInfo = m_blkFile.SetDataCell(pCell);
			m_strFName = m_baseInfo.GetFName();
			m_strStandard = m_baseInfo.GetBeizhu();

			CString strFName;
			CString strStandard = ArxDbgUtils::GetNameAndStandard(m_strFName, strFName);
			GetDlgItem(IDC_EDIT_NAME)->SetWindowText(strFName);
			GetDlgItem(IDC_EDIT_TYPE)->SetWindowText(strStandard);

			SetRegInfo();
		}
		else
		{
			m_strFName = _T("");
			m_strStandard = _T("");
			//GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_strFName);
			//GetDlgItem(IDC_EDIT_TYPE)->SetWindowText(m_strStandard);
			m_baseInfo.SetFName(_T(""));
			m_baseInfo.SetFNumber(_T(""));
			m_baseInfo.SetBeizhu(_T(""));
			m_baseInfo.SetCailiao(_T(""));
			COperateReg Reg;
			CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
			Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NAME"), _T(""));
			Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MARK"), _T(""));
			Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("STANDARD"), _T(""));
			Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MATERIAL"), _T(""));
		}
	}	
	*pResult = 0;
}


void CDlgDrawEquipment::ClearDatabase()
{
	m_gsView.DeleteAllItem();
	m_gsView.SetCurDatabase(NULL);

	if (m_pDb!=NULL)
	{
		delete m_pDb;
		m_pDb = NULL;
	}
}

void CDlgDrawEquipment::InitDatabase()
{
	ClearDatabase();
	CAcModuleResourceOverride rs;
	m_pDb = new AcDbDatabase(false);
	m_pDb->readDwgFile(m_strFileName);
}

bool CDlgDrawEquipment::InitGraphList()
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

HTREEITEM CDlgDrawEquipment::FindItem(HTREEITEM ht)
{
	CString strText;
	HTREEITEM hItem = NULL;
	m_TreeCtrl.Expand(ht,TVE_EXPAND);
	hItem = m_TreeCtrl.GetNextItem(ht,TVGN_NEXTVISIBLE);
	if(hItem != NULL)
	{
		strText = m_TreeCtrl.GetItemText(hItem);
		if (strText.CompareNoCase(m_strBlkName) == 0)
		{
			return hItem;
		}
		hItem = FindItem(hItem);
	}
	else
		return NULL;
	return hItem;
}

bool CDlgDrawEquipment::GetModified() const
{
	return m_bIsModified;
}

void CDlgDrawEquipment::SetModified(bool bModified)
{
	m_bIsModified = bModified;
}

AcGePoint3d CDlgDrawEquipment::GetInsertPt() const
{
	return m_InsertPt;
}

void CDlgDrawEquipment::SetInsertPt(AcGePoint3d pt)
{
	m_InsertPt = pt;
}


void CDlgDrawEquipment::DrawText()
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//处理文字
	CString strText;
	strText = m_strPipe + m_strCount;
	if (strText.IsEmpty())
	{
		return;
	}
	
	AcGePoint3d minPt,maxPt;
	minPt = m_Exts.minPoint();
	maxPt = m_Exts.maxPoint();
	double dLength = abs(maxPt.x - minPt.x);
	double dHeight = abs(maxPt.y - minPt.y);
	AcGePoint3d textPt;
	textPt.x = minPt.x + 1.5*dLength;
	textPt.y = minPt.y + 0.5*dHeight;
	textPt.z = 0;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//处理方向
	AcDbObjectId layerId = ArxDbgUtils::CreateNewLayer(_T("TEXT"), 6);
	if (!m_bZongxiang)
	{
		AcDbObjectId textId = MyDrawEntity::DrawText(textPt, strText, 2.23, AcDbObjectId::kNull, AcDb::kTextLeft, AcDb::kTextBase);
		MyEditEntity::openEntChangeLayer(textId, layerId);
	}
	else
	{
		AcDbObjectId textId = MyDrawEntity::DrawText(textPt, strText, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
		textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
		MyEditEntity::openEntChangeLayer(textId, layerId);
	}

}


void CDlgDrawEquipment::SetRegInfo()
{
	COperateReg Reg;
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NAME"), m_baseInfo.GetFName());
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MARK"), m_baseInfo.GetFNumber());
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("STANDARD"), m_baseInfo.GetBeizhu());
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MATERIAL"), m_baseInfo.GetCailiao());
}


void CDlgDrawEquipment::OnBnClickedRadioDown()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 2;
}

void CDlgDrawEquipment::OnBnClickedRadioRight()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 3;	
}
void CDlgDrawEquipment::OnBnClickedRadioUp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 0;
}

void CDlgDrawEquipment::OnBnClickedRadioLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 1;
}

void CDlgDrawEquipment::OnBnClickedCheckDisplay()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DISPLAY);
	m_bIsDisplay = (BOOL)pBtn->GetCheck();
}


void CDlgDrawEquipment::OnCbnSelchangeComboWeihao()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_CombWeihao.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombWeihao.SetCurSel(nCursel);
	m_CombWeihao.GetLBText(m_CombWeihao.GetCurSel(), m_strWeihao);
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(m_strCount);
	m_strCount = ArxDbgUtils::AddZeroForNumber(m_strCount, m_strWeihao);
	GetDlgItem(IDC_EDIT_NUM)->SetWindowText(m_strCount);
}

void CDlgDrawEquipment::OnRBClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint point;

	GetCursorPos(&point);

	CPoint pointInTree = point;

	m_TreeCtrl.ScreenToClient(&pointInTree);

	HTREEITEM item;

	UINT flag = TVHT_ONITEM;

	item = m_TreeCtrl.HitTest(pointInTree, &flag);

	if(item != NULL)

	{

		m_TreeCtrl.SelectItem(item);

		CMenu menu;

		menu.LoadMenu(IDR_MENU1);

		//menu.EnableMenuItem(ID_32771, MF_GRAYED);
		//menu.EnableMenuItem(ID_32772, MF_GRAYED);

		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | 

			TPM_RIGHTBUTTON, point.x, point.y, this, NULL);

	}

	//CRect rc;
	//m_TreeCtrl.GetWindowRect(rc);

	//if (PtInRect(rc, point))
	//{
	//	CMenu menu ;//定义下面要用到的cmenu对象  
	//	menu.LoadMenu(IDR_MENU1);//装载自定义的右键菜单  
	//	CPoint tmpPoint = point;
	//	::ScreenToClient(m_TreeCtrl, &tmpPoint);
	//	if (m_TreeCtrl.HitTest(tmpPoint)==NULL)
	//	{
	//		menu.EnableMenuItem(ID_32771, MF_GRAYED);
	//		menu.EnableMenuItem(ID_32772, MF_GRAYED);
	//	}
	//	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this); //在指定位置显示弹出菜单
	//}
}

void CDlgDrawEquipment::OnAddBlock()
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
	m_strBlkName = m_TreeCtrl.GetItemText(hItem);	
	m_TreeCtrl.SelectItem(hItem); //设置选中结点
	CString strPath = m_TreeCtrl.GetPath(hItem);
	if (m_TreeCtrl.ItemHasChildren(hItem) && (!m_TreeCtrl.GetParentItem(hItem)))//父节点
	{
		strPath = strPath + m_strBlkName + _T("\\");
	}
	else if (m_TreeCtrl.ItemHasChildren(hItem) && m_TreeCtrl.GetParentItem(hItem))//所选为节点
	{
		strPath = strPath + m_strBlkName + _T("\\");
	}
	BeginEditorCommand();
	if (!addBlockToPath(strPath))
	{
		CancelEditorCommand();
		return;
	}
	m_TreeCtrl.DisPlayTree(_T("设备"), TRUE);
	hItem = m_TreeCtrl.GetRootItem();
	hItem = FindItem(hItem);
	if (hItem == NULL)
	{
		HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
		HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
		m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
		m_TreeCtrl.Expand(hChild, TVE_EXPAND);

		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
		m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".dwg");
	}
	else
	{
		m_strBlkName = m_TreeCtrl.GetItemText(hItem);	
	}
	CompleteEditorCommand();
}

void CDlgDrawEquipment::OnDelBlock()
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
	m_strBlkName = m_TreeCtrl.GetItemText(hItem);	
	m_TreeCtrl.SelectItem(hItem); //设置选中结点
	m_strFileName = m_TreeCtrl.GetFullPath(hItem) + _T(".dwg");
	ClearDatabase();
	BOOL bIsOk = ::DeleteFile(m_strFileName);
	if (!bIsOk)
	{
		CString strTmp;
		strTmp.Format(_T("%s文件处于占用状态，请先关闭文件"), m_strFileName);
		AfxMessageBox(strTmp);
	}
	m_TreeCtrl.DisPlayTree(_T("设备"), TRUE);
	hItem = m_TreeCtrl.GetRootItem();
	hItem = FindItem(hItem);
	if (hItem == NULL)
	{
		HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
		HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
		m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
		m_TreeCtrl.Expand(hChild, TVE_EXPAND);

		HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
		m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".dwg");
	}
	else
	{
		m_strBlkName = m_TreeCtrl.GetItemText(hItem);	
	}
}

