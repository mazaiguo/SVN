#include "StdAfx.h"
#include "DataSort.h"
#include "ArxDbgUtils.h"

CDataSort::CDataSort(void)
{
	//m_RetData.clear();
	m_Data.clear();
	m_StrData.clear();
}

CDataSort::~CDataSort(void)
{
	//m_RetData.clear();
	m_Data.clear();
	m_StrData.clear();
}


//设置数据
bool CDataSort::setData(map<CString, pair<CDataBaseInfo*, int> > Data, LPCTSTR str)
{
	CString strFile;
	strFile = str;
	DataVec tmpVec,retVec;
	retVec.clear();
	tmpVec.clear();
	if (!m_FormatFile.Read(strFile))
	{
		return false;
	}
	tmpVec = m_FormatFile.GetKeyData();
	
	CString strChartNum;
	int nCount;
	CDataBaseInfo* pDB = NULL;
	pair<CDataBaseInfo*, int> tmpPair;
	for (map<CString, pair<CDataBaseInfo*, int> >::iterator iter = Data.begin();
		iter != Data.end();
		++iter)
	{
		retVec.clear();
		strChartNum = iter->first;
		tmpPair = iter->second;
		pDB = tmpPair.first;
		nCount = tmpPair.second;
		retVec = getDataByKey(pDB, tmpVec, nCount, strChartNum);
		if (retVec.size() < 1)
		{
			continue;
		}
		else
		{
			m_Data.insert(std::make_pair(strChartNum, retVec));
			SortDataByString(strChartNum);
		}
	}
	return true;
}

//返回结果
//map<CString, map<CString, DataVec> > CDataSort::getData()
map<CString, DataVec> CDataSort::getData()
{
	//return m_RetData;
	return m_Data;
}

map<CString, vector<CString> > CDataSort::getStrData()
{
	return m_StrData;
}

int CDataSort::getCombineCount() const
{
	int nCount;
	nCount = m_FormatFile.getCombine();
	return nCount;
}

int CDataSort::getStart() const
{
	int nCount;
	nCount = m_FormatFile.getStart();
	return nCount;
}

	//从databaseinfo中根据关键字获取数据
DataVec CDataSort::getDataByKey(CDataBaseInfo* pDb, DataVec vecKey, int nCount, LPCTSTR strChartNum)
{
	DataVec retVec;
	retVec.clear();
	CString tmpStr,strOut;
	int nOut = 0;
	pair<CString, int> tmpPair;
	pair<CString, int> retPair;
	for (DataVec::iterator iter = vecKey.begin();
		iter != vecKey.end();
		++iter)
	{
		tmpPair = *iter;
		tmpStr = tmpPair.first;
		nOut = tmpPair.second;
		if (tmpStr.CompareNoCase(_T("代号")) == 0)
		{
			strOut = pDb->GetFItemId();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("名称")) == 0)
		{
			strOut = pDb->GetFName();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("材料")) == 0)
		{
			strOut = pDb->GetCailiao();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("单重")) == 0)
		{
			strOut = pDb->GetDanzhong();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if ((tmpStr.CompareNoCase(_T("备注")) == 0)||
				(tmpStr.CompareNoCase(_T("规格")) == 0))
		{
			strOut = pDb->GetBeizhu();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("单位")) == 0)
		{
			strOut = pDb->GetDanwei();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("零件类型")) == 0)
		{
			strOut = pDb->GetFErpCls();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("物料代码")) == 0)
		{
			strOut = pDb->GetFNumber();
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else if (tmpStr.CompareNoCase(_T("流程图编号")) == 0)
		{
			strOut = strChartNum;
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		/*else if (tmpStr.CompareNoCase(_T("序号")) == 0)
		{
			strOut.Format(_T("%d"), nCount);
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}*/
		else if (tmpStr.CompareNoCase(_T("数量")) == 0)
		{
			strOut.Format(_T("%d"), nCount);
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
		else 
		{
			strOut = _T("");
			retPair.first = strOut;
			retPair.second = nOut;
			retVec.push_back(retPair);
		}
	}
	return retVec;
}



void CDataSort::SortDataByString(LPCTSTR str)
{
	CString strPrefix, strNext, strNum, strPreChart;
	int nlen = ArxDbgUtils::GetPileLengthEx(str, strPrefix, strNext);
	if (strPrefix.IsEmpty())
	{
		strPreChart = strNext+ strNum;
	}
	else
	{
		strPreChart = strPrefix;
	}
	vector<CString> tmpVec;
	tmpVec.clear();
	tmpVec.push_back(str);
	pair<map<CString, vector<CString> >::iterator, bool> bRet =	m_StrData.insert(make_pair(strPreChart, tmpVec));
	if (!bRet.second)
	{
		bRet.first->second.push_back(str);
	}
}





////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//COutCell::COutCell(void)
//{
//
//}
//COutCell::~COutCell(void)
//{
//
//}
//LPCTSTR COutCell::getKey1()
//{
//	return m_strKey1;
//}
//LPCTSTR COutCell::getKey2()
//{
//	return m_strKey2;
//}
//LPCTSTR COutCell::getKey3()
//{
//	return m_strKey3;
//}
//LPCTSTR COutCell::getKey4()
//{
//	return m_strKey4;
//}
//LPCTSTR COutCell::getKey5()
//{
//	return m_strKey5;
//}
//LPCTSTR COutCell::getKey6()
//{
//	return m_strKey6;
//}
//
////设置数据
//void COutCell::setKey1(LPCTSTR str)
//{
//	m_strKey1 = str;
//}
//void COutCell::setKey2(LPCTSTR str)
//{
//	m_strKey2 = str;
//}
//void COutCell::setKey3(LPCTSTR str)
//{
//	m_strKey3 = str;
//}
//void COutCell::setKey4(LPCTSTR str)
//{
//	m_strKey4 = str;
//}
//void COutCell::setKey5(LPCTSTR str)
//{
//	m_strKey5 = str;
//}
//void COutCell::setKey6(LPCTSTR str)
//{
//	m_strKey6 = str;
//}
