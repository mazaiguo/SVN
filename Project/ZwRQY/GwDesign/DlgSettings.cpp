//-----------------------------------------------------------------------------
//----- DlgSettings.cpp : Implementation of CDlgSettings
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgSettings.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgSettings, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgSettings, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTINGS, &CDlgSettings::OnTcnSelchangeTabSettings)
	ON_BN_CLICKED(IDOK, &CDlgSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSettings::OnBnClickedCancel)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgSettings::CDlgSettings (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgSettings::IDD, pParent, hInstance) {


}

CDlgSettings::~CDlgSettings()
{
	//delete []pDialog;
}

//-----------------------------------------------------------------------------
void CDlgSettings::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_TAB_SETTINGS, m_tab);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgSettings::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgSettings::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//ΪTab Control��������ҳ��
	m_tab.InsertItem(0, _T("��������"));
	m_tab.InsertItem(1, _T("����"));

	//���������Ի���
	m_pBasicSettings.Create(IDD_DLGBASICSETTINGS, &m_tab);
	m_pTextSettings.Create(IDD_DLGTEXTSETTINGS, &m_tab);
	//�趨��Tab����ʾ�ķ�Χ
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_pBasicSettings.MoveWindow(&rc);
	m_pTextSettings.MoveWindow(&rc);

	//�ѶԻ������ָ�뱣������
	pDialog[0] = &m_pBasicSettings;
	pDialog[1] = &m_pTextSettings;
	//��ʾ��ʼҳ��
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	//���浱ǰѡ��
	m_CurSelTab = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettings::OnTcnSelchangeTabSettings(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//�ѵ�ǰ��ҳ����������
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//�õ��µ�ҳ������
	m_CurSelTab = m_tab.GetCurSel();
	//���µ�ҳ����ʾ����
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CDlgSettings::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!m_pBasicSettings.CreateLayerId())
	{
		return;
	}
	m_pTextSettings.CreateTextStyleId();
	CDialog::OnOK();
}

void CDlgSettings::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDlgSettings::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CAcUiDialog::OnCancel();
}
