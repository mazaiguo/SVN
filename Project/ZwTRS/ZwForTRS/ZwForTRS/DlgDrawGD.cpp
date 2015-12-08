
//-----------------------------------------------------------------------------
//----- DlgDrawGD.cpp : Implementation of CDlgDrawGD
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgDrawGD.h"
#include "ArxDbgUtils.h"
#include "GDDataInDWG.h"
#include "MyTransFunc.h"
#include "Global.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgDrawGD, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgDrawGD, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgDrawGD::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CDlgDrawGD::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDOK, &CDlgDrawGD::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgDrawGD::OnBnClickedButtonAdd)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgDrawGD::CDlgDrawGD (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgDrawGD::IDD, pParent, hInstance) 
{
	m_strDescription = _T("主管道");
	m_strLayerColor = _T("1");
	m_strLineType = _T("ByLayer");
	m_strLineweight = _T("kLnWtByLwDefault");
	m_nDefaultItem = (int)gGlobal.GetIniValue(_T("绘制管道"), _T("默认"), 0);
}

//-----------------------------------------------------------------------------
void CDlgDrawGD::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST_GD, m_ListCtrl);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgDrawGD::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgDrawGD::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = 0;
	POSITION pos=m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos==NULL) 
	{
		TRACE0("No items were selected!\n")
	}
	else
	{
		while (pos)
		{
			int nItem=m_ListCtrl.GetNextSelectedItem(pos);
			TRACE1("Item %d was Selected!\n",nItem);

			m_ListCtrl.DeleteItem(nItem);
			//you could do your own processing on nItem here
		}
	}
}

void CDlgDrawGD::OnBnClickedButtonDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCursel = 0;
	POSITION pos=m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos==NULL) 
	{
		TRACE0("No items were selected!\n")
	}
	else
	{
		while (pos)
		{
			m_nDefaultItem=m_ListCtrl.GetNextSelectedItem(pos);
			TRACE1("Item %d was Selected!\n",m_nDefaultItem);

			COLORREF color=RGB(255,255,255);
			m_ListCtrl.SetAllItemColor(m_ListCtrl.GetItemCount(), color);

			color = RGB(255,0,0);
			m_ListCtrl.SetItemState(m_nDefaultItem, LVIS_FOCUSED, LVIS_FOCUSED);

			//m_ListCtrl.SetFocusRow(nItem);
			m_ListCtrl.SetItemColor(m_nDefaultItem, color);
			//you could do your own processing on nItem here
		}
	}
}

void CDlgDrawGD::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*for (int i=0; i<m_ListCtrl.GetItemCount(); i++)
	{
		m_strDescription = m_ListCtrl.GetItemText(i, 0);
		m_strLayerColor = m_ListCtrl.GetItemText(i, 1);
		m_strLineType = m_ListCtrl.GetItemText(i, 2);
		m_strLineweight = m_ListCtrl.GetItemText(i, 3);
		DrawLine();
	}*/
	if (m_ListCtrl.GetItemCount() < 1)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	POSITION pos=m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos==NULL) 
	{
		TRACE0("No items were selected!\n")
	}
	else
	{
		while (pos)
		{
			m_nDefaultItem=m_ListCtrl.GetNextSelectedItem(pos);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	BeginEditorCommand();
	m_strDescription = m_ListCtrl.GetItemText(m_nDefaultItem, 0);
	m_strLayerColor = m_ListCtrl.GetItemText(m_nDefaultItem, 1);
	m_strLineType = m_ListCtrl.GetItemText(m_nDefaultItem, 2);
	m_strLineweight = m_ListCtrl.GetItemText(m_nDefaultItem, 3);
	bool bRet = DrawLine();
	if (!bRet)
	{
		CancelEditorCommand();
		return;
	}
	CompleteEditorCommand();
	/*for (int i=0; i<m_ListCtrl.GetItemCount(); i++)
	{
		m_strDescription = m_ListCtrl.GetItemText(i, 0);
		m_strLayerColor = m_ListCtrl.GetItemText(i, 1);
		m_strLineType = m_ListCtrl.GetItemText(i, 2);
		m_strLineweight = m_ListCtrl.GetItemText(i, 3);
		SetGDData();
	}*/
	ReWriteFile();
	gGlobal.SetIniValue(_T("绘制管道"), _T("默认"), m_nDefaultItem);
	gGlobal.WriteIniFile();
	CAcUiDialog::OnOK();
}
static int _List_Type( int col )
{
	if ( col == 1 )
		return CEditList::eComboColor;
	else if (col == 2)
	{
		return CEditList::eComboLineType;
	}
	else if (col == 3)
	{
		return CEditList::eComboLineWeight;
	}
	return CEditList::eEdit;
}
BOOL CDlgDrawGD::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());
	m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);

	m_ListCtrl.InsertColumn( 0, _T("名称"));
	m_ListCtrl.InsertColumn( 1, _T("颜色索引"));
	m_ListCtrl.InsertColumn( 2, _T("线型"));
	m_ListCtrl.InsertColumn( 3, _T("线宽"));

	m_ListCtrl.SetColumnWidth( 0, 100);
	m_ListCtrl.SetColumnWidth( 1, 100);
	m_ListCtrl.SetColumnWidth( 2, 100);
	m_ListCtrl.SetColumnWidth( 3, 100);

	// set functionality of list according to column
	m_ListCtrl.SetColumnType ( (fGetType)_List_Type );	

	// insert string elements  for the ComboBox : 
	//for ( int i=0 ; i < 4 ; i++)
	//	m_cList.m_strList.AddTail( Product[i]);
	/*bool bRet = GetGDData();
	if (!bRet)
	{
		InsertEmpty();
	}*/
	ReadFile();
	// TODO:  在此添加额外的初始化

	m_ListCtrl.SetFocus();
	m_ListCtrl.SetItemState(m_nDefaultItem, LVIS_FOCUSED, LVIS_FOCUSED);
	COLORREF color=RGB(255,0,0);
	
	m_ListCtrl.SetItemColor(m_nDefaultItem, color);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDrawGD::InsertEmpty()
{
	int nItem = m_ListCtrl.GetItemCount();

	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
	m_ListCtrl.SetItemText(nItem, 0, m_strDescription);
	m_ListCtrl.SetItemText(nItem, 1, m_strLayerColor);
	m_ListCtrl.SetItemText(nItem, 2, m_strLineType);
	m_ListCtrl.SetItemText(nItem, 3, m_strLineweight);
	if (m_bDefault)
	{
		m_nDefaultItem = nItem;
	}
}
void CDlgDrawGD::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_ListCtrl.GetItemCount();
	m_strDescription = m_ListCtrl.GetItemText(nItem - 1, 0);
	CString strPrefix, strNext, strNum;
	int nlen = ArxDbgUtils::GetPileLengthEx(m_strDescription, strPrefix, strNext);
	nlen+=1;
	strNum.Format(_T("%d"), nlen);
	if (strPrefix.IsEmpty())
	{
		m_strDescription = strNext+ strNum;
	}
	else
	{
		m_strDescription = strPrefix+ strNum + strNext;
	}
	m_strLayerColor = m_ListCtrl.GetItemText(nItem - 1, 1);
	m_strLineType = m_ListCtrl.GetItemText(nItem - 1, 2);
	m_strLineweight = m_ListCtrl.GetItemText(nItem - 1, 3);
	m_bDefault = false;
	InsertEmpty();
}

bool CDlgDrawGD::DrawLine()
{
	int nColor = ArxDbgUtils::StringToInt(m_strLayerColor);
	AcDbObjectId layerId = ArxDbgUtils::CreateNewLayer(m_strDescription, nColor, TRUE);
	AcGePoint3d startPt,endPt;
	//打开正交模式ORTHOMODE
	int nOrthMode = 1;
	gGlobal.GetVar(_T("ORTHOMODE"), &nOrthMode);
	if (nOrthMode!= 1)
	{
		gGlobal.SetVar(_T("ORTHOMODE"), 1);
	}
	int nRet = acedGetPoint(NULL, _T("\n请选择起点"), asDblArray(startPt));
	if (nRet != RTNORM)
	{
		return false;
	}

	nRet = acedGetPoint(asDblArray(startPt), _T("\n请选择终点"), asDblArray(endPt));
	if (nRet != RTNORM)
	{
		return false;
	}
	gGlobal.SetVar(_T("ORTHOMODE"), nOrthMode);
	AcDb::LineWeight lineW = ArxDbgUtils::StrToLineType(m_strLineweight);
	Acad::ErrorStatus es;
	AcDbLine* pLine = new AcDbLine;
	es = pLine->setLayer(layerId);
	es = pLine->setLinetype(m_strLineType);
	es = pLine->setLineWeight(lineW);
	es = pLine->setStartPoint(startPt);
	es = pLine->setEndPoint(endPt);
	ArxDbgUtils::addToCurrentSpaceAndClose(pLine);
	MyTransFunc::DuiXiangPaiXu(pLine->objectId(), false);
	return true;
}

void CDlgDrawGD::SetGDData()
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = ArxDbgUtils::openDictionaryForWrite(
		CGDDataInDWG::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	Acad::ErrorStatus es;

	AcDbDictionaryIterator* pIter = testDict->newIterator();
	for (; !pIter->done(); pIter->next())
	{
		/*AcDbObject* pObj = NULL;
		es = pIter->getObject(pObj, AcDb::kForWrite);
		if (pObj->isKindOf(CGDDataInDWG::desc()))
		{
			CGDDataInDWG* pEnt = CGDDataInDWG::cast(pObj);
			es = pEnt->erase();
		}
		es = pObj->close();	*/
		AcDbObjectId entId = pIter->objectId();
		testDict->remove(entId);
	}
	delete pIter;
	for (int i=0; i<m_ListCtrl.GetItemCount(); i++)
	{	
		bool bDeFault = false;

		if (i == m_nDefaultItem)
		{
			bDeFault = true;
		}
		
		m_strDescription = m_ListCtrl.GetItemText(i, 0);
		m_strLayerColor = m_ListCtrl.GetItemText(i, 1);
		m_strLineType = m_ListCtrl.GetItemText(i, 2);
		m_strLineweight = m_ListCtrl.GetItemText(i, 3);
		CString strKword;
		strKword = m_strDescription + m_strLayerColor + m_strLineType + m_strLineweight;
		if (testDict->getAt(strKword, StyleId) != Acad::eOk)
		{
			CGDDataInDWG* newRec = new CGDDataInDWG;
			newRec->setDescription(m_strDescription);
			newRec->setLayerColor(m_strLayerColor);
			newRec->setLineType(m_strLineType);
			newRec->setLineWeight(m_strLineweight);
			newRec->setDeFault(bDeFault);
			es = testDict->setAt(strKword, newRec, StyleId);
			if (es == Acad::eOk) 
			{
				newRec->close();
			}
			else 
			{
				ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
	}
	testDict->close();
}

bool CDlgDrawGD::GetGDData()
{
	AcDbObjectIdArray logoStyles;
	logoStyles.removeAll();
	AcDbObjectId logoStyleDictId = ArxDbgUtils::getRegularDictionaryId(CGDDataInDWG::dictName(), true, acdbHostApplicationServices()->workingDatabase());

	ArxDbgUtils::collectDictionaryEntryNames(logoStyleDictId, logoStyles, CGDDataInDWG::desc());

	if (logoStyles.length() == 0) 
	{
		return false;
	} 
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId entId = AcDbObjectId::kNull;
	CString strName;
	CGDDataInDWG* DieDwgData = NULL;

	for (int i=0; i<logoStyles.length(); i++)
	{
		objId = logoStyles.at(i);
		if (acdbOpenAcDbObject((AcDbObject*&)DieDwgData, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		m_strDescription = DieDwgData->Description();
		m_strLayerColor = DieDwgData->LayerColor();
		m_strLineType = DieDwgData->LineType();
		m_strLineweight = DieDwgData->LineWeight();
		m_bDefault = DieDwgData->DeFault();
		DieDwgData->close();
		InsertEmpty();
	}
	return true;
}

void CDlgDrawGD::ReWriteFile()
{
	//删除文件中具体的行,重写文件
	int nCursel = m_ListCtrl.GetItemCount();			
	CString strFileName = gGlobal.GetAppPath()+_T("GD.txt");
	CFile::Remove(strFileName);//删除文件

	CStdioFile File;
	if (!File.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	{
		return;
	}
	CString strTmp;
	for ( int i=0; i<nCursel; i++)
	{
		m_strDescription = m_ListCtrl.GetItemText(i, 0);
		m_strLayerColor = m_ListCtrl.GetItemText(i, 1);
		m_strLineType = m_ListCtrl.GetItemText(i, 2);
		m_strLineweight = m_ListCtrl.GetItemText(i, 3);
		
		strTmp.Format(_T("%s,%s,%s,%s\n"), m_strDescription, m_strLayerColor, m_strLineType, m_strLineweight);
		File.WriteString(strTmp);
	}
	File.Close();
}

bool CDlgDrawGD::ReadFile()
{	
	CString strFileName;
	strFileName = gGlobal.GetAppPath()+_T("GD.txt");
	CStdioFile File;
	if (!File.Open(strFileName, CFile::modeRead | CFile::modeCreate |CFile::typeText)) 
	{
		return false;
	}
	if (File.GetLength() == 0)
	{
		InsertEmpty();
	}

	CString buffer, strShowData, strLayerName, strLayerIndex, strTypeData;
	while (1)
	{
		if (!File.ReadString(buffer))
			break;
		m_strDescription = gGlobal.SubString(buffer, _T(","), 0);
		m_strLayerColor = gGlobal.SubString(buffer, _T(","), 1);
		m_strLineType = gGlobal.SubString(buffer, _T(","), 2);
		m_strLineweight = gGlobal.SubString(buffer, _T(","), 3);
		
		InsertEmpty();
	}

	File.Close();
	return true;
}
