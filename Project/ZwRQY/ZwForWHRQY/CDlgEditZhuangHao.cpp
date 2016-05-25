//-----------------------------------------------------------------------------
//----- EditZhuangHao.cpp : Implementation of CDlgEditZhuangHao
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CDlgEditZhuangHao.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgEditZhuangHao, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgEditZhuangHao, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgEditZhuangHao::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgEditZhuangHao::CDlgEditZhuangHao (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgEditZhuangHao::IDD, pParent, hInstance) {
}

void CDlgEditZhuangHao::setZDMData(CZdmDataInfo* pData)
{
	m_pZDM.setLabel(pData->label());
	m_pZDM.setCount(pData->getCount());
	m_pZDM.setDesignDmx(pData->getDesignDmx());
	m_pZDM.setRealDmx(pData->getRealDmx());
	m_pZDM.setJiedian(pData->getJiedian());
	m_pZDM.setPipeType(pData->getPipeType());
	m_pZDM.setGuanDi(pData->getGuanDi());
	m_pZDM.setWaShen(pData->getWaShen());
	m_pZDM.setPoDu(pData->getPoDu());
	m_pZDM.setJuli(pData->getJuli());
	m_pZDM.setcurData(pData->getcurData());
	m_pZDM.setHasBulge(pData->getHasBulge());
	m_pZDM.setDesingDmxS(pData->getDesignDmxS());
	m_pZDM.setRealDmxS(pData->getRealDmxS());
	m_pZDM.setPipeDiameter(pData->getPipeDiameter());
}

CZdmDataInfo CDlgEditZhuangHao::getData()
{
	return m_pZDM;
}

//-----------------------------------------------------------------------------
void CDlgEditZhuangHao::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgEditZhuangHao::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgEditZhuangHao::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTmp;
	double dValue;
	dValue = m_pZDM.getDesignDmx();
	strTmp = MyTransFunc::doubleToStr(dValue, strTmp);
	GetDlgItem(IDC_EDIT_Desgin)->SetWindowText(strTmp);

	dValue = m_pZDM.getRealDmx();
	strTmp = MyTransFunc::doubleToStr(dValue, strTmp);
	GetDlgItem(IDC_EDIT_REAL)->SetWindowText(strTmp);

	dValue = m_pZDM.getcurData();
	strTmp = MyTransFunc::doubleToStr(dValue, strTmp);
	GetDlgItem(IDC_EDIT_CurData)->SetWindowText(strTmp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditZhuangHao::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	double dminElavation = CDMXUtils::getMinElavation();
	double dmaxElavation = CDMXUtils::getMaxElavation();
	double dDesign,dReal,dZhuanghao;
	CString strTmp;
	GetDlgItem(IDC_EDIT_REAL)->GetWindowText(strTmp);
	dReal = MyTransFunc::StringToDouble(strTmp);

	GetDlgItem(IDC_EDIT_Desgin)->GetWindowText(strTmp);
	dDesign = MyTransFunc::StringToDouble(strTmp);
	if ((dReal < dminElavation) || (dDesign < dminElavation))
	{
		AfxMessageBox(_T("地面线数据比基础地面线标高还低，请重新输入数据"));
		return;
	}
	if ((dReal > dmaxElavation) || (dDesign > dmaxElavation))
	{
		AfxMessageBox(_T("地面线数据比基础地面线标高还高，请重新输入数据"));
		return;
	}

	GetDlgItem(IDC_EDIT_CurData)->GetWindowText(strTmp);
	dZhuanghao = MyTransFunc::StringToInt(strTmp);
	
	m_pZDM.setcurData(dZhuanghao);
	m_pZDM.setRealDmx(dReal);
	m_pZDM.setDesignDmx(dDesign);

	CAcUiDialog::OnOK();
}
