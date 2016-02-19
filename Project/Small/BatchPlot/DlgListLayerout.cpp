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
//----- DlgListLayerout.cpp : Implementation of CDlgListLayerout
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgListLayerout.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgListLayerout, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgListLayerout, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgListLayerout::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgListLayerout::CDlgListLayerout (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgListLayerout::IDD, pParent, hInstance) 
{
	m_AllLayerOutNameArr.RemoveAll();
	m_strLayerOutName.RemoveAll();
}

CDlgListLayerout::~CDlgListLayerout()
{
	m_AllLayerOutNameArr.RemoveAll();
	m_strLayerOutName.RemoveAll();
}
//-----------------------------------------------------------------------------
void CDlgListLayerout::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST_LAYEROUT, m_ListBLayerOut);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgListLayerout::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgListLayerout::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnOK();

	int nCount = m_ListBLayerOut.GetSelCount();  
	CArray<int,int> aryListBoxSel;  

	aryListBoxSel.SetSize(nCount);  
	m_ListBLayerOut.GetSelItems(nCount, aryListBoxSel.GetData());   
	//得到总数  
	//得到选中的多项  
	for (int i=0;i<nCount;i++)  
	{  
		CString selStr;  
		m_ListBLayerOut.GetText(aryListBoxSel[i],selStr);  
		m_strLayerOutName.Add(selStr);
	}  
}

BOOL CDlgListLayerout::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetAllLayerOutName();
	for (int i=0; i<m_AllLayerOutNameArr.GetCount(); i++)
	{
		m_ListBLayerOut.AddString(m_AllLayerOutNameArr.GetAt(i));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgListLayerout::GetAllLayerOutName()
{
	AcDbDictionary* pDict = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayoutDictionary(pDict, AcDb::kForRead);
	AcDbDictionaryIterator* pDictIter = NULL;
	pDictIter = pDict->newIterator();
	CString strDictName;
	for (; !pDictIter->done(); pDictIter->next())
	{
		strDictName = pDictIter->name();
		if (strDictName.CompareNoCase(_T("Model")) != 0)
		{
			m_AllLayerOutNameArr.Add(strDictName);
		}
	}
	pDict->close();
	delete pDictIter;
}

void CDlgListLayerout::getDwgLayerOutName(CStringArray& strRet)
{
	strRet.RemoveAll();
	for (int i=0; i<m_strLayerOutName.GetSize(); i++)
	{
		strRet.Add(m_strLayerOutName.GetAt(i));
	}
}