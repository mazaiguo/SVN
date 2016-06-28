#include "StdAfx.h"
#include "DrawDMXProcess.h"
#include "DrawZDM.h"
#include "CDlgEditZhuangHao.h"
#include "ZDMUtils.h"
#include "CBiaochiForRQY.h"
#include "BcUtils.h"

extern CString TransFormStr(double dValue);

DrawDMXProcess::DrawDMXProcess(void)
{
	CString strCount = CDMXUtils::getNumCount();
	m_nCount = MyTransFunc::StringToInt(strCount);
	m_bDrawJiedian = CDMXUtils::getcreateJiedian();
	CString strLabel = BC_DICT + strCount;
	CBcUtils bcUtils;
	if (!bcUtils.get(m_nCount, m_pZdmInfo))
	{
		//m_pZdmInfo = new CZdmDataInfo();
		m_pZdmInfo.setCount(strCount);
		m_pZdmInfo.setJiedian(strCount);
		m_pZdmInfo.setLabel(strLabel);
	}

	if (m_nCount == 1)
	{
		m_dZhuanghao = CDMXUtils::startzh();
		m_dSJDmHeight = 0.00;
		m_strSoilType = _T("砂土");
	}
	else
	{
		//如果不为1，需要取上一个存储数据中的数据
		int nTmp = m_nCount - 1;
		strCount.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCount;

		bcUtils.get(nTmp, m_preZdmInfo);
		m_dZhuanghao = m_preZdmInfo.getcurData();
		m_dSJDmHeight = m_preZdmInfo.getDesignDmx();
		m_strSoilType = m_preZdmInfo.getSoilType();
	}
	m_dminElavation = CDMXUtils::getMinElavation();
	m_dmaxElavation = CDMXUtils::getMaxElavation();
	m_bIsExisted = false;
}

DrawDMXProcess::~DrawDMXProcess(void)
{
	/*if (m_pZdmInfo != NULL)
	{
		delete m_pZdmInfo;
		m_pZdmInfo = NULL;
	}*/
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
		zdm.setData(&m_pZdmInfo);
		AcDbObjectId groupId = zdm.add();
		return true;
	}
	else if (nRet == RTKWORD)
	{
		return true;
	}
	else if (nRet == RTNONE)
	{
		//如果是rtnone就将最后一条直线补全
		AcDbObjectId hxLayerId = MySymble::CreateNewLayer(_T("HX-TMP"), 7);
		m_basePt = CDMXUtils::getbasePt();
		m_dXScale = 1000/(CDMXUtils::getXScale());
		AcGePoint3d startPt,endPt;
		acutPolar(asDblArray(m_basePt), 0, 20 + (m_dZhuanghao - CDMXUtils::startzh())*m_dXScale, asDblArray(startPt));
		acutPolar(asDblArray(startPt), 3*PI/2, 152, asDblArray(startPt));
		acutPolar(asDblArray(startPt), 3*PI/2, 15, asDblArray(endPt));
		AcDbObjectId textId = MyDrawEntity::DrawLine(startPt, endPt, hxLayerId);
		MyEditEntity::AddObjToGroup(m_preZdmInfo.label(), textId);
		return false;
	}
	else
	{
		return false;
	}
	
	//m_GroupIds.append(groupId);
	return true;
}

bool DrawDMXProcess::Insert(bool bIsObstacle)
{
	CString strPrompt;
	CString strTmp = TransFormStr(m_dZhuanghao);
	if (bIsObstacle)
	{
		strPrompt.Format(_T("\n插入桩号值<m> <%s>:"), strTmp);
	}
	else
	{
		strPrompt.Format(_T("\n起始桩号值<m> <%s>:"), strTmp);
	}
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
	m_bIsExisted = false;
	CString strCur = CurNumPosition(m_dZhuanghao, m_bIsExisted);
	if (strCur.CompareNoCase(_T("0")) == 0)
	{
		return false;
	}	
	m_nCount = MyTransFunc::StringToInt(strCur);
	if (m_nCount < 1)
	{
		acutPrintf(_T("不能在此数据插入"));
		return false;
	}
	m_strLabel = BC_DICT + strCur;
	if (!m_bIsExisted)
	{	
		CBcUtils bc;
		bc.get(m_nCount, m_pZdmInfo);
		int nCurCount = MyTransFunc::StringToInt(CDMXUtils::getCurNum());
		int nTmp = m_nCount - 1;
		CString strCount;
		strCount.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCount;
		bc.get(nTmp, m_preZdmInfo);

		getSpecialInfo();
		m_pZdmInfo.setLabel(m_strLabel);
		m_pZdmInfo.setCount(strCur);
		m_pZdmInfo.setJiedian(strCur);
		m_pZdmInfo.setcurData(m_dZhuanghao);
		//////////////////////////////////////////////////////////////////////////
		//设置现状地面
		
		
		if (nRet == RTNORM)
		{
			if (!EntInteraction())
			{
				return false;
			}
			CDrawZDM zdm;
			zdm.setData(&m_pZdmInfo);
			AcDbObjectId groupId = zdm.insert();

			if (m_nCount < nCurCount)
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
	return true;
}

bool DrawDMXProcess::Insert(double dValue, bool bIsCalc, bool bIsAdded)
{
	bool bIsExisted = false;
	CString strCur = CurNumPosition(dValue, bIsExisted);
	if (strCur.CompareNoCase(_T("0")) == 0)
	{
		return false;
	}	
	int nCount = MyTransFunc::StringToInt(strCur);
	if (!bIsAdded)
	{
		nCount = nCount - 1;
		strCur.Format(_T("%d"), nCount);
	}
	if (nCount < 1)
	{
		acutPrintf(_T("不能在此数据插入"));
		return false;
	}
	m_dZhuanghao = dValue;
	m_strLabel = BC_DICT + strCur;
	if (!bIsExisted)
	{	
		CBcUtils bc;
		bc.get(nCount, m_pZdmInfo);	
		int nCurCount = MyTransFunc::StringToInt(CDMXUtils::getCurNum());

		if (bIsCalc)
		{
			int nTmp = nCount - 1;
			CString strCount;
			strCount.Format(_T("%d"), nTmp);
			CString strTmpLabel = BC_DICT + strCount;
			bc.get(nTmp, m_preZdmInfo);

			getSpecialInfo();
		}
		if (!bIsAdded)
		{
			nCount = nCount + 1;
			strCur.Format(_T("%d"), nCount);
			m_strLabel = BC_DICT + strCur;
		}
		m_pZdmInfo.setLabel(m_strLabel);
		m_pZdmInfo.setCount(strCur);
		m_pZdmInfo.setJiedian(strCur);
		m_pZdmInfo.setcurData(m_dZhuanghao);
		//////////////////////////////////////////////////////////////////////////
		//设置现状地面
	
		CDrawZDM zdm;
		zdm.setData(&m_pZdmInfo);
		AcDbObjectId groupId = zdm.insert();

		CDrawGd gd;
		gd.insert(m_pZdmInfo);

		return true;	
	}
	return false;
}

bool DrawDMXProcess::Del()
{
	//AcDbObjectId objId = setlectEnt(_T("\n选择要删除的桩号"));
	ads_name ssname;
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	int nRet = acedSSGet(NULL, NULL, NULL, NULL, ssname);
	if (nRet != RTNORM)
	{
		return false;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	map<CString, CString> infoMap;
	CString strGroupName;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
		int nFind = strGroupName.Find(BC_DICT_GD);
		if (nFind >= 0)
		{
			strGroupName.Replace(BC_DICT_GD, BC_DICT);
		}	
		nFind = strGroupName.Find(BC_DICT);
		if (nFind >= 0)
		{
			int nCount = MyParserString::GetCount(strGroupName);
			bool bRet =	del(nCount);
		}
	}
	acedSSFree(ssname);
	return true;
}

bool DrawDMXProcess::Mod()
{
	AcDbObjectId objId = setlectEnt(_T("\n选择要编辑的桩号"));
	if (objId.isNull())
	{
		return false;
	}
	CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
	CString strCur = CurNumPosition(strGroupName);
	int nCount = MyTransFunc::StringToInt(strCur);

	CBcUtils utils;
	CZdmDataInfo data;
	utils.get(nCount, data);

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

int DrawDMXProcess::getIndex()
{
	return m_nCount;
}

bool DrawDMXProcess::getIsExisted()
{
	return m_bIsExisted;
}

bool DrawDMXProcess::modify()
{
	AcDbObjectId objId = setlectEnt(_T("\n选择要编辑的数据："));
	if (objId.isNull())
	{
		return false;
	}
	
	CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
	int nFind = strGroupName.Find(BC_DICT_GD);
	if (nFind >= 0)
	{
		strGroupName.Replace(BC_DICT_GD, BC_DICT);
	}
	CString strCur = CurNumPosition(strGroupName);
	int nCount = MyTransFunc::StringToInt(strCur);

	CBcUtils utils;
	CZdmDataInfo data;
	utils.get(nCount, data);

	//////////////////////////////////////////////////////////////////////////
	int nTmp = nCount - 1;
	CString strTemp;
	strTemp.Format(_T("%d"), nTmp);
	CString strLabel = BC_DICT + strTemp;
	utils.get(nTmp, m_preZdmInfo);
	int nRet = acedGetReal(_T("\n输入修改后的数值："), &m_dValue);
	if (nRet == RTNORM)
	{

	}
	else if (nRet == RTNONE)
	{
		acutPrintf(_T("\n不进行修改"));
		return true;
	}
	else
	{
		return false;
	}
	if (doXdata(objId, ZDM_DESIGNDMX, data)||
		doXdata(objId, ZDM_REALDMX, data)||
		doXdata(objId, ZDM_JIEDIAN, data)||
		doXdata(objId, ZDM_CURDATA, data)||
		doXdata(objId, ZDM_GUANDI, data)||
		doXdata(objId, ZDM_WASHEN, data)||
		doXdata(objId, ZDM_PODU, data)||
		doXdata(objId, ZDM_JULI, data)||
		doXdata(objId, ZDM_DESINGDMXS, data)||
		doXdata(objId, ZDM_REALS, data))
	{
		
	}
	else
	{
		acutPrintf(_T("\n选择的实体不能修改数据"));
		return false;
	}


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

//交互相关


int DrawDMXProcess::GetZhuanghao()
{
	CString strPrompt;
	int nRet = RTNORM;
	bool bIsContinued = true;

	while (bIsContinued)
	{
		bIsContinued = false;
		CString strTemp = TransFormStr(m_dZhuanghao);
		if (m_nCount == 1)
		{
			strPrompt.Format(_T("\n起始桩号值<m> <%s>:"), strTemp);
		}
		else
		{
			acedInitGet(0, _T("Undo"));
			strPrompt.Format(_T("\n回退一步(U)/<下一点桩号值> <m> <%s>:"), strTemp);
		}
		double dResult = m_dZhuanghao;
		int nResult = acedGetReal(strPrompt, &m_dZhuanghao);
		if (nResult == RTNORM)
		{
			//return true;
			if (dResult > m_dZhuanghao)
			{
				AfxMessageBox(_T("桩号不能比前面的小"));
				bIsContinued = true;
				m_dZhuanghao = dResult;
				continue;
			}
		}
		else if (nResult == RTNONE)
		{
			if (m_nCount == 1)
			{
				m_dZhuanghao = m_dZhuanghao;
			}
			else
			{
				nRet = RTNONE;
			}
			break;
		}
		else if (nResult == RTKWORD)
		{
			doUndo();
			nRet = RTKWORD;
			break;
		}
		else
		{
			nRet = RTERROR;
		}
		break;
	}
	
	m_pZdmInfo.setcurData(m_dZhuanghao);

	if (m_nCount == 1)
	{
		CDMXUtils::SetInitZH(m_dZhuanghao);
	}
	return nRet;
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
	CString strTemp = TransFormStr(m_dSJDmHeight);
	strPrompt.Format(_T("\n设计地面标高<m> <%s>:"), strTemp);

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
	
	m_pZdmInfo.setDesignDmx(m_dSJDmHeight);
	return bRet;
}

bool DrawDMXProcess::GetXzDmHeight()
{
	CString strPrompt;
	CString strTemp = TransFormStr(m_dSJDmHeight);
	strPrompt.Format(_T("\n现状地面标高<m> <%s>:"), strTemp);
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
	m_pZdmInfo.setRealDmx(m_dXzDmHeight);
	return bRet;
}

bool DrawDMXProcess::GetSoilType()
{
	acutPrintf(_T("\n土壤类别有：砂土、亚砂土、亚黏土、黏土、含砾土卵石土、泥炭岩白垩土、干黄土"));
	TCHAR tempBuf[133];
	CString strTemp = m_strSoilType;
	CString strPrompt;
	strPrompt.Format(_T("\n土壤类别砂土(S)/亚砂土(YS)/黏土(N)/含砾土卵石土(H)/泥炭岩白垩土(NT)/干黄土(G)<%s>:"), strTemp);
	acedInitGet(0, _T("S YS YN N H NT G"));
	int nRet = acedGetKword(strPrompt,  tempBuf);
	if (nRet == RTNORM)
	{
		//return true;
		m_strSoilType = tempBuf;
		if (m_strSoilType.IsEmpty())
		{
			m_strSoilType = strTemp;
		}
		if (_tcscmp(tempBuf, _T("S")) == 0)
		{
			m_strSoilType = _T("砂土");
		}
		else if (_tcscmp(tempBuf, _T("YS")) == 0)
		{
			m_strSoilType = _T("亚砂土");
		}
		else if (_tcscmp(tempBuf, _T("N")) == 0)
		{
			m_strSoilType = _T("黏土");
		}
		else if (_tcscmp(tempBuf, _T("H")) == 0)
		{
			m_strSoilType = _T("含砾土卵石土");
		}
		else if (_tcscmp(tempBuf, _T("NT")) == 0)
		{
			m_strSoilType = _T("泥炭岩白垩土");
		}
		else if (_tcscmp(tempBuf, _T("G")) == 0)
		{
			m_strSoilType = _T("干黄土");
		}
	}
	else if (nRet == RTNONE)
	{
		m_strSoilType = strTemp;
	}
	else
	{
		return false;
	}
	m_pZdmInfo.setSoilType(m_strSoilType);
	return true;
}

bool DrawDMXProcess::GetIsPd()
{
	acedInitGet(0 , _T("Yes No"));
	TCHAR szKword [132];
	szKword[0] = _T('N');	//给szKword一个默认值N
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n是否有垂直堡坎?>>是(Y)/否(N) <N>:"), szKword);
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
			m_pZdmInfo.setHasBulge(m_bHasBugle);
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
	CString strTemp = TransFormStr(m_dSJDmHeight);
	strPrompt.Format(_T("\n堡坎设计地面标高<m> <%s>:"), strTemp);

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

	m_pZdmInfo.setDesingDmxS(m_dDesignDmxS);
	return bRet;
}

bool DrawDMXProcess::GetXzDmHeightS()
{
	CString strPrompt;
	CString strTemp = TransFormStr(m_dDesignDmxS);
	strPrompt.Format(_T("\n堡坎<m> <%s>:"), strTemp);
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
	m_pZdmInfo.setRealDmxS(m_dRealDmxS);
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
		acutPrintf(_T("地面线数据比基础地面线标高还高，请重新输入数据"));
		//return false;
	}
	return true;
}

bool DrawDMXProcess::doUndo()
{
	int nTmp = m_nCount - 1;
	CString strCount;
	strCount.Format(_T("%d"), nTmp);
	CString strTmpLabel = BC_DICT + strCount;
		
	MyEditEntity::EraseEntByGroupName(strTmpLabel);

	CBcUtils utils;
	utils.del(nTmp);

	CDMXUtils::setNumCount(strCount);
	return true;
}

CString DrawDMXProcess::CurNumPosition( double dValue, bool& bIsExisted)
{
	CString strCur = _T("0");
	double dZhuanghao;
	CBcUtils bcUtils;
	map<int, CZdmDataInfo> data = bcUtils.getAllData();

	for (map<int, CZdmDataInfo>::iterator iter = data.begin();
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
		if (abs(dZhuanghao - dValue) < GeTol)
		{
			bIsExisted = true;
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
	if (!GetSoilType())
	{
		return false;
	}
	if (m_nCount > 1)
	{
		if (!GetIsPd())
		{
			return false;
		}
	}
	
	return true;
}	


//CZdmDataInfo DrawDMXProcess::getXData(AcDbObjectId objId, double dValue, CZdmDataInfo tmpData)
//{
//	doXdata(objId, ZDM_DESIGNDMX);
//	doXdata(objId, ZDM_REALDMX);
//	doXdata(objId, ZDM_JIEDIAN);
//	doXdata(objId, ZDM_CURDATA);
//	doXdata(objId, ZDM_GUANDI);
//	doXdata(objId, ZDM_WASHEN);
//	doXdata(objId, ZDM_PODU);
//	doXdata(objId, ZDM_JULI);
//	doXdata(objId, ZDM_DESINGDMXS);
//	doXdata(objId, ZDM_REALS);	
//	//MyEditEntity::GetObjDoubleXdata(objId, ZDM_PIPETYPE);
//	//MyEditEntity::GetObjDoubleXdata(objId, ZDM_PIPEDIAMETER)
//}

bool DrawDMXProcess::doXdata(AcDbObjectId objId, CString strTmp, CZdmDataInfo& tmpData)
{
	if (!GetObjDoubleXdata(objId, strTmp))
	{
		return false;
	}
	double dRealDmx = tmpData.getRealDmx();
	double dGuandi = tmpData.getGuanDi();
	double dWashen = tmpData.getWaShen();
	if (strTmp.CompareNoCase(ZDM_DESIGNDMX) == 0)
	{
		tmpData.setDesignDmx(m_dValue);
	}
	else if (strTmp.CompareNoCase(ZDM_REALDMX) == 0)
	{
		tmpData.setRealDmx(m_dValue);
	}
	else if (strTmp.CompareNoCase(ZDM_JIEDIAN) == 0)
	{
		CString strTmp;
		MyTransFunc::doubleToStr(m_dValue, strTmp);
		tmpData.setJiedian(strTmp);
	}
	else if (strTmp.CompareNoCase(ZDM_CURDATA) == 0)
	{
		tmpData.setcurData(m_dValue);
	}
	else if (strTmp.CompareNoCase(ZDM_GUANDI) == 0)
	{		
		tmpData.setGuanDi(m_dValue);
		dWashen = dRealDmx - m_dValue;
		tmpData.setWaShen(dWashen);
	}
	else if (strTmp.CompareNoCase(ZDM_WASHEN) == 0)
	{
		tmpData.setWaShen(m_dValue);
		dGuandi = dRealDmx - m_dValue;
		tmpData.setGuanDi(dGuandi);
	}
	else if (strTmp.CompareNoCase(ZDM_PODU) == 0)
	{
		tmpData.setPoDu(m_dValue);	
		m_dValue /= 1000;
		double dDist = tmpData.getcurData() - m_preZdmInfo.getcurData();
		double dGuandi = m_preZdmInfo.getGuanDi();
		dGuandi = dGuandi + m_dValue*dDist;
		tmpData.setGuanDi(dGuandi);
		dRealDmx = tmpData.getRealDmx();
		dWashen = dRealDmx - dGuandi;
		tmpData.setWaShen(dWashen);
	}
	else if (strTmp.CompareNoCase(ZDM_JULI) == 0)
	{
		tmpData.setJuli(m_dValue);
	}
	else if (strTmp.CompareNoCase(ZDM_DESINGDMXS) == 0)
	{
		tmpData.setDesingDmxS(m_dValue);
	}
	else if (strTmp.CompareNoCase(ZDM_REALS) == 0)
	{
		tmpData.setRealDmxS(m_dValue);
	}
	return true;
}

bool DrawDMXProcess::GetObjDoubleXdata(AcDbObjectId objId, CString strTmp)
{
	double dXdata = 0.0;
	if (objId.isNull())
	{
		return false;
	}
	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		pObj->close();
		return false;
	}
	MyBaseAppXdataList  xdata(pObj);
	MyBaseAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		pObj->close();
		return false;
	}
	bool bIsFind = false;
	int nIndex = 0;
	CString strAppName;
	for (;!iter.done(); iter.next())
	{
		strAppName = iter.item()->getAppName();
		if (strTmp.CompareNoCase(strAppName) == 0)
		{
			bIsFind = true;
		}		
		else
		{
			nIndex++;
		}
	}
	if (!bIsFind)
	{
		pObj->close();
		return false;
	}
	MyBaseAppXdata* xdPtr = xdata.at(nIndex);
	bool bIsOk = xdPtr->getReal(XDT_XG_DOUBLE, dXdata);
	if (!bIsOk)
	{
		dXdata = 0.0;
	}
	pObj->close();
	return true;
}

void DrawDMXProcess::getSpecialInfo()
{
	double dLen1 = m_dZhuanghao - m_preZdmInfo.getcurData();
	double dLen2 = m_pZdmInfo.getcurData() - m_preZdmInfo.getcurData();
	double dRadio = dLen1/dLen2;

	m_dXzDmHeight = (m_pZdmInfo.getRealDmx() - m_preZdmInfo.getRealDmx())*dRadio + m_preZdmInfo.getRealDmx();
	m_dSJDmHeight = (m_pZdmInfo.getDesignDmx() - m_preZdmInfo.getDesignDmx())*dRadio + m_preZdmInfo.getDesignDmx();
	
	double dGuandi = (m_pZdmInfo.getGuanDi() - m_preZdmInfo.getGuanDi())*dRadio + m_preZdmInfo.getGuanDi();
	double dWashen = m_dXzDmHeight - dGuandi;
	double dPodu = m_pZdmInfo.getPoDu();
	double dJuli = m_dZhuanghao - m_preZdmInfo.getcurData();
	m_pZdmInfo.setRealDmx(m_dXzDmHeight);
	m_pZdmInfo.setDesignDmx(m_dSJDmHeight);
	m_pZdmInfo.setWaShen(dWashen);
	m_pZdmInfo.setGuanDi(dGuandi);
	m_pZdmInfo.setPoDu(dPodu);
	m_pZdmInfo.setJuli(dJuli);
}

bool DrawDMXProcess::del(int nCount)
{
	CString strCur;
	strCur.Format(_T("%d"), nCount);
	CString strGroupName = BC_DICT + strCur;
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
	utils.get(nCount, data);
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

CDrawGDProcess::CDrawGDProcess(void)
{
	CString strNumCount = CDMXUtils::getNumCount();
	m_nNumCount = MyTransFunc::StringToDouble(strNumCount);
	CString strCount = CDMXUtils::getCurNum();
	m_nCout = MyTransFunc::StringToInt(strCount);
	
	m_dminElavation = CDMXUtils::getMinElavation();
	m_dmaxElavation = CDMXUtils::getMaxElavation();
	CBcUtils bcUtils;

	CString strLabel = BC_DICT + strCount;
	bcUtils.get(m_nCout, m_pZdmInfo);
	m_dRealDmx = m_pZdmInfo.getRealDmx();
	if (m_nCout == 1)
	{
		m_dPipeDiameter = m_pZdmInfo.getPipeDiameter();
		m_dGuandi = m_dminElavation;
		m_dPodu = m_pZdmInfo.getPoDu();
		m_dWashen = m_pZdmInfo.getWaShen();
		m_strPipeType = _T("DN");
	}
	else
	{
		//如果不为1，需要取上一个存储数据中的数据
		int nTmp = m_nCout - 1;
		strCount.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCount;
		bcUtils.get(nTmp, m_preZdmInfo);
		m_dPipeDiameter = m_preZdmInfo.getPipeDiameter();
		m_dGuandi = m_preZdmInfo.getGuanDi();
		m_strPipeType = m_preZdmInfo.getPipeType();
		m_dPodu = m_preZdmInfo.getPoDu();
		m_dWashen = m_preZdmInfo.getWaShen();
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
	if (m_nCout > m_nNumCount - 1)
	{
		acutPrintf(_T("\n没有地面线数据"));
		return false;
	}
	
	if (!GetPipeDiameter())
	{
		return false;
	}
	if (!GetPipeType())
	{
		return false;
	}
	if (!GetKeyWord())
	{
		return false;
	}
	CDMXUtils::SetcreateGw(true);
	CDrawGd gd;
	gd.add(m_pZdmInfo);
	return true;
}


bool CDrawGDProcess::Insert(CString strCur)
{
	CBcUtils bcUtils;

	CString strLabel = BC_DICT + strCur;
	bcUtils.get(_tstoi(strCur), m_pZdmInfo);
	m_dRealDmx = m_pZdmInfo.getRealDmx();
	/*int nIndex = MyTransFunc::StringToInt(strCur);
	if ((m_nCout == 1) || (nIndex != m_nCout))
	{*/
		m_dPipeDiameter = m_pZdmInfo.getPipeDiameter();
		m_dGuandi = m_pZdmInfo.getGuanDi();
		m_dPodu = m_pZdmInfo.getPoDu();
		m_dWashen = m_pZdmInfo.getWaShen();
	//}
	//else
	//{
	//	//如果不为1，需要取上一个存储数据中的数据
	//	int nTmp = m_nCout - 1;
	//	strCur.Format(_T("%d"), nTmp);
	//	CString strTmpLabel = BC_DICT + strCur;
	//	bcUtils.get(nTmp, m_preZdmInfo);
	//	m_dPipeDiameter = m_preZdmInfo.getPipeDiameter();
	//	m_dGuandi = m_preZdmInfo.getGuanDi();
	//}
	
	if (!GetPipeDiameter())
	{
		return false;
	}
	if (!GetPipeType())
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
		strPrompt.Format(_T("\n管道管径<mm> <%.2f>:"), m_dPipeDiameter);
	}
	else
	{
		acedInitGet(0, _T("Undo"));
		strPrompt.Format(_T("\n回退一步(U)/<下一段管道管径> <mm> <%.2f>:"), m_dPipeDiameter);
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
		strPrompt = _T("\n输入燃气管底标高(D)/输入管底挖深(W)/<管底标高>:");
	}
	else
	{
		acedInitGet(0, _T("Di Wa Po Vertical"));
		//strPrompt = _T("\n输入管底(D)/输入挖深(W)/坡度(P)/垂直间距(V)/<管底>:");
		strPrompt = _T("\n输入燃气管底标高(D)/输入管底挖深(W)/坡度(P)/<管底标高>:");
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
	CString strTemp = TransFormStr(m_dGuandi);
	strPrompt.Format(_T("\n管底标高<m> <%s>:"), strTemp);

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
		m_dPodu = 1000*(m_dGuandi - dGuandi)/dDist;
		m_pZdmInfo.setPoDu(m_dPodu);
	}
	return bRet;
}

bool CDrawGDProcess::GetWaShen()
{
	CString strPrompt;
	CString strTemp = TransFormStr(m_dWashen);
	strPrompt.Format(_T("\n管底挖深<m> <%s>:"), strTemp);

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
		m_dPodu = 1000*(m_dGuandi - dGuandi)/dDist;
		m_pZdmInfo.setPoDu(m_dPodu);
	}
	return bRet;
}

bool CDrawGDProcess::GetPodu()
{
	CString strPrompt;
	CString strTemp = TransFormStr(m_dPodu);
	strPrompt.Format(_T("\n坡度<‰%s>:"), strTemp);

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
			m_dPodu = m_dPodu;
			bRet = true;
		}
		else if (nRet == RTNONE)
		{
			m_dPodu = m_dPodu;
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
	m_dPodu /= 1000;
	//根据坡度，计算管底，挖深
	double dDist = m_pZdmInfo.getcurData() - m_preZdmInfo.getcurData();
	double dGuandi = m_preZdmInfo.getGuanDi();
	m_dGuandi = dGuandi + m_dPodu*dDist;
	m_pZdmInfo.setGuanDi(m_dGuandi);
	m_dRealDmx = m_pZdmInfo.getRealDmx();
	m_dWashen = m_dRealDmx - m_dGuandi;
	m_pZdmInfo.setWaShen(m_dWashen);
	return bRet;
}

bool CDrawGDProcess::GetVertical()
{
	AfxMessageBox(_T("此功能还未实现"));
	return false;
	/*CString strPrompt;
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
	}*/
}

bool CDrawGDProcess::GetPipeType()
{
	TCHAR tempBuf[133];
	CString strPrompt;
	strPrompt.Format(_T("\n管道类型<%s>:"), m_strPipeType);
	CString strTemp = m_strPipeType;
	int nRet = acedGetString(1, strPrompt,  tempBuf);
	if (nRet == RTNORM)
	{
		//return true;
		m_strPipeType = tempBuf;
		if (m_strPipeType.IsEmpty())
		{
			m_strPipeType = strTemp;
		}
	}
	else if (nRet == RTNONE)
	{
		m_strPipeType = strTemp;
	}
	else
	{
		return false;
	}
	m_pZdmInfo.setPipeType(m_strPipeType);
	return true;
}

bool CDrawGDProcess::doUndo()
{
	CString strCount = CDMXUtils::getCurNum();
	int nCount = MyTransFunc::StringToInt(strCount);
	int nTmp = nCount - 1;
	strCount.Format(_T("%d"), nTmp);
	CString strTmpLabel = BC_DICT_GD + strCount;

	MyEditEntity::EraseEntByGroupName(strTmpLabel);
	CString strLabel = BC_DICT + strCount;
	CBcUtils bcUtils;
	bcUtils.get(nTmp, m_pZdmInfo);

	nTmp = nCount - 2;
	strCount.Format(_T("%d"), nTmp);
	strLabel = BC_DICT + strCount;
	bcUtils.get(nTmp, m_preZdmInfo);
	CDMXUtils::SetCurNum(strCount);
	return true;
}

