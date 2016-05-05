#include "StdAfx.h"
#include "StartDrawRQGD.h"
#include "ZDMUtils.h"
#include "DrawGDProcess.h"

CStartDrawRQGD::CStartDrawRQGD(void)
{
	CString strCount = CDMXUtils::getCurNum();
	m_nCout = MyTransFunc::StringToInt(strCount);
}

CStartDrawRQGD::~CStartDrawRQGD(void)
{
}

bool CStartDrawRQGD::start()
{
	bool bRet = true;
	acedInitGet(0 , _T("New Ground"));
	TCHAR szKword [132];
	szKword[0] = _T('N');	//给szKword一个默认值N
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n重新开始新的管道线设计(N)/继续原有的管道线设计(G)<N>:"), szKword);
	if (nRet == RTNORM)	//如果得到合理的关键字
	{
		if (_tcscmp(szKword, _T("New")) == 0)
			bRet = newLine();
		else if (_tcscmp(szKword, _T("Ground")) == 0)
			bRet = oldLine();
	}
	else if (nRet == RTNONE)	//如果用户输入为空值
	{
		bRet = newLine();
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CStartDrawRQGD::newLine()
{
	if (m_nCout > 1)
	{
		AfxMessageBox(_T("已有燃气管道，一个dwg里面只能建一个管道"));
		return false;
	}

	bool bContinued = true;
	while(bContinued)
	{	
		CDrawGDProcess zdm;
		if (!zdm.Draw())
		{
			bContinued = false;
		}	
	}
	return true;
}

bool CStartDrawRQGD::oldLine()
{
	bool bRet = false;
	bool bContinued = true;
	while(bContinued)
	{	
		CDrawGDProcess zdm;
		if (!zdm.Draw())
		{
			bContinued = false;
		}	
	}
	return true;
}
