#include "StdAfx.h"
#include "CutEntity.h"
#include "MyEditEntity.h"
#include "MyTransFunc.h"
#include "MyDrawEntity.h"
#include "map"
#include "ArxDbgUtils.h"

CCutEntity::CCutEntity(void)
{
	m_IdArrs.removeAll();
	m_tmpIdArrs.removeAll();
}

CCutEntity::~CCutEntity(void)
{
	m_IdArrs.removeAll();
	m_tmpIdArrs.removeAll();
}


void CCutEntity::doIt()
{
	bool bIsRet = AddRegion();
	if (!bIsRet)
	{
		return;
	}
	/*if (m_IdArrs.length()>0)
	{
		move();
	}*/
}

bool CCutEntity::AddRegion()
{
	int nRet = acedGetPoint(NULL, _T("请选取区域"), asDblArray(m_minPt));
	if (nRet!=RTNORM)
	{
		return false;
	}

	nRet = acedGetCorner(asDblArray(m_minPt), _T("请选取区域"), asDblArray(m_maxPt));
	if (nRet!=RTNORM)
	{
		return false;
	}
	Acad::ErrorStatus es = Acad::eOk;

	ads_name ss;
	nRet = acedSSGet(_T("C"), asDblArray(m_minPt), asDblArray(m_maxPt), NULL, ss);
	if (RTNORM != nRet)
		return false;
	long nLen;
	acedSSLength(ss, &nLen);
	if (nLen<1L)
	{
		acedSSFree(ss);
		return false;
	}
	//创建临时的多段线，后面要删掉
	AcDbObjectId tmpPlineId = AcDbObjectId::kNull;
	AcGePoint3dArray plinePoints;
	plinePoints.removeAll();
	plinePoints = MyTransFunc::OperateTwoPointsAndGetPoints(m_minPt, m_maxPt);
	tmpPlineId = MyDrawEntity::DrawPlineByPoints(plinePoints);
	AcDbPolyline* pPolyline = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pPolyline, tmpPlineId, AcDb::kForWrite)!=Acad::eOk)
	{
		pPolyline->close();
		return false;
	}
	//将角点坐下变换
	AcDbExtents exts;
	pPolyline->getGeomExtents(exts);
	m_minPt = exts.minPoint();
	m_maxPt = exts.maxPoint();
	////////////////////////////////////////////
	ads_name ename;
	ads_name_clear(ename);
	AcDbObjectId objId;
	AcDbEntity* pEnt = NULL;
	AcDbVoidPtrArray entitySet;
	entitySet.removeAll();
	for (int i=0; i<nLen; i++)
	{
		acedSSName(ss, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite)!=Acad::eOk)
		{
			pEnt = NULL;
			continue;
		}
		if (pEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pLine = AcDbLine::cast(pEnt);
			OperateLine(pPolyline, pLine);
		}
		else if (pEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc* pArc = AcDbArc::cast(pEnt);
			OperateArc(pPolyline, pArc);
		}
		else if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle* pCircle = AcDbCircle::cast(pEnt);
			OperateCircle(pPolyline, pCircle);
		}
		else if (pEnt->isKindOf(AcDbEllipse::desc()))
		{
			AcDbEllipse* pEllipse = AcDbEllipse::cast(pEnt);
			OperateEllipse(pPolyline, pEllipse);
		}
		else if (pEnt->isKindOf(AcDbSpline::desc()))
		{
			AcDbSpline* pSpline = AcDbSpline::cast(pEnt);
			OperateSpline(pPolyline, pSpline);
		}
		else if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pOperatePolyline = AcDbPolyline::cast(pEnt);
			OperatePolyline(pPolyline, pOperatePolyline);
		}
		else if (pEnt->isKindOf(AcDb2dPolyline::desc()))
		{
			AcDb2dPolyline* p2dOperatePolyline = AcDb2dPolyline::cast(pEnt);
			Operate2dPolyline(pPolyline, p2dOperatePolyline);
		}
		else if (pEnt->isKindOf(AcDb3dPolyline::desc()))
		{
			AcDb3dPolyline* p3dOperatePolyline = AcDb3dPolyline::cast(pEnt);
			Operate3dPolyline(pPolyline, p3dOperatePolyline);
		}
		else
		{
			//m_IdArrs.append(pEnt->objectId());	
			pEnt->close();
		}
	}
	pPolyline->erase();
	pPolyline->close();	
	acedSSFree(ss);
	return true;
}

void CCutEntity::OperateArc(AcDbPolyline* pPolyline, AcDbArc* pArc)
{
	Acad::ErrorStatus es;
	AcGePoint3dArray points;
	points.removeAll();
	es = pPolyline->intersectWith(pArc, AcDb::kOnBothOperands, points);
	if (es!=Acad::eOk)
	{
		pArc->close();
		return;
	}
	else
	{
		AcGePoint3d cenPt;
		if ((points.length()<1L))
		{
			AcGePoint3d startPt;
			pArc->getStartPoint(startPt);
			BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(startPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
			if (bIsInArea)
			{
				//m_IdArrs.append(pArc->objectId());
			}
			pArc->close();
			return;
		}
		else if (points.length()==1L)
		{
			cenPt = pArc->center();
			AcGePoint3d intersectPt, startPt, endPt;
			AcDbObjectId tmpEntId;
			intersectPt = points.at(0);
			pArc->getStartPoint(startPt);
			pArc->getEndPoint(endPt);
			double dRadius = pArc->radius();
			BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(startPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
			if (bIsInArea)	
			{
				tmpEntId = MyDrawEntity::DrawArc(cenPt, dRadius, startPt, intersectPt);
				tmpEntId = SetPropertiesFromEnt(pArc, tmpEntId);
			}
			else
			{
				tmpEntId = MyDrawEntity::DrawArc(cenPt, dRadius, intersectPt, endPt);
				tmpEntId = SetPropertiesFromEnt(pArc, tmpEntId);
			}
			//m_IdArrs.append(tmpEntId);
			m_tmpIdArrs.append(tmpEntId);
		}
		else if (points.length() == 2L)
		{
			cenPt = pArc->center();
			AcGePoint3d intersectPt1, intersectPt2, midPt;
			intersectPt1 = points.at(0);
			intersectPt2 = points.at(1);
			double dRadius = pArc->radius();
			double dDist1, dDist2;
			pArc->getDistAtPoint(intersectPt1, dDist1);
			pArc->getDistAtPoint(intersectPt2, dDist2);
			pArc->getPointAtDist((dDist1+dDist2)/2, midPt);
			BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(midPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
			if (bIsInArea)
			{
				AcDbObjectId tmpEntId;
				if (dDist1>dDist2)
				{
					tmpEntId = MyDrawEntity::DrawArc(cenPt, dRadius, intersectPt2, intersectPt1);
					tmpEntId = SetPropertiesFromEnt(pArc, tmpEntId);
				}
				else
				{
					tmpEntId = MyDrawEntity::DrawArc(cenPt, dRadius, intersectPt1, intersectPt2);
					tmpEntId = SetPropertiesFromEnt(pArc, tmpEntId);
				}
				//m_IdArrs.append(tmpEntId);
				m_tmpIdArrs.append(tmpEntId);
			}
		}
		pArc->close();
	}
	return;
}

void CCutEntity::OperateLine(AcDbPolyline* pPolyline, AcDbLine* pLine)
{
	Acad::ErrorStatus es;
	AcGePoint3dArray points;
	points.removeAll();
	es = pPolyline->intersectWith(pLine, AcDb::kOnBothOperands, points);
	if (es!=Acad::eOk)
	{
		pLine->close();
		return;
	}
	else
	{
		if ((points.length()<1L))
		{
			AcGePoint3d startPt;
			pLine->getStartPoint(startPt);
			BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(startPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
			if (bIsInArea)
			{
				//m_IdArrs.append(pLine->objectId());
			}
			pLine->close();
			return;
		}
		AcGePoint3d startPt,endPt;
		pLine->getStartPoint(startPt);
		pLine->getEndPoint(endPt);
		AcDbObjectId tmpEntId;
		if (points.length()==1L)
		{
			AcGePoint3d intersectPt = points.at(0);
			BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(startPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
			if (bIsInArea)
			{
				tmpEntId = MyDrawEntity::DrawLine(intersectPt, startPt);
				tmpEntId = SetPropertiesFromEnt(pLine, tmpEntId);
				//m_IdArrs.append(tmpEntId);
				m_tmpIdArrs.append(tmpEntId);
			}
			else
			{
				tmpEntId = MyDrawEntity::DrawLine(intersectPt, endPt);
				tmpEntId = SetPropertiesFromEnt(pLine, tmpEntId);
				//m_IdArrs.append(tmpEntId);
				m_tmpIdArrs.append(tmpEntId);
			}
		}
		else
		{
			AcGePoint3d intersectPt1, intersectPt2;
			intersectPt1 = points.at(0);
			intersectPt2 = points.at(1);
			tmpEntId = MyDrawEntity::DrawLine(intersectPt1, intersectPt2);
			tmpEntId = SetPropertiesFromEnt(pLine, tmpEntId);
			//m_IdArrs.append(tmpEntId);
			m_tmpIdArrs.append(tmpEntId);
		}
		pLine->close();
	}
	return;
}

void CCutEntity::OperateCircle(AcDbPolyline* pPolyline, AcDbCircle* pCircle)
{
	Acad::ErrorStatus es;
	AcGePoint3dArray points;
	points.removeAll();
	es = pPolyline->intersectWith(pCircle, AcDb::kOnBothOperands, points);
	if (es!=Acad::eOk)
	{
		pCircle->close();
		return;
	}
	else
	{
		AcGePoint3d cenPt;
		if ((points.length()<=1L))
		{
			cenPt = pCircle->center();
			BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(cenPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
			if (bIsInArea)
			{
				//m_IdArrs.append(pCircle->objectId());
			}
			pCircle->close();
			return;
		}
		else
		{
			double dRadius = pCircle->radius();
			cenPt = pCircle->center();
			sortPtArrayByCurve((AcDbCurve*)pCircle, points);
			for (int i=0; i<points.length(); i++)
			{
				if (i==points.length()-1)
				{
					double dStartAng,dEndAng;
					dStartAng = acutAngle(asDblArray(cenPt), asDblArray(points.at(i)));
					dEndAng = acutAngle(asDblArray(cenPt), asDblArray(points.at(0)));
					AcDbArc* pNewArc = new AcDbArc;
					pNewArc->setCenter(cenPt);
					pNewArc->setRadius(dRadius);
					pNewArc->setStartAngle(dStartAng);
					pNewArc->setEndAngle(dEndAng);
					ArxDbgUtils::addToCurrentSpace(pNewArc);
					OperateArc(pPolyline, pNewArc);
					pNewArc->close();
					m_tmpIdArrs.append(pNewArc->objectId());
				}
				else
				{
					double dStartAng,dEndAng;
					dStartAng = acutAngle(asDblArray(cenPt), asDblArray(points.at(i)));
					dEndAng = acutAngle(asDblArray(cenPt), asDblArray(points.at(i+1)));
					AcDbArc* pNewArc = new AcDbArc;
					pNewArc->setCenter(cenPt);
					pNewArc->setRadius(dRadius);
					pNewArc->setStartAngle(dStartAng);
					pNewArc->setEndAngle(dEndAng);
					ArxDbgUtils::addToCurrentSpace(pNewArc);
					OperateArc(pPolyline, pNewArc);
					pNewArc->close();
					m_tmpIdArrs.append(pNewArc->objectId());
				}
			}
		}
		pCircle->close();
	}
	return;
}

void CCutEntity::OperateEllipse(AcDbPolyline* pPolyline, AcDbEllipse* pEllipse)
{
	Acad::ErrorStatus es;
	AcGePoint3dArray points;
	points.removeAll();
	es = pPolyline->intersectWith(pEllipse, AcDb::kOnBothOperands, points);
	if (es!=Acad::eOk)
	{
		pEllipse->close();
		return;
	}
	else	
	{
		if ((points.length()<1L))
		{
			//m_IdArrs.append(pEllipse->objectId());
		}
		else
		{
			AcDbVoidPtrArray curveSegs;
			curveSegs.removeAll();

			AcDbCurve* tmpCurve;
			es = pEllipse->getSplitCurves(points, curveSegs);
			if (es==Acad::eOk)
			{				
				for (int i=0; i<curveSegs.length(); i++)
				{
					AcGePoint3d startPt, midPt, endPt;
					double dDist = 0.0;
					tmpCurve = static_cast<AcDbCurve*>(curveSegs[i]);
					es = tmpCurve->getEndPoint(endPt);
					es = tmpCurve->getDistAtPoint(endPt, dDist);
					es = tmpCurve->getPointAtDist(dDist/2, midPt);
					BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(midPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
					if (bIsInArea)
					{
						ArxDbgUtils::addToCurrentSpaceAndClose(tmpCurve);
						//m_IdArrs.append(tmpCurve->objectId());
						m_tmpIdArrs.append(tmpCurve->objectId());
					}
				}
			}
			else
			{
				 ArxDbgUtils::rxErrorMsg(es);
			}
			pEllipse->close();
		}
	}
}

void CCutEntity::OperateSpline(AcDbPolyline* pPolyline, AcDbSpline* pSpline)
{
	Acad::ErrorStatus es;
	AcGePoint3dArray points;
	points.removeAll();
	es = pPolyline->intersectWith(pSpline, AcDb::kOnBothOperands, points);
	if (es!=Acad::eOk)
	{
		pSpline->close();
		return;
	}
	else	
	{
		if ((points.length()<1L))
		{
			//m_IdArrs.append(pSpline->objectId());
		}
		else
		{
			AcDbVoidPtrArray curveSegs;
			curveSegs.removeAll();

			AcDbCurve* tmpCurve;
			es = pSpline->getSplitCurves(points, curveSegs);
			if (es==Acad::eOk)
			{				
				for (int i=0; i<curveSegs.length(); i++)
				{
					AcGePoint3d startPt, midPt, endPt;
					double dDist = 0.0;
					tmpCurve = static_cast<AcDbCurve*>(curveSegs[i]);
					es = tmpCurve->getEndPoint(endPt);
					es = tmpCurve->getDistAtPoint(endPt, dDist);
					es = tmpCurve->getPointAtDist(dDist/2, midPt);
					BOOL bIsInArea = MyEditEntity::JudgePointInArea(MyTransFunc::My3d22d(midPt), MyTransFunc::My3d22d(m_minPt), MyTransFunc::My3d22d(m_maxPt));
					if (bIsInArea)
					{
						ArxDbgUtils::addToCurrentSpaceAndClose(tmpCurve);
						//m_IdArrs.append(tmpCurve->objectId());
						m_tmpIdArrs.append(tmpCurve->objectId());
					}
				}
			}
			else
			{
				ArxDbgUtils::rxErrorMsg(es);
			}
		}	
		pSpline->close();
	}
}


void CCutEntity::OperatePolyline(AcDbPolyline* pPolyline, AcDbPolyline* pPoly)
{
	AcDbVoidPtrArray entitySet;
	entitySet.removeAll();
	pPoly->explode(entitySet);
	if (entitySet.isEmpty())
	{
		pPoly->close();
		return;
	}
	else
	{
		for (int i=0; i<entitySet.length(); i++)
		{
			AcDbEntity* pTempEnt = (AcDbEntity*)entitySet.at(i);
			if (pTempEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pDbLine = AcDbLine::cast(pTempEnt);
				if (pDbLine)
				{
					AcDbLine* pNewLine = new AcDbLine;
					pNewLine->setStartPoint(pDbLine->startPoint());
					pNewLine->setEndPoint(pDbLine->endPoint());
					ArxDbgUtils::addToCurrentSpace(pNewLine);
					OperateLine(pPolyline, pNewLine);
					pNewLine->close();
					m_tmpIdArrs.append(pNewLine->objectId());
				}
			}
			else if (pTempEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc* pDbArc = AcDbArc::cast(pTempEnt);
				if (pDbArc)
				{
					AcDbArc* pNewArc = new AcDbArc;
					pNewArc->setCenter(pDbArc->center());
					pNewArc->setRadius(pDbArc->radius());
					pNewArc->setStartAngle(pDbArc->startAngle());
					pNewArc->setEndAngle(pDbArc->endAngle());
					ArxDbgUtils::addToCurrentSpace(pNewArc);
					OperateArc(pPolyline, pNewArc);
					pNewArc->close();
					m_tmpIdArrs.append(pNewArc->objectId());
				}
			}
			else if (pTempEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle* pDbCircle = AcDbCircle::cast(pTempEnt);
				if (pDbCircle)
				{
					AcDbCircle* pNewCircle = new AcDbCircle;
					pNewCircle->setRadius(pDbCircle->radius());
					pNewCircle->setCenter(pDbCircle->center());
					ArxDbgUtils::addToCurrentSpace(pNewCircle);
					OperateCircle(pPolyline, pNewCircle);
					pNewCircle->close();
					m_tmpIdArrs.append(pNewCircle->objectId());
				}
			}
			if (pTempEnt)
			{
				delete pTempEnt;
			}
			pTempEnt = NULL;
		}
	}
	pPoly->close();
}

void CCutEntity::Operate2dPolyline(AcDbPolyline* pPolyline, AcDb2dPolyline* pPoly)
{
	AcDbVoidPtrArray entitySet;
	entitySet.removeAll();
	pPoly->explode(entitySet);
	if (entitySet.isEmpty())
	{
		pPoly->close();
		return;
	}
	else
	{
		for (int i=0; i<entitySet.length(); i++)
		{
			AcDbEntity* pTempEnt = (AcDbEntity*)entitySet.at(i);
			if (pTempEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pDbLine = AcDbLine::cast(pTempEnt);
				if (pDbLine)
				{
					AcDbLine* pNewLine = new AcDbLine;
					pNewLine->setStartPoint(pDbLine->startPoint());
					pNewLine->setEndPoint(pDbLine->endPoint());
					ArxDbgUtils::addToCurrentSpace(pNewLine);
					OperateLine(pPolyline, pNewLine);
					pNewLine->close();
					m_tmpIdArrs.append(pNewLine->objectId());
				}
			}
			else if (pTempEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc* pDbArc = AcDbArc::cast(pTempEnt);
				if (pDbArc)
				{
					AcDbArc* pNewArc = new AcDbArc;
					pNewArc->setCenter(pDbArc->center());
					pNewArc->setRadius(pDbArc->radius());
					pNewArc->setStartAngle(pDbArc->startAngle());
					pNewArc->setEndAngle(pDbArc->endAngle());
					ArxDbgUtils::addToCurrentSpace(pNewArc);
					OperateArc(pPolyline, pNewArc);
					pNewArc->close();
					m_tmpIdArrs.append(pNewArc->objectId());
				}
			}
			else if (pTempEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle* pDbCircle = AcDbCircle::cast(pTempEnt);
				if (pDbCircle)
				{
					AcDbCircle* pNewCircle = new AcDbCircle;
					pNewCircle->setRadius(pDbCircle->radius());
					pNewCircle->setCenter(pDbCircle->center());
					ArxDbgUtils::addToCurrentSpace(pNewCircle);
					OperateCircle(pPolyline, pNewCircle);
					pNewCircle->close();
					m_tmpIdArrs.append(pNewCircle->objectId());
				}
			}
			if (pTempEnt)
			{
				delete pTempEnt;
			}
			pTempEnt = NULL;
		}
	}
	pPoly->close();
}

void CCutEntity::Operate3dPolyline(AcDbPolyline* pPolyline, AcDb3dPolyline* pPoly)
{
	AcDbVoidPtrArray entitySet;
	entitySet.removeAll();
	pPoly->explode(entitySet);
	if (entitySet.isEmpty())
	{
		pPoly->close();
		return;
	}
	else
	{
		for (int i=0; i<entitySet.length(); i++)
		{
			AcDbEntity* pTempEnt = (AcDbEntity*)entitySet.at(i);
			if (pTempEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pDbLine = AcDbLine::cast(pTempEnt);
				if (pDbLine)
				{
					AcDbLine* pNewLine = new AcDbLine;
					pNewLine->setStartPoint(pDbLine->startPoint());
					pNewLine->setEndPoint(pDbLine->endPoint());
					ArxDbgUtils::addToCurrentSpace(pNewLine);
					OperateLine(pPolyline, pNewLine);
					pNewLine->close();
					m_tmpIdArrs.append(pNewLine->objectId());
				}
			}
			else if (pTempEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc* pDbArc = AcDbArc::cast(pTempEnt);
				if (pDbArc)
				{
					AcDbArc* pNewArc = new AcDbArc;
					pNewArc->setCenter(pDbArc->center());
					pNewArc->setRadius(pDbArc->radius());
					pNewArc->setStartAngle(pDbArc->startAngle());
					pNewArc->setEndAngle(pDbArc->endAngle());
					ArxDbgUtils::addToCurrentSpace(pNewArc);
					OperateArc(pPolyline, pNewArc);
					pNewArc->close();
					m_tmpIdArrs.append(pNewArc->objectId());
				}
			}
			else if (pTempEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle* pDbCircle = AcDbCircle::cast(pTempEnt);
				if (pDbCircle)
				{
					AcDbCircle* pNewCircle = new AcDbCircle;
					pNewCircle->setRadius(pDbCircle->radius());
					pNewCircle->setCenter(pDbCircle->center());
					ArxDbgUtils::addToCurrentSpace(pNewCircle);
					OperateCircle(pPolyline, pNewCircle);
					pNewCircle->close();
					m_tmpIdArrs.append(pNewCircle->objectId());
				}
			}
			if (pTempEnt)
			{
				delete pTempEnt;
			}
			pTempEnt = NULL;
		}
	}
	pPoly->close();
}


void CCutEntity::CloneAndMoveEntity(AcDbObjectIdArray idArrs, AcGeMatrix3d m)
{
	if (idArrs.length()<1L)
	{
		return;
	}
	AcDbEntity* ent = NULL;
	AcDbObjectId entId;
	for (int i=0; i<idArrs.length(); i++)
	{
		entId = idArrs.at(i);
		if (acdbOpenAcDbEntity(ent, entId, AcDb::kForRead)!=Acad::eOk)
		{
			continue;
		}
		AcDbObjectId ownerBlockId = ent->blockId();    // compiler complains!
		if (ArxDbgUtils::cloneAndXformObjects(acdbHostApplicationServices()->workingDatabase(), ent->objectId(),
			ownerBlockId, m, true) != Acad::eOk)
		{
			ArxDbgUtils::stopAlertBox(_T("ERROR: Couldn't clone entity!"));
		}
		ent->close();
	}
}

void CCutEntity::EraseEntity(AcDbObjectIdArray idArrs)
{
	if (idArrs.length()<1L)
	{
		return;
	}

	AcDbEntity* pEnt = NULL;
	AcDbObjectId entId;
	for (int i=0; i<idArrs.length(); i++)
	{
		entId = idArrs.at(i);
		if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite)!=Acad::eOk)
		{
			continue;
		}
		pEnt->erase();
		pEnt->close();
	}
}

void CCutEntity::move()
{
	AcGeMatrix3d xform;
	AcGeVector3d transVec;


	ArxDbgUiPrPoint prToPt(_T("\n指定插入点"), NULL/*, m_minPt*/);
	if (prToPt.go() != ArxDbgUiPrBase::kOk) {
		return;
	}
	transVec = (ArxDbgUtils::ucsToWcs(prToPt.value()) -
		ArxDbgUtils::ucsToWcs(m_minPt));

	xform.setToTranslation(transVec);
	CloneAndMoveEntity(m_IdArrs, xform);
	EraseEntity(m_tmpIdArrs);
}

Acad::ErrorStatus CCutEntity::ExplodeEntity(AcDbEntity* pEnt, AcDbVoidPtrArray& entitySet)
{
	Acad::ErrorStatus es = Acad::eOk;

	if (!pEnt)
	{
		return Acad::eNullObjectPointer;
	}
	else if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbVoidPtrArray entSet;
		es = pEnt->explode(entSet);
		if (entSet.length() > 0)
		{
			entitySet.append(entSet);
		}
	}
	else if (pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbVoidPtrArray entSet;
		es = pEnt->explode(entSet);
		if (entSet.length() > 0)
		{
			entitySet.append(entSet);
		}
	}
	else
	{
		return Acad::eNotApplicable;
	}

	return es;
}

void CCutEntity::sortPtArray(AcGePoint3d basePt, AcGePoint3dArray& ptArr)
{
	if (ptArr.length()<1)
	{
		return;
	}
	CDoubleArray distCountArr;
	double tmpDist = 0.0;
	AcGePoint3d tmpPt;
	distCountArr.RemoveAll();
	std::map<double, AcGePoint3d> sortMap;

	for (int i=0 ;i<ptArr.length(); i++)
	{
		tmpDist = acutDistance(asDblArray(basePt), asDblArray(ptArr.at(i)));
		std::pair<std::map<double, AcGePoint3d>::iterator, bool> ret = sortMap.insert(std::map<double, AcGePoint3d>::value_type(std::make_pair(tmpDist, ptArr.at(i))));
	}
	ptArr.removeAll();
	for (std::map<double, AcGePoint3d>::iterator iter = sortMap.begin(); iter!=sortMap.end(); ++iter)
	{	
		ptArr.append(iter->second);
	}
}

void CCutEntity::sortPtArrayByCurve(AcDbCurve* pCurve, AcGePoint3dArray& ptArr)
{
	if (ptArr.length()<1)
	{
		return;
	}
	CDoubleArray distCountArr;
	double tmpDist = 0.0;
	AcGePoint3d tmpPt;
	distCountArr.RemoveAll();
	for (int i=0 ;i<ptArr.length(); i++)
	{
		pCurve->getDistAtPoint(ptArr.at(i), tmpDist);
		distCountArr.Add(tmpDist);
	}

	for (int i=1; i<distCountArr.GetSize(); i++)
	{
		for (int j=0; j<distCountArr.GetSize()-i; j++)
		{
			if (distCountArr.GetAt(i)>distCountArr.GetAt(j))
			{
				tmpPt = ptArr.at(i);
				ptArr.at(i) = ptArr.at(j);
				ptArr.at(j) = tmpPt;
			}
		}
	}
}


void CCutEntity::OperateExplodeEntity(AcDbPolyline* pPolyline,AcDbVoidPtrArray entitySet)
{
	int expLength = entitySet.length();
	/*for (int j = 0; j < expLength; j++)
	{
		ExplodeEntity((AcDbEntity*)entitySet.at(j), entitySet);
	}

	for (int k=0; k<entitySet.length(); k++)
	{
		ExplodeEntity((AcDbEntity*)entitySet.at(k), entitySet);
	}

	expLength = entitySet.length();*/
	for (int j = 0; j < expLength; j++)
	{
		AcDbEntity* pTempEnt = (AcDbEntity*)entitySet.at(j);
		if (pTempEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pDbLine = AcDbLine::cast(pTempEnt);
			if (pDbLine)
			{
				AcDbLine* pNewLine = new AcDbLine;
				pNewLine->setStartPoint(pDbLine->startPoint());
				pNewLine->setEndPoint(pDbLine->endPoint());
				ArxDbgUtils::addToCurrentSpace(pNewLine);
				OperateLine(pPolyline, pNewLine);
				pNewLine->close();
				m_tmpIdArrs.append(pNewLine->objectId());
			}
		}
		else if (pTempEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc* pDbArc = AcDbArc::cast(pTempEnt);
			if (pDbArc)
			{
				AcDbArc* pNewArc = new AcDbArc;
				pNewArc->setCenter(pDbArc->center());
				pNewArc->setRadius(pDbArc->radius());
				pNewArc->setStartAngle(pDbArc->startAngle());
				pNewArc->setEndAngle(pDbArc->endAngle());
				ArxDbgUtils::addToCurrentSpace(pNewArc);
				OperateArc(pPolyline, pNewArc);
				pNewArc->close();
				m_tmpIdArrs.append(pNewArc->objectId());
			}
		}
		else if (pTempEnt->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle* pDbCircle = AcDbCircle::cast(pTempEnt);
			if (pDbCircle)
			{
				AcDbCircle* pNewCircle = new AcDbCircle;
				pNewCircle->setRadius(pDbCircle->radius());
				pNewCircle->setCenter(pDbCircle->center());
				ArxDbgUtils::addToCurrentSpace(pNewCircle);
				OperateCircle(pPolyline, pNewCircle);
				pNewCircle->close();
				m_tmpIdArrs.append(pNewCircle->objectId());
			}
		}
		if (pTempEnt)
		{
			delete pTempEnt;
		}
		pTempEnt = NULL;
	}
	entitySet.removeAll();
}

//将原实体的属性绑定到新实体
AcDbObjectId CCutEntity::SetPropertiesFromEnt(AcDbEntity* pBaseEnt, AcDbObjectId entId)
{
	if (entId.isNull())
	{
		return entId;
	}
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, entId, AcDb::kForWrite) != Acad::eOk)
	{
		return entId;
	}

	pEnt->setPropertiesFrom(pBaseEnt);
	pEnt->close();
	return pEnt->objectId();
}
