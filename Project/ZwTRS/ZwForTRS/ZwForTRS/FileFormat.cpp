#include "StdAfx.h"
#include "FileFormat.h"
#include "ArxDbgUtils.h"

CFileFormat::CFileFormat(void)
{
	m_Data.clear();
	m_nCombine = 1;
}

CFileFormat::~CFileFormat(void)
{
}

int CFileFormat::getCombine() const
{
	return m_nCombine;
}

int CFileFormat::getStart() const
{
	return m_nStart;
}

//int CFileFormat::getKey1() const
//{
//	return m_nKey1;
//}
//
//int CFileFormat::getKey2() const
//{
//	return m_nKey2;
//}
//
//int CFileFormat::getKey3() const
//{
//	return m_nKey3;
//}
//
//int CFileFormat::getKey4() const
//{
//	return m_nKey4;
//}
//
//int CFileFormat::getKey5() const
//{
//	return m_nKey5;
//}
//
//int CFileFormat::getKey6() const
//{
//	return m_nKey6;
//}
//
//
//LPCTSTR CFileFormat::getKey1()
//{
//	return m_strKey1;
//}
//
//LPCTSTR CFileFormat::getKey2()
//{
//	return m_strKey2;
//}
//
//LPCTSTR CFileFormat::getKey3()
//{
//	return m_strKey3;
//}
//
//LPCTSTR CFileFormat::getKey4()
//{
//	return m_strKey4;
//}
//
//LPCTSTR CFileFormat::getKey5()
//{
//	return m_strKey5;
//}
//
//LPCTSTR CFileFormat::getKey6()
//{
//	return m_strKey6;
//}

LPCTSTR CFileFormat::getData()
{
	return m_strData;
}

DataVec CFileFormat::GetKeyData()
{
	return m_Data;
}
//设置数据
//void CFileFormat::setCombine(int nCount)
//{
//
//}
//
//void CFileFormat::setStart(int nCount)
//{
//
//}
//void CFileFormat::setKey1(int nCount)
//{
//
//}
//void CFileFormat::setKey2(int nCount)
//{
//
//}
//void CFileFormat::setKey3(int nCount)
//{
//
//}
//void CFileFormat::setKey4(int nCount)
//{
//
//}
//void CFileFormat::setKey5(int nCount)
//{
//
//}
//void CFileFormat::setKey6(int nCount)
//{
//
//}
//
//void CFileFormat::setKey1(LPCTSTR str)
//{
//
//}
//void CFileFormat::setKey2(LPCTSTR str)
//{
//
//}
//void CFileFormat::setKey3(LPCTSTR str)
//{
//
//}
//void CFileFormat::setKey4(LPCTSTR str)
//{
//
//}
//void CFileFormat::setKey5(LPCTSTR str)
//{
//
//}
//void CFileFormat::setKey6(LPCTSTR str)
//{
//
//}

//从文件中读取数据
bool CFileFormat::Read(LPCTSTR strFileName)
{
	bool bRet = true;

	m_IniFile.SetPath(strFileName);
	m_IniFile.Reset();
	m_IniFile.ReadFile();
	
	//起始点位置
	m_nStart = (int)m_IniFile.GetValue(_T("Excel设置"), _T("起始位置"), 0);
	m_nCombine = (int)m_IniFile.GetValue(_T("Excel设置"), _T("合并行数"), 0);
	m_strData = m_IniFile.GetValue(_T("Excel设置"), _T("数据序列"));
	CString strInput;
	strInput = m_IniFile.GetValue(_T("Excel设置"), _T("关键字1"));
	bRet = GetDataFromString(strInput/*, m_nKey1, m_strKey1*/);
	if (!bRet)
	{
		return bRet;
	}
	strInput = m_IniFile.GetValue(_T("Excel设置"), _T("关键字2"));
	bRet = GetDataFromString(strInput/*, m_nKey2, m_strKey2*/);
	if (!bRet)
	{
		return bRet;
	}

	strInput = m_IniFile.GetValue(_T("Excel设置"), _T("关键字3"));
	bRet = GetDataFromString(strInput/*, m_nKey3, m_strKey3*/);
	if (!bRet)
	{
		return bRet;
	}

	strInput = m_IniFile.GetValue(_T("Excel设置"), _T("关键字4"));
	bRet = GetDataFromString(strInput/*, m_nKey4, m_strKey4*/);
	if (!bRet)
	{
		return bRet;
	}

	strInput = m_IniFile.GetValue(_T("Excel设置"), _T("关键字5"));
	bRet = GetDataFromString(strInput/*, m_nKey5, m_strKey5*/);
	if (!bRet)
	{
		return bRet;
	}

	strInput = m_IniFile.GetValue(_T("Excel设置"), _T("关键字6"));
	bRet = GetDataFromString(strInput/*, m_nKey6, m_strKey6*/);
	if (!bRet)
	{
		return bRet;
	}

	return bRet;
}

bool CFileFormat::GetDataFromString(CString strInput)
{ 
	int nOut;
	CString strOut;
	pair<CString, int> tmpPair;
	bool bRet = true;
	int nCount = strInput.Find(_T("、"));
	int nLength;
	if (nCount==-1)
	{
		AfxMessageBox(_T("ini文件中填写的数据不符合规范，数据读取出错"));
		return false;
	}
	CString strTmp;
	strOut = strInput.Left(nCount);
	nLength = strInput.GetLength();
#ifdef UNICODE
	strTmp = strInput.Right(nLength - nCount - 1);
#else
	strTmp = strInput.Right(nLength - nCount - 2);
#endif
	nOut = ArxDbgUtils::StringToInt(strTmp);
	tmpPair.first = strOut;
	tmpPair.second = nOut;
	m_Data.push_back(tmpPair);
	return true;
}

