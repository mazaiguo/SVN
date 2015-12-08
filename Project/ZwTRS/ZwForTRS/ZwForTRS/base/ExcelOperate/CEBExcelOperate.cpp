// CEBExcelOperate.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CEBExcelOperate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CMyExcelOperate construction

CMyExcelOperate::CMyExcelOperate()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("发生严重错误,退出!"));

		// {{MODIFY BY JJJ, 2007-10-21, 19:30:58, 93,MFC project can not use the eixt();
	    //exit(1); 
		return ;
		// {{MODIFY BY JJJ END
	};
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyExcelOperate object

//CMyExcelOperate theApp;


CMyExcelOperate::~CMyExcelOperate() 
{
	m_rgMyRge.ReleaseDispatch(); 
	m_wsMysheet.ReleaseDispatch(); 
	m_wssMysheets.ReleaseDispatch(); 
	m_wbMyBook.ReleaseDispatch(); 
	m_wbsMyBooks.ReleaseDispatch(); 
	m_ExcelApp.ReleaseDispatch();
	CoUninitialize();
}

BOOL CMyExcelOperate::CreateExcelFile(LPCTSTR szFileName)
{
	TRY
	{
		_Application    ExcelApp; 
		Workbooks	wbsMyBooks; 
		_Workbook	wbMyBook; 
		if (!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL)) 
		{ 
			return FALSE;
		}
		wbsMyBooks.AttachDispatch(ExcelApp.GetWorkbooks(),TRUE);
		wbMyBook.AttachDispatch(wbsMyBooks.Add(vtMissing));
		wbMyBook.SaveAs(_variant_t(szFileName),
			_variant_t((LONG)-4143),
			_variant_t(_T("")),
			_variant_t(_T("")),
			_variant_t((bool)FALSE),
			_variant_t((bool)FALSE),
			_variant_t((LONG)2),
			_variant_t((LONG)1),
			_variant_t((bool)FALSE),
			vtMissing,vtMissing);
		wbMyBook.ReleaseDispatch();
		wbsMyBooks.Close();
		wbsMyBooks.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		return TRUE;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return FALSE;
	}
	END_CATCH_ALL
}

BOOL CMyExcelOperate::WriteExcelDataA(const int nRow,CStringList * slData)
{
    return WriteData(nRow,slData);
}


BOOL CMyExcelOperate::OpenExcelFile(LPCTSTR szFileName, LPCTSTR szSheetName)
{
	TRY
	{
		if (!m_ExcelApp.CreateDispatch(_T("Excel.Application"),NULL)) 
		{ 
			return FALSE;
		}
		m_wbsMyBooks.AttachDispatch(m_ExcelApp.GetWorkbooks(),TRUE);
		m_wbMyBook.AttachDispatch(m_wbsMyBooks.Open(szFileName,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing));

		//Get Worksheets 
		m_wssMysheets.AttachDispatch(m_wbMyBook.GetWorksheets(),TRUE);

		long nlen = m_wssMysheets.GetCount();
		CString	strSheetName = szSheetName;
		if ((nlen > 0) &&(strSheetName.IsEmpty()))
		{
			m_wsMysheet.AttachDispatch(m_wssMysheets.GetItem(_variant_t(1)),TRUE);
		}
		else
		{
			//Get sheet1 
			m_wsMysheet.AttachDispatch(m_wssMysheets.GetItem(_variant_t(szSheetName)),TRUE); 
		}
		

		//Get All Cells，rgMyRge is Collection of cells
		m_rgMyRge.AttachDispatch(m_wsMysheet.GetCells(),TRUE); 
		Range cols;
		cols.AttachDispatch(m_rgMyRge.GetColumns());
		cols.AutoFit();
		return TRUE;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return FALSE;
	}
	END_CATCH_ALL
}

BOOL CMyExcelOperate::ReadData(int nRow, int nColCount, CStringList& szData)
{
	TRY
	{
		BOOL bTrue = FALSE;
		//Get X row X column unit's value
		for (int i = 1; i <= nColCount; i++)
		{
			_variant_t vaValue;
			vaValue = m_rgMyRge.GetItem(_variant_t((long)nRow),_variant_t((long)i));
			Range CurRange;
			CurRange.AttachDispatch(vaValue.pdispVal);
			CString szText;
			_variant_t va = CurRange.GetText();
			szText = va.bstrVal;
			if (szText != _T("")) 
			{
				bTrue = TRUE;
			}
			szData.AddTail(szText);	 
			CurRange.ReleaseDispatch();
		}
		return bTrue;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return FALSE;
    }
    END_CATCH_ALL
}

TCHAR* CMyExcelOperate::ReadDataEx(const int nRow,const int nCol)
{
	TRY
	{
		BOOL bTrue = FALSE;
		//Get X row X column unit's value
		_variant_t vaValue;
		vaValue = m_rgMyRge.GetItem(_variant_t((long)nRow),_variant_t((long)nCol));
		Range CurRange;
		CurRange.AttachDispatch(vaValue.pdispVal);
		CString szText;
		_variant_t va = CurRange.GetText();
		szText = va.bstrVal;
		if (szText != _T("")) 
		{
			bTrue = TRUE;
		}
		TCHAR* szData = new TCHAR[szText.GetLength() + 1];
		//		wcscpy(szData,szText);
		_tcscpy(szData,szText);
		CurRange.ReleaseDispatch();
		return szData;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return _T("");
	}
	END_CATCH_ALL
}

BOOL CMyExcelOperate::WriteData(int nRow, CStringList *slData)
{
	TRY
	{
		BOOL bTrue = FALSE;
		//Get X row X column unit's value
		int i = 1;
		POSITION pos = slData->GetHeadPosition();
		while (pos != NULL)
		{
			_variant_t vaValue;
			vaValue = m_rgMyRge.GetItem(_variant_t((long)nRow),_variant_t((long)i));
			Range CurRange;
			CurRange.AttachDispatch(vaValue.pdispVal);
			CString szText;
			szText = slData->GetNext(pos);
			_variant_t va(szText);
			CurRange.SetValue(va);
			CurRange.ReleaseDispatch();
			i++;
		}
		return bTrue;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return FALSE;
	}
    END_CATCH_ALL
}

BOOL CMyExcelOperate::WriteDataEx(const int nRow,const int nCol,LPCTSTR slData)
{
	TRY
	{
		_variant_t vaValue;
		vaValue = m_rgMyRge.GetItem(_variant_t((long)nRow),_variant_t((long)nCol));
		Range CurRange;
		CurRange.AttachDispatch(vaValue.pdispVal);
		_variant_t va(slData);
		CurRange.SetValue(va);
		CurRange.ReleaseDispatch();
		return TRUE;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return FALSE;
	}
	END_CATCH_ALL
}

BOOL CMyExcelOperate::CloseExcelFile()
{
	TRY
	{
		m_rgMyRge.ReleaseDispatch(); 
		m_wsMysheet.ReleaseDispatch(); 
		m_wssMysheets.ReleaseDispatch(); 
		m_wbMyBook.Close(_variant_t((bool)TRUE),vtMissing,vtMissing);
		m_wbMyBook.ReleaseDispatch(); 
		m_wbsMyBooks.Close();
		m_wbsMyBooks.ReleaseDispatch(); 
		m_ExcelApp.Quit();
		m_ExcelApp.ReleaseDispatch();
		return TRUE;
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(_T("严重错误!"));
		TCHAR   szCause[255];
		CString strFormatted;
		e->GetErrorMessage(szCause, 255);
		return FALSE;
	}
    END_CATCH_ALL    
}


bool CMyExcelOperate::MergeCell(const int nStartRow, const int nStartCol,
								const int nEndRow, const int nEndCol,
								const int nCombine)
{
	LPDISPATCH lpDisp = NULL;
	//CSheet sheet;
	Range  range;
	//lpDisp = m_wsMysheet.GetRange(_variant_t(strRow), _variant_t(strCol));
	for (int i=0; i<nCombine; i++)
	{
		_variant_t vaValue1,vaValue2;
		vaValue1 = m_rgMyRge.GetItem(_variant_t((long)nStartRow + i), _variant_t((long)nStartCol));
		vaValue2 = m_rgMyRge.GetItem(_variant_t((long)nEndRow + i), _variant_t((long)nEndCol));
		lpDisp = m_wsMysheet.GetRange(vaValue1, vaValue2);
		range.AttachDispatch(lpDisp);
		range.Merge((_variant_t )0);
	}
	
	if (nCombine > 1)
	{
		_variant_t vaValue1,vaValue2;
		vaValue1 = m_rgMyRge.GetItem(_variant_t((long)nStartRow), _variant_t((long)nStartCol));
		vaValue2 = m_rgMyRge.GetItem(_variant_t((long)nStartRow + nCombine -1), _variant_t((long)nStartCol));
		lpDisp = m_wsMysheet.GetRange(vaValue1, vaValue2);
		range.AttachDispatch(lpDisp);
		range.Merge((_variant_t )0);
	}
	

	/*lpDisp = m_wsMysheet.GetRange(_variant_t(_T("B13")), _variant_t(_T("S13")));
	range.AttachDispatch(lpDisp);
	range.Merge((_variant_t )0);
		
	lpDisp = m_wsMysheet.GetRange(_variant_t(_T("B12")), _variant_t(_T("B13")));
	range.AttachDispatch(lpDisp);
	range.Merge((_variant_t )0);*/
	return true;
}