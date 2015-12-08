#include "StdAfx.h"
#include "DataStore.h"
#include "Global.h"
#include "ArxDbgUtils.h"


CDataCell::CDataCell(void)
{

}

CDataCell::~CDataCell()
{

}


CString CDataCell::GetFItemId() const
{
	return m_FItemId;
}

CString CDataCell::GetFName() const
{
	return m_FName;
}

CString CDataCell::GetF105() const
{
	return m_F105;
}

CString CDataCell::GetF106() const
{
	return m_F106;
}

CString CDataCell::GetF107() const
{
	return m_F107;
}

CString CDataCell::GetFNumber() const
{
	return m_FNumber;
}

CString CDataCell::GetFErpCls() const
{
	return m_FErpCls;
}

CString CDataCell::GetFMaund() const
{
	return m_FMaund;
}


void CDataCell::SetFItemId(CString str)
{
	m_FItemId = str;
}


void CDataCell::SetFName(CString str)
{
	m_FName = str;
}

void CDataCell::SetF105(CString str)
{
	m_F105 = str;
}

void CDataCell::SetF106(CString str)
{
	m_F106 = str;
}

void CDataCell::SetF107(CString str)
{
	m_F107 = str;
}

void CDataCell::SetFNumber(CString str)
{
	m_FNumber = str;
}	

void CDataCell::SetFErpCls(CString str)
{
	m_FErpCls = str;
}

void CDataCell::SetFMaund(CString str)
{
	m_FMaund = str;
}


CString CDataCell::SetFullString() const//获取一串完整的字符串
{
	CString strBuff;
	strBuff.Format(_T("%s？%s？%s？%s？%s？%s？%s？%s\n"), 
		m_FItemId, 
		m_FName, 
		m_F105, 
		m_F106, 
		m_F107, 
		m_FNumber, 
		m_FErpCls, 
		m_FMaund);
	return strBuff;
}

COperateCsv::COperateCsv(void)
{
	clear();
	m_F105Map.clear();
	m_F106Map.clear();
}

COperateCsv::~COperateCsv()
{
	//clear();
	m_F105Map.clear();
	m_F106Map.clear();
}
//读取文件
bool COperateCsv::Read()
{
	CStdioFile File;
	if (!File.Open(m_strFileName, CFile::modeReadWrite  | CFile::typeText)) 
	{
		return false;
	}
	CString buffer;
	while(File.ReadString(buffer))
	{
		//CDataCell * pCell = new CDataCell;
		CDataCell pCell;
		pCell.SetFItemId(gGlobal.SubString(buffer, _T("？"), 0));
		pCell.SetFName(gGlobal.SubString(buffer, _T("？"), 1));
		pCell.SetF105(gGlobal.SubString(buffer, _T("？"), 2));
		pCell.SetF106(gGlobal.SubString(buffer, _T("？"), 3));
		m_F105Map.insert(std::make_pair(gGlobal.SubString(buffer, _T("？"), 2), _T("F105")));
		m_F106Map.insert(std::make_pair(gGlobal.SubString(buffer, _T("？"), 3), _T("F106")));
		pCell.SetF107(gGlobal.SubString(buffer, _T("？"), 4));
		pCell.SetFNumber(gGlobal.SubString(buffer, _T("？"), 5));
		pCell.SetFErpCls(gGlobal.SubString(buffer, _T("？"), 6));
		pCell.SetFMaund(gGlobal.SubString(buffer, _T("？"), 7));
		m_Data.push_back(pCell);
	}
	File.Close();
	return true;
}

bool COperateCsv::Write()
{
	CStdioFile File;
	if (!File.Open(m_strFileName, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	{
		return false;
	}
	CString strBuff;
	for (vector<CDataCell>::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		CDataCell pCell;
		pCell = *iter;
		strBuff = pCell.SetFullString();
		File.WriteString(strBuff);
	}
	File.Close();
	return true;
}

//
CString COperateCsv::GetFileName() const
{
	return m_strFileName;
}

void COperateCsv::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

bool COperateCsv::IsFileExit()
{
	CFileFind  findFile;
	bool bRet = (bool)findFile.FindFile(m_strFileName,0);
	return bRet;
}

vector<CDataCell> COperateCsv::GetData() const
{
	return m_Data;
}


void COperateCsv::SetData(vector<CDataCell> vec)
{
	m_Data.clear();
	m_Data.assign(vec.begin(), vec.end());
}

void COperateCsv::SetSingleData(CDataCell pCell)
{
	//if (pCell != NULL)
	{
		m_Data.push_back(pCell);
	}
}

void COperateCsv::clear()
{
	/*CDataCell* pCell = NULL;
	for (vector<CDataCell>::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		pCell = *iter;
		if (pCell != NULL)
		{
			delete pCell;
			pCell = NULL;
		}
	}*/
	m_Data.clear();
}


map<CString, CString> COperateCsv::GetStandardMap() const
{
	return m_F106Map;
}
map<CString, CString> COperateCsv::GetCailiaoMap() const
{
	return m_F105Map;
}


