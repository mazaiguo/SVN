//-----------------------------------------------------------------------------
//----- DlgDisplayText.cpp : Implementation of CDlgDisplayText
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDisplayText.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDisplayText, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDisplayText, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgDisplayText::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgDisplayText::CDlgDisplayText (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDisplayText::IDD, pParent, hInstance) , m_strCurText(_T(""))
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

//-----------------------------------------------------------------------------
void CDlgDisplayText::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strCurText);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDisplayText::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgDisplayText::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(m_strCurText);
	CAcUiDialog::OnOK();
}

BOOL CDlgDisplayText::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());
	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT_TEXT)->SetWindowText(m_strCurText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


CString CDlgDisplayText::GetText()
{
	return m_strCurText;
}

void CDlgDisplayText::SetText(const CString strCurText)
{
	m_strCurText = strCurText;
}	
