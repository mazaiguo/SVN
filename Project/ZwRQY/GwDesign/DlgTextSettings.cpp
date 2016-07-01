//-----------------------------------------------------------------------------
//----- DlgTextSettings.cpp : Implementation of CDlgTextSettings
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgTextSettings.h"
#include "MySplite.H"
#include <IO.h>
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgTextSettings, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgTextSettings, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgTextSettings::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgTextSettings::OnBnClickedButtonDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgTextSettings::OnLvnItemchangedList1)
END_MESSAGE_MAP()

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}

	CStringArray* pArr = (CStringArray* )lParam;
	/*if(_tcsicmp((char*)lpelfe->elfScript, "") == 0 &&
		(_tcsicmp((char*)lpelfe->elfStyle, "Regular") == 0 ||
		_tcsicmp((char*)lpelfe->elfStyle, "Italic") == 0 ||
		_tcsicmp((char*)lpelfe->elfStyle, "Bold") == 0 ||
		_tcsicmp((char*)lpelfe->elfStyle, "Bold Italic") == 0))
		pArr->Add((char*)lpelfe->elfLogFont.lfFaceName);*/
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		pArr->Add((TCHAR*)lpelfe->elfFullName);
	else
	{
		if (FontType & TRUETYPE_FONTTYPE)
			pArr->Add((TCHAR*)lpelfe->elfFullName);
		else
			pArr->Add((TCHAR*)lpelfe->elfLogFont.lfFaceName);
	}
	return 1;
}
//-----------------------------------------------------------------------------
CDlgTextSettings::CDlgTextSettings (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgTextSettings::IDD, pParent, hInstance) {

	m_strScale = _T("1.0");
	m_strDescription = _T("燃气管道");
	m_strFont = _T("宋体");
	m_strBigFont = _T("");
	m_strHeight = _T("3.0");
}

//-----------------------------------------------------------------------------
void CDlgTextSettings::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}
static int Text_List_Type( int col )
{
	if ( col == 1 )
		return CEditList::eCombo;
	else if (col == 2)
	{
		return CEditList::eCombo;
	}
	//else if (col == 3)
	//{
	//	return CEditList::eCombo;
	//}
	return CEditList::eEdit;
}
//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgTextSettings::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CDlgTextSettings::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	CenterWindow(acedGetAcadDwgView());

	// TODO:  Add extra initialization here
	GetAcadFont();
	GetWindowFont();

	m_ListCtrl.SetExtendedStyle( m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT/* | LVS_EX_CHECKBOXES*/);

	m_ListCtrl.InsertColumn( 0, _T("文字样式名"));
	m_ListCtrl.InsertColumn( 1, _T("字体名"));
	m_ListCtrl.InsertColumn( 2, _T("大字体"));
	m_ListCtrl.InsertColumn( 3, _T("高度"));
	m_ListCtrl.InsertColumn( 4, _T("宽度比"));

	m_ListCtrl.SetColumnWidth( 0, 100);
	m_ListCtrl.SetColumnWidth( 1, 100);
	m_ListCtrl.SetColumnWidth( 2, 100);
	m_ListCtrl.SetColumnWidth( 3, 100);
	m_ListCtrl.SetColumnWidth( 4, 100);

	// set functionality of list according to column
	m_ListCtrl.SetColumnType ( (fGetType)Text_List_Type);	
	//m_ListCtrl.ComboItem()
	vector<CString> tmpVec;
	for (int i=0; i<m_font.GetCount(); i++)
	{
		tmpVec.push_back(m_font.GetAt(i));
	}

	m_ListCtrl.m_Map.insert(std::make_pair(1, tmpVec));
	tmpVec.clear();
	for (int i=0; i<m_bigfont.GetCount(); i++)
	{
		tmpVec.push_back(m_bigfont.GetAt(i));
	}
	m_ListCtrl.m_Map.insert(std::make_pair(2, tmpVec));
	
	iniData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgTextSettings::InsertEmpty()
{
	int nItem = m_ListCtrl.GetItemCount();

	nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
	m_ListCtrl.SetItemText(nItem, 0, m_strDescription);
	m_ListCtrl.SetItemText(nItem, 1, m_strFont);
	m_ListCtrl.SetItemText(nItem, 2, m_strBigFont);
	m_ListCtrl.SetItemText(nItem, 3, m_strHeight);
	m_ListCtrl.SetItemText(nItem, 4, m_strScale);
}

void CDlgTextSettings::GetAcadFont()
{
	//搜索shx字型
	TCHAR prefix[2048];
	TCHAR path[_MAX_PATH];
	//char path[_MAX_PATH];
	//struct _finddata_t f_file;
	struct _tfinddata_t f_file;

	long hFile;
	CStringSplite Scfg;

	MyBaseUtils::GetVar(_T("acadprefix"), prefix);
	Scfg.Separator(_T(";"));    //对路径的分割符改为';'，00-07-16
	Scfg.Splite(prefix);
	Scfg.Separator(_T(" \t,;"));
	for (int i = 0; i < Scfg.GetCount(); i++)
	{
		if (Scfg[i][_tcslen(Scfg[i])-1] == '\\') 
			wsprintf(path, _T("%s*.shx"), Scfg[i]);
		//sprintf(path, "%s*.shx", Scfg[i]);
		else
			wsprintf(path, _T("%s\\*.shx"), Scfg[i]);
		//sprintf(path, "%s\\*.shx", Scfg[i]);
		if( (hFile = 	_tfindfirst(path, &f_file )) == -1L ) 
			continue;
		//if( (hFile = _findfirst(path, &f_file )) == -1L ) 
		//	continue;
		CString name;
		name = f_file.name;
		if (f_file.size > 1e5)
		{
			if(!PdThenameIsHave(name,m_bigfont))
				m_bigfont.Add(name);  //判定中英文字型
		}
		else
		{
			if(!PdThenameIsHave(name,m_font))
			{
				m_font.Add(name);
				m_cadfont.Add(name);
			}
		}

		//while(!_findnext( hFile, &f_file ))
		while(!_tfindnext( hFile, &f_file ))
		{
			name = f_file.name;
			if (f_file.size > 1e5)
			{
				if(!PdThenameIsHave(name,m_bigfont))
					m_bigfont.Add(name);  //判定中英文字型
			}
			else
			{
				if(!PdThenameIsHave(name,m_font))
				{
					m_font.Add(name);
					m_cadfont.Add(name);
				}
			}
		}
		_findclose( hFile );
	}
	m_bigfont.Add(_T(""));	//加一项，选这项就是说没有大字体
}

void CDlgTextSettings::GetWindowFont()
{
	HDC hdc = ::GetDC(m_hWnd);
	//LPLOGFONT lpLogFont = new LOGFONT;
	//memset(lpLogFont, 0, sizeof (LOGFONT));
	//lpLogFont->lfCharSet = GB2312_CHARSET;
	//lpLogFont->lfFaceName[0] = 0;	
	LOGFONT lpLogFont;
	lpLogFont.lfCharSet = GB2312_CHARSET;
	lpLogFont.lfFaceName[0] = 0;

	EnumFontFamiliesEx(hdc, &lpLogFont, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&m_font, 0); 
	::ReleaseDC(m_hWnd, hdc);
}


void CDlgTextSettings::CreateTextStyleId()
{
	int nItem = m_ListCtrl.GetItemCount();
	for (int i=0; i<nItem; i++)
	{
		m_strDescription = m_ListCtrl.GetItemText(i, 0);
		m_strFont = m_ListCtrl.GetItemText(i, 1);
		m_strBigFont = m_ListCtrl.GetItemText(i, 2);
		m_strHeight = m_ListCtrl.GetItemText(i, 3);
		m_strScale = m_ListCtrl.GetItemText(i, 4);
		MySymble::CreateTextStyle(m_strDescription, m_strFont, m_strBigFont, MyTransFunc::StringToDouble(m_strScale), MyTransFunc::StringToDouble(m_strHeight), TRUE);
	}
	ReWriteFile();
}

BOOL CDlgTextSettings::PdThenameIsHave(CString name, CStringArray& arr)
{
	CString str;
	name.MakeUpper();
	for(int i = 0; i < arr.GetSize(); i++)
	{
		str = arr[i];
		str.MakeUpper();
		if(str == name) return TRUE;
	}
	return FALSE;
}
void CDlgTextSettings::OnBnClickedButtonAdd()
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
	m_strFont = m_ListCtrl.GetItemText(nItem - 1, 1);
	m_strBigFont = m_ListCtrl.GetItemText(nItem - 1, 2);
	m_strHeight = m_ListCtrl.GetItemText(nItem - 1, 3);
	m_strScale = m_ListCtrl.GetItemText(nItem - 1, 4);
	InsertEmpty();
	GetParent()->SetFocus();
}

void CDlgTextSettings::OnBnClickedButtonDel()
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

void CDlgTextSettings::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int     iItem = pNMListView->iItem;
	int     iSubItem = pNMListView->iSubItem;

	// retrieve the ProductName & load corresponding cells with 
	// ProductPrice and ProductID
	/*if (iSubItem == 1)
	{*/
		CString sFontName = m_ListCtrl.GetItemText( iItem, 1); 
		int nCount = m_cadfont.GetSize();
		int i;
		for(i = 0; i < nCount; i++)
		{
			if(_tcsicmp(sFontName, m_cadfont[i]) == 0)
				break;
		}
		if(i == nCount)
		{
			m_ListCtrl.SetItemText(iItem, 2, _T(""));
		}
	//}
	
	/*for ( int i = 0 ;  i < 4; i++)
	{
		if ( sProductName == Product[i] )
		{

			m_cList.SetItemText( iItem, ePrice, Price[i]);
			m_cList.SetItemText( iItem, eID, ID[i]);
			break;
		}
	}*/

	*pResult = 0;
}


void CDlgTextSettings::iniData()
{
	CString strApp = MyBaseUtils::GetAppPath();
	CString strFileName = strApp + _T("design\\basic\\textstyle.ini");
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
		m_strFont = MyParserString::SubString(buffer, _T(","), 1);
		m_strBigFont = MyParserString::SubString(buffer, _T(","), 2);
		m_strHeight = MyParserString::SubString(buffer, _T(","), 3);
		m_strScale = MyParserString::SubString(buffer, _T(","), 4);
		int nItem = m_ListCtrl.GetItemCount();
		nItem = m_ListCtrl.InsertItem( nItem+1, _T("") );
		SetInfoToListCtrl(nItem);
	}

	File.Close();
}

void CDlgTextSettings::ReWriteFile()
{
	CString strApp = MyBaseUtils::GetAppPath();
	CString strFileName = strApp + _T("design\\basic\\textstyle.ini");
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

		strTmp.Format(_T("%s,%s,%s,%s,%s\n"), m_strDescription, m_strFont, m_strBigFont, m_strHeight, m_strScale);
		File.WriteString(strTmp);
	}
	File.Close();
}
void CDlgTextSettings::SetInfoToListCtrl(int nItem)
{
	m_ListCtrl.SetItemText(nItem, 0, m_strDescription);
	m_ListCtrl.SetItemText(nItem, 1, m_strFont);
	m_ListCtrl.SetItemText(nItem, 2, m_strBigFont);
	m_ListCtrl.SetItemText(nItem, 3, m_strHeight);
	m_ListCtrl.SetItemText(nItem, 4, m_strScale);
}
void CDlgTextSettings::GetInfoFromListCtrl(int i)
{
	m_strDescription = m_ListCtrl.GetItemText(i, 0);
	m_strFont = m_ListCtrl.GetItemText(i, 1);
	m_strBigFont = m_ListCtrl.GetItemText(i, 2);
	m_strHeight = m_ListCtrl.GetItemText(i, 3);
	m_strScale = m_ListCtrl.GetItemText(i, 4);
}
