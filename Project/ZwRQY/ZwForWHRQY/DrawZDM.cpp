#include "StdAfx.h"
#include "DrawZDM.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDrawZDM::CDrawZDM(void )
{
	//m_pZDM = new CBiaochiForRQY();
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
	m_pZDM.setDesingDmxS(pData->getDesingDmxS());
	m_pZDM.setRealDmxS(pData->getRealDmxS());
	m_pZDM.setJiedianS(pData->getJiedianS());
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


AcDbObjectId CDrawZDM::Draw()
{
	AcDbObjectId groupId = AcDbObjectId::kNull;
	initdata();

	DrawLine();

	DrawDMText();

	CString strCount = m_pZDM.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	if (nCount > 1)
	{
		DrawSMXLine();
	}

	CString strGroupName;
	strGroupName = BC_DICT + strCount;
	groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);
	
	nCount++;
	strCount.Format(_T("%d"), nCount);
	CUtils::setNumCount(strCount);
	CUtils::SetCurNum(strCount);
	
	//////////////////////////////////////////////////////////////////////////
	//将数据添加到图纸中
	CBcUtils bcUtils;
	bcUtils.add(strGroupName, m_pZDM);
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
	m_pZDM.setDesingDmxS(pData->getDesingDmxS());
	m_pZDM.setRealDmxS(pData->getRealDmxS());
	m_pZDM.setJiedianS(pData->getJiedianS());
	m_idArrs.removeAll();
}

AcDbObjectId CDrawZDM::insert()
{	
	AcDbObjectId groupId = AcDbObjectId::kNull;
	initdata();

	DrawLine();

	DrawDMText();

	CString strCount = m_pZDM.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	if (nCount > 1)
	{	
		ModifyDictbyInt(nCount);
		DrawSMXLine(false);
	}

	CString strGroupName;
	strGroupName = BC_DICT + strCount;
	groupId = MyDrawEntity::MakeGroup(m_idArrs, false, strGroupName);

	nCount++;
	strCount.Format(_T("%d"), nCount);
	CUtils::setNumCount(strCount);
	CUtils::SetCurNum(strCount);

	//////////////////////////////////////////////////////////////////////////
	//将数据添加到图纸中
	CBcUtils bcUtils;
	bcUtils.insert(strGroupName, m_pZDM);
	return groupId;
}

bool CDrawZDM::initdata()
{
	m_dXScale = 1000/(CUtils::getXScale());
	m_dYScale = 1000/(CUtils::getYScale());
	CString strCount = m_pZDM.getCount();
	int nCount = MyTransFunc::StringToInt(strCount);
	//if (nCount == 1)
	{
		m_dLen = m_dXScale*10;
	}

	m_dWidth = m_dYScale*15.2;
	return true;
}

bool CDrawZDM::DrawLine()
{
	//AcGePoint3d basePt = 
	AcGePoint3d basePt = CUtils::getbasePt();
	AcGePoint3d endPt;

	AcDbObjectId lineId = AcDbObjectId::kNull;

	//绘制横着的线
	for (int i=0; i<10; i++)
	{
		AcGePoint3d tmpPt;
		if (i >= 5)
		{
			acutPolar(asDblArray(basePt), 3*PI/2, (1.5*i + 1.7)*m_dYScale, asDblArray(tmpPt));
		}
		else
		{
			acutPolar(asDblArray(basePt), 3*PI/2, i*1.5*m_dYScale, asDblArray(tmpPt));
		}
		acutPolar(asDblArray(tmpPt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(endPt));
		lineId = MyDrawEntity::DrawLine(tmpPt, endPt);
		m_idArrs.append(lineId);
	}
	//先使用设计水面高来绘制图形，后续不对再修改
	CString strCount = m_pZDM.getCount();	
	int nCount = MyTransFunc::StringToInt(strCount);
	acutPolar(asDblArray(basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(basePt));
	double dDesignDmx = (m_pZDM.getDesignDmx() - CUtils::getMinElavation())*m_dYScale;
	AcGePoint3d tmpPt;
	acutPolar(asDblArray(basePt), PI/2, dDesignDmx, asDblArray(tmpPt));

	if (nCount == 0)
	{
		acutPolar(asDblArray(basePt), 3*PI/2, m_dWidth, asDblArray(endPt));
	}
	else
	{
		acutPolar(asDblArray(basePt), 3*PI/2, m_dWidth-1.5*m_dYScale, asDblArray(endPt));
	}
	lineId = MyDrawEntity::DrawLine(tmpPt, endPt);
	m_idArrs.append(lineId);

	return true;
}

bool CDrawZDM::DrawDMText()
{
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcGePoint3d basePt = CUtils::getbasePt();
	double dSJDmx = m_pZDM.getDesignDmx();
	double dXZDmx = m_pZDM.getRealDmx();
	double dCurData = m_pZDM.getcurData();
	bool bDrawBc = CUtils::getcreateBc();
	AcGePoint3d textPt;
	CString strTmp;
	CString strSJDmx = MyTransFunc::doubleToStr(dSJDmx, strTmp);
	CString strXZDmx = MyTransFunc::doubleToStr(dXZDmx, strTmp);
	
	CString strZhuanghao = MyTransFunc::doubleToStr(dCurData, strTmp);
	//绘制设计地面高文字
	acutPolar(asDblArray(basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(textPt));
	acutPolar(asDblArray(textPt), 3*PI/2, 0.75*m_dYScale, asDblArray(textPt));
	textId = MyDrawEntity::DrawText(textPt, strSJDmx, 3*m_dYScale/10, AcDbObjectId::kNull, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	m_idArrs.append(textId);
	
	//绘制现状地面高
	acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(textPt));
	textId = MyDrawEntity::DrawText(textPt, strXZDmx, 3*m_dYScale/10, AcDbObjectId::kNull, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	m_idArrs.append(textId);

	//绘制节点号
	if (bDrawBc)
	{
		CString strJiedian = m_pZDM.getJiedian();
		AcGePoint3d tmpPt;
		acutPolar(asDblArray(textPt), 3*PI/2, 1.5*m_dYScale, asDblArray(tmpPt));
		textId = MyDrawEntity::DrawText(tmpPt, strJiedian, 3*m_dYScale/10, AcDbObjectId::kNull, AcDb::kTextCenter);
		textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
		m_idArrs.append(textId);
	}
	//绘制桩号
	acutPolar(asDblArray(textPt), 3*PI/2, 3*m_dYScale, asDblArray(textPt));
	textId = MyDrawEntity::DrawText(textPt, strZhuanghao, 3*m_dYScale/10, AcDbObjectId::kNull, AcDb::kTextCenter);
	textId = MyEditEntity::openEntChangeRotation(textId, PI/2);
	m_idArrs.append(textId);

	//是否有凸起
	if (m_pZDM.getHasBulge())
	{

	}

	return true;
}

bool CDrawZDM::DrawXMText()
{
	return true;
}

bool CDrawZDM::DrawSMXLine(bool bIsDeFault)
{
	//先使用设计水面高来绘制图形，后续不对再修改
	AcGePoint3d basePt = CUtils::getbasePt();
	CString strCount = m_pZDM.getCount();	
	int nCount = MyTransFunc::StringToInt(strCount);
	CString strPreCount;
	strPreCount.Format(_T("%d"), nCount - 1);
	CString strLabel = BC_DICT + strPreCount;
	CBcUtils utls;
	CZdmDataInfo preData;
	preData = utls.get(strLabel);

	//////////////////////////////////////////////////////////////////////////
	//设计地面线
	AcDbObjectId xdmLayerId = MySymble::CreateNewLayer(_T("MQ-XDM"), 3);
	AcDbObjectId sdmLayerId = MySymble::CreateNewLayer(_T("MQ-SDM"), 4);
	double dDesignDmx = (m_pZDM.getDesignDmx() - CUtils::getMinElavation())*m_dYScale;
	double dPreDesignDmx = (preData.getDesignDmx() - CUtils::getMinElavation())*m_dYScale;
	
	double dRealDmx = (m_pZDM.getRealDmx() - CUtils::getMinElavation())*m_dYScale;
	double dPreRealDmx = (preData.getRealDmx() - CUtils::getMinElavation())*m_dYScale;

	AcGePoint3d startPt, endPt, tmpPt, preStartPt, preEndPt;	
	acutPolar(asDblArray(basePt), 0, m_dLen + m_pZDM.getcurData()*m_dXScale, asDblArray(tmpPt));
	acutPolar(asDblArray(tmpPt), PI/2, dDesignDmx, asDblArray(endPt));
	acutPolar(asDblArray(tmpPt), PI/2, dRealDmx, asDblArray(preEndPt));

	acutPolar(asDblArray(basePt), 0, m_dLen + preData.getcurData()*m_dXScale, asDblArray(tmpPt));
	acutPolar(asDblArray(tmpPt), PI/2, dPreDesignDmx, asDblArray(startPt));
	acutPolar(asDblArray(tmpPt), PI/2, dPreRealDmx, asDblArray(preStartPt));


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
		NextData = utls.get(strNextLabel);
		double dNextDesignDmx = (NextData.getDesignDmx() - CUtils::getMinElavation())*m_dYScale;
		double dNextRealDmx = (NextData.getRealDmx() - CUtils::getMinElavation())*m_dYScale;

		acutPolar(asDblArray(basePt), 0, m_dLen + NextData.getcurData()*m_dXScale, asDblArray(tmpPt));
		acutPolar(asDblArray(tmpPt), PI/2, dNextDesignDmx, asDblArray(startPt));
		acutPolar(asDblArray(tmpPt), PI/2, dNextRealDmx, asDblArray(preStartPt));
		
		AcDbObjectId nextsdmlineId = MyDrawEntity::DrawLine(endPt, startPt, sdmLayerId);
		AcDbObjectId nextxdmLineId = MyDrawEntity::DrawLine(preEndPt, preStartPt, xdmLayerId);
		AddObjToDict(strNextLabel, nextsdmlineId);
		AddObjToDict(strNextLabel, nextxdmLineId);
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

bool CDrawZDM::ModifyDictbyInt( int nCur )
{	
	int nCount = 1; 
	CString strCount = CUtils::getNumCount();
	nCount = MyTransFunc::StringToInt(strCount);
	CString strGroupName,strPreGroupNam, strTmp;
	for (int i=nCount; i>=nCur; i--)
	{
		strTmp.Format(_T("%d"), nCount);
		strGroupName = BC_DICT + strTmp;
		strTmp.Format(_T("%d"), nCount-1);
		strPreGroupNam = BC_DICT + strTmp;
		if (i == nCur)
		{
			ChangeDictName(strGroupName, strPreGroupNam, false);
		}
		else
		{
			ChangeDictName(strGroupName, strPreGroupNam);
		}
	}
	return true;
}

bool CDrawZDM::ChangeDictName( CString strGroupName, CString strPreGroupName, bool bIsDeFault /*= true*/ )
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
	pGroup->allEntityIds(objIdArr);
	if (!bIsDeFault)
	{
		AcDbEntity* pEnt = NULL;
		AcDbObjectId objId = AcDbObjectId::kNull;
		int nLength = 0;
		nLength = pGroup->allEntityIds(objIdArr);
		for (int i=0; i<objIdArr.length(); i++)
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
					||(strLayer.CompareNoCase(_T( "MQ-SDM")) == 0))
				{
					pEnt->erase();
				}
				pEnt->close();
			}
		}
	}
	pGroup->erase();
	pGroup->close();
	pGroupDict->close();
	
	
	
	MyDrawEntity::MakeGroup(objIdArr, false, strGroupName);
	return true;
}
