//-----------------------------------------------------------------------------
//----- DlgModQYD.cpp : Implementation of CDlgModQYD
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgModQYD.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgModQYD, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgModQYD, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgModQYD::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgModQYD::CDlgModQYD (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgModQYD::IDD, pParent, hInstance) 
{
	/*, m_strAmount(_T(""))
	, m_strPressure(_T(""))
		, m_strPipe(_T(""))
		, m_strTexture(_T(""))
		, m_strStartAmount(_T(""))*/

	m_strAmount = gGlobal.GetIniValue(_T("修改气源点"), _T("流量"));
	m_strPressure = gGlobal.GetIniValue(_T("修改气源点"), _T("气压"));
	m_strDiameter = gGlobal.GetIniValue(_T("修改气源点"), _T("管径数据"));
	m_strTexture = gGlobal.GetIniValue(_T("修改气源点"), _T("材质"));
	m_strStartAmount = gGlobal.GetIniValue(_T("修改气源点"), _T("初始流量"));
}

//-----------------------------------------------------------------------------
void CDlgModQYD::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_AMOUNT, m_strAmount);
	DDX_Text(pDX, IDC_EDIT_PRESSURE, m_strPressure);
	DDX_Text(pDX, IDC_EDIT_PIPE, m_strDiameter);
	DDX_Text(pDX, IDC_EDIT_TEXTURE, m_strTexture);
	DDX_Text(pDX, IDC_EDIT_STARTAMOUNT, m_strStartAmount);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgModQYD::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgModQYD::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	BeginEditorCommand();

	resbuf *rb = acutBuildList( -4,_T("<and"),RTDXF0,_T("INSERT"), 2, _T("qyd"), -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(NULL, NULL, NULL, rb, ssname);
	acutRelRb(rb);
	if (nRet != RTNORM)
	{
		CancelEditorCommand();
		return;
	}
	AcDbObjectId objId = AcDbObjectId::kNull;
	ads_name ename;
	long sslen;
	acedSSLength(ssname, &sslen);
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		MyEditEntity::OpenObjAppendStrToXdata(objId, GD_DIAMETER, m_strDiameter);
		MyEditEntity::OpenObjAppendStrToXdata(objId, GD_TEXTURE, m_strTexture);
		MyEditEntity::OpenObjAppendStrToXdata(objId, GD_AMOUNT, m_strAmount);
		MyEditEntity::OpenObjAppendStrToXdata(objId, GD_ST_AMOUNT, m_strStartAmount);
		MyEditEntity::OpenObjAppendStrToXdata(objId, GD_PRESSURE, m_strPressure);
	}


	acedSSFree(ssname);
	CompleteEditorCommand();
	

	gGlobal.SetIniValue(_T("修改气源点"), _T("流量"), m_strAmount);
	gGlobal.SetIniValue(_T("修改气源点"), _T("气压"), m_strPressure);
	gGlobal.SetIniValue(_T("修改气源点"), _T("管径数据"), m_strDiameter);
	gGlobal.SetIniValue(_T("修改气源点"), _T("材质"), m_strTexture);
	gGlobal.SetIniValue(_T("修改气源点"), _T("初始流量"), m_strStartAmount);
	gGlobal.WriteIniFile();
	CAcUiDialog::OnOK();
}

BOOL CDlgModQYD::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow(acedGetAcadDwgView());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
