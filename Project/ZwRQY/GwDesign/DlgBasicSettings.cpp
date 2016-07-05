//-----------------------------------------------------------------------------
//----- DlgBasicSettings.cpp : Implementation of CDlgBasicSettings
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgBasicSettings.h"
#include "GWDesingUtils.h"
#include "Global.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgBasicSettings, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgBasicSettings, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgBasicSettings::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgBasicSettings::OnBnClickedButtonDel)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgBasicSettings::CDlgBasicSettings (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgBasicSettings::IDD, pParent, hInstance) , m_strScale(_T(""))
{
	m_strScale = gGlobal.GetIniValue(_T("基础设置"), _T("比例"));
	m_strDescription = _T("燃气管道");
	m_strLayerColor = _T("1");
	m_strLineType = _T("ByLayer");
	m_strLineweight = _T("kLnWtByLwDefault");
}

//-----------------------------------------------------------------------------
void CDlgBasicSettings::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST_LAYER, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_strScale);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgBasicSettings::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
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
BOOL CDlgBasicSettings::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow(acedGetAcadDwgView());
	m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);

	m_ListCtrl.InsertColumn( 0, _T("图层名"));
	m_ListCtrl.InsertColumn( 1, _T("颜色"));
	m_ListCtrl.InsertColumn( 2, _T("线型"));
	m_ListCtrl.InsertColumn( 3, _T("线宽"));

	m_ListCtrl.SetColumnWidth( 0, 100);
	m_ListCtrl.SetColumnWidth( 1, 100);
	m_ListCtrl.SetColumnWidth( 2, 100);
	m_ListCtrl.SetColumnWidth( 3, 100);

	// set functionality of list according to column
	m_ListCtrl.SetColumnType ( (fGetType)_List_Type );	
	//InsertEmpty();
	iniData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgBasicSettings::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	int nItem = m_ListCtrl.GetItemCount();
	m_strDescription = m_ListCtrl.GetItemText(nItem - 1, 0);
	CString strPrefix, strNext, strNum;
	int nlen = MyParserString::GetPileLengthEx(m_strDescription, strPrefix, strNext);
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
	InsertEmpty();
	GetParent()->SetFocus();
}
void CDlgBasicSettings::InsertEmpty()
{
	int nItem = m_ListCtrl.GetItemCount();

	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
	m_ListCtrl.SetItemText(nItem, 0, m_strDescription);
	m_ListCtrl.SetItemText(nItem, 1, m_strLayerColor);
	m_ListCtrl.SetItemText(nItem, 2, m_strLineType);
	m_ListCtrl.SetItemText(nItem, 3, m_strLineweight);
}



void CDlgBasicSettings::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
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
	GetParent()->SetFocus();
}

bool CDlgBasicSettings::CreateLayerId()
{
	GetDlgItem(IDC_EDIT_SCALE)->GetWindowText(m_strScale);
	double dScale = MyTransFunc::StringToDouble(m_strScale);
	if (dScale < GeTol)
	{
		AfxMessageBox(_T("比例要大于0"));
		return false;
	}
	CGWDesingUtils::SetGlobalScale(dScale);
	gGlobal.SetIniValue(_T("基础设置"), _T("比例"), dScale);
	gGlobal.WriteIniFile();

	int nItem = m_ListCtrl.GetItemCount();
	for (int i=0; i<nItem; i++)
	{
		m_strDescription = m_ListCtrl.GetItemText(i, 0);
		m_strLayerColor = m_ListCtrl.GetItemText(i, 1);
		m_strLineType = m_ListCtrl.GetItemText(i, 2);
		m_strLineweight = m_ListCtrl.GetItemText(i, 3);
		AcDb::LineWeight lw = MyParserString::StrToLineWeight(m_strLineweight);
		MySymble::CreateNewLayer(m_strDescription, MyTransFunc::StringToInt(m_strLayerColor), TRUE, m_strLineType, lw);
	}
	ReWriteFile();
	return true;
}

void CDlgBasicSettings::iniData()
{
	CString strApp = MyBaseUtils::GetAppPath();
	CString strFileName = strApp + _T("design\\basic\\layer.ini");
	CStdioFile File;
	if (!File.Open(strFileName, CFile::modeRead | CFile::typeText)) 
	{
		return;
	}
	CString buffer;
	int i=0;
	while (1)
	{
		if (!File.ReadString(buffer))
			break;
		m_strDescription = MyParserString::SubString(buffer, _T(","), 0);
		m_strLayerColor = MyParserString::SubString(buffer, _T(","), 1);
		m_strLineType = MyParserString::SubString(buffer, _T(","), 2);
		m_strLineweight = MyParserString::SubString(buffer, _T(","), 3);
		int nItem = m_ListCtrl.GetItemCount();
		nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
		SetInfoToListCtrl(nItem);
	}

	File.Close();
}

void CDlgBasicSettings::ReWriteFile()
{
	CString strApp = MyBaseUtils::GetAppPath();
	CString strFileName = strApp + _T("design\\basic\\layer.ini");
	CStdioFile File;
	if (!File.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	{
		return;
	}
	CString strTmp;
	int nCursel = m_ListCtrl.GetItemCount();
	for ( int i=0; i<nCursel; i++)
	{
		GetInfoFromListCtrl(i);
		
		strTmp.Format(_T("%s,%s,%s,%s\n"), m_strDescription, m_strLayerColor, m_strLineType, m_strLineweight);
		File.WriteString(strTmp);
	}
	File.Close();
}
void CDlgBasicSettings::SetInfoToListCtrl(int nItem)
{
	m_ListCtrl.SetItemText(nItem, 0, m_strDescription);
	m_ListCtrl.SetItemText(nItem, 1, m_strLayerColor);
	m_ListCtrl.SetItemText(nItem, 2, m_strLineType);
	m_ListCtrl.SetItemText(nItem, 3, m_strLineweight);
}
void CDlgBasicSettings::GetInfoFromListCtrl(int i)
{
	m_strDescription = m_ListCtrl.GetItemText(i, 0);
	m_strLayerColor = m_ListCtrl.GetItemText(i, 1);
	m_strLineType = m_ListCtrl.GetItemText(i, 2);
	m_strLineweight = m_ListCtrl.GetItemText(i, 3);
}
