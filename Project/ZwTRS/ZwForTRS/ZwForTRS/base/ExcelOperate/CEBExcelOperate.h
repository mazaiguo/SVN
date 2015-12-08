
// CEBExcelOperate.h : main header file for the CEBEXCELOPERATE DLL
//

#if !defined(AFX_CEBEXCELOPERATE_H__06CC5EC5_6B56_4F90_A60A_39EF99C758AF__INCLUDED_)
#define AFX_CEBEXCELOPERATE_H__06CC5EC5_6B56_4F90_A60A_39EF99C758AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "..\resource.h"		// main symbols
#include "COMDEF.H"	// main symbols
#include "excel9.h"
/////////////////////////////////////////////////////////////////////////////
// CMyExcelOperate
// See CEBExcelOperate.cpp for the implementation of this class
//

class CMyExcelOperate
{
public:
	CMyExcelOperate();
	~CMyExcelOperate();


public:
	BOOL CloseExcelFile();
	BOOL ReadData(const int nRow,const int nColCount,CStringList& szData);
	TCHAR* ReadDataEx(const int nRow,const int nCol);
	BOOL WriteData(const int nRow,CStringList* slData);
	BOOL WriteDataEx(const int nRow,const int nCol,LPCTSTR slData);
	BOOL WriteExcelDataA(const int nRow,CStringList * slData);
	BOOL OpenExcelFile(LPCTSTR szFileName, LPCTSTR szSheetName = _T("Sheet1"));
	BOOL CreateExcelFile(LPCTSTR szFileName);
	//合并单元格
	//@param nStartRow 起始的行数
	//@param nStartCol 起始的列数
	//@param nEndRow   终止的行数
	//@param nEndCol   终止的列数
	//@param nCombine  合并的行数
	bool MergeCell(const int nStartRow, const int nStartCol,
					const int nEndRow, const int nEndCol,
					const int nCombine);
private:
	_Application	m_ExcelApp; 
	Workbooks	m_wbsMyBooks; 
	_Workbook	m_wbMyBook; 
	Sheets		m_wssMysheets; 
	_Worksheet	m_wsMysheet; 
	Range		m_rgMyRge;
	//{{AFX_MSG(CMyExcelOperate)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEBEXCELOPERATE_H__06CC5EC5_6B56_4F90_A60A_39EF99C758AF__INCLUDED_)
