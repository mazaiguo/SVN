#include "StdAfx.h"
#include "BiaochiInfo.h"
#include "DrawBiaochi.h"

CBiaochiInfo::CBiaochiInfo(void)
{
}

CBiaochiInfo::~CBiaochiInfo(void)
{
}

bool CBiaochiInfo::start()
{
	bool bRet = true;
	acedInitGet(0 , _T("New Ground"));
	TCHAR szKword [132];
	szKword[0] = _T('N');	//给szKword一个默认值N
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n重新开始新的地面线设计(N)/继续原有的地面线设计(G)<N>:"), szKword);
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
//创建新的地面线
bool CBiaochiInfo::newLine()
{
	//初始化数据
	initData();

	CDrawBiaochi drawBc;
	bool bRet = drawBc.Draw();
	return bRet;
}
//在原有的数据上继续
bool CBiaochiInfo::oldLine()
{
	bool bDrawBc = CDMXUtils::getcreateBc();
	bool bRet = false;
	CDrawBiaochi drawBc;
	if (!bDrawBc)
	{
		bRet = drawBc.Draw();
	}
	else
	{
		bRet = drawBc.DrawNext();
	}
	return bRet;
}	

void CBiaochiInfo::initData()
{
	CDMXUtils::setNumCount(_T("1"));
	CDMXUtils::SetCurNum(_T("1"));
	CDMXUtils::SetJdNum(_T("1"));
	CDMXUtils::SetcreateJiedian(false);
	CDMXUtils::SetcreateBc(true);
	CDMXUtils::SetXScale(500);
	CDMXUtils::SetYScale(100);
}
