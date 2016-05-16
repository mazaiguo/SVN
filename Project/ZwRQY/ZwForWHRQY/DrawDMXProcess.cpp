#include "StdAfx.h"
#include "DrawDMXProcess.h"
#include "DrawZDM.h"
#include "CDlgEditZhuangHao.h"
#include "ZDMUtils.h"
#include "CBiaochiForRQY.h"
#include "BcUtils.h"

DrawDMXProcess::DrawDMXProcess(void)
{
	CString strCount = CDMXUtils::getNumCount();
	m_nCout = MyTransFunc::StringToInt(strCount);
	m_bDrawJiedian = CDMXUtils::getcreateJiedian();
	m_pZdmInfo = new CZdmDataInfo();
	m_pZdmInfo->setCount(strCount);
	m_pZdmInfo->setJiedian(strCount);
	CString strLabel = BC_DICT + strCount;
	m_pZdmInfo->setLabel(strLabel);
	if (m_nCout == 1)
	{
		m_dZhuanghao = 0.00;
		m_dSJDmHeight = 0.00;
	}
	else
	{
		//如果不为1，需要取上一个存储数据中的数据
		int nTmp = m_nCout - 1;
		strCount.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCount;
		CBcUtils bcUtils;

		CZdmDataInfo pBiaoChi;
		bcUtils.get(strTmpLabel, pBiaoChi);
		m_dZhuanghao = pBiaoChi.getcurData();
		m_dSJDmHeight = pBiaoChi.getDesignDmx();
	}
	m_dminElavation = CDMXUtils::getMinElavation();
	m_dmaxElavation = CDMXUtils::getMaxElavation();
}

DrawDMXProcess::~DrawDMXProcess(void)
{
	if (m_pZdmInfo != NULL)
	{
		delete m_pZdmInfo;
		m_pZdmInfo = NULL;
	}
}

bool DrawDMXProcess::Draw()
{
	int nRet = GetZhuanghao();
	if (nRet == RTNORM)
	{
		if (!EntInteraction())
		{
			return false;
		}


		CDrawZDM zdm;
		zdm.setData(m_pZdmInfo);
		AcDbObjectId groupId = zdm.add();
		return true;
	}
	else if (nRet == RTKWORD)
	{
		return true;
	}
	else
	{
		return false;
	}
	
	//m_GroupIds.append(groupId);
	return true;
}

bool DrawDMXProcess::Insert()
{
	CString strPrompt;
	strPrompt.Format(_T("\n起始桩号值<m> <%.2f>:"), m_dZhuanghao);
	int nRet = acedGetReal(strPrompt, &m_dZhuanghao);
	if (nRet == RTNORM)
	{
		//return true;
	}
	/*else if (nRet == RTNONE)
	{
		m_dZhuanghao = m_dZhuanghao;
	}*/
	else
	{
		return false;
	}
	CString strCur = CurNumPosition(m_dZhuanghao);
	if (strCur.CompareNoCase(_T("0")) == 0)
	{
		return false;
	}
	CString strLabel = BC_DICT + strCur;
	m_pZdmInfo->setLabel(strLabel);
	m_pZdmInfo->setCount(strCur);
	m_pZdmInfo->setJiedian(strCur);
	m_pZdmInfo->setcurData(m_dZhuanghao);

	if (nRet == RTNORM)
	{
		if (!EntInteraction())
		{
			return false;
		}
		CDrawZDM zdm;
		zdm.setData(m_pZdmInfo);
		AcDbObjectId groupId = zdm.insert();

		if (CDMXUtils::getcreateGw())
		{
			CDrawGDProcess gd;
			gd.Insert(strCur);
		}
	
		return true;
	}
	else if (nRet == RTKWORD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DrawDMXProcess::Del()
{
	AcDbObjectId objId = setlectEnt(_T("\n选择要删除的桩号"));
	CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
	
	CString strCur = CurNumPosition(strGroupName);
	int nCount = MyTransFunc::StringToInt(strCur);
	if (nCount == 1)
	{
		AfxMessageBox(_T("不能删除第一个桩号"));
		return false;
	}
	else if (nCount == 0)
	{
		AfxMessageBox(_T("没有选中桩号"));
		return false;
	}
	
	//删除dictionary
	CBcUtils utils;
	CString strNextGroupName;
	nCount++;
	CString strTmpCur;
	strTmpCur.Format(_T("%d"), nCount);
	strNextGroupName = BC_DICT + strTmpCur;
	CZdmDataInfo data;
	utils.get(strNextGroupName, data);
	data.setLabel(strGroupName);
	data.setCount(strCur);
	data.setJiedian(strCur);

	MyEditEntity::EraseEntByGroupName(strGroupName);

	CDrawZDM zdm;
	zdm.setData(&data);
	bool bRet = zdm.del(strGroupName);

	if (CDMXUtils::getcreateGw())
	{
		//删除管道线
		CString strGdGroupName;
		strGdGroupName = BC_DICT_GD + strCur;
		MyEditEntity::EraseEntByGroupName(strGdGroupName);

		CDrawGd gd;
		gd.del(data);
	}
	
	return bRet;
}

bool DrawDMXProcess::Mod()
{
	AcDbObjectId objId = setlectEnt(_T("\n选择要编辑的桩号"));
	CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
	CString strCur = CurNumPosition(strGroupName);
	int nCount = MyTransFunc::StringToInt(strCur);

	CBcUtils utils;
	CZdmDataInfo data;
	utils.get(strGroupName, data);

	CDlgEditZhuangHao dlg;
	dlg.setZDMData(&data);
	if (dlg.DoModal() == IDOK)
	{
		data = dlg.getData();
		MyEditEntity::EraseEntByGroupName(strGroupName);

		CDrawZDM zdm;
		zdm.setData(&data);
		bool bRet = zdm.mod(strGroupName);

		if (CDMXUtils::getcreateGw())
		{
			CString strGdGroupName;
			strGdGroupName = BC_DICT_GD + strCur;
			MyEditEntity::EraseEntByGroupName(strGdGroupName);

			CDrawGd gd;
			gd.mod(data);
		}
		
		return bRet;
	}

	return false;
}

//交互相关


int DrawDMXProcess::GetZhuanghao()
{
	CString strPrompt;
	if (m_nCout == 1)
	{
		strPrompt.Format(_T("\n起始桩号值<m> <%.2f>:"), m_dZhuanghao);
	}
	else
	{
		acedInitGet(0, _T("Undo"));
		strPrompt.Format(_T("\n回退一步(U)/<下一点桩号值> <m> <%.2f>:"), m_dZhuanghao);
	}
	int nRet = acedGetReal(strPrompt, &m_dZhuanghao);
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
	m_pZdmInfo->setcurData(m_dZhuanghao);

	return RTNORM;
}

//是否节点
bool DrawDMXProcess::GetIsJiedian()
{
	acedInitGet(0 , _T("Yes No"));
	TCHAR szKword [132];
	szKword[0] = _T('N');	//给szKword一个默认值N
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n是否定节点号?>>是(Y)/否(N) <N>:"), szKword);
	if (nRet == RTNORM)	//如果得到合理的关键字
	{
		if (_tcscmp(szKword, _T("Yes")) == 0)
			m_bDrawJiedian = true;
		else if (_tcscmp(szKword, _T("No")) == 0)
			m_bDrawJiedian = false;
	}
	else if (nRet == RTNONE)	//如果用户输入为空值
	{
		m_bDrawJiedian = false;
	}
	
	CDMXUtils::SetcreateJiedian(m_bDrawJiedian);
	return m_bDrawJiedian;
}
//
bool DrawDMXProcess::GetSJDmHeight()
{
	CString strPrompt;
	strPrompt.Format(_T("\n设计地面标高<m> <%.2f>:"), m_dSJDmHeight);

	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dSJDmHeight);
		if (nRet == RTNORM)
		{
			//return true;
			if (verifyHeight(m_dSJDmHeight))
			{
				bRet = true;
			}
		}
		else if (nRet == RTNONE)
		{
			m_dSJDmHeight = m_dSJDmHeight;
			if (verifyHeight(m_dSJDmHeight))
			{
				bRet = true;
			}
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}
	
	m_pZdmInfo->setDesignDmx(m_dSJDmHeight);
	return bRet;
}

bool DrawDMXProcess::GetXzDmHeight()
{
	CString strPrompt;
	strPrompt.Format(_T("\n现状地面标高<m> <%.2f>:"), m_dSJDmHeight);
	bool bRet = false;
	while(!bRet)
	{
	int nRet = acedGetReal(strPrompt, &m_dXzDmHeight);
	if (nRet == RTNORM)
	{
		//return true;
		if (verifyHeight(m_dXzDmHeight))
		{
			bRet = true;
		}
	}
	else if (nRet == RTNONE)
	{
		m_dXzDmHeight = m_dSJDmHeight;
		if (verifyHeight(m_dXzDmHeight))
		{
			bRet = true;
		}
	}
	else
	{
		bRet = false;
		break;
	}
	}
	m_pZdmInfo->setRealDmx(m_dXzDmHeight);
	return bRet;
}

bool DrawDMXProcess::GetIsPd()
{
	acedInitGet(0 , _T("Yes No"));
	TCHAR szKword [132];
	szKword[0] = _T('N');	//给szKword一个默认值N
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n是否有坡度?>>是(Y)/否(N) <N>:"), szKword);
	if (nRet == RTNORM)	//如果得到合理的关键字
	{
		if (_tcscmp(szKword, _T("Yes")) == 0)
			m_bHasBugle = true;
		else if (_tcscmp(szKword, _T("No")) == 0)
			m_bHasBugle = false;
	}
	else if (nRet == RTNONE)	//如果用户输入为空值
	{
		m_bHasBugle = false;
	}
	if (m_bHasBugle)
	{
		if (GetPdHeight())
		{
			m_pZdmInfo->setHasBulge(m_bHasBugle);
		}
	}
	return true;
}

bool DrawDMXProcess::GetPdHeight()
{
	if (!GetSJDmHeightS())
	{
		return false;
	}
	if (!GetXzDmHeightS())
	{
		return false;
	}
	return true;
}

bool DrawDMXProcess::GetSJDmHeightS()
{
	CString strPrompt;
	strPrompt.Format(_T("\n坡度设计地面标高<m> <%.2f>:"), m_dSJDmHeight);

	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dDesignDmxS);
		if (nRet == RTNORM)
		{
			//return true;
			if (verifyHeight(m_dDesignDmxS))
			{
				bRet = true;
			}
		}
		else if (nRet == RTNONE)
		{
			m_dDesignDmxS = m_dSJDmHeight;
			if (verifyHeight(m_dDesignDmxS))
			{
				bRet = true;
			}
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}

	m_pZdmInfo->setDesingDmxS(m_dDesignDmxS);
	return bRet;
}

bool DrawDMXProcess::GetXzDmHeightS()
{
	CString strPrompt;
	strPrompt.Format(_T("\n坡度现状地面标高<m> <%.2f>:"), m_dDesignDmxS);
	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dRealDmxS);
		if (nRet == RTNORM)
		{
			//return true;
			if (verifyHeight(m_dRealDmxS))
			{
				bRet = true;
			}
		}
		else if (nRet == RTNONE)
		{
			m_dRealDmxS = m_dDesignDmxS;
			if (verifyHeight(m_dRealDmxS))
			{
				bRet = true;
			}
		}
		else
		{
			bRet = false;
			break;
		}
	}
	m_pZdmInfo->setRealDmxS(m_dRealDmxS);
	return bRet;
}

bool DrawDMXProcess::verifyHeight( double dHeight )
{
	if (dHeight < m_dminElavation)
	{
		AfxMessageBox(_T("地面线数据比基础地面线标高还低，请重新输入数据"));
		return false;
	}
	if (dHeight > m_dmaxElavation)
	{
		AfxMessageBox(_T("地面线数据比基础地面线标高还高，请重新输入数据"));
		return false;
	}
	return true;
}

bool DrawDMXProcess::doUndo()
{
	int nTmp = m_nCout - 1;
	CString strCount;
	strCount.Format(_T("%d"), nTmp);
	CString strTmpLabel = BC_DICT + strCount;
		
	MyEditEntity::EraseEntByGroupName(strTmpLabel);

	CBcUtils utils;
	utils.del(strTmpLabel);

	CDMXUtils::setNumCount(strCount);
	return true;
}

CString DrawDMXProcess::CurNumPosition( double dValue )
{
	CString strCur = _T("0");
	double dZhuanghao;
	CBcUtils bcUtils;
	map<CString, CZdmDataInfo> data = bcUtils.getAllData();

	for (map<CString, CZdmDataInfo>::iterator iter = data.begin();
		iter != data.end();
		++iter)
	{
		CZdmDataInfo pData = iter->second;
		dZhuanghao = pData.getcurData();
		//当桩号比真实值大时，说明位置就在这个地方
		if (dZhuanghao > dValue)
		{
			strCur = pData.getCount();
			break;
		}
	}
	return strCur;
}

CString DrawDMXProcess::CurNumPosition(CString strlabel)
{
	CString strTmp = BC_DICT;
	int nFind = strlabel.Find(BC_DICT);	
	CString strCur;

	if (nFind < 0)
	{
		strCur = _T("0");
		return strCur;
	}
	int nLen = strTmp.GetLength();
	strCur = strlabel.Right(strlabel.GetLength() - nLen);
	return strCur;
}

AcDbObjectId DrawDMXProcess::setlectEnt(CString strPromPt)
{	
	AcDbObjectId objId = AcDbObjectId::kNull;

	ads_name ename;
	AcGePoint3d pt;
	int nRet = acedEntSel(strPromPt, ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return objId;
	}
	acdbGetObjectId(objId, ename);

	return objId;
}

bool DrawDMXProcess::EntInteraction()
{	
	if (!m_bDrawJiedian)
	{
		GetIsJiedian();	
	}

	if (!GetSJDmHeight())
	{
		return false;
	}

	if (!GetXzDmHeight())
	{
		return false;
	}
	if (m_nCout > 1)
	{
		if (!GetIsPd())
		{
			return false;
		}
	}
	
	return true;
}	


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
		bcUtils.get(strTmpLabel, m_preZdmInfo);
		m_dPipeDiameter = m_preZdmInfo.getPipeDiameter();
		m_dGuandi = m_preZdmInfo.getGuanDi();
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
	CDMXUtils::SetcreateGw(true);
	CDrawZDM zdm;
	//zdm.setDrawGd(true);
	zdm.setData(&m_pZdmInfo);
	zdm.add();
	return true;
}


bool CDrawGDProcess::Insert(CString strCur)
{
	CBcUtils bcUtils;

	CString strLabel = BC_DICT + strCur;
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
		strCur.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCur;
		bcUtils.get(strTmpLabel, m_preZdmInfo);
		m_dPipeDiameter = m_preZdmInfo.getPipeDiameter();
		m_dGuandi = m_preZdmInfo.getGuanDi();
	}
	if (!GetPipeDiameter())
	{
		return false;
	}
	if (!GetKeyWord())
	{
		return false;
	}

	CDrawGd gd;
	gd.insert(m_pZdmInfo);
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
		//return true;
	}
	else if (nRet == RTKWORD)
	{
		doUndo();
		return true;
	}
	else
	{
		return false;
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
		else if (_tcscmp(val, _T("Vertical")) == 0)
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
				
			}*/
			bRet = true;
		}
		else if (nRet == RTNONE)
		{
			/*if (verifyHeight(m_dGuandi))
			{
				
			}*/
			bRet = true;
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
	if (m_nCout > 1)
	{
		double dGuandi = m_preZdmInfo.getGuanDi();
		double dDist = m_pZdmInfo.getcurData() - m_preZdmInfo.getcurData();
		m_dPodu = (m_dGuandi - dGuandi)/dDist;
		m_pZdmInfo.setPoDu(m_dPodu);
	}
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
				
			}*/
			bRet = true;
		}
		else if (nRet == RTNONE)
		{
			m_dGuandi = m_dWashen;
			/*if (verifyHeight(m_dWashen))
			{
				
			}*/
			bRet = true;
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
	if (m_nCout > 1)
	{
		double dGuandi = m_preZdmInfo.getGuanDi();
		double dDist = m_pZdmInfo.getcurData() - m_preZdmInfo.getcurData();
		m_dPodu = (m_dGuandi - dGuandi)/dDist;
		m_pZdmInfo.setPoDu(m_dPodu);
	}
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
				
			}*/
			bRet = true;
		}
		else if (nRet == RTNONE)
		{
			m_dGuandi = m_dPodu;
			/*if (verifyHeight(m_dPodu))
			{
				
			}*/
			bRet = true;
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}
	m_pZdmInfo.setPoDu(m_dPodu);
	//根据坡度，计算管底，挖深
	double dDist = m_pZdmInfo.getcurData() - m_preZdmInfo.getcurData();
	double dGuandi = m_preZdmInfo.getGuanDi();
	m_dGuandi = dGuandi + m_dPodu*dDist;
	m_pZdmInfo.setGuanDi(m_dGuandi);
	m_dRealDmx = m_pZdmInfo.getRealDmx();
	m_dWashen = m_dRealDmx - m_dGuandi;
	m_pZdmInfo.setWaShen(m_dWashen);
	return true;
}

bool CDrawGDProcess::GetVertical()
{
	CString strPrompt;
	strPrompt = _T("\n获取垂直间距");
	double dReal;
	int nRet = acedGetReal(strPrompt, &dReal);
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
	CString strLabel = BC_DICT + strCount;
	CBcUtils bcUtils;
	bcUtils.get(strLabel, m_pZdmInfo);
	CDMXUtils::SetCurNum(strCount);
	return true;
}

