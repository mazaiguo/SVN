#include "StdAfx.h"
#include "CreateTuli.h"
#include "ArxDbgUtils.h"
#include "MyTransFunc.h"
#include "MyEditEntity.h"
#include "MyDrag.h"
#include "Global.h"
#include "MyDrawEntity.h"

CCreateTuli::CCreateTuli(void)
{
	//m_EntitySet.clear();
	Clear();
	m_dScale = 1.0;
	m_BlkHeight = 10;
	m_BlkLength = 20;
	ads_name_clear(m_ssname);
	acedSSAdd(NULL, NULL, m_ssname);
}

CCreateTuli::~CCreateTuli(void)
{
	//m_EntitySet.clear();
	Clear();
	ads_name_clear(m_ssname);
}

void CCreateTuli::doIt()
{
	bool bRet = AddEntity();
	if (bRet)
	{
		//MoveEntity();
		CCreateFrame cFrame;
		cFrame.SetData(m_EntitySet);
		bRet = cFrame.DoIt();
	}
	return;
}

bool CCreateTuli::AddEntity()
{
	AcGePoint3d minPt,maxPt;
	int nRet = acedGetPoint(NULL, _T("请选取区域"), asDblArray(minPt));
	if (nRet!=RTNORM)
	{
		return false;
	}

	nRet = acedGetCorner(asDblArray(minPt), _T("请选取区域"), asDblArray(maxPt));
	if (nRet!=RTNORM)
	{
		return false;
	}
	Acad::ErrorStatus es = Acad::eOk;
	ads_name ssname;
	ads_name_clear(ssname);
	struct resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"),-4,_T("and>"),RTNONE);
	nRet = acedSSGet(_T("C"), asDblArray(minPt), asDblArray(maxPt), NULL, ssname);
	if (nRet!=RTNORM)
	{
		return false;
	}

	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen<1L)
	{
		acedSSFree(ssname);
		return false;
	}
	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId;
	ads_name ename;
	ads_name_clear(ename);
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite)!=Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			CString strBlockName;
			ArxDbgUtils::symbolIdToName(pBlkRef->blockTableRecord(), strBlockName);
			AcDbObjectId entId;
			entId = pBlkRef->objectId();
			AcGeScale3d dScale = pBlkRef->scaleFactors();
			pBlkRef->setScaleFactors(AcGeScale3d(1.0,1.0,1.0));
			AcDbExtents exts;
			pBlkRef->getGeomExtents(exts);
			pBlkRef->setScaleFactors(dScale);
			pair<CBlkRefInFo*, int> entPair;
			int nCount = 1;
			CBlkRefInFo* pBlkInfo = new CBlkRefInFo();
			pBlkInfo->SetBlkId(entId);
			pBlkInfo->SetExts(exts);
			pBlkInfo->SetBasePt(pBlkRef->position());
			pBlkInfo->SetBlkScale(pBlkRef->scaleFactors().sx);
			entPair.first = pBlkInfo;
			entPair.second = nCount;
			pair<DataMap::iterator, bool> bRet = m_EntitySet.insert(DataMap::value_type(strBlockName, entPair));
			if (!bRet.second)
			{
				bRet.first->second.second++;
			}
			pBlkRef->close();
		}
		else
		{
			pEnt->close();
		}
	}
	acedSSFree(ssname);
	return true;
}

void CCreateTuli::MoveEntity()
{

}

void CCreateTuli::CloneAndMoveEntity(AcDbObjectId entId, AcGeMatrix3d m)
{
	AcDbEntity* ent = NULL;
	if (acdbOpenAcDbEntity(ent, entId, AcDb::kForRead)!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId ownerBlockId = ent->blockId();    // compiler complains!
	if (ArxDbgUtils::cloneAndXformObjects(acdbHostApplicationServices()->workingDatabase(), ent->objectId(),
		ownerBlockId, m, true) != Acad::eOk)
	{
		ArxDbgUtils::stopAlertBox(_T("ERROR: Couldn't clone entity!"));
	}
	ent->close();
	
}

double CCreateTuli::GetBlkLength() const
{
	return m_BlkLength;
}

double CCreateTuli::GetBlkHeight() const
{
	return m_BlkHeight;
}

//double CCreateTuli::GetTextLength() const
//{
//	return m_TextLength;
//}
//
//double CCreateTuli::GetTextHeight() const
//{
//	return m_TextHeight;
//}


void CCreateTuli::SetBlkLength(double dBlkLength)
{
	m_BlkLength = dBlkLength;
}

void CCreateTuli::SetBlkHeight(double dBlkHeight)
{
	m_BlkHeight = dBlkHeight;
}

//void CCreateTuli::SetTextLength(double dTextLength)
//{
//	m_TextLength = dTextLength;
//}
//
//void CCreateTuli::SetTextHeight(double dTextHeight)
//{
//	m_TextHeight = dTextHeight;
//}

bool CCreateTuli::ChangeBlkRef(AcDbObjectId objId)
{
	Acad::ErrorStatus es;
	AcDbBlockReference* pBlkRef = NULL;
	AcGeScale3d scale;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, objId, AcDb::kForWrite) != Acad::eOk)
	{
		return false;
	}
	es = pBlkRef->setRotation(0);
	scale = pBlkRef->scaleFactors();
	scale *= m_dScale;
	es = pBlkRef->setScaleFactors(scale);
	pBlkRef->close();

	ads_name ssname;
	acdbGetAdsName(ssname, objId);
	acedSSAdd(ssname, m_ssname, m_ssname);
	return true;
}

void CCreateTuli::CalcScale(AcDbObjectId objId)
{
	AcDbExtents exts = MyEditEntity::OpenObjAndGetExts(objId);
	double dHeight = abs(exts.maxPoint().y - exts.minPoint().y);
	double dLength = abs(exts.maxPoint().x - exts.minPoint().x);
	double dScale1,dScale2;
	dScale1 = dScale2 = 0.0;
	dScale1 = m_BlkHeight/dHeight;
	dScale2 = m_BlkHeight/dLength;
	if (dScale1 < dScale2)
	{
		m_dScale = dScale1;
	}
	else
	{
		m_dScale = dScale2;
	}
}


void CCreateTuli::Clear()
{
	CBlkRefInFo* pBlkInfo = NULL;
	for (DataMap::iterator iter = m_EntitySet.begin();
		iter != m_EntitySet.end();
		++iter)
	{
		pBlkInfo = iter->second.first;
		if (pBlkInfo != NULL)
		{
			delete pBlkInfo;
		}
	}
	m_EntitySet.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CBlkRefInFo::CBlkRefInFo(void)
{

}

CBlkRefInFo::~CBlkRefInFo()
{

}

//获取信息
//获取插入点坐标
AcGePoint3d CBlkRefInFo::GetBasePt() const
{
	return m_BasePt;
}
AcDbExtents CBlkRefInFo::GetExts() const
{
	return m_Exts;
}
AcDbObjectId CBlkRefInFo::GetBlkId() const
{
	return m_blkId;
}	
//LPCTSTR CBlkRefInFo::GetBlkName() const
//{
//	return m_strBlkName;
//}
double CBlkRefInFo::GetBlkScale() const
{
	return m_dScale;
}

double CBlkRefInFo::GetLength() const
{
	double dLength = abs(m_Exts.maxPoint().x - m_Exts.minPoint().x);
	return dLength;
}

double CBlkRefInFo::GetWidth() const
{
	double dHeight = abs(m_Exts.maxPoint().y - m_Exts.minPoint().y);
	return dHeight;
}

AcGePoint3d CBlkRefInFo::GetMidPt() const
{
	AcGePoint3d midPt;
	midPt.x = (m_Exts.minPoint().x + m_Exts.maxPoint().x)/2;
	midPt.y = (m_Exts.minPoint().y + m_Exts.maxPoint().y)/2;
	midPt.z = (m_Exts.minPoint().z + m_Exts.maxPoint().z)/2;
	return midPt;
}

//设置信息
void CBlkRefInFo::SetBasePt(AcGePoint3d basePt)
{
	m_BasePt = basePt;
}
void CBlkRefInFo::SetExts(AcDbExtents exts)
{
	m_Exts.set(exts.minPoint(), exts.maxPoint());
}
void CBlkRefInFo::SetBlkId(AcDbObjectId blkId)
{
	m_blkId = blkId;
}
//void CBlkRefInFo::SetBlkName(LPCTSTR strName)
//{
//	m_strBlkName = strName;
//}
void CBlkRefInFo::SetBlkScale(double dScale)
{
	m_dScale = dScale;
}

CBlkRefInFo&   CBlkRefInFo::operator = (const CBlkRefInFo& scl)
{
	//m_strBlkName = scl.GetBlkName();
	m_BasePt = scl.GetBasePt();
	m_blkId = scl.GetBlkId();
	m_dScale = scl.GetBlkScale();
	m_Exts = scl.GetExts();
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CCreateFrame::CCreateFrame(void)
{
	m_dXuhao = gGlobal.GetIniValue(_T("生成图例"), _T("序号长度"), 0);
	m_dTuli = gGlobal.GetIniValue(_T("生成图例"), _T("图例长度"), 0);
	m_dSBMC = gGlobal.GetIniValue(_T("生成图例"), _T("设备名称长度"), 0);
	m_dCount = gGlobal.GetIniValue(_T("生成图例"), _T("数量长度"), 0);
	m_dBeizhu = gGlobal.GetIniValue(_T("生成图例"), _T("备注长度"), 0);
	m_dTextHeight = gGlobal.GetIniValue(_T("生成图例"), _T("文字所在行高度"), 0);
	m_dHeight = gGlobal.GetIniValue(_T("生成图例"), _T("图例所在行高度"), 0);
	m_dTxtHeight = gGlobal.GetIniValue(_T("生成图例"), _T("文字高度"), 0);
	m_dNumTextHeight = gGlobal.GetIniValue(_T("生成图例"), _T("数字文字高度"), 0);
	acedSSAdd(NULL, NULL , m_ssname);
	m_IdArrs.removeAll();
}

CCreateFrame::~CCreateFrame(void)
{
	//Clear();
}

//获取数据
AcGePoint3d CCreateFrame::GetBasePt() const
{
	return m_basePt;
}

//设置数据
void CCreateFrame::SetBasePt(AcGePoint3d basePt)
{
	m_basePt = basePt;
}


//获取所有的块数据
DataMap CCreateFrame::GetData() const
{
	return m_Data;
}
//设置所有的块数据
void CCreateFrame::SetData(DataMap data)
{
	Clear();
	for (DataMap::iterator iter = data.begin();
		iter != data.end();
		++iter)
	{
		m_Data.insert(*iter);
	}
}

bool CCreateFrame::DoIt()
{
	bool bRet = true;
	bRet = DrawFrame();
	if (bRet)
	{
		MoveEntity();
	}
	return bRet;
}

//操作
//绘制外框
bool CCreateFrame::DrawFrame()
{
	if (m_Data.size() < 1)
	{
		return false;
	}
	int nSize = m_Data.size();
	AcDbObjectId line1,line2,line3,line4,line5,line6,line7,lineId;
	double dLength,dWidth;
	dLength = m_dXuhao + m_dTuli + m_dCount + m_dBeizhu;
	dWidth = m_dTextHeight + nSize*m_dHeight;

	AcGePoint3d startPt,endPt;
	startPt = m_basePt;
	endPt.x = startPt.x + dLength;
	endPt.y = startPt.y;
	endPt.z = startPt.z;

	//绘制外围的四根线
	line1 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line1);

	endPt.x = startPt.x;
	endPt.y = startPt.y + dWidth;
	endPt.z = startPt.z;
	line2 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line2);

	startPt.x = m_basePt.x + dLength;
	startPt.y = m_basePt.y;
	startPt.z = m_basePt.z;

	endPt.x = startPt.x;
	endPt.y = startPt.y + dWidth;
	endPt.z = startPt.z;
	line3 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line3);

	startPt.x = m_basePt.x;
	startPt.y = m_basePt.y + dWidth;
	startPt.z = m_basePt.z;

	endPt.x = startPt.x + dLength;
	endPt.y = startPt.y;
	endPt.z = startPt.z;
	line4 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line4);
	//绘制横着的线
	for (int i=0; i<nSize; i++)
	{
		acutPolar(asDblArray(m_basePt), PI/2, m_dTextHeight + i*m_dHeight, asDblArray(startPt));
		acutPolar(asDblArray(startPt), 0, dLength, asDblArray(endPt));
		lineId = MyDrawEntity::DrawLine(startPt, endPt);
		AppendToSS(lineId);
	}
	//绘制竖着的三根线
	acutPolar(asDblArray(m_basePt), 0, m_dXuhao, asDblArray(startPt));
	acutPolar(asDblArray(startPt), PI/2, dWidth, asDblArray(endPt));
	line5 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line5);

	acutPolar(asDblArray(m_basePt), 0, m_dXuhao + m_dTuli, asDblArray(startPt));
	acutPolar(asDblArray(startPt), PI/2, dWidth, asDblArray(endPt));
	line6 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line6);

	acutPolar(asDblArray(m_basePt), 0, m_dXuhao + m_dTuli + m_dCount, asDblArray(startPt));
	acutPolar(asDblArray(startPt), PI/2, dWidth, asDblArray(endPt));
	line7 = MyDrawEntity::DrawLine(startPt, endPt);
	AppendToSS(line7);

	//绘制文字
	DrawSpecialText(m_basePt, m_dXuhao, m_dTextHeight, _T("序号"));
	acutPolar(asDblArray(m_basePt), 0, m_dXuhao, asDblArray(startPt));
	DrawSpecialText(startPt, m_dTuli, m_dTextHeight, _T("图例"));
	acutPolar(asDblArray(m_basePt), 0, m_dXuhao + m_dTuli, asDblArray(startPt));
	DrawSpecialText(startPt, m_dCount, m_dTextHeight, _T("数量"));
	acutPolar(asDblArray(m_basePt), 0, m_dXuhao + m_dTuli + m_dCount, asDblArray(startPt));
	DrawSpecialText(startPt, m_dBeizhu, m_dTextHeight, _T("备注"));


	//绘制块参照
	int i=1;
	CString strText,strCount;
	for (DataMap::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		double dWid = m_dTextHeight + (i-1)*m_dHeight;
		double dHeight = m_dXuhao + m_dTuli;
		strText.Format(_T("%d"), i);
		acutPolar(asDblArray(m_basePt), PI/2, dWid, asDblArray(startPt));
		DrawSpecialText(startPt, m_dXuhao, m_dHeight, strText, true);

		strCount.Format(_T("%d"), iter->second.second);
		acutPolar(asDblArray(m_basePt), PI/2, dWid, asDblArray(startPt));
		acutPolar(asDblArray(startPt), 0, dHeight, asDblArray(startPt));
		DrawSpecialText(startPt, m_dCount, m_dHeight, strCount, true);
		
		acutPolar(asDblArray(m_basePt), PI/2, dWid, asDblArray(startPt));
		acutPolar(asDblArray(startPt), 0, m_dXuhao, asDblArray(startPt));
		DrawSpecialBlkRef(startPt, iter->second.first);
		i++;
	}
	return true;
}

void CCreateFrame::DrawSpecialText(AcGePoint3d basePt, double dLen, double dWidth, CString strText, bool bIsNum)
{
	AcGePoint3d midPt;
	midPt.x = basePt.x + dLen/2;
	midPt.y = basePt.y + dWidth/2;
	midPt.z = 0;
	AcDbObjectId textId = AcDbObjectId::kNull;
	if (bIsNum)
	{
		textId = MyDrawEntity::DrawText(midPt, strText, m_dNumTextHeight, 
			AcDbObjectId::kNull, AcDb::kTextCenter, AcDb::kTextVertMid);
	}
	else
	{
		textId = MyDrawEntity::DrawText(midPt, strText, m_dTxtHeight, 
			AcDbObjectId::kNull, AcDb::kTextCenter, AcDb::kTextVertMid);
	}
	
	AppendToSS(textId);
}

void CCreateFrame::DrawSpecialBlkRef(AcGePoint3d basePt, CBlkRefInFo* blkInfo)
{
	AcGePoint3d midPt;
	midPt.x = basePt.x + m_dTuli/2;
	midPt.y = basePt.y + m_dHeight/2;
	midPt.z = basePt.z;
	AcDbObjectId blkId = blkInfo->GetBlkId();
	double dHeight = blkInfo->GetWidth();
	double dLength = blkInfo->GetLength();
	double dScale1,dScale2;
	dScale1 = dScale2 = 0.0;
	dScale1 = m_dHeight/dHeight;
	dScale2 = m_dTuli/dLength;
	if (dScale1 < dScale2)
	{
		m_dScale = dScale1;
	}
	else
	{
		m_dScale = dScale2;
	}

	m_dScale *= 0.9;//使其比填满小一点

	AcDbObjectId objId = AcDbObjectId::kNull;

	AcDbEntity* ent = NULL;
	if (acdbOpenAcDbEntity(ent, blkId, AcDb::kForRead)!=Acad::eOk)
	{
		return;
	}
	AcDbObjectId ownerBlockId = ent->blockId();    // compiler complains!
	ent->close();

	AcDbDatabase* db = acdbCurDwg();
	AcDbIdMapping idMap;
	AcDbObjectIdArray entsToClone;
	entsToClone.append(blkId);
	Acad::ErrorStatus es = db->deepCloneObjects(
		const_cast<AcDbObjectIdArray&>(entsToClone),
		const_cast<AcDbObjectId&>(ownerBlockId), idMap);

	if (es != Acad::eOk)
	{
		ArxDbgUtils::rxErrorMsg(es);
		return;
	}

	AcDbEntity* clonedEnt;
	AcDbIdPair idPair;
	AcDbIdMappingIter iter(idMap);
	for (iter.start(); !iter.done(); iter.next())
	{
		if (!iter.getMap(idPair))
			return;

		if (idPair.isCloned()) 
		{
			es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
			if (es == Acad::eOk)
			{
				clonedEnt->close();
			}
			else
				ArxDbgUtils::rxErrorMsg(es);
			objId = clonedEnt->objectId();
		}
	}
	if (objId.isNull())
	{
		return;
	}
	
	
	AcGeVector3d transVec;
	AcGeMatrix3d xform;
	xform.setToIdentity();
	if (acdbOpenAcDbEntity(ent, objId, AcDb::kForWrite)!=Acad::eOk)
	{
		return;
	}
	if (ent->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(ent);
		pBlkRef->setScaleFactors(AcGeScale3d(m_dScale, m_dScale, m_dScale));
		AcDbExtents exts;
		es = pBlkRef->getGeomExtents(exts);
		AcGePoint3d tmpMidPt;
		tmpMidPt.x = (exts.minPoint().x + exts.maxPoint().x)/2;
		tmpMidPt.y = (exts.minPoint().y + exts.maxPoint().y)/2;
		tmpMidPt.z = (exts.minPoint().z + exts.maxPoint().z)/2;
		transVec = midPt - tmpMidPt;
		xform.setToTranslation(transVec);
		es = pBlkRef->transformBy(xform);
		pBlkRef->close();
	}
	else
	{
		ent->close();
	}
	objId = ent->objectId();
	AppendToSS(objId);
}

void CCreateFrame::Clear()
{
	CBlkRefInFo* pBlkInfo = NULL;
	for (DataMap::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		pBlkInfo = iter->second.first;
		if (pBlkInfo != NULL)
		{
			delete pBlkInfo;
		}
	}
	m_Data.clear();
}

void CCreateFrame::AppendToSS(AcDbObjectId objId)
{
	if (objId.isNull())
	{
		return;
	}
	ads_name ename;
	Acad::ErrorStatus es;
	es = acdbGetAdsName(ename, objId);
	if (es != Acad::eOk)
	{
		return;
	}
	acedSSAdd(ename, m_ssname, m_ssname);
	m_IdArrs.append(objId);
}


void CCreateFrame::MoveEntity()
{
	int nCurMode = 0;
	gGlobal.GetVar(_T("OSMODE"), &nCurMode);
	gGlobal.SetVar(_T("OSMODE"), 0);
	int nRet = MyDrag::Draggen(_T("\n指定插入点:"), m_ssname, asDblArray(m_basePt), asDblArray(m_basePt), _T(""), -1);
	if (nRet == -1)
	{
		acedSSFree(m_ssname);
		gGlobal.SetVar(_T("OSMODE"), nCurMode);
		return;
	}
	gGlobal.SetVar(_T("OSMODE"), nCurMode);
	acedSSFree(m_ssname);
}

