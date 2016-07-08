//-----------------------------------------------------------------------------
//----- DlgModPipe.cpp : Implementation of CDlgModPipe
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgModPipe.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgModPipe, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgModPipe, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgModPipe::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgModPipe::CDlgModPipe (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgModPipe::IDD, pParent, hInstance) 
{
	m_strDiameter = gGlobal.GetIniValue(_T("修改管道"), _T("管径数据"));
	m_strTexture = gGlobal.GetIniValue(_T("修改管道"), _T("材质"));
	m_strAmount = gGlobal.GetIniValue(_T("修改管道"), _T("流量"));
}

//-----------------------------------------------------------------------------
void CDlgModPipe::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_DIAMETER, m_strDiameter);
	DDX_Text(pDX, IDC_EDIT_TEXTURE, m_strTexture);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, m_strAmount);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgModPipe::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgModPipe::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModPipe::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	BeginEditorCommand();

	resbuf *rb = acutBuildList( -4,_T("<and"),RTDXF0,_T("LWPOLYLINE"), 8, _T("燃气管道"), -4,_T("and>"), RTNONE);
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
	}
	

	acedSSFree(ssname);
	CompleteEditorCommand();

	gGlobal.SetIniValue(_T("修改管道"), _T("管径数据"), m_strDiameter);
	gGlobal.SetIniValue(_T("修改管道"), _T("材质"), m_strTexture);
	gGlobal.SetIniValue(_T("修改管道"), _T("流量"), m_strAmount);
	gGlobal.WriteIniFile();
	CAcUiDialog::OnOK();
}
