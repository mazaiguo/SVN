#include "StdAfx.h"
#include "DrawGDProcess.h"
#include "BcUtils.h"
#include "CBiaochiForRQY.h"
#include "ZDMUtils.h"


CDrawGDProcess::CDrawGDProcess(void)
{
	CString strCount = CDMXUtils::getCurNum();
	m_nCout = MyTransFunc::StringToInt(strCount);
	
	m_dminElavation = CDMXUtils::getMinElavation();
	m_dmaxElavation = CDMXUtils::getMaxElavation();
	CBcUtils bcUtils;

	CString strLabel = BC_DICT + strCount;
	bcUtils.get(strLabel, m_pZdmInfo);
	m_dRealDmx = m_pZdmInfo.getRealDmx();
	if (m_nCout == 1)
	{
		m_dPipeDiameter = m_pZdmInfo.getPipeDiameter();
		m_dGuandi = m_dminElavation;
	}
	else
	{
		//如果不为1，需要取上一个存储数据中的数据
		int nTmp = m_nCout - 1;
		strCount.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCount;
		CZdmDataInfo pBiaoChi;
		bcUtils.get(strTmpLabel, pBiaoChi);
		m_dPipeDiameter = pBiaoChi.getPipeDiameter();
		m_dGuandi = pBiaoChi.getGuanDi();
	}
	
}

CDrawGDProcess::~CDrawGDProcess(void)
{
	/*if (m_pZdmInfo != NULL)
	{
		delete m_pZdmInfo;
		m_pZdmInfo = NULL;
	}*/
}

bool CDrawGDProcess::Draw()
{
	if (!GetPipeDiameter())
	{
		return false;
	}
	if (!GetKeyWord())
	{
		return false;
	}
	CDrawZDM zdm;
	zdm.setDrawGd(true);
	zdm.add();
	return true;
}

//bool CDrawGDProcess::Insert()
//{
//	CDrawZDM zdm;
//	zdm.insert();
//	return true;
//}
//
//bool CDrawGDProcess::Del()
//{
//	return true;
//}
//
//bool CDrawGDProcess::Mod()
//{
//	return true;
//}

bool CDrawGDProcess::GetPipeDiameter()
{
	CString strPrompt;
	if (m_nCout == 1)
	{
		strPrompt.Format(_T("\n管道管径<m> <%.2f>:"), m_dPipeDiameter);
	}
	else
	{
		acedInitGet(0, _T("Undo"));
		strPrompt.Format(_T("\n回退一步(U)/<下一点管道管径> <m> <%.2f>:"), m_dPipeDiameter);
	}
	int nRet = acedGetReal(strPrompt, &m_dPipeDiameter);
	if (nRet == RTNORM)
	{
		//return true;
	}
	else if (nRet == RTNONE)
	{
		//m_dZhuanghao = m_dZhuanghao;
		return RTERROR;
	}
	else if (nRet == RTKWORD)
	{
		doUndo();
		return RTKWORD;
	}
	else
	{
		return RTERROR;
	}
	m_pZdmInfo.setPipeDiameter(m_dPipeDiameter);

	return true;
}

bool CDrawGDProcess::GetKeyWord()
{
	CString strPrompt;	
	bool bRet = true;
	if (m_nCout == 1)
	{
		acedInitGet(0, _T("Di Wa"));
		strPrompt = _T("\n输入管底(D)/输入挖深(W)/<管底>:");
	}
	else
	{
		acedInitGet(0, _T("Di Wa Po Vertical"));
		strPrompt = _T("\n输入管底(D)/输入挖深(W)/坡度(P)/垂直间距(V)/<管底>:");
	}
	TCHAR val[512];
	int nRet = acedGetKword(strPrompt, val);
	if (nRet == RTNORM)
	{
		if (_tcscmp(val, _T("Di")) == 0)
		{
			bRet = GetGuanDi();
		}
		else if (_tcscmp(val, _T("Wa")) == 0)
		{
			bRet = GetWaShen();
		}
		else if (_tcscmp(val, _T("Po")) == 0)
		{
			bRet = GetPodu();
		}
		else if (_tcscmp(val, _T("Vertical") == 0)
		{
			bRet = GetVertical();
		}
	}
	else if (nRet == RTNONE)
	{
		bRet = GetGuanDi();
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CDrawGDProcess::GetGuanDi()
{
	CString strPrompt;
	strPrompt.Format(_T("\n管底深<m> <%.2f>:"), m_dGuandi);

	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dGuandi);
		if (nRet == RTNORM)
		{
			//return true;
			/*if (verifyHeight(m_dGuandi))
			{
				bRet = true;
			}*/
		}
		else if (nRet == RTNONE)
		{
			/*if (verifyHeight(m_dGuandi))
			{
				bRet = true;
			}*/
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}	
	m_dWashen = m_dRealDmx - m_dGuandi;
	m_pZdmInfo.setGuanDi(m_dGuandi);
	m_pZdmInfo.setWaShen(m_dWashen);
	return true;
}

bool CDrawGDProcess::GetWaShen()
{
	CString strPrompt;
	strPrompt.Format(_T("\n管底深<m> <%.2f>:"), m_dWashen);

	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dWashen);
		if (nRet == RTNORM)
		{
			//return true;
			/*if (verifyHeight(m_dWashen))
			{
				bRet = true;
			}*/
		}
		else if (nRet == RTNONE)
		{
			m_dGuandi = m_dWashen;
			/*if (verifyHeight(m_dWashen))
			{
				bRet = true;
			}*/
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}
	m_dGuandi = m_dRealDmx - m_dWashen;
	m_pZdmInfo.setWaShen(m_dWashen);
	m_pZdmInfo.setGuanDi(m_dGuandi);
	return true;
}

bool CDrawGDProcess::GetPodu()
{
	CString strPrompt;
	strPrompt.Format(_T("\n坡度<m> <%.2f>:"), m_dPodu);

	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dPodu);
		if (nRet == RTNORM)
		{
			//return true;
		/*	if (verifyHeight(m_dPodu))
			{
				bRet = true;
			}*/
		}
		else if (nRet == RTNONE)
		{
			m_dGuandi = m_dPodu;
			/*if (verifyHeight(m_dPodu))
			{
				bRet = true;
			}*/
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}
	m_pZdmInfo.setPoDu(m_dPodu);
	return true;
}

bool CDrawGDProcess::GetVertical()
{
	CString strPrompt;
	strPrompt = _T("\n获取垂直间距");
	double dReal;
	int nRet = acedGetReal(strPrompt, dReal);
	if (nRet == RTNORM)
	{
		
		return true;
	}
	else
	{
		return false;
	}
}

bool CDrawGDProcess::doUndo()
{
	int nTmp = m_nCout - 1;
	CString strCount;
	strCount.Format(_T("%d"), nTmp);
	CString strTmpLabel = BC_DICT_GD + strCount;

	MyEditEntity::EraseEntByGroupName(strTmpLabel);

	CDMXUtils::SetCurNum(strCount);
	return true;
}