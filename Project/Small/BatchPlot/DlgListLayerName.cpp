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
//----- DlgListLayerName.cpp : Implementation of CDlgListLayerName
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgListLayerName.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgListLayerName, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgListLayerName, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CDlgListLayerName::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgListLayerName::CDlgListLayerName (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgListLayerName::IDD, pParent, hInstance)
{
	m_LayerNameArr.RemoveAll();
}

CDlgListLayerName::~CDlgListLayerName()
{
	m_LayerNameArr.RemoveAll();
}
//-----------------------------------------------------------------------------
void CDlgListLayerName::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST_LAYERNAME, m_ListbLayerName);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgListLayerName::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgListLayerName::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount = m_ListbLayerName.GetSelCount();  
	CArray<int,int> aryListBoxSel;  

	aryListBoxSel.SetSize(nCount);  
	m_ListbLayerName.GetSelItems(nCount, aryListBoxSel.GetData());   
	//得到总数  
	//得到选中的多项  
	for (int i=0;i<nCount;i++)  
	{  
		CString selStr;  
		m_ListbLayerName.GetText(aryListBoxSel[i],selStr);  
		selStr += _T(",");
		m_strLayerName += selStr;
	}  
	int nLength = m_strLayerName.GetLength();
	if (nLength > 2048)
	{
		AfxMessageBox(_T("数据长度超过2048，请减少数据量"));
		return;
	}
	CAcUiDialog::OnOK();

}

BOOL CDlgListLayerName::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	getDwgLayerName();
	for (int i=0; i<m_LayerNameArr.GetCount(); i++)
	{
		m_ListbLayerName.AddString(m_LayerNameArr.GetAt(i));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//获取图纸中所有的图层名
void CDlgListLayerName::getDwgLayerName()
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return;
	AcDbLayerTableIterator* pIter;
	pTable->newIterator(pIter);
	AcDbLayerTableRecord* pTblRec = NULL;
	for (; !pIter->done(); pIter->step())
	{
		CString strName;
		const TCHAR* locName;
		pIter->getRecord(pTblRec, AcDb::kForRead);
		pTblRec->getName(locName);
		strName = locName;
		m_LayerNameArr.Add(strName);
		pTblRec->close();
	}
	delete pIter;
	pTable->close();
}

CString CDlgListLayerName::GetLayerName()
{
	return m_strLayerName;
}
