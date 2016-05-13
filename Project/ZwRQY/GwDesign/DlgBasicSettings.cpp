// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- DlgBasicSettings.cpp : Implementation of CDlgBasicSettings
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgBasicSettings.h"

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
	m_strScale = _T("1.0");
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
	InsertEmpty();

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
}
