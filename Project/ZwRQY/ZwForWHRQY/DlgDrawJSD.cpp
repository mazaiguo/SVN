//-----------------------------------------------------------------------------
//----- DlgDrawJSD.cpp : Implementation of CDlgDrawJSD
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDrawJSD.h"
#include "ZDMUtils.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDrawJSD, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDrawJSD, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgDrawJSD::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDOK, &CDlgDrawJSD::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgDrawJSD::CDlgDrawJSD (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDrawJSD::IDD, pParent, hInstance) , m_strType(_T(""))
{
}

//-----------------------------------------------------------------------------
void CDlgDrawJSD::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_COMBO_TYPE, m_CombType);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strType);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDrawJSD::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}


BOOL CDlgDrawJSD::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTmp;
	CStringArray tmpArr;
	tmpArr.RemoveAll();
	CString strGanNature = gGlobal.GetIniValue(_T("警示带设置"), _T("警示带序列"));
	MyTransFunc::GetCommaFromString(strGanNature, tmpArr);

	for (int i=0; i<tmpArr.GetCount(); i++)
	{
		m_CombType.AddString(tmpArr.GetAt(i));
	}
	m_strType= gGlobal.GetIniValue(_T("警示带设置"), _T("警示带类型"));
	m_CombType.SetCurSel(m_CombType.FindString(-1, m_strType));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDrawJSD::OnCbnSelchangeComboType()
{
	// TODO: Add your control notification handler code here
	int nCursel = m_CombType.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombType.SetCurSel(nCursel);
	m_CombType.GetLBText(m_CombType.GetCurSel(), m_strType);
}

void CDlgDrawJSD::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	BeginEditorCommand();
	AcGePoint3d startPt,midPt,endPt;
	int nRet = acedGetPoint(NULL, _T("\n请指定警示带起点"), asDblArray(startPt));
	if (nRet != RTNORM)
	{
		CancelEditorCommand();
		return;
	}
	nRet = acedGetPoint(asDblArray(startPt), _T("\n请指定警示带终点"), asDblArray(midPt));
	if (nRet != RTNORM)
	{
		CancelEditorCommand();
		return;
	}
	acedGrDraw(asDblArray(startPt), asDblArray(midPt), 7, 1);
	int nOrtho;
	MyBaseUtils::GetVar(_T("ORTHOMODE"), &nOrtho);
	if (nOrtho == 0)
	{
		MyBaseUtils::SetVar(_T("ORTHOMODE"), 1);
	}
	nRet = acedGetPoint(asDblArray(midPt), _T("\n请指定方向"), asDblArray(endPt));
	if (nRet != RTNORM)
	{
		MyBaseUtils::SetVar(_T("ORTHOMODE"), nOrtho);
		CancelEditorCommand();
		return;
	}
	MyBaseUtils::SetVar(_T("ORTHOMODE"), nOrtho);

	AcGeVector3d vec = endPt - midPt;
	double dAng = vec.angleOnPlane(AcGePlane::kXYPlane);
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId plineId = AcDbObjectId::kNull;
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 3000.0/(CDMXUtils::getXScale()));
	AcGePoint3dArray points;
	double dLen;
	/*if (vec.x > 0)
	{*/
		textId = MyDrawEntity::DrawText(midPt, m_strType, 3.0, textStyleId, AcDb::kTextLeft);
		textId = MyEditEntity::openEntChangeRotation(textId, dAng);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		acutPolar(asDblArray(midPt), dAng, dLen, asDblArray(endPt));
		points.append(startPt);
		points.append(midPt);
		points.append(endPt);
		plineId = MyDrawEntity::DrawPlineByPoints(points);
	/*}
	else
	{
		textId = MyDrawEntity::DrawText(midPt, m_strType, 3.0, textStyleId, AcDb::kTextRight);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		acutPolar(asDblArray(midPt), dAng, dLen, asDblArray(endPt));
		points.append(startPt);
		points.append(midPt);
		points.append(endPt);
		plineId = MyDrawEntity::DrawPlineByPoints(points);
	}*/
	CAcUiDialog::OnOK();
	CompleteEditorCommand();
}
