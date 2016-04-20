//-----------------------------------------------------------------------------
//----- DlgTKSZ.cpp : Implementation of CDlgTKSZ
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgTKSZ.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgTKSZ, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgTKSZ, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAMETYPE, &CDlgTKSZ::OnCbnSelchangeComboFrametype)
	ON_BN_CLICKED(IDOK, &CDlgTKSZ::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgTKSZ::CDlgTKSZ (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgTKSZ::IDD, pParent, hInstance) , m_strFrameType(_T(""))
{
	m_strScale = gGlobal.GetIniValue(_T("ͼ������"), _T("����"));
	m_strFrameType = gGlobal.GetIniValue(_T("ͼ������"), _T("ͼ������"));
	if (m_strScale.IsEmpty())
	{
		m_strScale = _T("1.0");
	}
}

//-----------------------------------------------------------------------------
void CDlgTKSZ::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_COMBO_FRAMETYPE, m_CombFrameType);
	DDX_CBString(pDX, IDC_COMBO_FRAMETYPE, m_strFrameType);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_strScale);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgTKSZ::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgTKSZ::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CStringArray strArr;
	strArr.RemoveAll();
	CString strAppPath = MyBaseUtils::GetAppPath();
	CString strPath = strAppPath + _T("ͼ��\\*.dwg");
	MyOperateFile::GetFileNameFromThePath(strPath, strArr, _T(".DWG"));
	
	if (strArr.GetSize() < 1)
	{
		return FALSE;
	}

	for (int i=0; i<strArr.GetSize(); i++)
	{
		m_CombFrameType.AddString(strArr.GetAt(i));
	}
	int nCur = m_CombFrameType.FindString(-1, m_strFrameType);
	if (nCur == -1)
	{
		nCur = 0;
		m_CombFrameType.GetLBText(nCur, m_strFrameType);
	}
	m_CombFrameType.SetCurSel(nCur);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgTKSZ::OnCbnSelchangeComboFrametype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCursel = m_CombFrameType.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombFrameType.SetCurSel(nCursel);
	m_CombFrameType.GetLBText(m_CombFrameType.GetCurSel(), m_strFrameType);
}

void CDlgTKSZ::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_SCALE)->GetWindowText(m_strScale);
	double dScale = MyTransFunc::StringToDouble(m_strScale);
	if (dScale < GeTol)
	{
		AfxMessageBox(_T("������Ҫ����0"));
		return;
	}
	if (m_strFrameType.IsEmpty())
	{
		AfxMessageBox(_T("û��ͼ�����ݣ���鿴�ļ���װ�Ƿ�����"));
		return;
	}
	BeginEditorCommand();
	CString strFile = MyBaseUtils::GetAppPath() + _T("ͼ��\\") + m_strFrameType + _T(".dwg");
	
	AcDbObjectId objId = MyDrawEntity::GetBlkRef(strFile);
	AcGePoint3d basePt;
	basePt.set(0, 0,0 );
	ads_name ssname,ename;
	acedSSAdd(NULL, NULL, ssname);
	AcDbBlockReference *pBlkRef =new AcDbBlockReference(basePt, objId) ; 
	pBlkRef->setScaleFactors(dScale);
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	AcDbObjectId blkId = pBlkRef->objectId();
	acdbGetAdsName(ename, blkId);
	acedSSAdd(ename, ssname, ssname);

	int nRet = MyDrag::Draggen(_T("\nָ�������:"), ssname, asDblArray(basePt), asDblArray(basePt), _T(""), -1);
	if (nRet == -1)
	{
		acedSSFree(ssname);
		CancelEditorCommand();
		return;
	}
	acedSSFree(ssname);

	SaveToIniFile();
	CompleteEditorCommand();

	CAcUiDialog::OnOK();
}

bool CDlgTKSZ::SaveToIniFile()
{
	gGlobal.SetIniValue(_T("ͼ������"), _T("����"), m_strScale);
	gGlobal.SetIniValue(_T("ͼ������"), _T("ͼ������"), m_strFrameType);
	gGlobal.WriteIniFile();
	return true;
}
