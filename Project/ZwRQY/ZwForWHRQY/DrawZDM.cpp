#include "StdAfx.h"
#include "DrawZDM.h"
#include "ZDMUtils.h"
#include "BcUtils.h"
#include "CBiaochiForRQY.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDrawZDM::CDrawZDM(void )
{
	//m_pZDM = new CBiaochiForRQY();

	m_bIsDrawGD = false;
	m_idArrs.removeAll();
}

CDrawZDM::CDrawZDM( CZdmDataInfo* pData )
{
	m_pZDM.setLabel(pData->label());
	m_pZDM.setCount(pData->getCount());
	m_pZDM.setDesignDmx(pData->getDesignDmx());
	m_pZDM.setRealDmx(pData->getRealDmx());
	m_pZDM.setJiedian(pData->getJiedian());
	m_pZDM.setGuanDi(pData->getGuanDi());
	m_pZDM.setWaShen(pData->getWaShen());
	m_pZDM.setPoDu(pData->getPoDu());
	m_pZDM.setJuli(pData->getJuli());
	m_pZDM.setcurData(pData->getcurData());
	m_pZDM.setHasBulge(pData->getHasBulge());
	m_pZDM.setDesingDmxS(pData->getDesignDmxS());
	m_pZDM.setRealDmxS(pData->getRealDmxS());
	m_pZDM.setPipeDiameter(pData->getPipeDiameter());
	m_idArrs.removeAll();
}

CDrawZDM::~CDrawZDM(void)
{
 	/*if (m_pZDM != NULL)
 	{
 		delete m_pZDM;
 		m_pZDM = NULL;
 	}*/
	m_idArrs.removeAll();
}


AcDbObjectId CDrawZDM::add()
{
	AcDbObjectId groupId = AcDbObjectId::kNull;
	initdata();
	CString strCount = m_pZDM.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	CString strGroupName;
	

	if (!m_bIsDrawGD)
	{
		DrawLine();

		DrawDMText();
		if (nCount > 1)
		{
			DrawSMXLine();
		}
		strGroupName = BC_DICT + strCount;
		groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
		nCount++;
		strCount.Format(_T("%d"), nCount);
		CDMXUtils::setNumCount(strCount);
		//CDMXUtils::SetCurNum(strCount);
		CDMXUtils::SetJdNum(strCount);
		//////////////////////////////////////////////////////////////////////////
		//将数据添加到图纸中
		CBcUtils bcUtils;
		bcUtils.add(strGroupName, m_pZDM);
	}
	else
	{
		CDrawGd gd;
		gd.add(m_pZDM);
	}

	
	
	return groupId;
}


void CDrawZDM::setData( CZdmDataInfo* pData )
{
	m_pZDM.setLabel(pData->label());
	m_pZDM.setCount(pData->getCount());
	m_pZDM.setDesignDmx(pData->getDesignDmx());
	m_pZDM.setRealDmx(pData->getRealDmx());
	m_pZDM.setJiedian(pData->getJiedian());
	m_pZDM.setGuanDi(pData->getGuanDi());
	m_pZDM.setWaShen(pData->getWaShen());
	m_pZDM.setPoDu(pData->getPoDu());
	m_pZDM.setJuli(pData->getJuli());
	m_pZDM.setcurData(pData->getcurData());
	m_pZDM.setHasBulge(pData->getHasBulge());
	m_pZDM.setDesingDmxS(pData->getDesignDmxS());
	m_pZDM.setRealDmxS(pData->getRealDmxS());
	m_pZDM.setPipeDiameter(pData->getPipeDiameter());
	m_idArrs.removeAll();
}

AcDbObjectId CDrawZDM::insert()
{	
	AcDbObjectId groupId = AcDbObjectId::kNull;
	
	initdata();

	CString strCount = m_pZDM.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);

	if (nCount > 1)
	{	
		ModifyDictbyInt(nCount);	
	}

	CString strGroupName;
	strGroupName = BC_DICT + strCount;
	

	//////////////////////////////////////////////////////////////////////////
	//将数据添加到图纸中
	CBcUtils bcUtils;
	bcUtils.insert(strGroupName, m_pZDM);

	DrawLine(false);

	DrawDMText();
	DrawNextDMText();
	if (nCount > 1)
	{	
		DrawSMXLine(false);
	}
	groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
	
	strCount = CDMXUtils::getNumCount();
	nCount = MyTransFunc::StringToInt(strCount);
	nCount++;
	strCount.Format(_T("%d"), nCount);
	CDMXUtils::setNumCount(strCount);
	//CDMXUtils::SetCurNum(strCount);
	CDMXUtils::SetJdNum(strCount);
	return groupId;
}

bool CDrawZDM::del(CString strGroupName)
{
	initdata();
	m_bIsDel = true;

	CString strCurCount = m_pZDM.getCount();
	int ncurCount = MyTransFunc::StringToInt(strCurCount);

	if (ncurCount > 1)
	{	
		ModifyDictbyInt(ncurCount, false);
	}

	//////////////////////////////////////////////////////////////////////////
	//将数据添加到图纸中
	CBcUtils bcUtils;
	bcUtils.del(strGroupName);
	CString strCount = CDMXUtils::getNumCount();
	int nCount = MyTransFunc::StringToInt(strCount);

	if (ncurCount != nCount - 1)//如果不是最后一个桩号
	{
		DrawLine();
		DrawSMXLine();
		DrawDMText();
		AcDbObjectId objId = AcDbObjectId::kNull;
		for (int i=0; i<m_idArrs.length(); i++)
		{
			objId = m_idArrs.at(i);
			AddObjToDict(strGroupName, objId);
		}
	}
	

	nCount--;
	strCount.Format(_T("%d"), nCount);
	CDMXUtils::setNumCount(strCount);
	//CDMXUtils::SetCurNum(strCount);
	CDMXUtils::SetJdNum(strCount);
	return true;
}

bool CDrawZDM::mod(CString strGroupName)
{
	initdata();
	CString strCurCount = m_pZDM.getCount();
	int ncurCount = MyTransFunc::StringToInt(strCurCount);
	CString strCount = CDMXUtils::getNumCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	if (ncurCount != nCount - 1)//如果不是最后一个
	{	
		EditDict(ncurCount);
	}

	CBcUtils utls;
	utls.modify(strGroupName, m_pZDM);
	DrawLine(false);
	DrawSMXLine(false);
	DrawDMText();

	AcDbObjectId groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
	return true;
}

void CDrawZDM::setDrawGd(bool bIsDrawGd)
{
	m_bIsDrawGD = bIsDrawGd;
}

bool CDrawZDM::initdata()
{
	m_dXScale = 1000/(CDMXUtils::getXScale());
	m_dYScale = 1000/(CDMXUtils::getYScale());
	/*CString strCount = m_pZDM.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);*/
	//if (nCount == 1)
	{
		m_dLen = m_dXScale*10;
	}

#if 0
	m_dWidth = m_dYScale*15.2;
#else
	m_dWidth = m_dYScale*16.7;
#endif
	m_bIsDel = false;
	m_basePt = CDMXUtils::getbasePt();

	return true;
}

bool CDrawZDM::DrawLine(bool bIsDeFault)
{
	AcGePoint3d endPt;

	AcDbObjectId lineId = AcDbObjectId::kNull;
	CString strCount = m_pZDM.getCount();	
	int nCount = MyTransFunc::StringToInt(strCount);	

	if (nCount > 1)
	{	
		CString strPreCount;

		strPreCount.Format(_T("%d"), nCount - 1);
		CString strLabel = BC_DICT + strPreCount;

		CBcUtils utls;
		utls.get(strLabel, m_preData);
	}
	
	//绘制横着的线
	AcDbObjectId hxLayerId = MySymble::CreateNewLayer(_T("HX-TMP"), 7);
	for (int i=0; i<11; i++)
	{
		AcGePoint3d tmpPt;
#if 0
		if (i >= 5)
#else
		if (i >= 6)
#endif
		{
			acutPolar(asDblArray(m_basePt), 3*PI/2, (1.5*i + 1.7)*m_dYScale, asDblArray(tmpPt));
		}
		else
		{
			acutPolar(asDblArray(m_basePt), 3*PI/2, i*1.5*m_dYScale, asDblArray(tmpPt));
		}
		acutPolar(asDblArray(tmpPt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(endPt));
		if (nCount > 1)
		{
			acutPolar(asDblArray(tmpPt), 0, m_dLen + m_preData.getcurData()*m_dXScale, asDblArray(tmpPt));
		}
		lineId = MyDrawEntity::DrawLine(tmpPt, endPt, hxLayerId);
		m_idArrs.append(lineId);
	}
	//插入的时候需要将横向的线绘制一遍
	if (!bIsDeFault)
	{
		CBcUtils utls;
		CString strNext;
		strNext.Format(_T("%d"), nCount + 1);
		CString strNextLabel = BC_DICT + strNext;
		CZdmDataInfo NextData;
		bool bGetNextData = utls.get(strNextLabel, NextData);
		if (bGetNextData)
		{
			for (int i=0; i<11; i++)
			{
				AcGePoint3d tmpPt;
				if (i >= 6)
				{
					acutPolar(asDblArray(m_basePt), 3*PI/2, (1.5*i + 1.7)*m_dYScale, asDblArray(tmpPt));
				}
				else
				{
					acutPolar(asDblArray(m_basePt), 3*PI/2, i*1.5*m_dYScale, asDblArray(tmpPt));
				}
				acutPolar(asDblArray(tmpPt), 0, m_dLen + NextData.getcurData()*m_dXScale, asDblArray(endPt));
				if (nCount > 1)
				{
					acutPolar(asDblArray(tmpPt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(tmpPt));
				}
				lineId = MyDrawEntity::DrawLine(tmpPt, endPt, hxLayerId);
				AddObjToDict(strNextLabel, lineId);
			}
		}
		
	}
	//先使用设计水面高来绘制图形，后续不对再修改
	
	double dDesignDmx = (m_pZDM.getDesignDmx() - CDMXUtils::getMinElavation())*m_dYScale;
	double dRealDmx = (m_pZDM.getRealDmx() - CDMXUtils::getMinElavation())*m_dYScale;
	double dDesignDmxS = (m_pZDM.getDesignDmxS() - CDMXUtils::getMinElavation())*m_dYScale;
	double dRealDmxS = (m_pZDM.getRealDmxS()- CDMXUtils::getMinElavation())*m_dYScale;


	AcGePoint3d tmpPt,basePt;	
	acutPolar(asDblArray(m_basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(basePt));

	if (dDesignDmx < dRealDmx)
	{
		acutPolar(asDblArray(basePt), PI/2, dRealDmx, asDblArray(tmpPt));
	}
	else
	{
		acutPolar(asDblArray(basePt), PI/2, dDesignDmx, asDblArray(tmpPt));
	}

	if ((dDesignDmxS >= dRealDmxS) 
		&& (dDesignDmxS >= dRealDmx)
		&& (dDesignDmxS >= dDesignDmx))
	{
		acutPolar(asDblArray(basePt), PI/2, dDesignDmxS, asDblArray(tmpPt));
	}
	else if ((dRealDmxS >= dDesignDmxS) 
		&& (dRealDmxS >= dRealDmx)
		&& (dRealDmxS >= dDesignDmx))
	{
		acutPolar(asDblArray(basePt), PI/2, dRealDmxS, asDblArray(tmpPt));
	}
	else if ((dRealDmx >= dDesignDmxS) 
		&& (dRealDmx >= dRealDmxS)
		&& (dRealDmx >= dDesignDmx))
	{
		acutPolar(asDblArray(basePt), PI/2, dRealDmx, asDblArray(tmpPt));
	}
	else
	{
		acutPolar(asDblArray(basePt), PI/2, dDesignDmx, asDblArray(tmpPt));
	}

	if (nCount == 0)
	{
		acutPolar(asDblArray(basePt), 3*PI/2, m_dWidth, asDblArray(endPt));
	}
	else
	{
		acutPolar(asDblArray(basePt), 3*PI/2, m_dWidth-1.5*m_dYScale, asDblArray(endPt));
	}
	bool bJd = CDMXUtils::getcreateJiedian();
	if (!bJd)
	{		
		AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("ZX-TMP"), 7);
		lineId = MyDrawEntity::DrawLine(tmpPt, endPt, ZxLayerId);
		m_idArrs.append(lineId);
	}
	else
	{
		if (!m_bIsDel)
		{
			DrawJdText(basePt, tmpPt, endPt);
		}
	}
	

	return true;
}

bool CDrawZDM::DrawDMText()
{
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("DM-TEXT"), 7);
	double dSJDmx = m_pZDM.getDesignDmx();
	double dXZDmx = m_pZDM.getRealDmx();
	double dCurData = m_pZDM.getcurData();
	bool bDrawBc = CDMXUtils::getcreateBc();
	AcGePoint3d textPt;
	CString strTmp;
	CString strSJDmx = MyTransFunc::doubleToStr(dSJDmx, strTmp);
	CString strXZDmx = MyTransFunc::doubleToStr(dXZDmx, strTmp);
	
	CString strZhuanghao = MyTransFunc::doubleToStr(dCurData, strTmp);
	//绘制设计地面高文字
	acutPolar(asDblArray(m_basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(textPt));
	acutPolar(asDblArray(textPt), 3*PI/2, 0.75*m_dYScale, asDblArray(textPt));
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
	textId = MyDrawEntity::DrawText(textPt, strSJDmx, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
	m_idArrs.append(textId);
	
	//绘制现状地面高
	acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(textPt));
	textId = MyDrawEntity::DrawText(textPt, strXZDmx, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
	m_idArrs.append(textId);

	////绘制节点号
	//if (bDrawBc)
	//{
	//	CString strJiedian = m_pZDM.getJiedian();
	//	AcGePoint3d tmpPt;
	//	acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(tmpPt));
	//	textId = MyDrawEntity::DrawText(tmpPt, strJiedian, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
	//	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	//	m_idArrs.append(textId);
	//}
	//绘制桩号
	acutPolar(asDblArray(textPt), 3*PI/2, 3*m_dYScale, asDblArray(textPt));
	textId = MyDrawEntity::DrawText(textPt, strZhuanghao, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
	m_idArrs.append(textId);

	//是否有凸起
	if (m_pZDM.getHasBulge())
	{
		double dSJDmxS = m_pZDM.getDesignDmxS();
		double dXZDmxS = m_pZDM.getRealDmxS();
		CString strSJDmxS = MyTransFunc::doubleToStr(dSJDmxS, strTmp);
		CString strXZDmxS = MyTransFunc::doubleToStr(dXZDmxS, strTmp);
		acutPolar(asDblArray(m_basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(textPt));
		acutPolar(asDblArray(textPt), 3*PI/2, 0.75*m_dYScale, asDblArray(textPt));
		AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
		textId = MyDrawEntity::DrawText(textPt, strSJDmxS, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeRotation(textId, 3*PI/2);
		textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
		m_idArrs.append(textId);

		//绘制现状地面高
		acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(textPt));
		textId = MyDrawEntity::DrawText(textPt, strXZDmxS, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeRotation(textId, 3*PI/2);
		textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
		m_idArrs.append(textId);
	}

	return true;
}

bool CDrawZDM::DrawNextDMText()
{
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("DM-TEXT"), 7);

	CString strCount = m_pZDM.getCount();	
	int nCount = MyTransFunc::StringToInt(strCount);
	CBcUtils utls;
	CString strNext;
	strNext.Format(_T("%d"), nCount + 1);
	CString strNextLabel = BC_DICT + strNext;
	CZdmDataInfo NextData;
	bool bGetNextData = utls.get(strNextLabel, NextData);
	if (bGetNextData)
	{
		double dSJDmx = NextData.getDesignDmx();
		double dXZDmx = NextData.getRealDmx();
		double dCurData = NextData.getcurData();
		bool bDrawBc = CDMXUtils::getcreateBc();
		AcGePoint3d textPt;
		CString strTmp;
		CString strSJDmx = MyTransFunc::doubleToStr(dSJDmx, strTmp);
		CString strXZDmx = MyTransFunc::doubleToStr(dXZDmx, strTmp);

		CString strZhuanghao = MyTransFunc::doubleToStr(dCurData, strTmp);
		//绘制设计地面高文字
		acutPolar(asDblArray(m_basePt), 0, m_dLen + NextData.getcurData()*m_dXScale, asDblArray(textPt));
		acutPolar(asDblArray(textPt), 3*PI/2, 0.75*m_dYScale, asDblArray(textPt));
		AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
		textId = MyDrawEntity::DrawText(textPt, strSJDmx, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
		textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
		AddObjToDict(strNextLabel, textId);

		//绘制现状地面高
		acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(textPt));
		textId = MyDrawEntity::DrawText(textPt, strXZDmx, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
		textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
		AddObjToDict(strNextLabel, textId);
		//绘制桩号
		acutPolar(asDblArray(textPt), 3*PI/2, 3*m_dYScale, asDblArray(textPt));
		textId = MyDrawEntity::DrawText(textPt, strZhuanghao, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
		textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
		AddObjToDict(strNextLabel, textId);
		//是否有凸起
		if (NextData.getHasBulge())
		{
			double dSJDmxS = NextData.getDesignDmxS();
			double dXZDmxS = NextData.getRealDmxS();
			CString strSJDmxS = MyTransFunc::doubleToStr(dSJDmxS, strTmp);
			CString strXZDmxS = MyTransFunc::doubleToStr(dXZDmxS, strTmp);
			acutPolar(asDblArray(m_basePt), 0, m_dLen + NextData.getcurData()*m_dXScale, asDblArray(textPt));
			acutPolar(asDblArray(textPt), 3*PI/2, 0.75*m_dYScale, asDblArray(textPt));
			AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
			textId = MyDrawEntity::DrawText(textPt, strSJDmxS, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
			textId = MyEditEntity::openEntChangeRotation(textId, 3*PI/2);
			textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
			AddObjToDict(strNextLabel, textId);

			//绘制现状地面高
			acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(textPt));
			textId = MyDrawEntity::DrawText(textPt, strXZDmxS, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
			textId = MyEditEntity::openEntChangeRotation(textId, 3*PI/2);
			textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
			AddObjToDict(strNextLabel, textId);
		}
		return true;
	}

	return false;
}

bool CDrawZDM::DrawXMText()
{
	return true;
}


bool CDrawZDM::DrawJdText(AcGePoint3d basePt, AcGePoint3d TopPt, AcGePoint3d endPt)
{
	AcGePoint3d line1Pt,line2Pt,cenPt;
	double dRadius = 0.3*m_dYScale;	
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("ZX-TMP"), 7);

	acutPolar(asDblArray(basePt), 3*PI/2, 3.75*m_dYScale, asDblArray(cenPt));
	acutPolar(asDblArray(cenPt), PI/2, dRadius, asDblArray(line1Pt));
	acutPolar(asDblArray(cenPt), 3*PI/2, dRadius, asDblArray(line2Pt));
	AcDbObjectId line1Id,line2Id,cirId,textId;
	line1Id = MyDrawEntity::DrawLine(TopPt, line1Pt, ZxLayerId);
	line2Id = MyDrawEntity::DrawLine(line2Pt, endPt, ZxLayerId);

	cirId = MyDrawEntity::DrawCircle(cenPt, dRadius, ZxLayerId);
	CString strText;
	CString strTmp = m_pZDM.getJiedian();
	strText = _T("T") + strTmp;
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
	textId = MyDrawEntity::DrawText(cenPt, strText, 0.3*m_dYScale, textStyleId, AcDb::kTextMid, AcDb::kTextBase);
	textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
	m_idArrs.append(line1Id);
	m_idArrs.append(line2Id);
	m_idArrs.append(cirId);
	m_idArrs.append(textId);
	return true;
}

bool CDrawZDM::DrawSMXLine(bool bIsDeFault)
{
	//先使用设计水面高来绘制图形，后续不对再修改
	CString strCount = m_pZDM.getCount();	
	int nCount = MyTransFunc::StringToInt(strCount);
	
	CBcUtils utls;
	//////////////////////////////////////////////////////////////////////////
	//设计地面线
	AcDbObjectId xdmLayerId = MySymble::CreateNewLayer(_T("MQ-XDM"), 3);
	AcDbObjectId sdmLayerId = MySymble::CreateNewLayer(_T("MQ-SDM"), 4);
	double dDesignDmx = (m_pZDM.getDesignDmx() - CDMXUtils::getMinElavation())*m_dYScale;
	double dPreDesignDmx = (m_preData.getDesignDmx() - CDMXUtils::getMinElavation())*m_dYScale;
	
	double dRealDmx = (m_pZDM.getRealDmx() - CDMXUtils::getMinElavation())*m_dYScale;
	double dPreRealDmx = (m_preData.getRealDmx() - CDMXUtils::getMinElavation())*m_dYScale;

	AcGePoint3d startPt, endPt, tmpPt, preStartPt, preEndPt,endPtS, preEndPtS;	
	acutPolar(asDblArray(m_basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(tmpPt));
	acutPolar(asDblArray(tmpPt), PI/2, dDesignDmx, asDblArray(endPt));
	acutPolar(asDblArray(tmpPt), PI/2, dRealDmx, asDblArray(preEndPt));
	if (m_pZDM.getHasBulge())
	{										
		acutPolar(asDblArray(tmpPt), PI/2, (m_pZDM.getDesignDmxS() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(endPtS));
		acutPolar(asDblArray(tmpPt), PI/2, (m_pZDM.getRealDmxS() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(preEndPtS));
	}

	acutPolar(asDblArray(m_basePt), 0, m_dLen + m_preData.getcurData()*m_dXScale, asDblArray(tmpPt));
	if (m_preData.getHasBulge())
	{
		acutPolar(asDblArray(tmpPt), PI/2, (m_preData.getDesignDmxS() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(startPt));
		acutPolar(asDblArray(tmpPt), PI/2, (m_preData.getRealDmxS() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(preStartPt));
	}
	else
	{
		acutPolar(asDblArray(tmpPt), PI/2, dPreDesignDmx, asDblArray(startPt));
		acutPolar(asDblArray(tmpPt), PI/2, dPreRealDmx, asDblArray(preStartPt));
	}



	AcDbObjectId lineId = MyDrawEntity::DrawLine(startPt, endPt, sdmLayerId);
	m_idArrs.append(lineId);
	
	AcDbObjectId preLineId = MyDrawEntity::DrawLine(preStartPt, preEndPt, xdmLayerId);
	m_idArrs.append(preLineId);
	
	if (!bIsDeFault)//属于中间插入，会产生另外的2条水面线
	{
		CString strNext;
		strNext.Format(_T("%d"), nCount + 1);
		CString strNextLabel = BC_DICT + strNext;
		CZdmDataInfo NextData;
		bool bRet = utls.get(strNextLabel, NextData);
		if (bRet)
		{
			double dNextDesignDmx = (NextData.getDesignDmx() - CDMXUtils::getMinElavation())*m_dYScale;
			double dNextRealDmx = (NextData.getRealDmx() - CDMXUtils::getMinElavation())*m_dYScale;

			acutPolar(asDblArray(m_basePt), 0, m_dLen + NextData.getcurData()*m_dXScale, asDblArray(tmpPt));
			acutPolar(asDblArray(tmpPt), PI/2, dNextDesignDmx, asDblArray(startPt));
			acutPolar(asDblArray(tmpPt), PI/2, dNextRealDmx, asDblArray(preStartPt));
		
			AcDbObjectId nextsdmlineId = AcDbObjectId::kNull;
			AcDbObjectId nextxdmLineId = AcDbObjectId::kNull;

			if (m_pZDM.getHasBulge())
			{
				nextsdmlineId = MyDrawEntity::DrawLine(endPtS, startPt, sdmLayerId);
				nextxdmLineId = MyDrawEntity::DrawLine(preEndPtS, preStartPt, xdmLayerId);
			}
			else
			{
				nextsdmlineId = MyDrawEntity::DrawLine(endPt, startPt, sdmLayerId);
				nextxdmLineId = MyDrawEntity::DrawLine(preEndPt, preStartPt, xdmLayerId);
			}
			
			AddObjToDict(strNextLabel, nextsdmlineId);
			AddObjToDict(strNextLabel, nextxdmLineId);
		}		
	}
	return true;
}

bool CDrawZDM::AddObjToDict( CString strGroupName, AcDbObjectId objId )
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
	}
	pGroup->append(objId);
	pGroup->close();
	pGroupDict->close();

	return true;
}

bool CDrawZDM::ModifyDictbyInt( int nCur , bool bIsAdded)
{	
	int nCount = 1; 
	CString strCount = CDMXUtils::getNumCount();
	nCount = MyTransFunc::StringToInt(strCount);
	CString strGroupName,strPreGroupNam, strTmp;
	if (bIsAdded)
	{
		for (int i=nCount; i>nCur; i--)
		{
			strTmp.Format(_T("%d"), i);
			strGroupName = BC_DICT + strTmp;
			strTmp.Format(_T("%d"), i-1);
			strPreGroupNam = BC_DICT + strTmp;

			if (i == nCur+1)
			{
				ChangeDictName(strGroupName, strPreGroupNam, i, false);
			}
			else
			{
				ChangeDictName(strGroupName, strPreGroupNam, i);
			}

		}
	}
	else
	{
		for (int i=nCur; i<=nCount; i++)
		{
			strTmp.Format(_T("%d"), i);
			strGroupName = BC_DICT + strTmp;
			strTmp.Format(_T("%d"), i+1);
			strPreGroupNam = BC_DICT + strTmp;
			if ((i == nCur))
			{
				ChangeDictName(strGroupName, strPreGroupNam,  i, false);
			}
			else
			{
				ChangeDictName(strGroupName, strPreGroupNam,  i);
			}
		}
		
	}
	return true;
}

bool CDrawZDM::ChangeDictName( CString strGroupName, CString strPreGroupName, int nCount, bool bIsDeFault /*= true*/ )
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strPreGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
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
			CString strLayer = pEnt->layer();
			if ((strLayer.CompareNoCase(_T("MQ-XDM")) == 0)
				||(strLayer.CompareNoCase(_T( "MQ-SDM")) == 0)
				||(strLayer.CompareNoCase(_T( "HX-TMP")) == 0)
				||(strLayer.CompareNoCase(_T("DM-TEXT")) == 0))
			{
				if (!bIsDeFault)
				{
					pEnt->erase();
				}
			}
			else if (strLayer.CompareNoCase(_T("ZX-TMP")) == 0)//修改节点
			{
				if (pEnt->isKindOf(AcDbText::desc()))
				{
					CString strText;
					strText.Format(_T("T%d"), nCount);
					AcDbText* pText = AcDbText::cast(pEnt);
					pText->setTextString(strText);
					pText->close();
				}
			}
			pEnt->close();
		}
	}
	pGroup->erase();
	pGroup->close();
	pGroupDict->close();

	MyDrawEntity::MakeGroup(objIdArr, false, strGroupName);
	return true;
}


bool CDrawZDM::EditDict(int nCur)
{
	CString strTmp, strGroupName;
	strTmp.Format(_T("%d"),nCur+1);
	strGroupName = BC_DICT + strTmp;
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
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
			CString strLayer = pEnt->layer();
			if ((strLayer.CompareNoCase(_T("MQ-XDM")) == 0)
				||(strLayer.CompareNoCase(_T( "MQ-SDM")) == 0)
				||(strLayer.CompareNoCase(_T( "HX-TMP")) == 0))
			{
				pEnt->erase();
			}
			pEnt->close();
		}
	}
	pGroup->close();
	pGroupDict->close();
	return true;
}

CDrawGd::CDrawGd(void)
{
	initdata();
	m_bIsModified = false;
}

CDrawGd::~CDrawGd(void)
{

}

bool CDrawGd::add(CZdmDataInfo pZDM)
{
	m_pZDM = pZDM;
	drawGd();
	m_nCount++;
	CString strCount;
	strCount.Format(_T("%d"), m_nCount);
	CDMXUtils::SetCurNum(strCount);
	return true;
}

bool CDrawGd::del(CZdmDataInfo pZDM)
{
	m_pZDM = pZDM;
	CString strCurCount = m_pZDM.getCount();
	m_nCount = MyTransFunc::StringToInt(strCurCount);

	if (m_nCount > 1)
	{	
		ModifyDictbyInt(m_nCount, false);
	}
	CString strCount = CDMXUtils::getNumCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	if (m_nCount != nCount - 1)//如果不是最后一个桩号
	{
		drawGd();
	}
	nCount--;
	strCount.Format(_T("%d"), nCount);
	CDMXUtils::SetCurNum(strCount);
	return true;
}

bool CDrawGd::insert(CZdmDataInfo pZDM)
{
	m_pZDM = pZDM;
	m_bIsModified = true;
	CString strCount = m_pZDM.getCount();
	m_nCount = MyTransFunc::StringToInt(strCount);

	if (m_nCount > 1)
	{	
		ModifyDictbyInt(m_nCount);	
	}

	strCount = CDMXUtils::getNumCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	if (m_nCount != nCount - 1)//如果不是最后一个桩号
	{
		drawGd();
	}
	nCount++;
	strCount.Format(_T("%d"), nCount);
	CDMXUtils::SetCurNum(strCount);
	return true;
}

bool CDrawGd::mod(CZdmDataInfo pZDM)
{
	m_pZDM = pZDM;
	m_bIsModified = true;

	CString strCurCount = m_pZDM.getCount();
	m_nCount = MyTransFunc::StringToInt(strCurCount);
	CString strCount = CDMXUtils::getNumCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	if (m_nCount != nCount - 1)//如果不是最后一个
	{	
		EditDict(m_nCount);
	}
	drawGd();
	CString strGdGroupName;
	strGdGroupName = BC_DICT_GD + strCurCount;
	AcDbObjectId groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGdGroupName);
	return true;
}

//void CDrawGd::setData(CZdmDataInfo pData)
//{
//	m_pZDM.setLabel(pData.label());
//	m_pZDM.setCount(pData.getCount());
//	m_pZDM.setDesignDmx(pData.getDesignDmx());
//	m_pZDM.setRealDmx(pData.getRealDmx());
//	m_pZDM.setJiedian(pData.getJiedian());
//	m_pZDM.setGuanDi(pData.getGuanDi());
//	m_pZDM.setWaShen(pData.getWaShen());
//	m_pZDM.setPoDu(pData.getPoDu());
//	m_pZDM.setJuli(pData.getJuli());
//	m_pZDM.setcurData(pData.getcurData());
//	m_pZDM.setHasBulge(pData.getHasBulge());
//	m_pZDM.setDesingDmxS(pData.getDesignDmxS());
//	m_pZDM.setRealDmxS(pData.getRealDmxS());
//	m_pZDM.setPipeDiameter(pData.getPipeDiameter());
//}

bool CDrawGd::initdata()
{
	m_dXScale = 1000/(CDMXUtils::getXScale());
	m_dYScale = 1000/(CDMXUtils::getYScale());
	{
		m_dLen = m_dXScale*10;
	}
	m_dWidth = m_dYScale*16.7;
	m_bIsDel = false;
	m_basePt = CDMXUtils::getbasePt();
	return true;
}

bool CDrawGd::drawGd()
{
	CString strCount = m_pZDM.getCount();
	m_nCount = MyTransFunc::StringToInt(strCount);
	CString strGroupName, strLabel;
	strGroupName = BC_DICT_GD + strCount;
	strLabel = BC_DICT + strCount;

	if (m_nCount > 1)
	{
		CString strPreCount;

		strPreCount.Format(_T("%d"), m_nCount - 1);
		CString strLabel = BC_DICT + strPreCount;

		CBcUtils utls;
		utls.get(strLabel, m_preData);
		
	}
	drawCirlceOrEllipse();

	AcDbObjectId groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
	/*m_nCount++;
	strCount.Format(_T("%d"), m_nCount);
	CDMXUtils::SetCurNum(strCount);*/
	
	CBcUtils utls;
	utls.modify(strLabel, m_pZDM);

	return true;
}


AcDbObjectIdArray CDrawGd::drawGdflat(AcGePoint3d pretmpPt, AcGePoint3d tmpPt)
{
	AcGePoint3d startPt,endPt;
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();

	acutPolar(asDblArray(pretmpPt), 3*PI/2, 9.1*m_dYScale, asDblArray(startPt));
	acutPolar(asDblArray(tmpPt), 3*PI/2, 9.1*m_dYScale, asDblArray(endPt));
	AcDbObjectId RqLayerId = MySymble::CreateNewLayer(_T("燃气管道"), 1);
	AcDbObjectId objId = MyDrawEntity::DrawPlineByTwoPoint(startPt, endPt, RqLayerId);
	objId = MyEditEntity::openPlineChangeWidth(objId, m_pZDM.getPipeDiameter()/1000);
	objIdArr.append(objId);
	return objIdArr;
}

AcDbObjectIdArray CDrawGd::drawText(AcGePoint3d basePt)
{
	AcGePoint3d guandiPt,washenPt;
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();

	acutPolar(asDblArray(basePt), 3*PI/2, 11.45*m_dYScale, asDblArray(guandiPt));
	acutPolar(asDblArray(guandiPt), 3*PI/2, 1.5*m_dYScale, asDblArray(washenPt));
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("GD-TEXT"), 7);
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
	AcDbObjectId textId = AcDbObjectId::kNull;
	double dGuandi = m_pZDM.getGuanDi();
	double dWashen = m_pZDM.getWaShen();
	CString strTmp;
	CString strGuandiText = MyTransFunc::doubleToStr(dGuandi, strTmp);
	CString strWashen = MyTransFunc::doubleToStr(dWashen, strTmp);

	textId = MyDrawEntity::DrawText(guandiPt, strGuandiText, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
	objIdArr.append(textId);

	textId = MyDrawEntity::DrawText(washenPt, strWashen, 3*m_dYScale/10, textStyleId, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	textId = MyEditEntity::openEntChangeLayer(textId, ZxLayerId);
	objIdArr.append(textId);

	return objIdArr;
}

AcDbObjectIdArray CDrawGd::drawTextAndLine(AcGePoint3d pretmpPt, AcGePoint3d tmpPt, double dDist, double dPodu)
{
	AcGePoint3d startPt,endPt,pdPt,distPt;
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 6.0);
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("GD-TMP"), 7);

	CString strTmp;
	CString strDist = MyTransFunc::doubleToStr(dDist, strTmp);

	AcDbObjectId textId1,textId2;
	CString strPd = MyTransFunc::doubleToStr(dPodu, strTmp);
	strPd += _T("%");
	
	if (abs(dPodu) < 0.000001)
	{
		acutPolar(asDblArray(pretmpPt), 3*PI/2, 14.45*m_dYScale, asDblArray(startPt));
		acutPolar(asDblArray(tmpPt), 3*PI/2, 14.45*m_dYScale, asDblArray(endPt));

		textId1 = MyDrawEntity::DrawText(endPt, strDist, 3*m_dYScale/10, textStyleId, AcDb::kTextRight, AcDb::kTextBottom);
		textId1 = MyEditEntity::openEntChangeLayer(textId1, ZxLayerId);
		objIdArr.append(textId1);	
		textId2 = MyDrawEntity::DrawText(startPt, _T("0"), 3*m_dYScale/10, textStyleId, AcDb::kTextLeft, AcDb::kTextBottom);
		textId2 = MyEditEntity::openEntChangeLayer(textId2, ZxLayerId);
		objIdArr.append(textId2);	
	}
	else if (dPodu > 0.000001)
	{
		acutPolar(asDblArray(pretmpPt), 3*PI/2, 15.2*m_dYScale, asDblArray(startPt));
		acutPolar(asDblArray(tmpPt), 3*PI/2, 13.7*m_dYScale, asDblArray(endPt));

		textId1 = MyDrawEntity::DrawText(endPt, strDist, 3*m_dYScale/10, textStyleId, AcDb::kTextRight, AcDb::kTextTop);
		textId1 = MyEditEntity::openEntChangeLayer(textId1, ZxLayerId);
		objIdArr.append(textId1);	
		textId2 = MyDrawEntity::DrawText(startPt, strPd, 3*m_dYScale/10, textStyleId, AcDb::kTextLeft, AcDb::kTextBottom);
		textId2 = MyEditEntity::openEntChangeLayer(textId2, ZxLayerId);
		objIdArr.append(textId2);	
	}
	else
	{
		acutPolar(asDblArray(pretmpPt), 3*PI/2, 13.7*m_dYScale, asDblArray(startPt));
		acutPolar(asDblArray(tmpPt), 3*PI/2, 15.2*m_dYScale, asDblArray(endPt));

		textId1 = MyDrawEntity::DrawText(endPt, strDist, 3*m_dYScale/10, textStyleId, AcDb::kTextRight, AcDb::kTextBottom);
		textId1 = MyEditEntity::openEntChangeLayer(textId1, ZxLayerId);
		objIdArr.append(textId1);	
		textId2 = MyDrawEntity::DrawText(startPt, strPd, 3*m_dYScale/10, textStyleId, AcDb::kTextLeft, AcDb::kTextTop);
		textId2 = MyEditEntity::openEntChangeLayer(textId2, ZxLayerId);
		objIdArr.append(textId2);	
	}
	AcDbObjectId objId = AcDbObjectId::kNull;
	objId = MyDrawEntity::DrawLine(startPt, endPt, ZxLayerId);
	objIdArr.append(objId);
	//////////////////////////////////////////////////////////////////////////
	return objIdArr;
}

bool CDrawGd::drawCirlceOrEllipse()
{
	AcGePoint3d tmpPt,guandiPt,guandiTopPt,cenPt;
	double dRadius = (m_dYScale*m_pZDM.getPipeDiameter())/2000;
	acutPolar(asDblArray(m_basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(tmpPt));
	acutPolar(asDblArray(tmpPt), PI/2, (m_pZDM.getGuanDi() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(guandiPt));
	acutPolar(asDblArray(guandiPt), PI/2, 2*dRadius, asDblArray(guandiTopPt));
	acutPolar(asDblArray(guandiPt), PI/2, dRadius, asDblArray(cenPt));
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId ZxLayerId = MySymble::CreateNewLayer(_T("GD-TMP"), 7);
	AcDbObjectId RqLayerId = MySymble::CreateNewLayer(_T("燃气管道"), 1);
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();
	if (m_dXScale != m_dYScale)
	{
		//绘制椭圆
		double dRadio = m_dXScale/m_dYScale;
		AcGeVector3d unitVec;
		unitVec = guandiTopPt - cenPt;
		objId = MyDrawEntity::DrawEllipse(cenPt, AcGeVector3d::kZAxis, unitVec, dRadio);
		objId = MyEditEntity::openEntChangeLayer(objId, ZxLayerId);
		m_idArrs.append(objId);		
		//
	}
	else
	{
		//绘制圆
		objId = MyDrawEntity::DrawCircle(cenPt, dRadius, ZxLayerId);
		m_idArrs.append(objId);
	}
	//绘制管道线
	if (m_nCount > 1)
	{
		AcGePoint3d preguandiPt,preguandiTopPt,pretmpPt;
		acutPolar(asDblArray(m_basePt), 0, m_dLen + m_preData.getcurData()*m_dXScale, asDblArray(pretmpPt));
		acutPolar(asDblArray(pretmpPt), PI/2, (m_preData.getGuanDi() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(preguandiPt));
		acutPolar(asDblArray(preguandiPt), PI/2, 2*dRadius, asDblArray(preguandiTopPt));
		AcDbObjectId lineId1,lineId2;
		lineId1 = MyDrawEntity::DrawLine(preguandiPt, guandiPt, RqLayerId);
		lineId2 = MyDrawEntity::DrawLine(preguandiTopPt, guandiTopPt, RqLayerId);
		m_idArrs.append(lineId1);
		m_idArrs.append(lineId2);
		//绘制示意图
		objIdArr = drawGdflat(pretmpPt, tmpPt);
		for (int i=0; i<objIdArr.length(); i++)
		{
			m_idArrs.append(objIdArr.at(i));
		}
		double dDist = m_pZDM.getcurData() - m_preData.getcurData();
		double dPodu = 1000*(m_pZDM.getGuanDi() - m_preData.getGuanDi())/dDist;
		objIdArr = drawTextAndLine(pretmpPt, tmpPt, dDist, dPodu);
		for (int i=0; i<objIdArr.length(); i++)
		{
			m_idArrs.append(objIdArr.at(i));
		}
	}
	objIdArr = drawText(tmpPt);
	for (int i=0; i<objIdArr.length(); i++)
	{
		m_idArrs.append(objIdArr.at(i));
	}
	if (m_bIsModified)
	{
		CBcUtils utls;
		CString strNext;
		strNext.Format(_T("%d"), m_nCount + 1);
		CString strNextLabel = BC_DICT + strNext;
		CString strGdGroup = BC_DICT_GD + strNext;
		CZdmDataInfo NextData;
		bool bGetNextData = utls.get(strNextLabel, NextData);
		if (bGetNextData)
		{
			dRadius = (m_dYScale*NextData.getPipeDiameter())/2000;
			acutPolar(asDblArray(m_basePt), 0, m_dLen + NextData.getcurData()*m_dXScale, asDblArray(tmpPt));
			acutPolar(asDblArray(tmpPt), PI/2, (NextData.getGuanDi() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(guandiPt));
			acutPolar(asDblArray(guandiPt), PI/2, 2*dRadius, asDblArray(guandiTopPt));
			acutPolar(asDblArray(guandiPt), PI/2, dRadius, asDblArray(cenPt));
			if (m_dXScale != m_dYScale)
			{
				//绘制椭圆
				double dRadio = m_dXScale/m_dYScale;
				AcGeVector3d unitVec;
				unitVec = guandiTopPt - cenPt;
				objId = MyDrawEntity::DrawEllipse(cenPt, AcGeVector3d::kZAxis, unitVec, dRadio);
				objId = MyEditEntity::openEntChangeLayer(objId, ZxLayerId);
				MyEditEntity::AddObjToGroup(strGdGroup, objId);		
				//
			}
			else
			{
				//绘制圆
				objId = MyDrawEntity::DrawCircle(cenPt, dRadius, ZxLayerId);
				MyEditEntity::AddObjToGroup(strGdGroup, objId);
			}
			//绘制管道线
			if (m_nCount > 1)
			{
				AcGePoint3d preguandiPt,preguandiTopPt,pretmpPt;
				acutPolar(asDblArray(m_basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(pretmpPt));
				acutPolar(asDblArray(pretmpPt), PI/2, (m_pZDM.getGuanDi() - CDMXUtils::getMinElavation())*m_dYScale, asDblArray(preguandiPt));
				acutPolar(asDblArray(preguandiPt), PI/2, 2*dRadius, asDblArray(preguandiTopPt));
				AcDbObjectId lineId1,lineId2;
				lineId1 = MyDrawEntity::DrawLine(preguandiPt, guandiPt, RqLayerId);
				lineId2 = MyDrawEntity::DrawLine(preguandiTopPt, guandiTopPt, RqLayerId);
				MyEditEntity::AddObjToGroup(strGdGroup, lineId1);
				MyEditEntity::AddObjToGroup(strGdGroup, lineId2);
				//绘制示意图
				objIdArr = drawGdflat(pretmpPt, tmpPt);
				for (int i=0; i<objIdArr.length(); i++)
				{
					MyEditEntity::AddObjToGroup(strGdGroup, objIdArr.at(i));
				}
				double dDist = NextData.getcurData() - m_pZDM.getcurData();
				double dPodu = 1000*(NextData.getGuanDi() - m_pZDM.getGuanDi())/dDist;
				objIdArr = drawTextAndLine(pretmpPt, tmpPt, dDist, dPodu);
				for (int i=0; i<objIdArr.length(); i++)
				{
					MyEditEntity::AddObjToGroup(strGdGroup, objIdArr.at(i));
				}
			}
			/*objIdArr = drawText(tmpPt);
			for (int i=0; i<objIdArr.length(); i++)
			{
				MyEditEntity::AddObjToGroup(strGdGroup, objIdArr.at(i));
			}*/
		}
		
	}
	return true;
}

bool CDrawGd::ModifyDictbyInt( int nCur , bool bIsAdded)
{	
	int nCount = 1; 
	CString strCount = CDMXUtils::getNumCount();
	nCount = MyTransFunc::StringToInt(strCount);
	CString strGroupName,strPreGroupNam, strTmp;
	if (bIsAdded)
	{
		for (int i=nCount; i>nCur; i--)
		{
			strTmp.Format(_T("%d"), i);
			strGroupName = BC_DICT_GD + strTmp;
			strTmp.Format(_T("%d"), i-1);
			strPreGroupNam = BC_DICT_GD + strTmp;

			if (i == nCur+1)
			{
				ChangeDictName(strGroupName, strPreGroupNam, i, false);
			}
			else
			{
				ChangeDictName(strGroupName, strPreGroupNam, i);
			}

		}
	}
	else
	{
		for (int i=nCur; i<=nCount; i++)
		{
			strTmp.Format(_T("%d"), i);
			strGroupName = BC_DICT_GD + strTmp;
			strTmp.Format(_T("%d"), i+1);
			strPreGroupNam = BC_DICT_GD + strTmp;
			if ((i == nCur))
			{
				ChangeDictName(strGroupName, strPreGroupNam,  i, false);
			}
			else
			{
				ChangeDictName(strGroupName, strPreGroupNam,  i);
			}
		}

	}
	return true;
}

bool CDrawGd::ChangeDictName( CString strGroupName, CString strPreGroupName, int nCount, bool bIsDeFault /*= true*/ )
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strPreGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
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
			CString strLayer = pEnt->layer();
			if (/*(strLayer.CompareNoCase(_T("MQ-XDM")) == 0)
				||(strLayer.CompareNoCase(_T( "MQ-SDM")) == 0)
				||*/(strLayer.CompareNoCase(_T( "GD-TMP")) == 0)
				||(strLayer.CompareNoCase(_T("燃气管道")) == 0))
			{
				if (!bIsDeFault)
				{
					pEnt->erase();
				}
			}
			//else if (strLayer.CompareNoCase(_T("ZX-TMP")) == 0)//修改节点
			//{
			//	if (pEnt->isKindOf(AcDbText::desc()))
			//	{
			//		CString strText;
			//		strText.Format(_T("T%d"), nCount);
			//		AcDbText* pText = AcDbText::cast(pEnt);
			//		pText->setTextString(strText);
			//		pText->close();
			//	}
			//}
			pEnt->close();
		}
	}
	pGroup->erase();
	pGroup->close();
	pGroupDict->close();

	MyDrawEntity::MakeGroup(objIdArr, false, strGroupName);
	return true;
}

bool CDrawGd::EditDict(int nCur)
{

	CString strTmp, strGroupName;
	strTmp.Format(_T("%d"),nCur+1);
	strGroupName = BC_DICT_GD + strTmp;
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
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
			CString strLayer = pEnt->layer();
			if ((strLayer.CompareNoCase(_T("燃气管道")) == 0)
				||(strLayer.CompareNoCase(_T( "GD-TMP")) == 0))
			{
				pEnt->erase();
			}
			pEnt->close();
		}
	}
	pGroup->close();
	pGroupDict->close();
	return true;
}
