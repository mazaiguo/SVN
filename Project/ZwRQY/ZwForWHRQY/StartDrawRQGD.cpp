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
	szKword[0] = _T('N');	//��szKwordһ��Ĭ��ֵN
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n���¿�ʼ�µĹܵ������(N)/����ԭ�еĹܵ������(G)<N>:"), szKword);
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

bool CStartDrawRQGD::newLine()
{
	if (m_nCout > 1)
	{
		AfxMessageBox(_T("����ȼ���ܵ���һ��dwg����ֻ�ܽ�һ���ܵ�"));
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
