#include "StdAfx.h"
#include "DrawDMXProcess.h"
#include "DrawZDM.h"
#include "CDlgEditZhuangHao.h"

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
		return true;
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
