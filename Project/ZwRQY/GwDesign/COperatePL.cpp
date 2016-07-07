#include "StdAfx.h"
#include "COperatePL.h"
#include <algorithm>
#include "GWDesingUtils.h"
#include <gecomp3d.h>
#include "SerialNoUtils.h"
#include "SerialNo.h"
#include <iterator>

COperatePL::COperatePL(void)
{
}

COperatePL::~COperatePL(void)
{
}

vector<PlineInfo> COperatePL::JoinTwoPline(AcDbObjectId preId, AcDbObjectId plineId)
{
	vector<PlineInfo> tmpData;

	AcDbObjectId objId = AcDbObjectId::kNull;
	if (preId.isNull() || plineId.isNull())
	{
		return tmpData;
	}
	vector<PlineInfo> preData = getdatabyObjId(preId);
	vector<PlineInfo> data = getdatabyObjId(plineId);

	if ((preData.size() < 1)
		||(data.size() < 1))
	{
		acutPrintf(_T("\n没有数据，不能合并"));
		return tmpData;
	}
	if ((preData.at(0).bIsClosed)
		||(data.at(0).bIsClosed))
	{
		acutPrintf(_T("\n至少一条多段线闭合，不能合并"));
		return tmpData;
	}
	double dDist1,dDist2,dDist3,dDist4;
	AcGePoint3d prestartPt,preEndPt,startPt,endPt;
	prestartPt = preData.at(0).insertPt;
	preEndPt = preData.at(preData.size()-1).insertPt;

	startPt = data.at(0).insertPt;
	endPt = data.at(data.size()-1).insertPt;
	dDist1 = acutDistance(asDblArray(prestartPt), asDblArray(startPt));
	dDist2 = acutDistance(asDblArray(preEndPt), asDblArray(endPt));
	dDist3 = acutDistance(asDblArray(prestartPt), asDblArray(endPt));
	dDist4 = acutDistance(asDblArray(preEndPt), asDblArray(startPt));
	map<double, int> mData;
	mData.insert(make_pair(dDist1, 1));
	mData.insert(make_pair(dDist2, 2));
	mData.insert(make_pair(dDist3, 3));
	mData.insert(make_pair(dDist4, 4));
	map<double, int>::iterator Iter = mData.begin();
	int nCount = Iter->second;
	//reverse

	if (nCount == 1)//
	{
		reverse(data.begin(), data.end());
		data.insert(data.end(), preData.begin(), preData.end());
		tmpData.insert(tmpData.end(), data.begin(), data.end());
	}
	else if (nCount == 2)
	{
		reverse(data.begin(), data.end());
		preData.insert(preData.end(), data.begin(), data.end());
		tmpData.insert(tmpData.end(), preData.begin(), preData.end());
	}
	else if (nCount == 3)
	{
		data.insert(data.end(), preData.begin(), preData.end());
		tmpData.insert(tmpData.end(), data.begin(), data.end());
	}
	else
	{
		preData.insert(preData.end(), data.begin(), data.end());
		tmpData.insert(tmpData.end(), preData.begin(), preData.end());
	}
	return tmpData;
}

vector<PlineInfo> COperatePL::getdatabyObjId(AcDbObjectId plineId)
{	
	vector<PlineInfo> data;
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return data;
	}
	double dBugle,dStartWidth,dEndWidth;
	if (pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pPolyline = AcDbPolyline::cast(pEnt);
		AcGePoint3d tmpPt;
		bool bIsClosed = pPolyline->isClosed();
		int	numSegs = pPolyline->numVerts();
		for (int i=0; i<numSegs; i++)
		{
			PlineInfo plineInfo;
			pPolyline->getPointAt(i, tmpPt);
			pPolyline->getBulgeAt(i, dBugle);
			pPolyline->getWidthsAt(i, dStartWidth, dEndWidth);
			//plineInfo.nIndex = i;
			plineInfo.insertPt = tmpPt;
			plineInfo.dBulge = dBugle;
			plineInfo.dStartWidth = dStartWidth;
			plineInfo.dEndWidth = dEndWidth;
			plineInfo.bIsClosed = bIsClosed;
			data.push_back(plineInfo);
		}
	}
	pEnt->close();
	return data;
}

set<CString> COperatePL::getMapInfoById(AcDbObjectId plineId)
{
	set<CString> data;
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return data;
	}
	double dBugle,dStartWidth,dEndWidth;
	if (pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pPolyline = AcDbPolyline::cast(pEnt);
		AcGePoint3d tmpPt;
		bool bIsClosed = pPolyline->isClosed();
		int	numSegs = pPolyline->numVerts();
		CString strTmp;
		for (int i=0; i<numSegs; i++)
		{
			pPolyline->getPointAt(i, tmpPt);
			pPolyline->getBulgeAt(i, dBugle);
			pPolyline->getWidthsAt(i, dStartWidth, dEndWidth);
			strTmp.Format(_T("%.f,%.f,%.f,%.f,%.f"), tmpPt.x, tmpPt.y, dBugle, dStartWidth, dEndWidth);
			data.insert(strTmp);
		}
	}
	pEnt->close();
	return data;
}

AcDbObjectId COperatePL::drawPlineBydata(vector<PlineInfo> data)
{
	AcDbPolyline* pLine = new AcDbPolyline;
	int i=0;
	pLine->setClosed(data.at(0).bIsClosed);
	for (vector<PlineInfo>::iterator iter = data.begin();
		iter != data.end();
		++iter)
	{
		PlineInfo info = *iter;
		pLine->addVertexAt(i, info.insertPt.convert2d(AcGePlane::kXYPlane), info.dBulge, info.dStartWidth, info.dEndWidth);
		i++;
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pLine);
	return pLine->objectId();
}

double COperatePL::getLength(AcDbObjectId plineId)
{
	double dLen = 0.0;
	AcDbPolyline* pLine = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return dLen;
	}
	AcGePoint3d endPt;
	pLine->getEndPoint(endPt);
	pLine->getDistAtPoint(endPt, dLen);
	pLine->close();
	return dLen;
}

AcDbObjectId COperatePL::trimbycircle(AcDbObjectId plineId)
{
	AcDbObjectId entId = AcDbObjectId::kNull;
	entId = trimstartBycircle(plineId);
	entId = trimendBycircle(entId);
	return entId;
}

AcDbObjectId COperatePL::trimendBycircle(AcDbObjectId plineId)
{
	double dradius = CGWDesingUtils::getGlobalRadius();
	AcDbPolyline* pLine = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return plineId;
	}
	AcGePoint3d endPt;
	pLine->getEndPoint(endPt);
	
	AcDbObjectId cirId = MyDrawEntity::DrawCircle(endPt, dradius);
	AcDbCircle* pCircle;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pCircle, cirId, AcDb::kForWrite) != Acad::eOk)
	{
		pLine->close();
		return plineId;
	}
	AcGePoint3dArray points;
	Acad::ErrorStatus es;
	es = pLine->intersectWith(pCircle, AcDb::kOnBothOperands, points);
	bool bIsErased = false;
	//闭合的多段线不考虑
	if ((points.length() == 1) && (!pLine->isClosed()))//只有一个点的时候处理
	{	
		int nNum = pLine->numVerts();
		AcGePoint3d tmpPt = points.at(0);
		double dParam = 0;
		pLine->getParamAtPoint(tmpPt, dParam);
		int nCount = ceil(dParam);
		pLine->upgradeOpen();
		es = pLine->setPointAt(nCount, tmpPt.convert2d(AcGePlane::kXYPlane));
		for (int i=nCount + 1; i<nNum; i++)
		{
			pLine->removeVertexAt(i);
		}
		/*AcDbVoidPtrArray curveSegs;
		curveSegs.removeAll();
		es = pLine->getSplitCurves(points, curveSegs);
		if (es == Acad::eOk)
		{
			AcDbCurve* tmpCurve;
			for (int i=0; i<curveSegs.length(); i++)
			{
				AcGePoint3d tmpPt;
				double dDist = 0.0;
				tmpCurve = static_cast<AcDbCurve*>(curveSegs[i]);
				es = tmpCurve->getEndPoint(tmpPt);
				if (tmpPt.isEqualTo(endPt))
				{
					delete tmpCurve;
					tmpCurve = NULL;
				}
				else
				{
					MyBaseUtils::addToCurrentSpaceAndClose(tmpCurve);
					plineId = tmpCurve->objectId();

					bIsErased = true;
				}
			}
		}*/

	}
	if (bIsErased)
	{
		pLine->upgradeOpen();
		pLine->erase();
	}
	pLine->close();
	pCircle->erase();
	pCircle->close();
	return plineId;
}

AcDbObjectId COperatePL::trimstartBycircle(AcDbObjectId plineId)
{
	double dradius = CGWDesingUtils::getGlobalRadius();
	AcDbPolyline* pLine = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return plineId;
	}
	AcGePoint3d startPt;
	pLine->getStartPoint(startPt);

	AcDbObjectId cirId = MyDrawEntity::DrawCircle(startPt, dradius);
	AcDbCircle* pCircle;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pCircle, cirId, AcDb::kForWrite) != Acad::eOk)
	{
		pLine->close();
		return plineId;
	}
	AcGePoint3dArray points;
	Acad::ErrorStatus es;
	es = pLine->intersectWith(pCircle, AcDb::kOnBothOperands, points);
	bool bIsErased = false;
	//闭合的多段线不考虑

	if ((points.length() == 1)&&(!pLine->isClosed()))//只有一个点的时候处理
	{
		AcGePoint3d tmpPt = points.at(0);
		double dParam = 0;
		pLine->getParamAtPoint(tmpPt, dParam);
		pLine->upgradeOpen();
		int nCount = floor(dParam);
		pLine->upgradeOpen();
		es = pLine->setPointAt(nCount, tmpPt.convert2d(AcGePlane::kXYPlane));
		for (int i=0; i<nCount; i++)
		{
			pLine->removeVertexAt(i);
		}
		/*AcDbVoidPtrArray curveSegs;
		curveSegs.removeAll();
		es = pLine->getSplitCurves(points, curveSegs);
		if (es == Acad::eOk)
		{
			AcDbCurve* tmpCurve;
			for (int i=0; i<curveSegs.length(); i++)
			{
				AcGePoint3d tmpPt;
				double dDist = 0.0;
				tmpCurve = static_cast<AcDbCurve*>(curveSegs[i]);
				es = tmpCurve->getStartPoint(tmpPt);
				if (tmpPt.isEqualTo(startPt))
				{
					delete tmpCurve;
					tmpCurve = NULL;
				}
				else
				{
					MyBaseUtils::addToCurrentSpaceAndClose(tmpCurve);
					plineId = tmpCurve->objectId();
					bIsErased = true;
				}
			}
		}*/

	}
	if (bIsErased)
	{
		pLine->upgradeOpen();
		pLine->erase();
	}
	pLine->close();
	pCircle->erase();
	pCircle->close();
	return plineId;
}

AcDbObjectId COperatePL::restorePolyline(AcDbObjectId plineId, AcGePoint3d pt)
{
	vector<PlineInfo> dataInfo;
	dataInfo = getdatabyObjId(plineId);

	double dradius = CGWDesingUtils::getGlobalRadius();
	AcDbPolyline* pLine = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return plineId;
	}

	AcGePoint3d sPt,ePt;
	pLine->getStartPoint(sPt);
	pLine->getEndPoint(ePt);
	//pLine->close();

	AcDbObjectId objId = plineId;
	double dDist1 = acutDistance(asDblArray(sPt), asDblArray(pt));
	double dDist2 = acutDistance(asDblArray(ePt), asDblArray(pt));
	double dRadius = CGWDesingUtils::getGlobalRadius();
	if (abs(dDist1 - dRadius) < GeTol)
	{
		/*MyEditEntity::EraseObj(plineId);
		PlineInfo info;
		info = dataInfo.at(0);
		info.insertPt = pt;
		dataInfo.insert(dataInfo.begin(), info);
		plineId = drawPlineBydata(dataInfo);*/
		pLine->upgradeOpen();
		pLine->setPointAt(0, pt.convert2d(AcGePlane::kXYPlane));
	}

	if (abs(dDist2 - dRadius) < GeTol)//处理终点
	{
		/*dataInfo = getdatabyObjId(plineId);
		MyEditEntity::EraseObj(plineId);
		PlineInfo info;
		info = dataInfo.at(dataInfo.size()-1);
		info.insertPt = pt;
		dataInfo.insert(dataInfo.end(), info);
		plineId = drawPlineBydata(dataInfo);*/
		pLine->upgradeOpen();
		int nNum = pLine->numVerts();
		pLine->setPointAt(nNum - 1, pt.convert2d(AcGePlane::kXYPlane));
	}
	pLine->close();
	return plineId;
}

AcGeCompositeCurve3d convertDbCurveToGeCurve3d(AcDbObjectId plineId)
{
	int nSegs;
	AcGeLineSeg3d *pLine; 
	AcGeLineSeg3d line;
	AcGeCircArc3d *pArc;
	AcGeCircArc3d arc;
	AcGeVoidPointerArray geCurves;

	AcDbPolyline* pPoly = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pPoly, plineId, AcDb::kForRead) != Acad::eOk)
	{
		return NULL;
	}
	if (pPoly->isClosed())
	{
		nSegs = pPoly->numVerts();
	}
	else
	{
		nSegs = pPoly->numVerts() - 1;
	}

	for( int i = 0; i < nSegs; i++ )
	{
		if( pPoly->segType(i) == AcDbPolyline::kLine )
		{
			pPoly->getLineSegAt( i, line );
			pLine = new AcGeLineSeg3d( line );
			geCurves.append( pLine ); 
		}
		else if( pPoly->segType( i ) == AcDbPolyline::kArc )
		{
			pPoly->getArcSegAt( i, arc );
			pArc = new AcGeCircArc3d( arc );
			geCurves.append( pArc );
		}
	}

	pPoly->close();
	if(geCurves.isEmpty()) {
		return NULL;
	}
	AcGeCompositeCurve3d pCurve(geCurves);
	return pCurve;
}

bool COperatePL::CompareTwoPline(AcDbObjectId preId, AcDbObjectId curId)
{
	int nFlag = 0;
	double dLen1,dLen2;
	dLen1 = getLength(preId);
	dLen2 = getLength(curId);
	if (dLen1 >= dLen2)
	{
		nFlag = 1;
	}
	else
	{
		nFlag = -1;
	}
	AcDbCurve* pPreCurve = NULL;
	AcDbCurve* pCurCurve = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pPreCurve, preId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	if (acdbOpenAcDbEntity((AcDbEntity*&)pCurCurve, preId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	double dDist;
	if ((pPreCurve->isKindOf(AcDbPolyline::desc()))
		&&(pCurCurve->isKindOf(AcDbPolyline::desc())))
	{
		pPreCurve->close();
		pCurCurve->close();
		AcGeCompositeCurve3d preCurve = convertDbCurveToGeCurve3d(preId);
		AcGeCompositeCurve3d curCuve = convertDbCurveToGeCurve3d(curId);
		dDist = preCurve.distanceTo(curCuve);
		if (dDist >= GeTol)
		{
			return false;
		}
	}
	pPreCurve->close();
	pCurCurve->close();
	return true;
}


bool COperatePL::JudgeTheSame(AcDbObjectId preId, AcDbObjectId curId)
{
	set<CString> preData = getMapInfoById(preId);
	set<CString> curData = getMapInfoById(curId);
	int nPreSize = preData.size();
	int nCurSize = curData.size();
	set<CString> data;
	data.clear();
	set_intersection(preData.begin(), preData.end(), curData.begin(), curData.end(), insert_iterator<set<CString> >(data, data.begin()));
	int nSize = data.size();
	if (nSize > 0)
	{
		if ((nSize == nCurSize) || (nSize == nPreSize))
		{
			return true;
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
}

bool COperatePL::HalfPlByPt(AcDbObjectId curId, AcGePoint3d pt)
{
	AcDbObjectId preId;
	AcDbObjectId nexId;
	CString strStartNo = MyEditEntity::GetObjStrXdata(curId, START_ENT);
	CString strEndNo = MyEditEntity::GetObjStrXdata(curId, END_ENT);
	AcDbPolyline* pLine = NULL;
	AcGePoint3dArray points;
	points.append(pt);

	if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, curId, AcDb::kForWrite) != Acad::eOk)
	{
		return false;
	}

	AcGePoint3d startPt,endPt;
	pLine->getStartPoint(startPt);
	pLine->getEndPoint(endPt);
	Acad::ErrorStatus es;
	AcDbVoidPtrArray curveSegs;
	curveSegs.removeAll();
	es = pLine->getSplitCurves(points, curveSegs);
	if (es == Acad::eOk)
	{
		AcDbCurve* tmpCurve;
		for (int i=0; i<curveSegs.length(); i++)
		{
			AcGePoint3d tmpPt;
			double dDist = 0.0;
			tmpCurve = static_cast<AcDbCurve*>(curveSegs[i]);
			es = tmpCurve->getStartPoint(tmpPt);
			if (tmpPt.isEqualTo(startPt))
			{
				MyBaseUtils::addToCurrentSpaceAndClose(tmpCurve);
				preId = tmpCurve->objectId();
			}
			else
			{
				MyBaseUtils::addToCurrentSpaceAndClose(tmpCurve);
				nexId = tmpCurve->objectId();
			}
		}
	}
	else
	{
		AfxMessageBox(_T("点不在管道上"));
		pLine->close();
		return false;
	}
	pLine->erase();
	pLine->close();
	
	//////////////////////////////////////////////////////////////////////////
	
	CSerialNoUtils No;
	AcDbObjectId startId = No.getIdByNo(strStartNo);
	AcDbObjectId endId = No.getIdByNo(strEndNo);

	preId = trimendBycircle(preId);
	nexId = trimstartBycircle(nexId);
	//////////////////////////////////////////////////////////////////////////
	//绘制序号
	CSerialNo* pNo = new CSerialNo;
	pNo->setBasePt(pt);
	pNo->addObjId(preId);
	pNo->addObjId(nexId);
	MyBaseUtils::addToCurrentSpaceAndClose(pNo);

	CString strNo = pNo->strText();
	MyEditEntity::OpenObjAppendStrToXdata(preId, START_ENT, strStartNo);
	MyEditEntity::OpenObjAppendStrToXdata(preId, END_ENT, strNo);

	MyEditEntity::OpenObjAppendStrToXdata(nexId, START_ENT, strNo);
	MyEditEntity::OpenObjAppendStrToXdata(nexId, END_ENT, strEndNo);
	//////////////////////////////////////////////////////////////////////////

	No.removeId(startId, curId, preId);
	No.removeId(endId, curId, nexId);
	return true;
}

AcGePoint3d COperatePL::startPt(AcDbObjectId curId)
{
	AcDbCurve* pCurve = NULL;
	AcGePoint3d startPt;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pCurve, curId, AcDb::kForRead) != Acad::eOk)
	{
		return startPt;
	}
	pCurve->getStartPoint(startPt);
	pCurve->close();
	return startPt;
}

AcGePoint3d COperatePL::endPt(AcDbObjectId curId)
{
	AcDbCurve* pCurve = NULL;
	AcGePoint3d endPt;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pCurve, curId, AcDb::kForRead) != Acad::eOk)
	{
		return endPt;
	}
	pCurve->getEndPoint(endPt);
	pCurve->close();
	return endPt;
}
