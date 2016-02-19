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
//----- DlgBlockName.cpp : Implementation of CDlgBlockName
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgBlockName.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgBlockName, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgBlockName, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgBlockName::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgBlockName::CDlgBlockName (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgBlockName::IDD, pParent, hInstance) 
{
	m_BlockNameArr.RemoveAll();
}


CDlgBlockName::~CDlgBlockName()
{
	m_BlockNameArr.RemoveAll();
}
//-----------------------------------------------------------------------------
void CDlgBlockName::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST_LAYERNAME, m_ListbBlockName);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgBlockName::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgBlockName::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcUiDialog::OnOK();
	int nCount = m_ListbBlockName.GetSelCount();  
	CArray<int,int> aryListBoxSel;  

	aryListBoxSel.SetSize(nCount);  
	m_ListbBlockName.GetSelItems(nCount, aryListBoxSel.GetData());   
	//得到总数  
	//得到选中的多项  
	for (int i=0;i<nCount;i++)  
	{  
		CString selStr;  
		m_ListbBlockName.GetText(aryListBoxSel[i],selStr);  
		selStr += _T(",");
		m_strBlockName += selStr;
	}  
}

BOOL CDlgBlockName::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	getDwgBlockName();
	for (int i=0; i<m_BlockNameArr.GetCount(); i++)
	{
		m_ListbBlockName.AddString(m_BlockNameArr.GetAt(i));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//获取图纸中所有的图块名
void CDlgBlockName::getDwgBlockName()
{
	AcDbBlockTable* pTable;
	if (acdbCurDwg()->getBlockTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return;
	AcDbBlockTableIterator* pIter;
	pTable->newIterator(pIter);
	AcDbBlockTableRecord* pTblRec = NULL;
	for (; !pIter->done(); pIter->step())
	{
		CString strName;
		const TCHAR* locName;
		pIter->getRecord(pTblRec, AcDb::kForRead);
		pTblRec->getName(locName);
		strName = locName;
		//去掉带"*"的块名
		int nPos = strName.Find(_T("*"));
		if (nPos==0)
		{
			pTblRec->close();
			continue;
		}
		m_BlockNameArr.Add(strName);
		Acad::ErrorStatus es;
		es = pTblRec->close();
	}
	delete pIter;
	pTable->close();
}

CString CDlgBlockName::GetBlockName()
{
	return m_strBlockName;
}
