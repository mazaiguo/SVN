#include "StdAfx.h"
#include "DrawObstacle.h"
#include "BcUtils.h"

CDrawObstacle::CDrawObstacle(void)
{
}

CDrawObstacle::CDrawObstacle(int nIndex, CString strName)
{
	m_nIndex = nIndex;
	m_strName = strName;
}

CDrawObstacle::~CDrawObstacle(void)
{
}

bool CDrawObstacle::doIt()
{
	
	return true;
}

CString CDrawObstacle::getName()
{
	return m_strName;
}

void CDrawObstacle::setName(LPCTSTR strName)
{	
	m_strName = strName;
}

int CDrawObstacle::getIndex()
{
	return m_nIndex;
}

void CDrawObstacle::setIndex(int nIndex)
{
	m_nIndex = nIndex;
}

bool CDrawObstacle::getZhuanghao()
{
	DrawDMXProcess dm;
	if (!dm.Insert(true))
	{
		return false;
	}
	CString strLabel = dm.getLabelName();
	CBcUtils bc;
	bc.get(strLabel, m_zdmdata);
	//////////////////////////////////////////////////////////////////////////
	if (!gethasBase())
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	if (!getTopOrBottom())
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	


	return true;
}

bool CDrawObstacle::gethasBase()
{
	CString strPrompt;	
	bool bRet = true;

	acedInitGet(0, _T("A B"));
	strPrompt = _T("\n输入顶高(A)/低高（B）/<管底>:");

	TCHAR val[512];
	int nRet = acedGetKword(strPrompt, val);
	if (nRet == RTNORM)
	{
		if (_tcscmp(val, _T("A")) == 0)
		{
			bRet = GetGuanDing();
		}
		else if (_tcscmp(val, _T("B")) == 0)
		{
			bRet = GetGuandi();
		}		
	}
	else if (nRet == RTNONE)
	{
		bRet = GetGuandi();
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CDrawObstacle::getTopOrBottom()
{
	
	CString strPrompt;	
	bool bRet = true;

	acedInitGet(0, _T("A B"));
	strPrompt = _T("\n输入顶高(A)/低高（B）/<管底>:");

	TCHAR val[512];
	int nRet = acedGetKword(strPrompt, val);
	if (nRet == RTNORM)
	{
		if (_tcscmp(val, _T("A")) == 0)
		{
			bRet = GetGuanDing();
		}
		else if (_tcscmp(val, _T("B")) == 0)
		{
			bRet = GetGuandi();
		}		
	}
	else if (nRet == RTNONE)
	{
		bRet = GetGuandi();
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CDrawObstacle::GetGuanDing()
{
	//throw std::logic_error("The method or operation is not implemented.");
	double dResult = 0.0;
	int nResult = acedGetReal(_T("\n请输入管顶数据："), &dResult);
	if (nResult != RTNORM)
	{
		return false;
	}
	
	return true;
}

bool CDrawObstacle::GetGuandi()
{
	//throw std::logic_error("The method or operation is not implemented.");
	double dResult = 0.0;
	int nResult = acedGetReal(_T("\n请输入管底数据："), &dResult);
	if (nResult != RTNORM)
	{
		return false;
	}

	return true;
}
