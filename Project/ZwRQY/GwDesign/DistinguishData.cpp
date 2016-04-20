#include "StdAfx.h"
#include "DistinguishData.h"

CDistinguishData::CDistinguishData(void)
{
	m_pDataStore = new CGDDataStore();
}

CDistinguishData::~CDistinguishData(void)
{
	if (m_pDataStore != NULL)
	{
		delete m_pDataStore;
		m_pDataStore = NULL;
	}
	m_lineVec.clear();
}

bool CDistinguishData::doIt()
{
	bool bRet = selectEnt();
	if (!bRet)
	{
		return bRet;
	}
	collectData();
	return true;
}

bool CDistinguishData::selectEnt()
{
	resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("*POLYLINE,LINE"),-4,_T("and>"),RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(NULL, NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return false;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return false;
	}
	AcDbEntity* pEnt = NULL;
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pLine = AcDbLine::cast(pEnt);
			CLine tmpLine;
			tmpLine.setStartPt(pLine->startPoint());
			tmpLine.setEndPt(pLine->endPoint());
			tmpLine.setObjectId(objId);
			m_lineVec.push_back(tmpLine);
		}
		else if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine = AcDbPolyline::cast(pEnt);
			AcDbVoidPtrArray entitySet;
			pLine->explode(entitySet);
			for (int j=0; j<entitySet.length(); j++)
			{
				AcDbEntity* pTmpEnt = (AcDbEntity*)entitySet.at(j);
				if (pTmpEnt->isKindOf(AcDbLine::desc()))
				{
					AcDbLine* pTmpLine = AcDbLine::cast(pTmpEnt);
					AcDbObjectId retId = AcDbObjectId::kNull;
					MyBaseUtils::AddtoModelSpaceAndClose(pTmpLine, retId);
					CLine tmpLine;
					tmpLine.setStartPt(pTmpLine->startPoint());
					tmpLine.setEndPt(pTmpLine->endPoint());
					tmpLine.setObjectId(retId);
					m_pDataStore->getLineData().push_back(tmpLine);
				}
				else if (pTmpEnt->isKindOf(AcDbArc::desc()))
				{
					AcDbArc* pTmpArc = AcDbArc::cast(pTmpEnt);
					AcDbObjectId retId = AcDbObjectId::kNull;
					MyBaseUtils::AddtoModelSpaceAndClose(pTmpArc, retId);
					CArc tmpArc;
					tmpArc.setCenter(pTmpArc->center());
					tmpArc.setRadius(pTmpArc->radius());
					tmpArc.setStartAngle(pTmpArc->startAngle());
					tmpArc.setEndAngle(pTmpArc->endAngle());
					tmpArc.setObjId(retId);
					m_pDataStore->getArcData().push_back(tmpArc);
				}
			}
		}
		pEnt->close();
	}
	return true;
}

bool CDistinguishData::collectData()
{
	map<CString, CLine> multipleLine;
	for(vector<CLine>::size_type ix=0; ix<m_lineVec.size(); ++ix)
	{
		for(vector<CLine>::size_type iy=ix+1; iy<m_lineVec.size(); ++iy)
		{
			CLine line1,line2;
			line1 = m_lineVec.at(ix);
			line2 = m_lineVec.at(iy);
			if (CompareTwoEnt(line1, line2))
			{
				CString strTmp;
				strTmp.Format(_T("%f,%f,%f,%f,%f,%f"), line1.startPt().x, line1.startPt().y,line1.startPt().z,line1.endPt().x,line1.endPt().y,line1.endPt().z);
				multipleLine.insert(std::make_pair(strTmp, line1));
			}
		}
	}


	for (map<CString, CLine>::iterator iter = multipleLine.begin();
		iter != multipleLine.end();
		++iter)
	{
		CLine tmpLine = iter->second;
		AcDbObjectId objId = tmpLine.objId();
		MyEditEntity::openEntChangeColor(objId, 1);
	}
	return true;
}

bool CDistinguishData::doData()
{
	return true;
}
