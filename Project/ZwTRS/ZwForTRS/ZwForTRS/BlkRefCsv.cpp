#include "StdAfx.h"
#include "BlkRefCsv.h"
#include "Global.h"

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CBlkRefCsv::CBlkRefCsv(void)
{

}

CBlkRefCsv::~CBlkRefCsv()
{

}
//读取文件
bool CBlkRefCsv::Read()
{
	CStdioFile File;
	if (!File.Open(m_strFileName, CFile::modeReadWrite  | CFile::typeText)) 
	{
		return false;
	}
	CString buffer;
	while(File.ReadString(buffer))
	{
		m_pCell.SetFItemId(gGlobal.SubString(buffer, _T("？"), 0));
		m_pCell.SetFName(gGlobal.SubString(buffer, _T("？"), 1));
		m_pCell.SetF105(gGlobal.SubString(buffer, _T("？"), 2));
		m_pCell.SetF106(gGlobal.SubString(buffer, _T("？"), 3));
		m_pCell.SetF107(gGlobal.SubString(buffer, _T("？"), 4));
		m_pCell.SetFNumber(gGlobal.SubString(buffer, _T("？"), 5));
		m_pCell.SetFErpCls(gGlobal.SubString(buffer, _T("？"), 6));
		m_pCell.SetFMaund(gGlobal.SubString(buffer, _T("？"), 7));
	}
	File.Close();
	return true;
}

bool CBlkRefCsv::Write()
{

	CFileFind  findFile;
	BOOL bFind = findFile.FindFile(m_strFileName,0);
	if (bFind)
	{
		::DeleteFile(m_strFileName);
	}
	CStdioFile File;
	if (!File.Open(m_strFileName, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	{
		return false;
	}

	CString strBuff = m_pCell.SetFullString();
	File.WriteString(strBuff);
	File.Close();
	return true;
}

//
CString CBlkRefCsv::GetFileName() const
{
	return m_strFileName;
}

void CBlkRefCsv::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

bool CBlkRefCsv::IsFileExit()
{
	CFileFind  findFile;
	bool bRet = (bool)findFile.FindFile(m_strFileName,0);
	return bRet;
}

CDataCell CBlkRefCsv::GetData() const
{
	return m_pCell;
}


void CBlkRefCsv::SetData(CDataCell pCell)
{
	m_pCell = pCell;
}



CDataBaseInfo CBlkRefCsv::SetDataCell(CDataCell pCell)
{
	CDataBaseInfo DBInfo;
	DBInfo.SetFName(pCell.GetFName());
	DBInfo.SetFErpCls(pCell.GetFErpCls());
	DBInfo.SetDanzhong(pCell.GetF107());
	DBInfo.SetCailiao(pCell.GetF105());
	DBInfo.SetFItemId(pCell.GetFItemId());
	DBInfo.SetBeizhu(pCell.GetF106());
	DBInfo.SetDanwei(pCell.GetFMaund());
	DBInfo.SetFNumber(pCell.GetFNumber());
	return DBInfo;
}

CDataCell CBlkRefCsv::GetDataCellByDB(CDataBaseInfo DBInfo)
{
	CDataCell pCell;
	pCell.SetFName(DBInfo.GetFName());
	pCell.SetFErpCls(DBInfo.GetFErpCls());
	pCell.SetF107(DBInfo.GetDanzhong());
	pCell.SetF105(DBInfo.GetCailiao());
	pCell.SetFItemId(DBInfo.GetFItemId());
	pCell.SetF106(DBInfo.GetBeizhu());
	pCell.SetFMaund(DBInfo.GetDanwei());
	pCell.SetFNumber(DBInfo.GetFNumber());
	return pCell;
}



void CBlkRefCsv::SetSpecialString(CDataBaseInfo dbInfo)
{

	CFileFind  findFile;
	BOOL bFind = findFile.FindFile(m_strFileName,0);
	if (bFind)
	{
		::DeleteFile(m_strFileName);
	}
	CStdioFile File;
	if (!File.Open(m_strFileName, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	{
		return;
	}

	CString strBuff;
	strBuff.Format(_T("%s？%s？%s？%s？%s？%s？%s？%s？%s？%s\n"), 
		dbInfo.GetFItemId(), 
		dbInfo.GetFName(), 
		dbInfo.GetCailiao(), 
		dbInfo.GetBeizhu(), 
		dbInfo.GetDanzhong(), 
		dbInfo.GetFNumber(), 
		dbInfo.GetFErpCls(), 
		dbInfo.GetDanwei(),
		dbInfo.GetShuliang(),//数量
		dbInfo.GetZongzhong());//总重
	File.WriteString(strBuff);
	File.Close();
	return;
}
