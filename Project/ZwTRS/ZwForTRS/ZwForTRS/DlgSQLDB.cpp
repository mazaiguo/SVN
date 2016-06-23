

//-----------------------------------------------------------------------------
//----- DlgSQLDB.cpp : Implementation of CDlgSQLDB
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgSQLDB.h"
#include "DlgSQLDbSet.h"
#include "OperateReg.h"
#include "ArxDbgUtils.h"
#include "MyTransFunc.h"
#include "DBAdo.h"
#include "DataStore.h"
#include "Global.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgSQLDB, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgSQLDB, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	//ON_BN_CLICKED(IDC_BUTTON_WORK, &CDlgSQLDB::OnBnClickedButtonWork)
	ON_BN_CLICKED(IDC_CHECK_UPDATE, &CDlgSQLDB::OnBnClickedCheckUpdate)
	ON_BN_CLICKED(IDC_CHECK_NOTIFYUPDATE, &CDlgSQLDB::OnBnClickedCheckNotifyupdate)
	ON_BN_CLICKED(IDC_BUTTON_SETDB, &CDlgSQLDB::OnBnClickedButtonSetdb)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgSQLDB::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDOK, &CDlgSQLDB::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSQLDB::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_STANDARD, &CDlgSQLDB::OnCbnSelchangeComboStandard)
	ON_CBN_SELCHANGE(IDC_COMBO_MATERIAL, &CDlgSQLDB::OnCbnSelchangeComboMaterial)
	//ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CDlgSQLDB::OnBnClickedButtonDefault)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlgSQLDB::OnNMClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CDlgSQLDB::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgSQLDB::OnBnClickedButtonClear)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CDlgSQLDB::OnLvnKeydownList1)
	ON_WM_CHAR()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CDlgSQLDB::OnGetdispinfoList)
	ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST1, &CDlgSQLDB::OnOdfinditemList)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST1, &CDlgSQLDB::OnOdcachehintList)
	ON_EN_CHANGE(IDC_EDIT_MARK, &CDlgSQLDB::OnEnChangeEditMark)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CDlgSQLDB::OnBnClickedCheckDefault)
END_MESSAGE_MAP()

//static extern int _List_Type( int col );
static int List_Type( int col )
{
	return CColorList::eColor;
}
//-----------------------------------------------------------------------------
CDlgSQLDB::CDlgSQLDB (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgSQLDB::IDD, pParent, hInstance) , m_strName(_T(""))
, m_strMark(_T(""))
, m_strStandard(_T(""))
, m_strMaterial(_T(""))
, m_bUpdate(FALSE)
, m_bNoticeUpdate(FALSE)
, m_nCount(0)
, m_strJiliang(_T(""))
{
	GetInfoFromReg();
	m_nDefaultItem = 0;
	//clearData(m_Data);
	m_F105Map.clear();
	m_F106Map.clear();
	m_dTime = 1000*gGlobal.GetIniValue(_T("Server"), _T("时间"), 0);
	m_nCount = 1;
	m_strJiliang = _T("");
	m_nCurMark = 0;
}

CDlgSQLDB::~CDlgSQLDB()
{
	//clearData(m_Data);
	m_F105Map.clear();
	m_F106Map.clear();
}

//-----------------------------------------------------------------------------
void CDlgSQLDB::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MARK, m_strMark);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_STANDARD, m_CombStandard);
	DDX_CBString(pDX, IDC_COMBO_STANDARD, m_strStandard);
	DDX_Control(pDX, IDC_COMBO_MATERIAL, m_CombMaterial);
	DDX_CBString(pDX, IDC_COMBO_MATERIAL, m_strMaterial);
	DDX_Check(pDX, IDC_CHECK_UPDATE, m_bUpdate);
	DDX_Check(pDX, IDC_CHECK_NOTIFYUPDATE, m_bNoticeUpdate);
	DDX_Check(pDX, IDC_CHECK_DEFAULT, m_bIsDeFault);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDV_MinMaxInt(pDX, m_nCount, 0, 1000000000);
	DDX_Text(pDX, IDC_EDIT_JILIANG, m_strJiliang);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgSQLDB::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgSQLDB::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	UpdateData(FALSE);

	// TODO:  在此添加额外的初始化
	CenterWindow(acedGetAcadDwgView());
	//m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);
	ListView_SetExtendedListViewStyle(m_ListCtrl.m_hWnd, LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn( 0, _T("物料代码"), LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn( 1, _T("名称"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn( 2, _T("执行标准"), LVCFMT_LEFT, 120);
	m_ListCtrl.InsertColumn( 3, _T("材料"), LVCFMT_LEFT, 80);
	//m_ListCtrl.InsertColumn( 4, _T("零件类型"), LVCFMT_LEFT, 60);
	m_ListCtrl.InsertColumn( 4, _T("单重"), LVCFMT_LEFT, 80);
	//m_ListCtrl.InsertColumn( 6, _T("单位"), LVCFMT_LEFT, 50);
	//m_ListCtrl.InsertColumn( 7, _T("代号"), LVCFMT_LEFT, 50);


	//m_ListCtrl.SetColumnWidth( 0, 150);
	//m_ListCtrl.SetColumnWidth( 1, 280);
	//m_ListCtrl.SetColumnWidth( 2, 100);
	//m_ListCtrl.SetColumnWidth( 3, 60);
	//m_ListCtrl.SetColumnWidth( 4, 60);
	//m_ListCtrl.SetColumnWidth( 5, 80);
	//m_ListCtrl.SetColumnWidth( 6, 50);
	//m_ListCtrl.SetColumnWidth( 7, 50);

	if (m_bUpdate)
	{
		OnBnClickedButtonUpdate();
		if (!GetDataFromFile())
		{
			m_strName = _T("");
			m_strMark = _T("");
			m_strStandard = _T("");
			m_strMaterial = _T("");
			return FALSE;
		}	
	}
	else
	{
#ifdef UNICODE
		DWORD start_time = GetTickCount();
		DWORD end_time;
#endif
		
		if (!GetDataFromFile())
		{
			AfxMessageBox(_T("数据没有与服务器连接起来，请处理服务器设置"));
			m_strName = _T("");
			m_strMark = _T("");
			m_strStandard = _T("");
			m_strMaterial = _T("");
			return FALSE;
		}	
#ifdef UNICODE
		end_time = GetTickCount();
		int nTimeCount = end_time - start_time;
		CString strTmp;
		strTmp.Format(_T("时间为%d"), nTimeCount);
		AfxMessageBox(strTmp);
#endif
	}


	if (!KMP())
	{
		AfxMessageBox(_T("没有找到匹配数据"));
		return FALSE;
	}

	//m_ListCtrl.SetItemCount(m_Data.size());

	SetCurCell();

	////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//添加数据
	CDataCell pCell;
	pCell = m_TmpData.at(m_nDefaultItem);
	m_BlkInfo.SetFName(pCell.GetFName());
	m_BlkInfo.SetFErpCls(pCell.GetFErpCls());
	m_BlkInfo.SetDanzhong(pCell.GetF107());
	m_BlkInfo.SetCailiao(pCell.GetF105());
	m_BlkInfo.SetFItemId(pCell.GetFItemId());
	m_BlkInfo.SetBeizhu(pCell.GetF106());
	m_BlkInfo.SetDanwei(pCell.GetFMaund());
	m_BlkInfo.SetFNumber(pCell.GetFNumber());

	int nLen = m_CombMaterial.GetDroppedWidth();
	m_CombMaterial.SetDroppedWidth(1.2*nLen);

	m_CombStandard.SetDroppedWidth(1.2*nLen);
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



//void CDlgSQLDB::OnBnClickedButtonWork()//应用
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//UpdateData(TRUE);
//	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
//	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
//	if ((m_strName.IsEmpty())
//		&&(m_strMark.IsEmpty())
//		&&(m_strStandard.IsEmpty())
//		&&(m_strMaterial.IsEmpty()))
//	{
//		AfxMessageBox(_T("请填写数据"));
//		return;
//	}
//	m_TmpData.clear();
//	m_ListCtrl.DeleteAllItems();
//	if (!KMP())
//	{
//		AfxMessageBox(_T("没有找到匹配数据"));
//		return;
//	}
//	if (m_ListCtrl.GetItemCount() > 0)
//	{
//		COLORREF color=RGB(255,255,255);
//		m_ListCtrl.SetAllItemColor(m_ListCtrl.GetItemCount(), color);
//		m_ListCtrl.SetFocus();
//		m_ListCtrl.SetItemState(0, LVIS_FOCUSED, LVIS_FOCUSED);
//		color=RGB(255,0,0);
//
//		m_ListCtrl.SetItemColor(0, color);
//	}
//	CDataCell* pCell = NULL;
//	pCell = m_TmpData.at(m_nDefaultItem);
//	m_BlkInfo.SetFName(pCell.GetFName());
//	m_BlkInfo.SetFErpCls(pCell.GetFErpCls());
//	m_BlkInfo.SetDanzhong(pCell.GetF107());
//	m_BlkInfo.SetCailiao(pCell.GetF105());
//	m_BlkInfo.SetFItemId(pCell.GetFItemId());
//	m_BlkInfo.SetBeizhu(pCell.GetF106());
//	m_BlkInfo.SetDanwei(pCell.GetFMaund());
//	m_BlkInfo.SetFNumber(pCell.GetFNumber());
//	SaveInfoToReg();
//}

void CDlgSQLDB::OnBnClickedCheckUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_ListCtrl.GetItemCount();
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_UPDATE);
	m_bUpdate= (BOOL)pBtn->GetCheck();
}

void CDlgSQLDB::OnBnClickedCheckNotifyupdate()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_ListCtrl.GetItemCount();
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_NOTIFYUPDATE);
	m_bNoticeUpdate = (BOOL)pBtn->GetCheck();
}

void CDlgSQLDB::OnBnClickedButtonSetdb()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);

	CAcModuleResourceOverride rs;
	CDlgSQLDbSet dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_F105Map.clear();
		m_F106Map.clear();
		if (!GetDataFromFile())
		{
			AfxMessageBox(_T("数据没有与服务器连接起来，请处理服务器设置"));
			m_strName = _T("");
			m_strMark = _T("");
			m_strStandard = _T("");
			m_strMaterial = _T("");
			return ;
		}

		if (!KMP())
		{
			AfxMessageBox(_T("没有找到匹配数据"));
			return ;
		}
		if (m_ListCtrl.GetItemCount() > 0)
		{
			m_ListCtrl.SetFocus();
			m_nDefaultItem = 0;
			m_ListCtrl.SetItemState(0, LVIS_FOCUSED, LVIS_FOCUSED);
			//COLORREF color=RGB(255,0,0);

			//m_ListCtrl.SetItemColor(0, color);
		}
		////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//添加数据
		/*	CDataCell* pCell = NULL;
		pCell = m_TmpData.at(m_nDefaultItem);
		m_BlkInfo.SetFName(pCell.GetFName());
		m_BlkInfo.SetFErpCls(pCell.GetFErpCls());
		m_BlkInfo.SetDanzhong(pCell.GetF107());
		m_BlkInfo.SetCailiao(pCell.GetF105());
		m_BlkInfo.SetFItemId(pCell.GetFItemId());
		m_BlkInfo.SetBeizhu(pCell.GetF106());
		m_BlkInfo.SetDanwei(pCell.GetFMaund());
		m_BlkInfo.SetFNumber(pCell.GetFNumber());*/
	}
}

void CDlgSQLDB::OnBnClickedButtonUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	CDBAdo dbAdo;
	dbAdo.SetConnectionString(_T("SQLOLEDB.1"), m_strAddress, m_strSQLName, m_strUserName, m_strPassWord);
	bool bRet = dbAdo.CreateInstance();
	if (!bRet)
	{
		return;
	}
	bRet = dbAdo.OpenConnection();
	if (!bRet)
	{	
		CString strTmp = dbAdo.m_strConnect;
		AfxMessageBox(strTmp);
		return;
	}
	bRet = dbAdo.IsConnecting();
	if (!bRet)
	{
		AfxMessageBox(_T("没有连接到数据库,请配置数据库信息"));
		return;
	}

	CString strSQL;
	strSQL.Format(_T("select t_ICItemCore.fitemid, t_ICItemCore.Fname, t_ICItemCore.fnumber, t_item.FName as F_105, t_ICItemCustom.F_106, t_ICItemCustom.F_108,t_ICItemDesign.FMaund,t_ICItemBase.FErpClsID from t_ICItemCore LEFT JOIN t_ICItemCustom on t_ICItemCore.fitemid = t_ICItemCustom.fitemid left join t_ICItemDesign on t_ICItemCustom.fitemid = t_ICItemDesign.fitemid left join t_ICItemBase on t_ICItemDesign.fitemid = t_ICItemBase.fitemid  left join  t_Item on  t_Item.FItemID =t_ICItemCustom.F_105 where  t_ICItemCore.FDeleted<>'1' and LEFT(t_ICItemCore.FNumber,1)<>'f'	and LEFT(t_ICItemCore.FNumber,1)<>'6'"));
	bRet = dbAdo.OpenConnection();
	bRet = dbAdo.OpenRecordset(strSQL);
	CString strFname, strStandard,strfItem,strf_105, strF_107, strNumber, strFErpClsID,strFMaund;
	vector<CDataCell> dataSet;
	dataSet.clear();
	if (dbAdo.IsRecordsetOpened())
	{
		dbAdo.MoveToFirst();
		while(!dbAdo.IsEndRecordset())
		{
			double dMark = 0.0;	
			CDataCell pCell;
			if (dbAdo.GetFieldValue(_T("FItemID"), dMark))
			{
				ArxDbgUtils::doubleToStr(dMark, strfItem, -1, 0);
				pCell.SetFItemId(strfItem);
			}
			if (dbAdo.GetFieldValue(_T("Fname"), strFname))
			{
				pCell.SetFName(strFname);
			}
			if (dbAdo.GetFieldValue(_T("FNumber"), strNumber))
			{
				pCell.SetFNumber(strNumber);
			}
			if (dbAdo.GetFieldValue(_T("F_105"), strf_105))
			{
				//ArxDbgUtils::doubleToStr(dMark, strf_105, -1, 0);
				pCell.SetF105(strf_105);
			}
			if (dbAdo.GetFieldValue(_T("F_106"), strStandard))
			{
				pCell.SetF106(strStandard);
			}
			if (dbAdo.GetFieldValue(_T("F_108"), dMark))
			{
				ArxDbgUtils::doubleToStr(dMark, strF_107, -1, 0);
				pCell.SetF107(strF_107);
			}
			if (dbAdo.GetFieldValue(_T("FMaund"), dMark))
			{
				ArxDbgUtils::doubleToStr(dMark, strFMaund, -1, 0);
				pCell.SetFMaund(strFMaund);
			}
			if (dbAdo.GetFieldValue(_T("FErpClsID"), dMark))
			{
				ArxDbgUtils::doubleToStr(dMark, strFErpClsID, -1, 0);
				pCell.SetFErpCls(strFErpClsID);
			}
			dataSet.push_back(pCell);

			dbAdo.MoveToNext();

		}
	}
	COperateCsv csv;
	csv.SetFileName(gGlobal.GetCsvFile());
	if (csv.IsFileExit())
	{
		::DeleteFile(csv.GetFileName());
	}
	csv.SetData(dataSet);
	csv.Write();
	AfxMessageBox(_T("测试成功"));
}

void CDlgSQLDB::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (m_BlkInfo.GetFItemId().IsEmpty())
	{
	AfxMessageBox(_T("请设置选择到的数据"));
	return;
	}*/
	//////////////////////////////////////////////////////////////////////////
	int nCount = m_ListCtrl.GetItemCount();
	if (nCount == 0)
	{
		AfxMessageBox(_T("没有数据，请重新查询"));
		return;
	}
	if (m_nDefaultItem == -1)
	{
		AfxMessageBox(_T("没有选到数据"));
		return;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		/*m_TmpData.clear();
		for (int i=0; i<nCount; i++)
		{
			CDataCell pCell;

			CString strMark ;
			CString strName;
			CString strStandard;
			CString strMaterial;
			CString strLingjianType;
			CString strFNumber;
			CString strF107;
			CString strFMaund;

			strFNumber = m_ListCtrl.GetItemText(i, 0);
			strName = m_ListCtrl.GetItemText(i, 1);
			strStandard = m_ListCtrl.GetItemText(i, 2);
			strMaterial = m_ListCtrl.GetItemText(i, 3);
			strLingjianType = m_ListCtrl.GetItemText(i, 4);
			strF107 = m_ListCtrl.GetItemText(i, 5);
			strFMaund =	m_ListCtrl.GetItemText(i, 6);
			strMark = m_ListCtrl.GetItemText(i, 7);

			pCell.SetFItemId(strMark);
			pCell.SetFName(strName);
			pCell.SetF106(strStandard);
			pCell.SetF105(strMaterial);
			pCell.SetFErpCls(strLingjianType);
			pCell.SetFNumber(strFNumber);
			pCell.SetF107(strF107);
			pCell.SetFMaund(strFMaund);
			m_TmpData.push_back(pCell);
		}*/
		CString strCount;
		GetDlgItem(IDC_EDIT_JILIANG)->GetWindowText(m_strJiliang);
		GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(strCount);
		m_nCount = ArxDbgUtils::StringToInt(strCount);
		CDataCell pCell;
		pCell = m_TmpData.at(m_nDefaultItem);
		CString strTmp;
		if (!m_strJiliang.IsEmpty())
		{
			CString strName;
			strName = pCell.GetFName();
			strTmp = _T(" L=") + m_strJiliang;
			strName += strTmp;
			m_BlkInfo.SetFName(strName);
		}
		else
		{
			m_BlkInfo.SetFName(pCell.GetFName());
		}
		m_BlkInfo.SetFErpCls(pCell.GetFErpCls());
		/*if (m_nCount != 1)
		{*/
		CString strF107;
		double dF107;
		dF107 = ArxDbgUtils::StringToDouble(pCell.GetF107());
		if (!m_strJiliang.IsEmpty())
		{
			double dJiliang = ArxDbgUtils::StringToDouble(m_strJiliang);
			dF107 = dF107*dJiliang/1000;
		}
		ArxDbgUtils::doubleToStr(dF107, strF107);
		m_BlkInfo.SetDanzhong(strF107);
		double dZongzhong;
		dZongzhong = m_nCount*dF107;
		ArxDbgUtils::doubleToStr(dZongzhong, strTmp);
		m_BlkInfo.SetZongzhong(strTmp);//添加总重
		/*}
		else
		{
		m_BlkInfo.SetDanzhong(pCell.GetF107());
		}*/
		m_BlkInfo.SetCailiao(pCell.GetF105());
		m_BlkInfo.SetFItemId(pCell.GetFItemId());
		m_BlkInfo.SetBeizhu(pCell.GetF106());
		m_BlkInfo.SetDanwei(pCell.GetFMaund());
		m_BlkInfo.SetFNumber(pCell.GetFNumber());
		m_BlkInfo.SetShuliang(strCount);//添加数量的数据

		//////////////////////////////////////////////////////////////////////////
		//填写csv文件
		CBlkRefCsv m_blkFile;//csv文件
		CString strFile = gGlobal.GetAppPath() + _T("tmp.txt");
		m_blkFile.SetFileName(strFile);
		m_blkFile.SetSpecialString(m_BlkInfo);
	}
	SaveInfoToReg();

	//////////////////////////////////////////////////////////////////////////
	CAcUiDialog::OnOK();
}

void CDlgSQLDB::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnCancel();
}


void CDlgSQLDB::InsertEmpty(CDataCell pCell)
{
	CString strMark = pCell.GetFItemId();
	CString strName = pCell.GetFName();
	CString strStandard = pCell.GetF106();
	CString strMaterial = pCell.GetF105();
	CString strLingjianType = pCell.GetFErpCls();
	CString strFNumber = pCell.GetFNumber();
	CString strF107 = pCell.GetF107();
	CString strFMaund = pCell.GetFMaund();

	int nItem = m_ListCtrl.GetItemCount();

	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
	//m_ListCtrl.SetItemText(nItem, 7, strMark);
	m_ListCtrl.SetItemText(nItem, 1, strName);
	m_ListCtrl.SetItemText(nItem, 2, strStandard);
	m_ListCtrl.SetItemText(nItem, 3, strMaterial);
	//m_ListCtrl.SetItemText(nItem, 4, strLingjianType);
	m_ListCtrl.SetItemText(nItem, 0, strFNumber);
	m_ListCtrl.SetItemText(nItem, 4, strF107);
	//m_ListCtrl.SetItemText(nItem, 6, strFMaund);
}

CDataBaseInfo CDlgSQLDB::GetData()
{
	return m_BlkInfo;
}

void CDlgSQLDB::SaveInfoToReg()
{
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	COperateReg Reg;
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("ADDRESS"), m_strAddress);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("SQLNAME"), m_strSQLName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("USERNAME"), m_strUserName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("PASSWORD"), m_strPassWord);

	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NAME"), m_strName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MARK"), m_strMark);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("STANDARD"), m_strStandard);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MATERIAL"), m_strMaterial);
	//Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("LINGJIAN"), m_strLingjianType);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("UPDATE"), (DWORD)m_bUpdate);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NOTICEUPDATE"), (DWORD)m_bNoticeUpdate);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DEFAULT"), (DWORD)m_bIsDeFault);
	CTime tm(1990, 1, 1, 0, 0, 0);
	tm = CTime::GetCurrentTime();
	int nYear,nMonth,nDay;
	nYear = tm.GetYear();
	nMonth = tm.GetMonth();
	nDay = tm.GetDay();
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("YEAR"), (DWORD)nYear);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MONTH"), (DWORD)nMonth);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DAY"), (DWORD)nDay);
	
}

void CDlgSQLDB::GetInfoFromReg()
{
	m_strAddress = GetStringFromReg(_T("ADDRESS"));
	m_strSQLName = GetStringFromReg(_T("SQLNAME"));
	m_strUserName = GetStringFromReg(_T("USERNAME"));
	m_strPassWord = GetStringFromReg(_T("PASSWORD"));
	m_bIsDeFault = (BOOL)GetDwordFromReg(_T("DEFAULT"));
	if (m_bIsDeFault)
	{
		m_strName = GetStringFromReg(_T("NAME"));
		m_strMark = GetStringFromReg(_T("MARK"));
		m_strStandard = GetStringFromReg(_T("STANDARD"));
		m_strMaterial = GetStringFromReg(_T("MATERIAL"));
		m_strLingjianType = GetStringFromReg(_T("LINGJIAN"));
		m_bUpdate = (BOOL)GetDwordFromReg(_T("UPDATE"));
		m_bNoticeUpdate = (BOOL)GetDwordFromReg(_T("NOTICEUPDATE"));
	}
}

CString CDlgSQLDB::GetStringFromReg(LPCTSTR strKey)
{
	CString strResult;
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	TCHAR ResStr[MAX_PATH];
	COperateReg Reg;
	if (Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, _T(""), ResStr) > 0)
	{
		strResult = ResStr;
	}
	return strResult;
}

int CDlgSQLDB::GetDwordFromReg(LPCTSTR strKey) const
{
	int nRet = 0;
	CString strResult;
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	DWORD dRes = 0;
	COperateReg Reg;
	nRet = Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, dRes);
	return nRet;
}

bool CDlgSQLDB::GetDataFromFile()
{
	COperateCsv csv;
	csv.SetFileName(gGlobal.GetCsvFile());
	if (!csv.Read())
	{
		return false;
	}
	m_Data = csv.GetData();
	m_F106Map = csv.GetStandardMap();
	m_F105Map = csv.GetCailiaoMap();
	//map<CString, CString> m_F106Map = csv.GetStandardMap();
	//map<CString, CString> m_F105Map = csv.GetCailiaoMap();
	//for (map<CString, CString>::iterator iter = m_F105Map.begin();
	//	iter != m_F105Map.end();
	//	++iter)
	//{
	//	m_F105Vec.push_back(iter->first);
	//}

	//for (map<CString, CString>::iterator itr = m_F106Map.begin();
	//	itr != m_F106Map.end();
	//	++itr)
	//{
	//	m_F106Vec.push_back(itr->first);
	//}
	return true;
}

void CDlgSQLDB::OnCbnSelchangeComboStandard()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_CombStandard.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombStandard.SetCurSel(nCursel);
	m_CombStandard.GetLBText(m_CombStandard.GetCurSel(), m_strStandard);

	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
	GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
	m_ListCtrl.DeleteAllItems();
	if (!getListData())
	{
		AfxMessageBox(_T("没有找到匹配数据"));
	}
	else
	{
		SetCurCell();
	}
}

void CDlgSQLDB::OnCbnSelchangeComboMaterial()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListCtrl.DeleteAllItems();
	int nCursel = m_CombMaterial.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombMaterial.SetCurSel(nCursel);
	m_CombMaterial.GetLBText(m_CombMaterial.GetCurSel(), m_strMaterial);
	//GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	//GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
	//GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
	if (!getListData())
	{
		AfxMessageBox(_T("没有找到匹配数据"));
	}
	else
	{
		SetCurCell();
	}
}

bool CDlgSQLDB::KMP(bool bIsKMP)
{
	bool bRet = false;
	CString strMaterial,strStandard,strName,strDaihao;
	m_F105Map.clear();
	m_F106Map.clear();
	m_ListCtrl.DeleteAllItems();
	m_TmpData.clear();
	DWORD start_time = GetTickCount();
	DWORD end_time;

	int nTimeCount = 0;
	CDataCell pCell;
	for (vector<CDataCell>::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		pCell = *iter;
		strMaterial = pCell.GetF105();
		strStandard = pCell.GetF106();
		strName = pCell.GetFName();
		strDaihao = pCell.GetFNumber();
		end_time = GetTickCount();
		nTimeCount += end_time - start_time;
		start_time = end_time;
		//
		if (nTimeCount > m_dTime)
		{
			AfxMessageBox(_T("超过设定时间，不再查找,请输入更多筛选条件"));
			break;
		}
		if ((MyTransFunc::KMP(strMaterial, m_strMaterial) >= 0)
			&&(MyTransFunc::KMP(strStandard, m_strStandard) >= 0))
		{
			if ((!m_strName.IsEmpty())
				&&(!m_strMark.IsEmpty()))
			{
				if (MyTransFunc::KMP(strName, m_strName) >= 0)
				{
					/*int nLen = m_strMark.GetLength();
					CString strTmp = strDaihao.Left(nLen);*/				
					if (MyTransFunc::KMP(strDaihao, m_strMark) >= 0)
					{
						m_TmpData.push_back(pCell);
						m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
						m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));
						
						continue;
					}
					else
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
			else if ((!m_strName.IsEmpty())&&(m_strMark.IsEmpty()))
			{
				if (MyTransFunc::KMP(strName, m_strName) >= 0)
				{
					m_TmpData.push_back(pCell);
					m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
					m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));

					continue;
				}
				else
				{
					continue;
				}
			}
			else if ((m_strName.IsEmpty())&&(!m_strMark.IsEmpty()))
			{
				/*int nLen = m_strMark.GetLength();
				CString strTmp = strDaihao.Left(nLen);*/
				if (MyTransFunc::KMP(strDaihao, m_strMark) >= 0)
				{
					m_TmpData.push_back(pCell);
					m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
					m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));

					continue;
				}
				else
				{
					continue;
				}
			}
			else
			{
				m_TmpData.push_back(pCell);
				m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
				m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));

				continue;
			}

		}
		else
		{
			continue;
		}
	}

	bRet = true;
	if (bIsKMP)
	{
		//对数据进行排序
		AddDataToCombo();
		if (m_TmpData.size() > 0)
		{
			int count = m_TmpData.size();
			if ((count > 1)&&(count < 200))
			{
				for (int i = 1; i < count; i++)  
				{  
					for (int j = count - 1; j >= i; j--) 
					{
						CString str1,str2;
						str1 = m_TmpData.at(j).GetFNumber();
						str2 = m_TmpData.at(j-1).GetFNumber();
						CString tmpStr1, tmpStr2;
						int m_nComp = 0;

						int index = str1.FindOneOf(_T("0123456789"));
						if(index!=-1)
							tmpStr1 = str1.Right(str1.GetLength()-index);
						index = str2.FindOneOf(_T("0123456789"));
						if(index!=-1)
							tmpStr2 = str2.Right(str2.GetLength()-index);

						tmpStr1 = tmpStr1.SpanIncluding(_T("0123456789"));
						tmpStr2 = tmpStr2.SpanIncluding(_T("0123456789"));

						if((tmpStr1==_T("")) && (tmpStr2==_T("")))
							m_nComp = str1.CompareNoCase(str2);
						else
						{
							int num1 = _ttoi(tmpStr1);
							int num2 = _ttoi(tmpStr2);

							tmpStr1 = str1.SpanExcluding(_T("0123456789"));
							tmpStr2 = str2.SpanExcluding(_T("0123456789"));

							if(tmpStr1 == tmpStr2)
							{
								if(num1 > num2)
									m_nComp = 1;
								else
									if(num1 < num2)
										m_nComp = -1;
									else
										m_nComp = str1.CompareNoCase(str2);
							}
							else
								m_nComp = str1.CompareNoCase(str2);			
						}
						CDataCell temp;
						if (m_nComp < 0)
						{
							temp = m_TmpData.at(j - 1);  
							m_TmpData.at(j - 1) = m_TmpData.at(j);  
							m_TmpData.at(j) = temp;  
						}
						//temp = NULL;
					}
				}
			}

			/*int i=0;
			for (vector<CDataCell*>::iterator itr = m_TmpData.begin();
				itr != m_TmpData.end();
				++itr)
			{	
				if (i>500)
				{
					break;
				}
				pCell = *itr;
				InsertEmpty(pCell);
				i++;
				}*/		
			m_ListCtrl.SetItemCount(m_TmpData.size());
		}	
		else
		{
			AfxMessageBox(_T("没有找到匹配数据"));
		}
	}
	return bRet;
}


BOOL CDlgSQLDB::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((WM_KEYDOWN == pMsg-> message) && 
		(VK_RETURN ==  pMsg-> wParam))
	{
		if (  GetDlgItem(IDC_EDIT_NAME) == GetFocus())
		{
			GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
			GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
			GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
			GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
			m_ListCtrl.DeleteAllItems();
			if (!KMP())
			{
				AfxMessageBox(_T("没有找到匹配数据"));
			}
			else
			{
				SetCurCell();
			}
			return FALSE;
		}
		if (GetDlgItem(IDC_EDIT_MARK) == GetFocus())
		{
			GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
			GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
			GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
			GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
			m_ListCtrl.DeleteAllItems();
			if (!getListData())
			{
				AfxMessageBox(_T("没有找到匹配数据"));
			}
			else
			{
				SetCurCell();
			}
			return FALSE;
		}
		CEdit *pEdit = (CEdit*)m_CombStandard.GetWindow(GW_CHILD);
		if(pMsg->hwnd == pEdit->m_hWnd )
		{
			GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
			GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
			GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
			GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
			m_ListCtrl.DeleteAllItems();
			if (!getListData())
			{
				AfxMessageBox(_T("没有找到匹配数据"));
			}
			else
			{
				SetCurCell();
			}
			return FALSE;
		}
		CEdit *pEdit2 = (CEdit*)m_CombMaterial.GetWindow(GW_CHILD);
		if(pMsg->hwnd == pEdit2->m_hWnd )
		{
			GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
			GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
			GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
			GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
			m_ListCtrl.DeleteAllItems();
			if (!getListData())
			{
				AfxMessageBox(_T("没有找到匹配数据"));
			}
			else
			{
				SetCurCell();
			}
			return FALSE;
		}
	}
	//else if ((WM_CHAR == pMsg->message))
	//{
	//	UINT nChar = pMsg->wParam;
	//	CString dd;
	//	if (nChar==0xD)
	//	{
	//		dd="";
	//	}
	//	CString ss;
	//	ss.Format(_T("%c"),nChar);
	//	dd=dd+ss;
	//
	//	CString strText;
	//	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(strText);

	//	if (GetDlgItem(IDC_EDIT_MARK) == GetFocus())
	//	{
	//		m_nCurMark++;
	//		m_strMark += ss;

	//		if (m_nCurMark > 2)
	//		{
	//			GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	//			//GetDlgItem(IDC_EDIT_MARK)->SetWindowText(m_strMark);
	//			GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
	//			GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
	//			m_ListCtrl.DeleteAllItems();
	//			if (!KMP())
	//			{
	//				AfxMessageBox(_T("没有找到匹配数据"));
	//			}
	//			else
	//			{
	//				SetCurCell();
	//			}
	//		}
	//		
	//		return FALSE;
	//	}
	//	return FALSE;
	//}
	//else if ((WM_KEYDOWN == pMsg-> message) && 
	//	(VK_BACK ==  pMsg-> wParam))
	//{
	//	if (GetDlgItem(IDC_EDIT_MARK) == GetFocus())
	//	{
	//		/*GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
	//		GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
	//		GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
	//		GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
	//		m_ListCtrl.DeleteAllItems();
	//		if (!KMP(false))
	//		{

	//		}
	//		else
	//		{
	//			SetCurCell();
	//		}*/
	//		m_nCurMark--;
	//		CString strTmp = m_strMark;
	//		m_strMark = strTmp.Left(strTmp.GetLength() - 1);
	//		if (!KMP())
	//		{
	//			AfxMessageBox(_T("没有找到匹配数据"));
	//		}
	//		else
	//		{
	//			SetCurCell();
	//		}
	//		return FALSE;
	//	}
	//	
	//}
	//else
	//{
	//	CString strText;
	//	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(strText);
	//	if (!strText.IsEmpty())
	//	{
	//		int nMes = pMsg-> message;
	//		CString strTmp;
	//		strTmp.Format(_T("\n消息映射%d."), nMes);
	//		acutPrintf(strTmp);
	//		acutPrintf(strText);
	//	}
	//}
	return CAcUiDialog::PreTranslateMessage(pMsg);
}


void CDlgSQLDB::AddDataToCombo()
{
	m_CombMaterial.ResetContent();
	m_CombStandard.ResetContent();

	if ((m_strMark.IsEmpty())&&(m_strName.IsEmpty()))
	{

	}
	else
	{
		for (map<CString, CString>::iterator iter = m_F105Map.begin();
			iter != m_F105Map.end();
			++iter)
		{
			m_CombMaterial.AddString(iter->first);
		}
		int nCur = m_CombMaterial.FindString(-1, m_strMaterial);
		if (nCur == -1)
		{
			m_strMaterial = _T("");
			nCur = 0;
		}
		else
		{
			m_CombMaterial.SetCurSel(nCur);
			m_CombMaterial.GetLBText(m_CombMaterial.GetCurSel(), m_strMaterial);
		}



		for (map<CString, CString>::iterator itr = m_F106Map.begin();
			itr	!= m_F106Map.end();
			++itr)
		{
			m_CombStandard.AddString(itr->first);
		}

		nCur = m_CombStandard.FindString(-1, m_strStandard);
		if (nCur == -1)
		{
			m_strStandard = _T("");
			nCur = 0;
		}
		else
		{
			m_CombStandard.SetCurSel(nCur);
			m_CombStandard.GetLBText(nCur, m_strStandard);
		}
	}	
}


void CDlgSQLDB::SetCurCell()
{
	if (m_ListCtrl.GetItemCount() > 0)
	{
		m_ListCtrl.EnsureVisible(0,FALSE);//默认选中第一行
		m_ListCtrl.SetItemState(m_nDefaultItem, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);	
		m_ListCtrl.SetFocus();
	}
}

void CDlgSQLDB::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nDefaultItem = pNMListView->iItem;

	TRACE1("Item %d was Selected!\n",m_nDefaultItem);

	COLORREF color=RGB(255,255,255);
	//m_ListCtrl.SetAllItemColor(m_ListCtrl.GetItemCount(), color);

	color = RGB(255,0,0);
	m_ListCtrl.SetItemState(m_nDefaultItem, LVIS_FOCUSED, LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible(m_nDefaultItem,FALSE);//
	//m_ListCtrl.SetFocusRow(nItem);
	//m_ListCtrl.SetItemColor(m_nDefaultItem, color);
	/*m_strMark = m_ListCtrl.GetItemText(m_nDefaultItem, 0);
	m_strName = m_ListCtrl.GetItemText(m_nDefaultItem, 1);
	m_strStandard = m_ListCtrl.GetItemText(m_nDefaultItem, 2);
	m_strMaterial = m_ListCtrl.GetItemText(m_nDefaultItem, 3);
	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_strName);
	GetDlgItem(IDC_EDIT_MARK)->SetWindowText(m_strMark);
	GetDlgItem(IDC_COMBO_STANDARD)->SetWindowText(m_strStandard);
	GetDlgItem(IDC_COMBO_MATERIAL)->SetWindowText(m_strMaterial);*/
	*pResult = 0;
}

typedef struct _info
{
	CListCtrl * m_list;
	int col;
	bool asc;
}INFO;

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{ 
	CString str1,str2,strT;
	INFO* info = (INFO*)lParamSort;

	str1 = info->m_list->GetItemText((int)lParam1, info->col);
	str1.Trim();

	str2 = info->m_list->GetItemText((int)lParam2, info->col);
	str2.Trim();

	CString tmpStr1, tmpStr2;
	int m_nComp = 0;

	int index = str1.FindOneOf(_T("0123456789"));
	if(index!=-1)
		tmpStr1 = str1.Right(str1.GetLength()-index);
	index = str2.FindOneOf(_T("0123456789"));
	if(index!=-1)
		tmpStr2 = str2.Right(str2.GetLength()-index);

	tmpStr1 = tmpStr1.SpanIncluding(_T("0123456789"));
	tmpStr2 = tmpStr2.SpanIncluding(_T("0123456789"));

	if((tmpStr1==_T("")) && (tmpStr2==_T("")))
		m_nComp = str1.CompareNoCase(str2);
	else
	{
		int num1 = _ttoi(tmpStr1);
		int num2 = _ttoi(tmpStr2);

		tmpStr1 = str1.SpanExcluding(_T("0123456789"));
		tmpStr2 = str2.SpanExcluding(_T("0123456789"));

		if(tmpStr1 == tmpStr2)
		{
			if(num1 > num2)
				m_nComp = 1;
			else
				if(num1 < num2)
					m_nComp = -1;
				else
					m_nComp = str1.CompareNoCase(str2);
		}
		else
			m_nComp = str1.CompareNoCase(str2);			
	}

	return m_nComp;
}

bool asc = true;

void CDlgSQLDB::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	static int iSorted = -1; //排序列号
	if (pNMLV->iSubItem != 0)
	{
		*pResult = 0;
		return;
	}

	if (pNMLV->iSubItem == iSorted)
		asc = !asc;

	CListCtrl* m_ListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST1);

	INFO info;
	info.col = iSorted = 0;//只针对第一列排序
	info.m_list = m_ListCtrl;
	info.asc = asc;

	m_ListCtrl->SortItemsEx((PFNLVCOMPARE)CompareFunc, (LPARAM)&info);

	*pResult = 0;
}

void CDlgSQLDB::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strMark = _T("");
	m_strName = _T("");
	m_strStandard = _T("");
	m_strMaterial = _T("");
	m_nCount = 1;
	m_strJiliang = _T("");
	m_nCurMark = 0;

	////////////////////////////////////////////////////////////////
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	COperateReg Reg;
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("NAME"), m_strName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MARK"), m_strMark);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("STANDARD"), m_strStandard);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MATERIAL"), m_strMaterial);
	UpdateData(FALSE);
}

void CDlgSQLDB::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	WORD nChar = pLVKeyDow->wVKey;
	int nCount = m_ListCtrl.GetItemCount();
	if (nChar == 40)
	{
		if (m_nDefaultItem < nCount-1)
		{
			m_nDefaultItem++;
		}
	}
	if (nChar == 38)
	{
		if (m_nDefaultItem > 0)
		{
			m_nDefaultItem--;
		}
	}
	*pResult = 0;
}

bool CDlgSQLDB::getListData()
{
	bool bRet = false;
	CString strMaterial,strStandard,strName,strDaihao;
	m_F105Map.clear();
	m_F106Map.clear();
	//m_ListCtrl.DeleteAllItems();
	DWORD start_time = GetTickCount();
	DWORD end_time;
	vector<CDataCell> tmpData;
	

	int nTimeCount = 0;
	CDataCell pCell;
	if (m_TmpData.size() > 0)
	{
		for (vector<CDataCell>::iterator iter = m_TmpData.begin();
			iter != m_TmpData.end();
			++iter)
		{
			pCell = *iter;
			strMaterial = pCell.GetF105();
			strStandard = pCell.GetF106();
			strName = pCell.GetFName();
			strDaihao = pCell.GetFNumber();
			end_time = GetTickCount();
			nTimeCount += end_time - start_time;
			start_time = end_time;
			//
			if (nTimeCount > m_dTime)
			{
				AfxMessageBox(_T("超过设定时间，不再查找,请输入更多筛选条件"));
				break;
			}
			if ((MyTransFunc::KMP(strMaterial, m_strMaterial) >= 0)
				&&(MyTransFunc::KMP(strStandard, m_strStandard) >= 0))
			{
				if ((!m_strName.IsEmpty())
					&&(!m_strMark.IsEmpty()))
				{
					if (MyTransFunc::KMP(strName, m_strName) >= 0)
					{
						/*int nLen = m_strMark.GetLength();
						CString strTmp = strDaihao.Left(nLen);*/				
						if (MyTransFunc::KMP(strDaihao, m_strMark) >= 0)
						{
							tmpData.push_back(pCell);
							m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
							m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));
							continue;
						}
						else
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				else if ((!m_strName.IsEmpty())&&(m_strMark.IsEmpty()))
				{
					if (MyTransFunc::KMP(strName, m_strName) >= 0)
					{
						tmpData.push_back(pCell);
						m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
						m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));

						continue;
					}
					else
					{
						continue;
					}
				}
				else if ((m_strName.IsEmpty())&&(!m_strMark.IsEmpty()))
				{
					/*int nLen = m_strMark.GetLength();
					CString strTmp = strDaihao.Left(nLen);*/
					if (MyTransFunc::KMP(strDaihao, m_strMark) >= 0)
					{
						tmpData.push_back(pCell);
						m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
						m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));

						continue;
					}
					else
					{
						continue;
					}
				}
				else
				{
					tmpData.push_back(pCell);
					m_F105Map.insert(make_pair(pCell.GetF105(), pCell.GetF105()));
					m_F106Map.insert(make_pair(pCell.GetF106(), pCell.GetF106()));

					continue;
				}

			}
			else
			{
				continue;
			}
		}
	}
	bRet = true;
	
	AddDataToCombo();
	//对数据进行排序
	if (tmpData.size() > 0)
	{
		int count = tmpData.size();
		if ((count > 1)&&(count < 200))
		{
			for (int i = 1; i < count; i++)  
			{  
				for (int j = count - 1; j >= i; j--) 
				{
					CString str1,str2;
					str1 = tmpData.at(j).GetFNumber();
					str2 = tmpData.at(j-1).GetFNumber();
					CString tmpStr1, tmpStr2;
					int m_nComp = 0;

					int index = str1.FindOneOf(_T("0123456789"));
					if(index!=-1)
						tmpStr1 = str1.Right(str1.GetLength()-index);
					index = str2.FindOneOf(_T("0123456789"));
					if(index!=-1)
						tmpStr2 = str2.Right(str2.GetLength()-index);

					tmpStr1 = tmpStr1.SpanIncluding(_T("0123456789"));
					tmpStr2 = tmpStr2.SpanIncluding(_T("0123456789"));

					if((tmpStr1==_T("")) && (tmpStr2==_T("")))
						m_nComp = str1.CompareNoCase(str2);
					else
					{
						int num1 = _ttoi(tmpStr1);
						int num2 = _ttoi(tmpStr2);

						tmpStr1 = str1.SpanExcluding(_T("0123456789"));
						tmpStr2 = str2.SpanExcluding(_T("0123456789"));

						if(tmpStr1 == tmpStr2)
						{
							if(num1 > num2)
								m_nComp = 1;
							else
								if(num1 < num2)
									m_nComp = -1;
								else
									m_nComp = str1.CompareNoCase(str2);
						}
						else
							m_nComp = str1.CompareNoCase(str2);			
					}
					CDataCell temp ;
					if (m_nComp < 0)
					{
						temp = tmpData.at(j - 1);  
						tmpData.at(j - 1) = tmpData.at(j);  
						tmpData.at(j) = temp;  
					}
					//temp = NULL;
				}
			}
		}

		m_TmpData.clear();
		m_TmpData.insert(m_TmpData.begin(), tmpData.begin(), tmpData.end());

		/*int i=0;
		for (vector<CDataCell*>::iterator itr = tmpData.begin();
			itr != tmpData.end();
			++itr)
		{	
			if (i>500)
			{
				break;
			}
			pCell = *itr;
			InsertEmpty(pCell);
			i++;
		}*/
		m_ListCtrl.SetItemCount(m_TmpData.size());
	}	
	else
	{
		AfxMessageBox(_T("没有找到匹配数据"));
	}
	tmpData.clear();

	return bRet;
}

void CDlgSQLDB::clearData(vector<CDataCell> data)
{
	/*CDataCell* pCell = NULL;
	for (vector<CDataCell>::iterator iter = data->begin();
		iter != data->end();
		++iter)
	{
		pCell = *iter;
		if (pCell != NULL)
		{
			delete pCell;
			pCell = NULL;
		}
	}
	data->clear();*/
	data.clear();
}

void CDlgSQLDB::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CAcUiDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CDlgSQLDB::OnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	//// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	//Create a pointer to the item
	LV_ITEM* pItem= &(pDispInfo)->item;

	//Which item number?
	int itemid = pItem->iItem;

	if (m_TmpData.size() < 1)
	{
		return;
	}
	//Do the list need text information?
	if (pItem->mask & LVIF_TEXT)
	{
		CString text;

		CString strMark = m_TmpData[itemid].GetFItemId();
		CString strName = m_TmpData[itemid].GetFName();
		CString strStandard = m_TmpData[itemid].GetF106();
		CString strMaterial = m_TmpData[itemid].GetF105();
		CString strLingjianType = m_TmpData[itemid].GetFErpCls();
		CString strFNumber = m_TmpData[itemid].GetFNumber();
		CString strF107 = m_TmpData[itemid].GetF107();
		CString strFMaund = m_TmpData[itemid].GetFMaund();

		//Which column?
		if(pItem->iSubItem == 0)
		{
			//Text is name
			text = strFNumber;
		}
		else if (pItem->iSubItem == 1)
		{
			//Text is slogan
			text = strName;
		}
		else if (pItem->iSubItem == 2)
		{
			//Text is slogan
			text = strStandard;
		}
		else if (pItem->iSubItem == 3)
		{
			//Text is slogan
			text = strMaterial;
		}
		//else if (pItem->iSubItem == 4)
		//{
		//	//Text is slogan
		//	text = strLingjianType;
		//}
		else if (pItem->iSubItem == 4)
		{
			//Text is slogan
			text = strF107;
		}
		//else if (pItem->iSubItem == 6)
		//{
		//	//Text is slogan
		//	text = strFMaund;
		//}
		//else if (pItem->iSubItem == 7)
		//{
		//	text = strMark;
		//}
		//Copy the text to the LV_ITEM structure
		//Maximum number of characters is in pItem->cchTextMax
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
	}

	*pResult = 0;
}

//This functions is called when the user "writes" in the list box to find an item.
void CDlgSQLDB::OnOdfinditemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// pNMHDR has information about the item we should find
	// In pResult we should save which item that should be selected
	NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;

	/* pFindInfo->iStart is from which item we should search.
	   We search to bottom, and then restart at top and will stop
	   at pFindInfo->iStart, unless we find an item that match
	 */

	// Set the default return value to -1
	// That means we didn't find any match.
	*pResult = -1;

//	//Is search NOT based on string?
//	if( (pFindInfo->lvfi.flags & LVFI_STRING) == 0 )
//	{
//		//This will probably never happend...
//		return;
//	}
//
//	/*
//		Let's look on a sample list;
//		
//		  Name
//		  Anders
//		* Anna
//		  Annika
//		  Bob
//		  Emma
//		  Emmanuel
//
//		Anna is selected. 
//		If "A" is written, Annika should be selected.
//		If "AND" is written, Anders should be selected. 
//		If "ANNK" is written, the selection should stay on Anna.
//		If "E" is written, Emma should be selected.
//
//    */
//
//	//This is the string we search for
	CString searchstr = pFindInfo->lvfi.psz;

//	TRACE(_T("Find: %s\n"), searchstr);
	
	int startPos = pFindInfo->iStart;
	//Is startPos outside the list (happens if last item is selected)
	if(startPos >= m_ListCtrl.GetItemCount())
		startPos = 0;

	int currentPos=startPos;
	
	//Let's search...
	do
	{		
		//Do this word begins with all characters in searchstr?
		if( _tcsnicmp(m_TmpData[currentPos].GetFNumber(), searchstr, searchstr.GetLength()) == 0)
		{
			//Select this item and stop search.
			*pResult = currentPos;
			break;
		}

		//Go to next item
		currentPos++;

		//Need to restart at top?
		if(currentPos >= m_ListCtrl.GetItemCount())
			currentPos = 0;

	//Stop if back to start
	}while(currentPos != startPos);	
	m_nDefaultItem = currentPos;
}

//This is called to give you a chance to catch data. Useless in most cases :-)
void CDlgSQLDB::OnOdcachehintList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;

/*	TRACE(	_T("Chache item %d to item %d\n"),
			pCacheHint->iFrom,
			pCacheHint->iTo );*/

	//... cache the data pCacheHint->iFrom to pCacheHint->iTo ...

	*pResult = 0;
}



void CDlgSQLDB::OnEnChangeEditMark()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CAcUiDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_MARK)->GetWindowText(m_strMark);
	m_nCurMark = m_strMark.GetLength();

	if (m_nCurMark > 2)
	{
		GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
		//GetDlgItem(IDC_EDIT_MARK)->SetWindowText(m_strMark);
		GetDlgItem(IDC_COMBO_STANDARD)->GetWindowText(m_strStandard);
		GetDlgItem(IDC_COMBO_MATERIAL)->GetWindowText(m_strMaterial);
		m_ListCtrl.DeleteAllItems();
		if (!KMP())
		{
			AfxMessageBox(_T("没有找到匹配数据"));
		}
		else
		{
			SetCurCell();
		}
	}

	return ;
}


void CDlgSQLDB::OnBnClickedCheckDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_ListCtrl.GetItemCount();
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DEFAULT);
	m_bIsDeFault = (BOOL)pBtn->GetCheck();
}
