//-----------------------------------------------------------------------------
//----- DlgObstacle.cpp : Implementation of CDlgObstacle
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgObstacle.h"
#include "DlgDisplayText.h"
#include "DrawObstacle.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgObstacle, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgObstacle, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_RADIO24, &CDlgObstacle::OnBnClickedRadio24)
	ON_BN_CLICKED(IDOK, &CDlgObstacle::OnBnClickedOk)
END_MESSAGE_MAP()

//定义交叉管名称
static CString gSpecailPipeName[] = {
	_T("给水管"),
	_T("低压给水管"),
	_T("高压给水管"),
	_T("消火栓管"),
	_T("雨水管"),
	_T("雨水沟"),
	_T("污水管"),
	_T("污水沟"),
	_T("天然气管"),
	_T("高压天然气管"),
	_T("中压天然气管"),
	_T("低压天然气管"),
	_T("煤气管"),
	_T("高压煤气管"),
	_T("中压煤气管"),
	_T("低压煤气管"),
	_T("液化气管"),
	_T("电力沟"),
	_T("直埋电力电缆管"),
	_T("电信电缆管块"),
	_T("直埋通信电缆管"),
	_T("热力管"),
	_T("热力管沟"),
	_T("自定义管道名称"),
	_T("路灯"),
	_T("交警信号"),
	_T("人防"),
	_T("工业"),
	_T("综合管廊")
};
//-----------------------------------------------------------------------------
CDlgObstacle::CDlgObstacle (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgObstacle::IDD, pParent, hInstance) , m_nRadio(0)
{
	m_strCurName = _T("");
}

//-----------------------------------------------------------------------------
void CDlgObstacle::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgObstacle::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgObstacle::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgObstacle::OnBnClickedRadio24()
{
	// TODO: Add your control notification handler code here
	CDlgDisplayText dlg1;
	if (dlg1.DoModal() == IDOK)
	{
		m_strCurName = dlg1.GetText();
	}
}

void CDlgObstacle::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	BeginEditorCommand();
	if (m_nRadio == 23)
	{
		if (m_strCurName.IsEmpty())
		{
			AfxMessageBox(_T("自定义管道名称不能为空"));
			CancelEditorCommand();
			return;
		}
	}
	else
	{
		m_strCurName = gSpecailPipeName[m_nRadio];
	}
	
	CDrawObstacle ob(m_nRadio, m_strCurName);
	if (!ob.doIt())
	{
		CancelEditorCommand();
		return;
	}
	
	CAcUiDialog::OnOK();
	CompleteEditorCommand();
}
