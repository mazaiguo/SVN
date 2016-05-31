#include "StdAfx.h"
#include "DrawObstacle.h"
#include "BcUtils.h"
#include "DrawDMXProcess.h"

CDrawObstacle::CDrawObstacle(void)
{
	m_idArrs.removeAll();
	m_dBase = 0;
	m_idArrs.removeAll();
	m_bIsGdingType = false;
	m_dHeight = 0.0;
}

CDrawObstacle::CDrawObstacle(int nIndex, CString strName)
{
	m_nIndex = nIndex;
	m_strName = strName;
	m_dBase = 0;
	m_idArrs.removeAll();
	m_bIsGdingType = false;
	m_dHeight = 0.0;
}

CDrawObstacle::~CDrawObstacle(void)
{
	m_idArrs.removeAll();
}

bool CDrawObstacle::doIt()
{
	if (!getZhuanghao())
	{
		return false;
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
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		bRet = drawRectangle();
		break;
	default:
		bRet = drawOther();
		break;
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
	strPrompt = _T("\n输入顶高(A)/低高（B）/<管底>:");

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
		int nResult = acedGetReal(_T("\n请输入管顶数据："), &dResult);
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
		int nResult = acedGetReal(_T("\n请输入管底数据："), &dResult);
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
	

	drawCirlceOrEllipse();
	drawText();

	makeGroup();
	return true;
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
	strTmp.Format(_T("jc\\JCFH%d.dwg"), m_nIndex + 1);
	strFile = strAppName + strTmp;
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	AcGePoint3d insertPt;

	acutPolar(asDblArray(basePt), 0, 20 + m_zdmdata.getcurData()*1000/(CDMXUtils::getXScale()), asDblArray(insertPt));
	acutPolar(asDblArray(insertPt), 3*PI/2, 91, asDblArray(insertPt));
	AcDbObjectId refId = MyDrawEntity::InsertBlkRef(strFile, insertPt);
	if (refId.isNull())
	{
		acutPrintf(_T("\n程序安装文件有损，请重新安装。"));
		return false;
	}
	
	m_idArrs.append(refId);

	return true;
}

bool CDrawObstacle::drawText()
{
	AcGePoint3d insertPt;
	AcGePoint3d basePt = CDMXUtils::getbasePt();

	acutPolar(asDblArray(basePt), 0, 20 + m_zdmdata.getcurData()*1000/(CDMXUtils::getXScale()), asDblArray(insertPt));
	acutPolar(asDblArray(insertPt), PI/2, 5, asDblArray(insertPt));
	AcDbObjectId textId = MyDrawEntity::DrawText(insertPt, m_strText, 3.0, AcDbObjectId::kNull, AcDb::kTextLeft, AcDb::kTextBottom);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	m_idArrs.append(textId);

	return true;
}

bool CDrawObstacle::drawCirlceOrEllipse()
{
	AcGePoint3d tmpPt,guandiPt,guandiTopPt,cenPt;
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	double dXScale = 1000/(CDMXUtils::getXScale());
	double dYScale = 1000/(CDMXUtils::getYScale());	
	double dRadius = m_dPipeDiameter/2;

	acutPolar(asDblArray(basePt), 0, 20 + m_zdmdata.getcurData()*dXScale, asDblArray(tmpPt));
	//////////////////////////////////////////////////////////////////////////
	//绘制base
	if (m_dBase)
	{
		if (!m_bIsGdingType)
		{
			m_dHeight = m_dHeight + dRadius + m_dBase/1000;
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
	acutPolar(asDblArray(cenPt), PI/2, dRadius, asDblArray(guandiTopPt));
	acutPolar(asDblArray(cenPt), 3*PI/2, dRadius, asDblArray(guandiPt));
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("JC-TMP"), 7);
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();
	if (dXScale != dYScale)
	{
		//绘制椭圆
		double dRadio = dXScale/dYScale;
		AcGeVector3d unitVec;
		unitVec = guandiTopPt - cenPt;
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
	if (m_dBase > 0)
	{
		AcGePoint3d baseDiPt,baseminPt,baseMaxPt;
		acutPolar(asDblArray(guandiPt), 3*PI/2, dYScale*m_dBase/1000, asDblArray(baseDiPt));
		acutPolar(asDblArray(baseDiPt), PI, dXScale*m_dBase/1000, asDblArray(baseminPt));
		acutPolar(asDblArray(guandiPt), 0, dXScale*m_dBase/1000, asDblArray(baseMaxPt));
		AcGePoint3dArray ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(baseminPt, baseMaxPt);
		AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(ptArr);
		m_idArrs.append(plineId);
	}
	return true;
}

void CDrawObstacle::makeGroup()
{
	CString strJcNum = CDMXUtils::getJcNum();
	int nCount = MyTransFunc::StringToInt(strJcNum);
	CString strGroupName = JC_DICT + strJcNum;
	AcDbObjectId groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
	nCount++;
	strJcNum.Format(_T("%d"), nCount);
	CDMXUtils::SetJcNum(strJcNum);
}

bool CDrawObstacle::GetUseBase()
{
	//throw std::logic_error("The method or operation is not implemented.");
	CString strPrompt;	
	bool bRet = true;
	double dHeigth = 0.0;
	int nResult = acedGetReal(_T("\n请输入基础高度<mm>:"), &dHeigth);
	if (nResult != RTNORM)
	{
		return false;
	}
	m_dBase = dHeigth;
	CString strTmp;
	strTmp.Format(_T("带基础%.f"), m_dBase);
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
	double dXScale = 1000/(CDMXUtils::getXScale());
	double dYScale = 1000/(CDMXUtils::getYScale());
	acutPolar(asDblArray(basePt), 0, 20 + m_zdmdata.getcurData()*dXScale, asDblArray(tmpPt));

	acutPolar(asDblArray(tmpPt), PI/2, (m_dHeight - CDMXUtils::getMinElavation())*dYScale, asDblArray(tmpPt));

	//////////////////////////////////////////////////////////////////////////
	if (!m_bIsGdingType)//管底
	{
		acutPolar(asDblArray(tmpPt), PI, dXScale*dWidth/2000, asDblArray(minPt));
		acutPolar(asDblArray(tmpPt), PI/2,dYScale*dHeigth/1000, asDblArray(maxPt));
		acutPolar(asDblArray(maxPt), 0, dXScale*dWidth/2000, asDblArray(maxPt));
		AcGePoint3dArray ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(minPt, maxPt);
		AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(ptArr);
		m_idArrs.append(plineId);
	}
	else
	{
		acutPolar(asDblArray(tmpPt), 0, dXScale*dWidth/2000, asDblArray(maxPt));
		acutPolar(asDblArray(tmpPt), 3*PI/2,dYScale*dHeigth/1000, asDblArray(minPt));
		acutPolar(asDblArray(minPt), PI, dXScale*dWidth/2000, asDblArray(minPt));
		AcGePoint3dArray ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(minPt, maxPt);
		AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(ptArr);
		m_idArrs.append(plineId);
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
