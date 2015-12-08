

//-----------------------------------------------------------------------------
//----- DlgSQLDbSet.cpp : Implementation of CDlgSQLDbSet
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgSQLDbSet.h"
#include "OperateReg.h"
#include "ArxDbgUtils.h"
#include "DataStore.h"
#include "Global.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgSQLDbSet, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgSQLDbSet, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgSQLDbSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CDlgSQLDbSet::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgSQLDbSet::CDlgSQLDbSet (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgSQLDbSet::IDD, pParent, hInstance) , m_strAddress(_T(""))
, m_strUserName(_T(""))
, m_strPassWord(_T(""))
{
	GetInfoFromReg();
	m_bSuccess = false;
}

CDlgSQLDbSet::~CDlgSQLDbSet()
{
};
//-----------------------------------------------------------------------------
void CDlgSQLDbSet::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassWord);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgSQLDbSet::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgSQLDbSet::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	UpdateData(FALSE);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSQLDbSet::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(m_strAddress);
	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(m_strUserName);
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(m_strPassWord);
	if ((m_strAddress.IsEmpty())||(m_strUserName.IsEmpty())||(m_strPassWord.IsEmpty()))
	{
		AfxMessageBox(_T("数据不能为空"));
		return;
	}
	CDBAdo dbAdo;
	dbAdo.SetConnectionString(_T("SQLOLEDB.1"), m_strAddress, _T("AIS20120813194456"), m_strUserName, m_strPassWord);
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
	//strSQL.Format(_T("select t_ICItemCore.fitemid, t_ICItemCore.Fname, t_ICItemCore.fnumber, t_ICItemCustom.F_105, t_ICItemCustom.F_106, t_ICItemCustom.F_107,t_ICItemDesign.FMaund,t_ICItemBase.FErpClsID from t_ICItemCore LEFT JOIN t_ICItemCustom on t_ICItemCore.fitemid = t_ICItemCustom.fitemid left join t_ICItemDesign on t_ICItemCustom.fitemid = t_ICItemDesign.fitemid left join t_ICItemBase on t_ICItemDesign.fitemid = t_ICItemBase.fitemid"));
	//strSQL.Format(_T("select t_ICItemCore.fitemid, t_ICItemCore.Fname, t_ICItemCore.fnumber, t_ICItemCustom.F_105, t_ICItemCustom.F_106, t_ICItemCustom.F_107,t_ICItemDesign.FMaund,t_ICItemBase.FErpClsID from t_ICItemCore LEFT JOIN t_ICItemCustom on t_ICItemCore.fitemid = t_ICItemCustom.fitemid left join t_ICItemDesign on t_ICItemCustom.fitemid = t_ICItemDesign.fitemid left join t_ICItemBase on t_ICItemDesign.fitemid = t_ICItemBase.fitemid where  t_ICItemCore.FDeleted<>'1' and LEFT(t_ICItemCore.FNumber,1)<>'f'	and LEFT(t_ICItemCore.FNumber,1)<>'6'"));
	strSQL.Format(_T("select t_ICItemCore.fitemid, t_ICItemCore.Fname, t_ICItemCore.fnumber, t_item.FName as F_105, t_ICItemCustom.F_106, t_ICItemCustom.F_108,t_ICItemDesign.FMaund,t_ICItemBase.FErpClsID from t_ICItemCore LEFT JOIN t_ICItemCustom on t_ICItemCore.fitemid = t_ICItemCustom.fitemid left join t_ICItemDesign on t_ICItemCustom.fitemid = t_ICItemDesign.fitemid left join t_ICItemBase on t_ICItemDesign.fitemid = t_ICItemBase.fitemid  left join  t_Item on  t_Item.FItemID =t_ICItemCustom.F_105 where  t_ICItemCore.FDeleted<>'1' and LEFT(t_ICItemCore.FNumber,1)<>'f'	and LEFT(t_ICItemCore.FNumber,1)<>'6'"));
	//strSQL.Format(_T("Create Table #Temptb (Fname VARCHAR(50),fnumber nvarchar(50),	F_105 float,	F_106 nVARCHAR(50),	F_107 float ,FMaund float,FErpClsID float)	iNSERT INTO #Temptb	select t_ICItemCore.Fname,	t_ICItemCore.fnumber,t_item.FName ,	t_ICItemCustom.F_106,t_ICItemCustom.F_107,	t_ICItemDesign.FMaund,	t_ICItemBase.FErpClsID	from t_ICItemCore 	LEFT JOIN t_ICItemCustom on t_ICItemCore.fitemid = t_ICItemCustom.fitemid left join t_ICItemDesign on t_ICItemCustom.fitemid = t_ICItemDesign.fitemid	left join t_ICItemBase on t_ICItemDesign.fitemid = t_ICItemBase.fitemid	left join  t_Item on  t_Item.FItemID =t_ICItemCustom.F_105	--where t_ICItemCore.fnumber='50.01.25AT/1/N/00/01'	select * from #Temptb	drop table #Temptb"));
	//strSQL.Format(_T(" Create Table #Temptb	(Fname VARCHAR(80),	fnumber nvarchar(80), F_105 nVARCHAR(80),F_106 nvarchar(80),F_107 nVARCHAR(80) ,FMaund float, FErpClsID float)  INSERT INTO #Temptb	select t_ICItemCore.Fname,	t_ICItemCore.fnumber,t_item.FName ,	t_ICItemCustom.F_106,t_ICItemCustom.F_107,	t_ICItemDesign.FMaund,	t_ICItemBase.FErpClsID	from t_ICItemCore LEFT JOIN t_ICItemCustom on t_ICItemCore.fitemid = t_ICItemCustom.fitemid	left join t_ICItemDesign on t_ICItemCustom.fitemid = t_ICItemDesign.fitemid	left join t_ICItemBase on t_ICItemDesign.fitemid = t_ICItemBase.fitemid	left join  t_Item on  t_Item.FItemID =t_ICItemCustom.F_105 where  t_ICItemCore.FDeleted<>'1' and LEFT(t_ICItemCore.FNumber,1)<>'f' and LEFT(t_ICItemCore.FNumber,1)<>'6' select * from #Temptb	drop table #Temptb"));
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
	m_bSuccess = true;
	AfxMessageBox(_T("测试成功"));
}

void CDlgSQLDbSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (!m_bSuccess)
	{
		AfxMessageBox(_T("数据库连接不成功，请测试连接"));
		return;
	}
	SaveInfoToReg();
	CAcUiDialog::OnOK();
}

void CDlgSQLDbSet::SaveInfoToReg()
{
	CString strRootKey = _T("Software\\ZWSOFT\\ZWSQL");
	COperateReg Reg;
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("ADDRESS"), m_strAddress);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("USERNAME"), m_strUserName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("PASSWORD"), m_strPassWord);
}

void CDlgSQLDbSet::GetInfoFromReg()
{
	m_strAddress = GetStringFromReg(_T("ADDRESS"));
	m_strUserName = GetStringFromReg(_T("USERNAME"));
	m_strPassWord = GetStringFromReg(_T("PASSWORD"));
}

CString CDlgSQLDbSet::GetStringFromReg(LPCTSTR strKey)
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
