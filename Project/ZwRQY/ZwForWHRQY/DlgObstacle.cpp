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

//���彻�������
static CString gSpecailPipeName[] = {
	_T("��ˮ��"),
	_T("��ѹ��ˮ��"),
	_T("��ѹ��ˮ��"),
	_T("����˨��"),
	_T("��ˮ��"),
	_T("��ˮ��"),
	_T("��ˮ��"),
	_T("��ˮ��"),
	_T("��Ȼ����"),
	_T("��ѹ��Ȼ����"),
	_T("��ѹ��Ȼ����"),
	_T("��ѹ��Ȼ����"),
	_T("ú����"),
	_T("��ѹú����"),
	_T("��ѹú����"),
	_T("��ѹú����"),
	_T("Һ������"),
	_T("������"),
	_T("ֱ��������¹�"),
	_T("���ŵ��¹ܿ�"),
	_T("ֱ��ͨ�ŵ��¹�"),
	_T("������"),
	_T("�����ܹ�"),
	_T("�Զ���ܵ�����"),
	_T("·��"),
	_T("�����ź�"),
	_T("�˷�"),
	_T("��ҵ"),
	_T("�ۺϹ���")
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
			AfxMessageBox(_T("�Զ���ܵ����Ʋ���Ϊ��"));
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
