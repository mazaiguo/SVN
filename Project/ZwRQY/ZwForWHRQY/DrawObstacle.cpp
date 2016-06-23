#include "StdAfx.h"
#include "DrawObstacle.h"
#include "BcUtils.h"
#include "DrawDMXProcess.h"
#include "GWDesingUtils.h"
#include "ZdmUtility.h"
#include "DrawZDM.h"
#include <map>

AcDbObjectIdArray drawLineAndText(AcGePoint3d startPt, CString strText)
{
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();

	AcGePoint3d midPt,endPt,txtPt, tmpPt;
	/*int nRet = acedGetPoint(asDblArray(startPt), _T("\n请指定终点"), asDblArray(midPt));
	if (nRet != RTNORM)
	{
		return objIdArr;
	}	
	acedGrDraw(asDblArray(startPt), asDblArray(midPt), 7, 1);*/
	int nRet = acedGetPoint(asDblArray(startPt), _T("\n请指定终点"), asDblArray(endPt));
	if (nRet != RTNORM)
	{
		return objIdArr;
	}
	acedGrDraw(asDblArray(startPt), asDblArray(endPt), 7, 1);

	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId plineId = AcDbObjectId::kNull;
	AcDbObjectId textStyleId = CGWDesingUtils::getGlobalTextStyle();
	AcGePoint3dArray points;
	double dLen;
	AcGePoint3d textPt;
	acutPolar(asDblArray(endPt), PI/2, 1, asDblArray(textPt));
	textId = MyDrawEntity::DrawText(textPt, strText, 3.0, textStyleId, AcDb::kTextLeft);
	dLen = MyEditEntity::OpenObjAndGetLength(textId);
	acutPolar(asDblArray(endPt), 0, dLen, asDblArray(tmpPt));
	points.append(startPt);
	points.append(endPt);
	points.append(tmpPt);
	plineId = MyDrawEntity::DrawPlineByPoints(points);
	objIdArr.append(textId);
	objIdArr.append(plineId);
	return objIdArr;
}


CDrawObstacle::CDrawObstacle(void)
{
	m_idArrs.removeAll();
	m_dYBase = 0;
	m_dXBase = 0;
	m_idArrs.removeAll();
	m_bIsGdingType = false;
	m_dHeight = 0.0;
	m_bIsModGd = false;
}

CDrawObstacle::CDrawObstacle(int nIndex, CString strName)
{
	m_nIndex = nIndex;
	m_strName = strName;
	m_dYBase = 0;
	m_dXBase = 0;
	m_idArrs.removeAll();
	m_bIsGdingType = false;
	m_dHeight = 0.0;
	m_bIsModGd = false;
}

CDrawObstacle::~CDrawObstacle(void)
{
	m_idArrs.removeAll();
}

bool CDrawObstacle::doIt()
{
	m_dXScale = 1000/(CDMXUtils::getXScale());
	m_dYScale = 1000/(CDMXUtils::getYScale());

	if (!getZhuanghao())
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//处理相交
	//先判定是否相交
	if (!verifyIntersect())
	{
		return true;
	}
	return true;
}

CString CDrawObstacle::getName()
{
	return m_strName;
}

void CDrawObstacle::setName(LPCTSTR strName)
{	
	m_strName = strName;
}

int CDrawObstacle::getIndex()
{
	return m_nIndex;
}

void CDrawObstacle::setIndex(int nIndex)
{
	m_nIndex = nIndex;
}

bool CDrawObstacle::getZhuanghao()
{
	bool bRet = true;
	DrawDMXProcess dm;
	if (!dm.Insert(true))
	{
		return false;
	}
	CString strLabel = dm.getLabelName();
	CBcUtils bc;
	bc.get(strLabel, m_zdmdata);
	m_strCount = m_zdmdata.getCount();

	m_dMin = CDMXUtils::getMinElavation();
	m_dMax	= m_zdmdata.getRealDmx();
	
	switch(m_nIndex)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		bRet = drawCircle();
		break;
	case 5:
	case 7:
	case 17:
	case 19:
	case 22:
		bRet = drawRectangle();
		break;
	default:
		bRet = drawOther();
		break;
	}
	if (!bRet)
	{
		dm.del(strLabel);
		makeGroup(false);
	}

	return bRet;
}

bool CDrawObstacle::gethasBase()
{
	CString strPrompt;	
	bool bRet = true;

	acedInitGet(0, _T("Yes No"));
	strPrompt = _T("\n有基础(Y)/无基础(N)<无基础>:");
	TCHAR val[512];
	int nRet = acedGetKword(strPrompt, val);
	if (nRet == RTNORM)
	{
		if (_tcscmp(val, _T("Yes")) == 0)
		{
			bRet = GetUseBase();
		}
		else if (_tcscmp(val, _T("No")) == 0)
		{
			bRet = true;
		}		
	}
	else if (nRet == RTNONE)
	{
		bRet = true;
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CDrawObstacle::getTopOrBottom()
{
	CString strPrompt;	
	bool bRet = true;

	acedInitGet(0, _T("A B"));
	strPrompt = _T("\n输入管顶标高(A)/管底标高（B）/<管底标高>:");

	TCHAR val[512];
	int nRet = acedGetKword(strPrompt, val);
	if (nRet == RTNORM)
	{
		if (_tcscmp(val, _T("A")) == 0)
		{
			bRet = GetGuanDing();
		}
		else if (_tcscmp(val, _T("B")) == 0)
		{
			bRet = GetGuandi();
		}		
	}
	else if (nRet == RTNONE)
	{
		bRet = GetGuandi();
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CDrawObstacle::GetGuanDing()
{
	//throw std::logic_error("The method or operation is not implemented.");
	double dResult = 0.0;
	bool bIsContinued = true;
	bool bRet = true;
	while (bIsContinued)
	{
		int nResult = acedGetReal(_T("\n请输入管底标高："), &dResult);
		if (nResult != RTNORM)
		{
			bRet = false;
			break;
		}
		if (!verifyValue(dResult))
		{
			AfxMessageBox(_T("输入的管顶高度不能低于最低的数值，也不能超过真实水面线高度,请重新输入数据"));
			bIsContinued = true;
		}
		else
		{
			bIsContinued = false;
			bRet = true;
		}
	}
	if (bRet)
	{
		CString strTmp;
		strTmp.Format(_T("  管顶%.2f"), dResult);
		m_strText = m_strText + m_strName + strTmp;
		m_dHeight = dResult;
		m_bIsGdingType = true;
	}
	
	return bRet;
}

bool CDrawObstacle::GetGuandi()
{
	//throw std::logic_error("The method or operation is not implemented.");
	double dResult = 0.0;
	bool bIsContinued = true;
	bool bRet = true;
	while (bIsContinued)
	{
		int nResult = acedGetReal(_T("\n请输入管底标高："), &dResult);
		if (nResult != RTNORM)
		{
			bRet = false;
			break;
		}
		if (!verifyValue(dResult))
		{
			AfxMessageBox(_T("输入的管底高度不能低于最低的数值，也不能超过真实水面线高度,请重新输入数据"));
			bIsContinued = true;
		}
		else
		{
			bIsContinued = false;
			bRet = true;
		}
	}
	if (bRet)
	{
		CString strTmp;
		strTmp.Format(_T("  管底%.2f"), dResult);
		m_strText = m_strText + m_strName + strTmp;
		m_bIsGdingType = false;
		m_dHeight = dResult;
	}
	
	return bRet;
}

bool CDrawObstacle::drawCircle()
{
	//throw std::logic_error("The method or operation is not implemented.");
	//////////////////////////////////////////////////////////////////////////
	if (!gethasBase())
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	if (!getTopOrBottom())
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取管径
	double dResult = 0.0;
	int nResult = acedGetReal(_T("\n请输入管径<mm>:"), &dResult);
	if (nResult != RTNORM)
	{
		return false;
	}
	m_dPipeDiameter = dResult/1000;

	//////////////////////////////////////////////////////////////////////////
	if (!insertRef())
	{
		return false;
	}
	CString strTmp;
	strTmp.Format(_T(" 管径%.fmm"), dResult);
	m_strText += strTmp;
	

	bool bRet = drawCirlceOrEllipse();
	//drawText();
	//AcDbObjectIdArray objIdArr = drawLineAndText()

	makeGroup();
	return bRet;
}

bool CDrawObstacle::drawRectangle()
{
	//throw std::logic_error("The method or operation is not implemented.");
	if (!getTopOrBottom())
	{
		return false;
	}

	if (!getHeightOrWidth())
	{
		return false;
	}

	drawText();

	makeGroup();
	return true;
}

bool CDrawObstacle::drawOther()
{
	//throw std::logic_error("The method or operation is not implemented.");
	bool bRet = drawCircle();
	return bRet;
}

bool CDrawObstacle::verifyValue(double dValue)
{
	if (dValue > m_dMax)
	{
		return false;
	}
	if (dValue < m_dMin)
	{
		return false;
	}
	return true;
}

bool CDrawObstacle::insertRef()
{
	CString strAppName = MyBaseUtils::GetAppPath();
	CString strFile,strTmp;
	strTmp.Format(_T("zdm\\jc\\JCFH%d.dwg"), m_nIndex + 1);
	strFile = strAppName + strTmp;
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	AcGePoint3d insertPt;

	acutPolar(asDblArray(basePt), 0, 20 + (m_zdmdata.getcurData() - CDMXUtils::startzh())*1000/(CDMXUtils::getXScale()), asDblArray(insertPt));
	acutPolar(asDblArray(insertPt), 3*PI/2, 91, asDblArray(insertPt));
	m_objId = MyDrawEntity::InsertBlkRef(strFile, insertPt);
	if (m_objId.isNull())
	{
		acutPrintf(_T("\n程序安装文件有损，请重新安装。"));
		return false;
	}
	
	m_idArrs.append(m_objId);

	return true;
}

bool CDrawObstacle::drawText()
{
	AcGePoint3d insertPt;
	AcGePoint3d basePt = CDMXUtils::getbasePt();

	acutPolar(asDblArray(basePt), 0, 20 + (m_zdmdata.getcurData() - CDMXUtils::startzh())*1000/(CDMXUtils::getXScale()), asDblArray(insertPt));
	acutPolar(asDblArray(insertPt), PI/2, (m_dHeight - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale(), asDblArray(insertPt));

	/*acutPolar(asDblArray(insertPt), PI/2, 5, asDblArray(insertPt));
	AcDbObjectId textId = MyDrawEntity::DrawText(insertPt, m_strText, 3.0, AcDbObjectId::kNull, AcDb::kTextLeft, AcDb::kTextBottom);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	m_idArrs.append(textId);*/

	AcDbObjectIdArray objIdArr = drawLineAndText(insertPt, m_strText);
	if (objIdArr.length() < 1)
	{
		return false;
	}
	for (int i=0; i<objIdArr.length(); i++)
	{
		m_idArrs.append(objIdArr.at(i));
	}

	return true;
}

bool CDrawObstacle::drawCirlceOrEllipse()
{
	AcGePoint3d tmpPt,cenPt;
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	double dXScale = 1000/(CDMXUtils::getXScale());
	double dYScale = 1000/(CDMXUtils::getYScale());	
	double dRadius = m_dPipeDiameter/2;
	m_dFlag = dRadius;

	acutPolar(asDblArray(basePt), 0, 20 + (m_zdmdata.getcurData() - CDMXUtils::startzh())*dXScale, asDblArray(tmpPt));
	//////////////////////////////////////////////////////////////////////////
	//绘制base
	if (m_dYBase)
	{
		if (!m_bIsGdingType)
		{
			m_dHeight = m_dHeight + dRadius + m_dYBase/1000;
		}
		else
		{
			m_dHeight = m_dHeight - dRadius;
		}
	}
	else
	{
		if (!m_bIsGdingType)
		{
			m_dHeight += dRadius;
		}
		else
		{
			m_dHeight -= dRadius;
		}
	}
	
	dRadius *= dYScale;
	acutPolar(asDblArray(tmpPt), PI/2, (m_dHeight - CDMXUtils::getMinElavation())*dYScale, asDblArray(cenPt));
	acutPolar(asDblArray(cenPt), PI/2, dRadius, asDblArray(m_guandiTopPt));
	acutPolar(asDblArray(cenPt), 3*PI/2, dRadius, asDblArray(m_guandiPt));
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("JC-TMP"), 7);
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();
	if (dXScale != dYScale)
	{
		//绘制椭圆
		double dRadio = dXScale/dYScale;
		AcGeVector3d unitVec;
		unitVec = m_guandiTopPt - cenPt;
		objId = MyDrawEntity::DrawEllipse(cenPt, AcGeVector3d::kZAxis, unitVec, dRadio);
		objId = MyEditEntity::openEntChangeLayer(objId, ZxLayerId);
		objId = MyEditEntity::openEntChangeColor(objId, 3);
		m_idArrs.append(objId);		
		//
	}
	else
	{
		//绘制圆
		objId = MyDrawEntity::DrawCircle(cenPt, dRadius, ZxLayerId);
		objId = MyEditEntity::openEntChangeColor(objId, 3);
		m_idArrs.append(objId);
	}
	if (m_dYBase > 0)
	{
		AcGePoint3d baseDiPt,baseminPt,baseMaxPt;	
		acutPolar(asDblArray(m_guandiPt), 0, dXScale*m_dXBase/2000, asDblArray(baseMaxPt));
		acutPolar(asDblArray(m_guandiPt), 3*PI/2, dYScale*m_dYBase/1000, asDblArray(m_guandiPt));
		acutPolar(asDblArray(m_guandiPt), PI, dXScale*m_dXBase/2000, asDblArray(baseminPt));
		AcGePoint3dArray ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(baseminPt, baseMaxPt);
		AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(ptArr);
		m_idArrs.append(plineId);
	}
	/*if (m_bIsGdingType)
	{*/
	objIdArr = drawLineAndText(m_guandiTopPt, m_strText);
	/*}
	else
	{
		objIdArr = drawLineAndText(m_guandiPt, m_strText);
	}*/
	if (objIdArr.length() < 1)
	{
		return false;
	}
	for (int i=0; i<objIdArr.length(); i++)
	{
		m_idArrs.append(objIdArr.at(i));
	}

	return true;
}

void CDrawObstacle::makeGroup(bool bIsAdded)
{
	if (!m_bIsModGd)
	{
		CString strJcNum = CDMXUtils::getJcNum();
		int nCount = MyTransFunc::StringToInt(strJcNum);
		CString strGroupName = JC_DICT + strJcNum;
		if (bIsAdded)
		{
			AcDbObjectId groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
			nCount++;
			strJcNum.Format(_T("%d"), nCount);
		}
		else
		{
			nCount--;
			strJcNum.Format(_T("%d"), nCount);
			strGroupName = JC_DICT + strJcNum;
			MyEditEntity::EraseEntByGroupName(strGroupName);

		}
		CDMXUtils::SetJcNum(strJcNum);
	}
	else
	{
		CString strGroupName = JC_DICT_ZA + m_strCount;
		if (bIsAdded)
		{
			AcDbObjectId groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
			if (m_bIsModGd)//如果修改了管道，那么要修改障碍物的名称
			{
				selAllJCGroup();
			}
		}
		else
		{
			MyEditEntity::EraseEntByGroupName(strGroupName);
		}
	}
}

bool CDrawObstacle::GetUseBase()
{
	//throw std::logic_error("The method or operation is not implemented.");
	CString strPrompt;	
	bool bRet = true;
	double dHeigth = 0.0;
	int nRet = acedGetReal(_T("\n请输入基础长度<mm>:"), &dHeigth);
	if (nRet != RTNORM)
	{
		return false;
	}
	m_dXBase = dHeigth;
	int nResult = acedGetReal(_T("\n请输入基础高度<mm>:"), &dHeigth);
	if (nResult != RTNORM)
	{
		return false;
	}
	m_dYBase = dHeigth;
	
	CString strTmp;
	strTmp.Format(_T("带基础%.fX%.f"), m_dXBase, m_dYBase);
	m_strText += strTmp;
	return bRet;
}

bool CDrawObstacle::getHeightOrWidth()
{
	//throw std::logic_error("The method or operation is not implemented.");
	CString strPrompt;	
	bool bRet = true;

	acedInitGet(0, _T("H W"));
	strPrompt = _T("\n输入高度(H)/宽度（W）/<高度>:");

	TCHAR val[512];
	int nRet = acedGetKword(strPrompt, val);
	if (nRet == RTNORM)
	{
		if (_tcscmp(val, _T("H")) == 0)
		{
			bRet = getRecHeight();
		}
		else if (_tcscmp(val, _T("W")) == 0)
		{
			bRet = getRecWidth();
		}		
	}
	else if (nRet == RTNONE)
	{
		bRet = getRecHeight();
	}
	else
	{
		return false;
	}
	return bRet;
}

bool CDrawObstacle::getRecHeight()
{
	double dHeigth = 0.0;
	double dWidth = 0.0;
	int nResult = acedGetReal(_T("\n请输入高度<mm>:"), &dHeigth);
	if (nResult != RTNORM)
	{
		return false;
	}

	nResult = acedGetReal(_T("\n请输入宽度<mm>:"), &dWidth);
	if (nResult != RTNORM)
	{
		return false;
	}
	CString strTmp;
	strTmp.Format(_T(" %.fX%.f"), dHeigth, dWidth);
	m_strText += strTmp;
	drawRec(dHeigth, dWidth);
	return true;
}

bool CDrawObstacle::getRecWidth()
{
	//throw std::logic_error("The method or operation is not implemented.");
	double dHeigth = 0.0;
	double dWidth = 0.0;
	int nResult = acedGetReal(_T("\n请输入宽度<mm>:"), &dHeigth);
	if (nResult != RTNORM)
	{
		return false;
	}
	
	nResult = acedGetReal(_T("\n请输入高度<mm>:"), &dHeigth);
	if (nResult != RTNORM)
	{
		return false;
	}
	CString strTmp;
	strTmp.Format(_T(" %.fX%.f"), dHeigth, dWidth);
	m_strText += strTmp;
	drawRec(dHeigth, dWidth);
	return true;
}

void CDrawObstacle::drawRec(double dHeigth, double dWidth)
{
	AcGePoint3d tmpPt,minPt,maxPt;
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	acutPolar(asDblArray(basePt), 0, 20 + (m_zdmdata.getcurData() - CDMXUtils::startzh())*m_dXScale, asDblArray(tmpPt));

	acutPolar(asDblArray(tmpPt), PI/2, (m_dHeight - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(tmpPt));

	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId objId = AcDbObjectId::kNull;
	if (!m_bIsGdingType)//管底
	{
		acutPolar(asDblArray(tmpPt), PI, m_dXScale*dWidth/2000, asDblArray(minPt));
		acutPolar(asDblArray(tmpPt), PI/2,m_dYScale*dHeigth/1000, asDblArray(maxPt));
		acutPolar(asDblArray(maxPt), 0, m_dXScale*dWidth/2000, asDblArray(maxPt));
		AcGePoint3dArray ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(minPt, maxPt);
		objId = MyDrawEntity::DrawPlineByPoints(ptArr);
		m_idArrs.append(objId);
	}
	else
	{
		acutPolar(asDblArray(tmpPt), 0, m_dXScale*dWidth/2000, asDblArray(maxPt));
		acutPolar(asDblArray(tmpPt), 3*PI/2,m_dYScale*dHeigth/1000, asDblArray(minPt));
		acutPolar(asDblArray(minPt), PI, m_dXScale*dWidth/2000, asDblArray(minPt));
		AcGePoint3dArray ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(minPt, maxPt);
		objId = MyDrawEntity::DrawPlineByPoints(ptArr);
		m_idArrs.append(objId);
	}
	m_dFlag = dWidth/2000;
}

bool CDrawObstacle::verifyIntersect()
{
	//throw std::logic_error("The method or operation is not implemented.");
	AcGePoint3d minPt,maxPt;
	acutPolar(asDblArray(m_guandiPt), 3*PI/2, 0.15*m_dYScale, asDblArray(minPt));
	acutPolar(asDblArray(m_guandiTopPt), PI/2, 0.15*m_dYScale, asDblArray(maxPt));
	AcGePoint3d guandiPt,guantopPt;

	CZdmUtility util;
	guandiPt = util.getGuandiPt(m_zdmdata);
	guantopPt = util.getGuanTopPt(m_zdmdata);

	AcGeLine3d line(minPt, maxPt);
	double dDist1,dDist2;
	bool bo1 = line.isOn(guandiPt, dDist1);
	bool bo2 = line.isOn(guantopPt, dDist2);
	if (bo1 || bo2)//有交集
	{
		CString strPrompt;
		acedInitGet(0, _T("Up Down"));
		TCHAR temp[133];
		int nRet = acedGetKword(_T("上翻(U)/下翻(D)<下翻>："), temp);
		if (nRet == RTNONE)
		{
			//下翻
			m_SpecialPt = minPt;
			acutPolar(asDblArray(m_SpecialPt), 3*PI/2, m_dYScale*m_zdmdata.getPipeDiameter()/1000, asDblArray(m_SpecialPt));
		}
		else if (nRet == RTNORM)
		{
			if (_tcscmp(temp, _T("Up")) == 0)
			{
				m_SpecialPt = maxPt;
			}
			else if (_tcscmp(temp, _T("Down")) == 0)
			{
				m_SpecialPt = minPt;
				acutPolar(asDblArray(m_SpecialPt), 3*PI/2, m_dYScale*m_zdmdata.getPipeDiameter()/1000, asDblArray(m_SpecialPt));
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		//////////////////////////////////////////////////////////////////////
			

		//////////////////////////////////////////////////////////////////////////
		int nCount = MyTransFunc::StringToInt(m_zdmdata.getCount());
		int nTmp = nCount - 1;
		CString strCur;
		strCur.Format(_T("%d"), nTmp);
		CString strLablel = BC_DICT + strCur;
		CBcUtils bc;
		bc.get(strLablel, m_preZdm);
		AcGePoint3d preGuandiPt = util.getGuandiPt(m_preZdm);
		double dLen1 = abs(preGuandiPt.y - m_SpecialPt.y)/m_dYScale;

		nTmp = nCount + 1;
		strCur.Format(_T("%d"), nTmp);
		strLablel = BC_DICT + strCur;
		bc.get(strLablel, m_nextZdm);
		AcGePoint3d nextGuandiPt = util.getGuandiPt(m_nextZdm);
		double dLen2 = abs(nextGuandiPt.y - m_SpecialPt.y)/m_dYScale;

		//////////////////////////////////////////////////////////////////////////
		double dLeftZh1,dLeftZh2,dRightZh3,dRightZh4, dCurData;
		dCurData = m_zdmdata.getcurData();
		dLeftZh1 = dCurData - (0.15 + m_dFlag + dLen1);
		dLeftZh2 = dLeftZh1 + dLen1;

		dRightZh3 = dCurData + (0.15 + m_dFlag);
		dRightZh4 = dRightZh3 + dLen2;
		DrawDMXProcess dm;
		dm.Insert(dLeftZh1, true);
		dm.Insert(dRightZh4, true);

		//修改当前
		modCurZh();

		dm.Insert(dLeftZh2, false);
		dm.Insert(dRightZh3,  false, false);
		MyEditEntity::OpenObjAppendStrToXdata(m_objId, ZDM_JC_ADD, _T("障碍物与管道相交"));
		m_bIsModGd = true;
	}
	else
	{
		return false;
	}
	return true;
}

void CDrawObstacle::modCurZh()
{
	//先修改当前的桩号
	CZdmUtility util;
	double dGuandi = util.getLength(m_zdmdata, m_SpecialPt);
	m_zdmdata.setGuanDi(dGuandi);
	double dRealDmx = m_zdmdata.getRealDmx();
	double dWashen = dRealDmx - dGuandi;
	m_zdmdata.setWaShen(dWashen);
	CString strCount = m_zdmdata.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	nCount++;
	strCount.Format(_T("%d"), nCount);
	m_zdmdata.setCount(strCount);
	m_zdmdata.setJiedian(strCount);
	CString strLabel = BC_DICT + strCount;
	m_zdmdata.setLabel(strLabel);
	MyEditEntity::EraseEntByGroupName(m_zdmdata.label());

	CDrawZDM zdm;
	zdm.setData(&m_zdmdata);
	bool bRet = zdm.mod(m_zdmdata.label());

	if (CDMXUtils::getcreateGw())
	{
		CString strGdGroupName;
		strGdGroupName = BC_DICT_GD + m_zdmdata.getCount();
		MyEditEntity::EraseEntByGroupName(strGdGroupName);

		CDrawGd gd;
		gd.mod(m_zdmdata);
	}
}

void CDrawObstacle::editGroupName(CString strPreName, CString strGroupName)
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strPreName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return;
	}
	Acad::ErrorStatus es;
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();
	int nLength = 0;
	nLength = pGroup->allEntityIds(objIdArr);

	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId = AcDbObjectId::kNull;
	for (int i=0; i<nLength; i++)
	{
		objId = objIdArr.at(i);
		es = acdbOpenAcDbEntity((AcDbEntity*&)pEnt, objId, AcDb::kForWrite);
		if (es!= Acad::eOk)
		{
			pEnt->close();
		}
		else
		{
			//需要将水面线删除
			pEnt->erase();
			pEnt->close();
		}
	}
	pGroup->erase();
	pGroup->close();
	pGroupDict->close();

	MyDrawEntity::MakeGroup(objIdArr, false, strGroupName);
}

void CDrawObstacle::selAllJCGroup()
{
	int nCount = MyTransFunc::StringToInt(m_strCount);
	ads_name ssname;
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	int nRet = acedSSGet(_T("X"), NULL, NULL, NULL, ssname);
	if (nRet != RTNORM)
	{
		return;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	CString strGroupName;
	map<int, int> Info;
	CString strCur,strTmp;
	int nCur = 0;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
		int nFind = strGroupName.Find(JC_DICT_ZA);
		if (nFind >= 0)
		{
			nCur = MyParserString::GetPileLength(strGroupName, strTmp);
			if (nCur > nCount)
			{
				Info.insert(std::make_pair(nCur, nCur));
			}
		}
	}
	
	CString strPreGroupName;
	for (map<int, int>::reverse_iterator iter = Info.rbegin();
		iter != Info.rend();
		iter++)
	{
		nCount = iter->first;
		nCur = nCount + 4;
		strCur.Format(_T("%d"), nCount);
		strTmp.Format(_T("%d"), nCur);
		strPreGroupName = JC_DICT_ZA + nCount;
		strGroupName = JC_DICT_ZA + nCur;
		editGroupName(strPreGroupName, strGroupName);
	}
}

//bool CDrawObstacle::GetBaseHeight()
//{
//	//throw std::logic_error("The method or operation is not implemented.");
//	//获取管径
//	double dHeigth = 0.0;
//	double dWidth = 0.0;
//	int nResult = acedGetReal(_T("\n请输入高度<mm>:"), &dHeigth);
//	if (nResult != RTNORM)
//	{
//		return false;
//	}
//	
//}
//
//bool CDrawObstacle::GetBaseWidth()
//{
//	//throw std::logic_error("The method or operation is not implemented.");
//	double dResult = 0.0;
//	int nResult = acedGetReal(_T("\n请输入宽度<mm>:"), &dResult);
//	if (nResult != RTNORM)
//	{
//		return false;
//	}
//
//}
