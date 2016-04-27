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
	szKword[0] = _T('N');	//��szKwordһ��Ĭ��ֵN
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n���¿�ʼ�µĵ��������(N)/����ԭ�еĵ��������(G)<N>:"), szKword);
	if (nRet == RTNORM)	//����õ�����Ĺؼ���
	{
		if (_tcscmp(szKword, _T("New")) == 0)
			bRet = newLine();
		else if (_tcscmp(szKword, _T("Ground")) == 0)
			bRet = oldLine();
	}
	else if (nRet == RTNONE)	//����û�����Ϊ��ֵ
	{
		bRet = newLine();
	}
	else
	{
		return false;
	}
	return bRet;
}
//�����µĵ�����
bool CBiaochiInfo::newLine()
{
	//��ʼ������
	initData();

	CDrawBiaochi drawBc;
	bool bRet = drawBc.Draw();
	return bRet;
}
//��ԭ�е������ϼ���
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
