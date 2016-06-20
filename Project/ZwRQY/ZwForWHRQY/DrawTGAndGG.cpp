#include "StdAfx.h"
#include "DrawTGAndGG.h"

CDrawTGAndGG::CDrawTGAndGG(void)
{
}

CDrawTGAndGG::~CDrawTGAndGG(void)
{
}

bool CDrawTGAndGG::drawTG()
{
	
	return true;
}

bool CDrawTGAndGG::drawGG()
{
	if (!getStartZH())
	{
		return false;
	}
	if (!getEndZH())
	{
		return false;
	}
	
	if (!getDescription())
	{
		return false;
	}
	drawlineAndText();
	return true;
}

bool CDrawTGAndGG::getStartZH()
{
	int nRet = acedGetReal(_T("\n请选择起始桩号："),&m_dStartZH);
	if (nRet != RTNORM)
	{
		return false;
	}
	return true;
}

bool CDrawTGAndGG::getEndZH()
{
	int nRet = acedGetReal(_T("\n请选择终止桩号："),&m_dEndZH);
	if (nRet != RTNORM)
	{
		return false;
	}
	return true;
}

bool CDrawTGAndGG::drawlineAndText()
{
	return true;
}

bool CDrawTGAndGG::getDescription()
{
	TCHAR tempBuf[255];
	CString strPrompt;
	CString strTmp;
	strTmp.Format(_T("埋深不足管沟保护%.fm"), m_dEndZH - m_dStartZH);
	strPrompt = _T("\n说明文字") + strTmp;
	int nRet = acedGetString(1, strPrompt,  tempBuf);
	if (nRet == RTNONE)
	{
		m_strDescription = strTmp;
	}
	else if (nRet == RTNORM)
	{
		m_strDescription = tempBuf;
		if (m_strDescription.IsEmpty())
		{
			m_strDescription = strTmp;
		}
	}
	else
	{
		return false;
	}
	return true;
}
