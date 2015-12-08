
//-----------------------------------------------------------------------------
//----- DlgFindBlock.cpp : Implementation of CDlgFindBlock
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgFindBlock.h"
#include "ArxDbgUtils.h"
#include "Global.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgFindBlock, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgFindBlock, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDlgFindBlock::OnBnClickedButtonFind)
	ON_CBN_SELCHANGE(IDC_COMBO_BLOCK, &CDlgFindBlock::OnCbnSelchangeComboBlock)
END_MESSAGE_MAP()
extern Adesk::Boolean endFINDBLKDlg();

//-----------------------------------------------------------------------------
CDlgFindBlock::CDlgFindBlock (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgFindBlock::IDD, pParent, hInstance)
//, m_strText(_T(""))
, m_strBlockName(_T(""))
{
	m_AllData.clear();
	m_nDeFault = 0;
	//m_strText = _T("");
	m_strBlockName = gGlobal.GetIniValue(_T("查找图块"), _T("图块名"));
	m_strPreName = m_strBlockName;
	m_dScale = gGlobal.GetIniValue(_T("查找图块"), _T("比例"), 0);
	if (abs(m_dScale) < 0.000001)
	{
		m_dScale = 1.0;
	}
	m_CombBlockName.ResetContent();
}

CDlgFindBlock::~CDlgFindBlock()
{
	m_AllData.clear();
	m_nDeFault = 0;
	//m_strText = _T("");
	m_DoEnt.ReStoreFrame();
};

//-----------------------------------------------------------------------------
void CDlgFindBlock::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	//DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	DDX_Control(pDX, IDC_COMBO_BLOCK, m_CombBlockName);
	DDX_CBString(pDX, IDC_COMBO_BLOCK, m_strBlockName);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgFindBlock::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgFindBlock::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	bool bRet = GetData();
	if (!bRet)
	{
		m_strBlockName = _T("");
		GetDlgItem(IDC_COMBO_BLOCK)->SetWindowText(m_strBlockName);
		return FALSE;
	}
	
	for (map<CString, vector<AcDbExtents> >::iterator iter = m_AllData.begin();
		iter != m_AllData.end();
		++iter)
	{
		m_CombBlockName.AddString(iter->first);
	}
	int nCur = m_CombBlockName.FindString(-1, m_strBlockName);
	if (nCur == -1)
	{
		nCur = 0;
	}

	m_CombBlockName.SetCurSel(nCur);
	m_CombBlockName.GetLBText(m_CombBlockName.GetCurSel(), m_strBlockName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgFindBlock::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CAcUiDialog::OnClose();
	m_DoEnt.ReStoreFrame();
	endFINDBLKDlg();
}

void CDlgFindBlock::OnCancel()
{
	CAcUiDialog::OnCancel();
	m_DoEnt.ReStoreFrame();
	endFINDBLKDlg();
}

void CDlgFindBlock::OnDestroy()
{
	CAcUiDialog::OnDestroy();
	m_DoEnt.ReStoreFrame();
	endFINDBLKDlg();
	// TODO: 在此处添加消息处理程序代码
}

BOOL CDlgFindBlock::Create(CWnd* pParent)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::Create(CDlgFindBlock::IDD, pParent);
}

void CDlgFindBlock::OnBnClickedButtonFind()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strBlockName.IsEmpty())
	{
		AfxMessageBox(_T("图块名不能为空."));
		return;
	}
	if (m_strPreName.CompareNoCase(m_strBlockName) !=0)
	{
		m_strPreName = m_strBlockName;
		m_nDeFault = 0;
	}
	if (m_AllData.size() < 1)
	{
		return;
	}
	int nCount = 0;
	std::map<CString, std::vector<AcDbExtents> >::iterator iter = m_AllData.find(m_strBlockName);
	if (iter == m_AllData.end())
	{
		return;
	}
	std::vector<AcDbExtents> tmpVec = iter->second;
	if (m_nDeFault > tmpVec.size())
	{
		AfxMessageBox(_T("同名图块已遍历完毕"));
		m_nDeFault = 0;
		return;
	}
	for (std::vector<AcDbExtents>::iterator itr = tmpVec.begin();
		itr != tmpVec.end(); ++itr)
	{
		if (nCount == m_nDeFault)
		{
			AcGePoint3d minPt,maxPt, cenPt;
			AcDbExtents exts = *itr;
			minPt = exts.minPoint();
			maxPt = exts.maxPoint();
			cenPt.x = (minPt.x + maxPt.x)/2;
			cenPt.y = (minPt.y + maxPt.y)/2;
			cenPt.z = (minPt.z + maxPt.z)/2;
			minPt = minPt.scaleBy(1.1*m_dScale, cenPt);
			maxPt = maxPt.scaleBy(1.1*m_dScale, cenPt);
			//ArxDbgUtils::ZOOMWINDOW(minPt, maxPt);	
			ArxDbgUtils::ZOOMWindowByDeFault(cenPt);
			//ArxDbgUtils::ZOOMEXT();
			m_DoEnt.SetEntFrame(minPt, maxPt);
			//DrawExts(exts);
		}
		nCount ++;
	}
	m_nDeFault++;
	acedGetAcadDwgView()->SetFocus();
	gGlobal.SetIniValue(_T("查找图块"), _T("图块名"), m_strBlockName);
	gGlobal.WriteIniFile();
}

bool CDlgFindBlock::GetData()
{
	Acad::ErrorStatus es = Acad::eOk;

	ads_name ss;
	struct resbuf*	filter=acutBuildList(-4,_T("<and"), RTDXF0, _T("INSERT"), -4, _T("and>"), RTNONE);
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ss);
	acutRelRb(filter);
	if (RTNORM != nRet)
		return false;

	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId = AcDbObjectId::kNull;
	ads_name ename;
	CString strText;
	long ssLen;
	acedSSLength(ss, &ssLen);
	for (int i=0; i<ssLen; i++)
	{
		acedSSName(ss, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		AcDbExtents exts;
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			ArxDbgUtils::symbolIdToName(pBlkRef->blockTableRecord(), strText);
			pBlkRef->getGeomExtents(exts);			
		}
		pEnt->close();

		std::vector<AcDbExtents> tmpVec;
		tmpVec.push_back(exts);
		
		std::pair<std::map<CString, std::vector<AcDbExtents> >::iterator, bool> bRet = m_AllData.insert(std::make_pair(strText, tmpVec));
		if (!bRet.second)
		{
			bRet.first->second.push_back(exts);
		}
	}
	acedSSFree(ss);
	return true;
}

void CDlgFindBlock::DrawExts(AcDbExtents exts)
{
	AcGePoint3d minPt, maxPt, pt1, pt2;
	minPt = exts.minPoint();
	maxPt = exts.maxPoint();

	pt1.x = minPt.x;
	pt1.y = maxPt.y;
	pt1.z = 0;

	pt2.x = maxPt.x;
	pt2.y = minPt.y;
	pt2.z = 0;
	
	int nRet;
	nRet = acedGrDraw(asDblArray(pt1), asDblArray(maxPt), 1, 1);
	nRet = acedGrDraw(asDblArray(minPt), asDblArray(pt1), 1, 1);
	nRet = acedGrDraw(asDblArray(pt1), asDblArray(maxPt), 1, 1);
	nRet = acedGrDraw(asDblArray(maxPt), asDblArray(pt2), 1, 1);
	nRet = acedCommand(RTSTR, _T(".regen"), RTSTR, _T(""), RTNONE);
}


void CDlgFindBlock::OnCbnSelchangeComboBlock()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = m_CombBlockName.GetCurSel();
	if (nCursel == -1)
	{
		nCursel = 0;
	}
	m_CombBlockName.SetCurSel(nCursel);
	m_CombBlockName.GetLBText(m_CombBlockName.GetCurSel(), m_strBlockName);
}
