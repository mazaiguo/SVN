

//-----------------------------------------------------------------------------
//----- DlgDrawInstrument.cpp : Implementation of CDlgDrawInstrument
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDrawInstrument.h"
#include "Global.h"
#include "DlgSQLDB.h"
#include "ArxDbgUtils.h"
#include "MyEditEntity.h"
#include "MyDrawEntity.h"
#include "MyTransFunc.h"
#include "OperateReg.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDrawInstrument, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDrawInstrument, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_NAME, &CDlgDrawInstrument::OnCbnSelchangeComboName)
	ON_BN_CLICKED(IDC_CHECK_FIX, &CDlgDrawInstrument::OnBnClickedCheckFix)
	ON_BN_CLICKED(IDC_RADIO_HENGXIANG, &CDlgDrawInstrument::OnBnClickedRadioHengxiang)
	ON_BN_CLICKED(IDC_BUTTON_DATA, &CDlgDrawInstrument::OnBnClickedButtonData)
	ON_BN_CLICKED(IDOK, &CDlgDrawInstrument::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgDrawInstrument::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_CHECK_UP, &CDlgDrawInstrument::OnBnClickedCheckUp)
	//ON_BN_CLICKED(IDC_CHECK_LEFT, &CDlgDrawInstrument::OnBnClickedCheckLeft)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgDrawInstrument::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_RADIO_ZONGXIANG, &CDlgDrawInstrument::OnBnClickedRadioZongxiang)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY, &CDlgDrawInstrument::OnBnClickedCheckDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgDrawInstrument::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_RADIO_UP, &CDlgDrawInstrument::OnBnClickedRadioUp)
	ON_BN_CLICKED(IDC_RADIO_LEFT, &CDlgDrawInstrument::OnBnClickedRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_DOWN, &CDlgDrawInstrument::OnBnClickedRadioDown)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, &CDlgDrawInstrument::OnBnClickedRadioRight)
	ON_CBN_SELCHANGE(IDC_COMBO_WEIHAO, &CDlgDrawInstrument::OnCbnSelchangeComboWeihao)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CDlgDrawInstrument::OnRBClick)
	ON_COMMAND(ID_32771, &CDlgDrawInstrument::OnAddBlock)
	ON_COMMAND(ID_32772, &CDlgDrawInstrument::OnDelBlock)
END_MESSAGE_MAP()

extern bool addBlockToPath(CString strPath);
//-----------------------------------------------------------------------------
CDlgDrawInstrument::CDlgDrawInstrument (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDrawInstrument::IDD, pParent, hInstance) , m_bFixed(FALSE)
, m_bZongxiang(FALSE)
//, m_bUp(FALSE)
//, m_bLeft(FALSE)
, m_pDb(NULL)
, m_bIsDisplay(FALSE)
, m_nPosition(0)
, m_strType(_T(""))
, m_strWeihao(_T(""))
{
	m_bIsModified = false;
	m_InsertPt.set(0, 0, 0);
	GetInfoFromFile();
	m_strIniFile = gGlobal.GetAppPath() + _T("仪表\\") + m_strBlkName + _T(".txt");
	m_blkFile.SetFileName(m_strIniFile);
	bool bRet = m_blkFile.Read();
	if (bRet)
	{
		CDataCell pCell = m_blkFile.GetData();
		m_baseInfo = m_blkFile.SetDataCell(pCell);
		SetRegInfo();
	}
	else
	{
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
	m_blkId = AcDbObjectId::kNull;
	ClearDatabase();
}

CDlgDrawInstrument::~CDlgDrawInstrument()
{
	ClearDatabase();
}
//-----------------------------------------------------------------------------
void CDlgDrawInstrument::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Check(pDX, IDC_CHECK_FIX, m_bFixed);
	DDX_Radio(pDX, IDC_RADIO_HENGXIANG, m_bZongxiang);
	//DDX_Check(pDX, IDC_CHECK_UP, m_bUp);
	//DDX_Check(pDX, IDC_CHECK_LEFT, m_bLeft);
	DDX_Control(pDX, IDC_COMBO_NAME, m_CombName);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PREWH, m_strPipe);
	DDX_Text(pDX, IDC_EDIT_NUM, m_strCount);
	DDX_Text(pDX, IDC_EDIT_XUHAO, m_strNo);
	DDX_Check(pDX, IDC_CHECK_DISPLAY, m_bIsDisplay);
	DDX_Radio(pDX, IDC_RADIO_UP, m_nPosition);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ComboType);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strType);
	DDX_Control(pDX, IDC_COMBO_WEIHAO, m_CombWeihao);
	DDX_CBString(pDX, IDC_COMBO_WEIHAO, m_strWeihao);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDrawInstrument::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgDrawInstrument::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	UpdateData(FALSE);

	CenterWindow(acedGetAcadDwgView());
	// TODO:  在此添加额外的初始化
	m_TreeCtrl.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);
	m_strParentPath = gGlobal.GetAppPath();
	m_TreeCtrl.SetParentPath(m_strParentPath);
	m_TreeCtrl.DisPlayTree(_T("仪表"), TRUE);
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

	/*HTREEITEM hRoot = m_TreeCtrl.GetRootItem();

	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hRoot);
	m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
	m_TreeCtrl.Expand(hChild, TVE_EXPAND);

	HTREEITEM hNextItem = m_TreeCtrl.GetChildItem(hChild);
	m_strFileName = m_TreeCtrl.GetFullPath(hNextItem) + _T(".dwg");*/

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CString strGanType = gGlobal.GetIniValue(_T("插入仪器"), _T("仪器类型序列"));
	CStringArray tmpArr;
	tmpArr.RemoveAll();
	ArxDbgUtils::GetCommaFromString(strGanType, tmpArr);
	for (int i=0; i<tmpArr.GetSize(); i++)
	{
		m_CombName.AddString(tmpArr.GetAt(i));
	}
	m_strName = gGlobal.GetIniValue(_T("插入仪器"), _T("仪器类型"));
	int nCur = m_CombName.FindString(-1, m_strName);
	if (nCur == -1)
	{
		nCur = 0;
	}
	m_CombName.SetCurSel(nCur);
	m_CombName.GetLBText(nCur, m_strName);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	strGanType = gGlobal.GetIniValue(_T("插入仪器"), _T("阀门类型序列"));
	tmpArr.RemoveAll();
	ArxDbgUtils::GetCommaFromString(strGanType, tmpArr);
	for (int i=0; i<tmpArr.GetSize(); i++)
	{
		m_ComboType.AddString(tmpArr.GetAt(i));
	}
	m_strType = gGlobal.GetIniValue(_T("插入仪器"), _T("阀门类型"));
	nCur = m_ComboType.FindString(-1, m_strType);
	if (nCur == -1)
	{
		nCur = 0;
	}
	m_ComboType.SetCurSel(nCur);
	m_ComboType.GetLBText(nCur, m_strType);
	//////////////////////////////////////////////////////////////////////////

	tmpArr.RemoveAll();
	CString strGanNature = gGlobal.GetIniValue(_T("插入仪器"), _T("位数序列"));
	ArxDbgUtils::GetCommaFromString(strGanNature, tmpArr);
	for (int i=0; i<tmpArr.GetSize(); i++)
	{
		m_CombWeihao.AddString(tmpArr.GetAt(i));
	}
	nCur = m_CombWeihao.FindString(-1, m_strWeihao);
	if (nCur = -1)
	{
		nCur = 0;
	}
	m_CombWeihao.SetCurSel(nCur);

	m_strCount = ArxDbgUtils::AddZeroForNumber(m_strCount, m_strWeihao);
	//////////////////////////////////////////////////////////////////////////
	InitDatabase();
	InitGraphList();

	//////////////////////////////////////////////////////////////////////////
	//填写列表框的数据

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HTREEITEM CDlgDrawInstrument::FindItem(HTREEITEM ht)
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

void CDlgDrawInstrument::OnCbnSelchangeComboName()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_CombName.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombName.SetCurSel(nCursel);
	m_CombName.GetLBText(m_CombName.GetCurSel(), m_strName);
}

void CDlgDrawInstrument::OnBnClickedCheckFix()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_FIX);
	m_bFixed = (BOOL)pBtn->GetCheck();
}

void CDlgDrawInstrument::OnBnClickedRadioHengxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bZongxiang = FALSE;
}

void CDlgDrawInstrument::OnBnClickedButtonData()
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
	}
}

void CDlgDrawInstrument::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_baseInfo.GetFNumber().IsEmpty())//判定没有Fitemid的块都用空数据状态
	{
		AfxMessageBox(_T("请使用导入数据将数据导入"));
		return;
	}
	GetDlgItem(IDC_EDIT_XUHAO)->GetWindowText(m_strNo);
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(m_strCount);
	GetDlgItem(IDC_EDIT_PREWH)->GetWindowText(m_strPipe);
	BeginEditorCommand();
	int nUserI = 0;
	gGlobal.GetVar(_T("useri1"), &nUserI);
	if (nUserI == 0)
	{
		m_bIsModified = true;
	}
	else
	{
		m_bIsModified = false;
	}	
	AcGePoint3d startPt,endPt;
	if (m_bIsModified)
	{
		int nRet = acedGetPoint(NULL, _T("\n请选择一点"), asDblArray(startPt));
		if (nRet != RTNORM)
		{
			CancelEditorCommand();
			return;
		}

		int nOrthMode = 1;
		gGlobal.GetVar(_T("ORTHOMODE"), &nOrthMode);
		if (nOrthMode!= 1)
		{
			gGlobal.SetVar(_T("ORTHOMODE"), 1);
		}
		nRet = acedGetPoint(asDblArray(startPt), _T("\n请选择方向"), asDblArray(endPt));
		if (nRet != RTNORM)
		{
			gGlobal.SetVar(_T("ORTHOMODE"), nOrthMode);
			CancelEditorCommand();
			return;
		}
		gGlobal.SetVar(_T("ORTHOMODE"), nOrthMode);
		AcGeVector3d vec = endPt - startPt;
		AcGeVector2d vec2d = vec.convert2d(AcGePlane::kXYPlane);
		double dAng = vec2d.angle();
		double dLength = GetLength();
		acutPolar(asDblArray(startPt), dAng, dLength, asDblArray(m_InsertPt));		
	}

	HTREEITEM hCurSel = m_TreeCtrl.GetSelectedItem();
	m_strBlkName = m_TreeCtrl.GetItemText(hCurSel);	
	CBlkInfo blkInfo;
	blkInfo.SetInsertPt(m_InsertPt);
	blkInfo.SetBlkName(m_strBlkName);
	blkInfo.SetFilePath(m_strFileName);
	blkInfo.SetBlkType(_T("仪器"));

	m_strIniFile = gGlobal.GetAppPath() + _T("仪表\\") + m_strBlkName + _T(".txt");

	CString strText;
	strText = m_strPipe + m_strCount;
	CBlkInsert blkInsert(blkInfo, m_baseInfo);
	blkInsert.SetZongxiang(m_bZongxiang);
	blkInsert.SetModified(m_bIsModified);
	blkInsert.SetText(strText);
	blkInsert.SetName(m_strName);
	blkInsert.SetNo(m_strNo);
	blkInsert.SetTmpPoint(startPt);
	//////////////////////////////////////////////////////////////////////////
	blkInsert.SetPosition(m_nPosition);
	blkInsert.SetDrawText(m_bIsDisplay);
	//////////////////////////////////////////////////////////////////////////
	if (blkInsert.Insert())
	{
		m_Exts = blkInsert.GetObjExts();
		m_blkId = blkInsert.GetObjectId();
		m_InsertPt = blkInsert.GetPosition();

		CDataCell pCell = m_blkFile.GetDataCellByDB(m_baseInfo);
		m_blkFile.SetData(pCell);
		m_blkFile.SetFileName(m_strIniFile);
		m_blkFile.Write();

		//DrawText();
		//if (m_bIsModified)
		//{
		//	AcDbObjectId lineId = MyDrawEntity::DrawLine(startPt, m_InsertPt);
		//	//做一次排序处理
		//	MyTransFunc::DuiXiangPaiXu(lineId, false);
		//}
		//else
		//{
		//	DrawLine();
		//}
		SaveInfoToFile();
	}


	CompleteEditorCommand();
	CAcUiDialog::OnOK();
}

void CDlgDrawInstrument::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnCancel();
}

//void CDlgDrawInstrument::OnBnClickedCheckUp()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_UP);
//	m_bUp = (BOOL)pBtn->GetCheck();
//}
//
//void CDlgDrawInstrument::OnBnClickedCheckLeft()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_LEFT);
//	m_bLeft = (BOOL)pBtn->GetCheck();
//}

void CDlgDrawInstrument::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_strBlkName = m_TreeCtrl.GetItemText(hCurSel);
		InitDatabase();
		InitGraphList();

		m_strIniFile = gGlobal.GetAppPath() + _T("仪表\\") + m_strBlkName + _T(".txt");
		m_blkFile.SetFileName(m_strIniFile);
		bool bRet = m_blkFile.Read();
		if (bRet)
		{
			CDataCell pCell = m_blkFile.GetData();
			m_baseInfo = m_blkFile.SetDataCell(pCell);
			SetRegInfo();
		}
		else
		{
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

void CDlgDrawInstrument::ClearDatabase()
{
	m_gsView.DeleteAllItem();
	m_gsView.SetCurDatabase(NULL);

	if (m_pDb!=NULL)
	{
		delete m_pDb;
	}
}

void CDlgDrawInstrument::InitDatabase()
{
	ClearDatabase();
	CAcModuleResourceOverride rs;
	m_pDb = new AcDbDatabase(false);
	m_pDb->readDwgFile(m_strFileName);
}

bool CDlgDrawInstrument::InitGraphList()
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

bool CDlgDrawInstrument::GetModified() const
{
	return m_bIsModified;
}

void CDlgDrawInstrument::SetModified(bool bModified)
{
	m_bIsModified = bModified;
}

AcGePoint3d CDlgDrawInstrument::GetInsertPt() const
{
	return m_InsertPt;
}

void CDlgDrawInstrument::SetInsertPt(AcGePoint3d pt)
{
	m_InsertPt = pt;
}

void CDlgDrawInstrument::OnBnClickedRadioZongxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bZongxiang = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CDlgDrawInstrument::DrawText()
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
	//绘制管道位号
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
	
	//绘制名称
	AcDbObjectId textId1 = MyDrawEntity::DrawText(m_InsertPt, m_strName, 2.23, AcDbObjectId::kNull, AcDb::kTextCenter, AcDb::kTextBase);
	textId1 = MyEditEntity::openEntChangeLayer(textId1, layerId);
	MyTransFunc::DuiXiangPaiXu(textId1, true);
	//绘制流程图序号
	AcDbObjectId textId2 = MyDrawEntity::DrawText(m_InsertPt, m_strNo, 2.23, AcDbObjectId::kNull, AcDb::kTextCenter, AcDb::kTextTop);
	textId2 = MyEditEntity::openEntChangeLayer(textId2, layerId);
	MyTransFunc::DuiXiangPaiXu(textId2, true);

}

void CDlgDrawInstrument::DrawLine()
{
	if (m_blkId.isNull())
	{
		return;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, m_blkId, AcDb::kForRead) != Acad::eOk)
	{
		return;
	}
	AcGePoint3d insertPt;
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
		insertPt = pBlkRef->position();
		pBlkRef->close();
	}
	else
	{
		pEnt->close();
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//绘制线
	AcGePoint3d endPt;
	int nRet = acedGetPoint(asDblArray(insertPt), _T("\n请指定终点："), asDblArray(endPt));
	if (nRet != RTNORM)
	{
		return;
	}
	AcDbObjectId lineId = MyDrawEntity::DrawLine(insertPt, endPt);
	//做一次排序处理
	MyTransFunc::DuiXiangPaiXu(lineId, false);
}

void CDlgDrawInstrument::SaveInfoToFile()
{
	gGlobal.SetIniValue(_T("插入仪器"), _T("名称"), m_strName);
	gGlobal.SetIniValue(_T("插入仪器"), _T("序号"), m_strNo);
	gGlobal.SetIniValue(_T("插入仪器"), _T("块名"), m_strBlkName);
	gGlobal.SetIniValue(_T("插入仪器"), _T("管道名"), m_strPipe);
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
	gGlobal.SetIniValue(_T("插入设备"), _T("位数"), m_strWeihao);
	gGlobal.SetIniValue(_T("插入仪器"), _T("位号"), m_strCount);
	gGlobal.SetIniValue(_T("插入仪器"), _T("固定"), m_bFixed);
	//gGlobal.SetIniValue(_T("插入仪器"), _T("向右"), m_bLeft);
	//gGlobal.SetIniValue(_T("插入仪器"), _T("向下"), m_bUp);
	gGlobal.SetIniValue(_T("插入仪器"), _T("文字位置"), m_nPosition);
	gGlobal.SetIniValue(_T("插入仪器"), _T("显示"), m_bIsDisplay);
	gGlobal.SetIniValue(_T("插入仪器"), _T("阀门类型"), m_strType);
	gGlobal.WriteIniFile();
}

void CDlgDrawInstrument::GetInfoFromFile()
{
	m_strName = gGlobal.GetIniValue(_T("插入仪器"), _T("名称"));
	m_strNo = gGlobal.GetIniValue(_T("插入仪器"), _T("序号"));
	m_strBlkName = gGlobal.GetIniValue(_T("插入仪器"), _T("块名"));
	m_strPipe = gGlobal.GetIniValue(_T("插入仪器"), _T("管道名"));
	m_strCount = gGlobal.GetIniValue(_T("插入仪器"), _T("位号"));
	m_bFixed = (BOOL)gGlobal.GetIniValue(_T("插入仪器"), _T("固定"), 0);
	//m_bLeft = (BOOL)gGlobal.GetIniValue(_T("插入仪器"), _T("向右"), 0);
	//m_bUp = (BOOL)gGlobal.GetIniValue(_T("插入仪器"), _T("向下"), 0);
	m_nPosition = (int)gGlobal.GetIniValue(_T("插入仪器"), _T("文字位置"), 0);
	m_bIsDisplay = (BOOL)gGlobal.GetIniValue(_T("插入仪器"), _T("显示"), 0);
	m_strWeihao = gGlobal.GetIniValue(_T("插入仪器"), _T("位数"));

}

void CDlgDrawInstrument::SetRegInfo()
{
	COperateReg Reg;
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NAME"), m_baseInfo.GetFName());
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MARK"), m_baseInfo.GetFNumber());
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("STANDARD"), m_baseInfo.GetBeizhu());
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MATERIAL"), m_baseInfo.GetCailiao());
}



void CDlgDrawInstrument::OnBnClickedCheckDisplay()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DISPLAY);
	m_bIsDisplay = (BOOL)pBtn->GetCheck();
}

void CDlgDrawInstrument::OnCbnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_ComboType.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_ComboType.SetCurSel(nCursel);
	m_ComboType.GetLBText(m_ComboType.GetCurSel(), m_strType);
}

void CDlgDrawInstrument::OnBnClickedRadioUp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 0;
}

void CDlgDrawInstrument::OnBnClickedRadioLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 1;
}

void CDlgDrawInstrument::OnBnClickedRadioDown()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 2;
}

void CDlgDrawInstrument::OnBnClickedRadioRight()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosition = 3;
}

double CDlgDrawInstrument::GetLength() const
{
	double dLength = 0;
	int nCount = 0;
	if (m_strType.CompareNoCase(_T("单阀")) == 0)
	{
		dLength = gGlobal.GetIniValue(_T("插入仪器"), _T("单阀距离"), 0);
		nCount = 1;
	}
	else if (m_strType.CompareNoCase(_T("双阀")) == 0)
	{
		dLength = gGlobal.GetIniValue(_T("插入仪器"), _T("双阀距离"), 0);
		nCount = 2;
	}
	else if (m_strType.CompareNoCase(_T("无阀")) == 0)
	{
		dLength = gGlobal.GetIniValue(_T("插入仪器"), _T("无阀距离"), 0);
	}
	gGlobal.SetIniValue(_T("插入仪器"), _T("阀门"), nCount);
	gGlobal.WriteIniFile();
	return dLength;
}

void CDlgDrawInstrument::OnCbnSelchangeComboWeihao()
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
void CDlgDrawInstrument::OnRBClick(NMHDR *pNMHDR, LRESULT *pResult)
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

	
}

void CDlgDrawInstrument::OnAddBlock()
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
	m_TreeCtrl.DisPlayTree(_T("仪表"), TRUE);
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

void CDlgDrawInstrument::OnDelBlock()
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
	m_TreeCtrl.DisPlayTree(_T("仪表"), TRUE);
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