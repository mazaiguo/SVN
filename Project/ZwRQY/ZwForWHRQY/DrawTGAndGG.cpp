#include "StdAfx.h"
#include "DrawTGAndGG.h"
#include "GWDesingUtils.h"
#include "BcUtils.h"
#include <map>
#include "ZdmDataInfo.h"
using namespace std;

CDrawTGAndGG::CDrawTGAndGG(void)
{
	m_bIsTG = false;
}

CDrawTGAndGG::~CDrawTGAndGG(void)
{

}

bool CDrawTGAndGG::drawTG()
{
	m_bIsTG = true;
	if (!midProcess())
	{
		return false;
	}
	return true;
}

bool CDrawTGAndGG::drawGG()
{
	m_bIsTG = false;
	if (!midProcess())
	{
		return false;
	}
	return true;
}

bool CDrawTGAndGG::del()
{
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
		int nFind = strGroupName.Find(TG_DICT);
		if (nFind >= 0)
		{
			MyEditEntity::EraseEntByGroupName(strGroupName);
		}
	}
	acedSSFree(ssname);
	return true;
}

bool CDrawTGAndGG::midProcess()
{
	if (!getStartZH())
	{
		return false;
	}
	if (!getEndZH())
	{
		return false;
	}
	if (!getUpText())
	{
		return false;
	}

	if (!getDownText())
	{
		return false;
	}
	if (!getSpecailData())
	{
		return false;
	}
	
	int nBlkRefCount = 0;
	MyBaseUtils::GetVar(_T("USERI2"), &nBlkRefCount);
	CString strName;
	strName.Format(_T("%s%d"), TG_DICT, nBlkRefCount);
	MyDrawEntity::MakeGroup(m_idArrs, false, strName);
	nBlkRefCount++;
	MyBaseUtils::SetVar(_T("USERI2"), nBlkRefCount);
	return true;
}

bool CDrawTGAndGG::getStartZH()
{
	CString strPromPt;
	if (m_bIsTG)
	{
		strPromPt = _T("\n请选择起始桩号：");
	}
	else
	{
		strPromPt = _T("\n请选择起始桩号：");
	}
	int nRet = acedGetReal(strPromPt, &m_dStartZH);
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
	AcGePoint3d midPt,endPt;
	AcGePoint3d pt;
	int nOsmode = 512;
	MyBaseUtils::GetVar(_T("OSMODE"), &nOsmode);
	if (nOsmode != 512)
	{
		MyBaseUtils::SetVar(_T("OSMODE"), 512);
	}
	int nRet = acedGetPoint(NULL, _T("\n请指定插入说明文字的起点"), asDblArray(pt));
	if (nRet != RTNORM)
	{
		MyBaseUtils::SetVar(_T("OSMODE"), nOsmode);
		return false;
	}
	nRet = acedGetPoint(asDblArray(pt), _T("\n请指定终点"), asDblArray(midPt));
	if (nRet != RTNORM)
	{
		MyBaseUtils::SetVar(_T("OSMODE"), nOsmode);
		return false;
	}
	AcDbObjectId textStyleId = CGWDesingUtils::getGlobalTextStyle();

	AcGePoint3d textPt1, textPt2;
	acutPolar(asDblArray(midPt), PI/2, 1, asDblArray(textPt1));
	acutPolar(asDblArray(midPt), 3*PI/2, 1, asDblArray(textPt2));
	AcDbObjectId textId1 = MyDrawEntity::DrawText(textPt1, m_strUpText, 3.0, textStyleId, AcDb::kTextLeft);
	double dLen1 = MyEditEntity::OpenObjAndGetLength(textId1);
	double dLen2 = 0.0;
	if (!m_strDownText.IsEmpty())
	{
		AcDbObjectId textId2 = MyDrawEntity::DrawText(textPt2, m_strDownText, 3.0, textStyleId, AcDb::kTextLeft, AcDb::kTextTop);
		dLen2 = MyEditEntity::OpenObjAndGetLength(textId2);
		m_idArrs.append(textId2);
	}
	
	double dLen;
	if (dLen1 > dLen2)
	{
		dLen = dLen1;
	}
	else
	{
		dLen = dLen2;
	}
	acutPolar(asDblArray(midPt), 0, dLen, asDblArray(endPt));

	AcGePoint3dArray points;
	points.append(pt);
	points.append(midPt);
	points.append(endPt);
	AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(points);
	plineId = MyEditEntity::openEntChangeColor(plineId, 2);
	m_idArrs.append(plineId);
	m_idArrs.append(textId1);
	MyBaseUtils::SetVar(_T("OSMODE"), nOsmode);
	return true;
}

bool CDrawTGAndGG::getDownText()
{
	TCHAR tempBuf[133];

	int nRet = acedGetString(1, _T("\n请输入通用图做法<做法详见>："),tempBuf);
	if (nRet == RTNORM)
	{
		CString strTmp = tempBuf;
		if (!strTmp.IsEmpty())
		{
			m_strDownText.Format(_T("做法详见%s"), strTmp);
		}
	}
	else if (nRet == RTNONE)
	{
		m_strDownText = _T("做法详见");
	}
	else
	{
		return false;
	}

	return true;
}

bool CDrawTGAndGG::getUpText()
{
	TCHAR tempBuf[255];
	CString strPrompt;
	CString strTmp;
	if (m_bIsTG)
	{
		strTmp = _T("间距不足套管长");
	}
	else
	{
		strTmp = _T("埋深不足管沟长");
	}
	strPrompt.Format(_T("\n说明文字<%s>"), strTmp); 
	int nRet = acedGetString(1, strPrompt,  tempBuf);
	if (nRet == RTNONE)
	{
		m_strDownText = strTmp;
	}
	else if (nRet == RTNORM)
	{
		m_strUpText = tempBuf;
		if (m_strUpText.IsEmpty())
		{
			m_strUpText = strTmp;
		}
	}
	else
	{
		return false;
	}
	double dLen = m_dEndZH - m_dStartZH;
	CString strTmp1;
	strTmp1.Format(_T("%.fm"), dLen);
	m_strUpText += strTmp1;
	return true;
}
CString CDrawTGAndGG::CurNumPosition( double dValue, bool& bIsExisted)
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

bool CDrawTGAndGG::getSpecailData()
{
	bool bIsExisted1 = false;
	bool bIsExisted2 = false;
	CString strCur1 = CurNumPosition(m_dStartZH, bIsExisted1);
	int nCount1 = MyTransFunc::StringToInt(strCur1);
	if (nCount1 < 1)
	{
		return false;
	}
	CString strCur2 = CurNumPosition(m_dEndZH, bIsExisted2);
	int nCount2 = MyTransFunc::StringToInt(strCur2);
	if (nCount2 < 1)
	{
		return false;
	}

	CBcUtils bc;
	CZdmDataInfo preData,curData;
	CString strLabel = BC_DICT + strCur1;
	bc.get(nCount1, curData);
	int nTmp = nCount1 - 1;
	CString strCount;
	strCount.Format(_T("%d"), nTmp);
	CString strTmpLabel = BC_DICT + strCount;
	bc.get(nTmp, preData);

	double dLen1 = m_dStartZH - preData.getcurData();
	double dLen2 = curData.getcurData() - preData.getcurData();
	double dRadio = dLen1/dLen2;
	double dGuandi = (curData.getGuanDi() - preData.getGuanDi())*dRadio + preData.getGuanDi();
	AcGePoint3d basePt,downPt, upPt, endDownPt,endUpPt, tmpPt1,tmpPt2;
	AcGePoint3dArray downpoints, upPoints;
	double dRadius =(CDMXUtils::globalYScale()*curData.getPipeDiameter())/2000;
	basePt = CDMXUtils::getbasePt();
	acutPolar(asDblArray(basePt), 0, 20 + (m_dStartZH - CDMXUtils::startzh())*CDMXUtils::globalXScale(), asDblArray(downPt));
	acutPolar(asDblArray(downPt), PI/2, (dGuandi - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale() - dRadius, asDblArray(downPt));
	acutPolar(asDblArray(downPt), PI/2, 4*dRadius, asDblArray(upPt));

	downpoints.append(downPt);
	upPoints.append(upPt);
	if (m_bIsTG)
	{
		for (int i=nCount1; i < nCount2; i++)
		{
			nTmp = i;
			strCount.Format(_T("%d"), nTmp);
			strLabel = BC_DICT + strCount;
			bc.get(nTmp, curData);
			acutPolar(asDblArray(basePt), 0, 20 + (curData.getcurData() - CDMXUtils::startzh())*CDMXUtils::globalXScale(), asDblArray(tmpPt1));
			acutPolar(asDblArray(tmpPt1), PI/2, (curData.getGuanDi() - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale() - dRadius, asDblArray(tmpPt1));
			acutPolar(asDblArray(tmpPt1), PI/2, 4*dRadius, asDblArray(tmpPt2));
			downpoints.append(tmpPt1);
			upPoints.append(tmpPt2);
		}
	}
	
	//处理最后一个
	nTmp = nCount2 - 1;
	strCount.Format(_T("%d"), nTmp);
	strTmpLabel = BC_DICT + strCount;
	bc.get(nTmp, preData);
	strLabel = BC_DICT + strCur2;
	bc.get(nCount2, curData);

	dLen1 = m_dEndZH - preData.getcurData();
	dLen2 = curData.getcurData() - preData.getcurData();
	dRadio = dLen1/dLen2;
	dGuandi = (curData.getGuanDi() - preData.getGuanDi())*dRadio + preData.getGuanDi();
	acutPolar(asDblArray(basePt), 0, 20 + (m_dEndZH - CDMXUtils::startzh())*CDMXUtils::globalXScale(), asDblArray(endDownPt));
	acutPolar(asDblArray(endDownPt), PI/2, (dGuandi - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale() - dRadius, asDblArray(endDownPt));
	acutPolar(asDblArray(endDownPt), PI/2, 4*dRadius, asDblArray(endUpPt));
	downpoints.append(endDownPt);
	upPoints.append(endUpPt);
	//////////////////////////////////////////////////////////////////////////////////////////////
	AcDbObjectId plineId1 = MyDrawEntity::DrawPlineByPoints(downpoints);
	AcDbObjectId plineId2 = MyDrawEntity::DrawPlineByPoints(upPoints);
	AcDbObjectId plineId3 = MyDrawEntity::DrawPlineByTwoPoint(downPt, upPt);
	AcDbObjectId plineId4 = MyDrawEntity::DrawPlineByTwoPoint(endDownPt, endUpPt);
	plineId1 = MyEditEntity::openEntChangeColor(plineId1, 2);
	plineId2 = MyEditEntity::openEntChangeColor(plineId2, 2);
	plineId3 = MyEditEntity::openEntChangeColor(plineId3, 2);
	plineId4 = MyEditEntity::openEntChangeColor(plineId4, 2);
	m_idArrs.append(plineId1);
	m_idArrs.append(plineId2);
	m_idArrs.append(plineId3);
	m_idArrs.append(plineId4);

	drawlineAndText();
	return true;
}
