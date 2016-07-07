#include "StdAfx.h"
#include "DistinguishData.h"
//#include "CGasPipe.h"
#include "SerialNo.h"
#include "COperatePL.h"
#include "GWDesingUtils.h"

CDistinguishData::CDistinguishData(void)
{
	m_PlineVec.clear();
	m_MapInfo.clear();
	m_MulData.clear();
	m_dataInfo.clear();
}

CDistinguishData::~CDistinguishData(void)
{
	m_PlineVec.clear();
	m_MapInfo.clear();
	m_MulData.clear();
	m_dataInfo.clear();
}

bool CDistinguishData::doIt()
{
	bool bRet = selectEnt();
	if (!bRet)
	{
		return bRet;
	}
	bRet = collectData();
	if (!bRet)
	{
		AfxMessageBox(_T("请处理重合的线"));
	}
	doData();
	return true;
}

bool CDistinguishData::selectEnt()
{
	resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("LWPOLYLINE"),-4,_T("and>"),RTNONE);
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
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine = AcDbPolyline::cast(pEnt);
			double dWidth = 0.0;
			pLine->getConstantWidth(dWidth);
			AcGePoint3d startPt,endPt;
			pLine->getStartPoint(startPt);
			pLine->getEndPoint(endPt);
			pLine->close();

			if (dWidth > 0)
			{
				CString strStart,strEnd,strTmp;
				strStart.Format(_T("%f,%f,%f"),  startPt.x, startPt.y, startPt.z);
				strEnd.Format(_T("%f,%f,%f"), endPt.x, endPt.y, endPt.z);
				strTmp.Format(_T("%f,%f,%f,%f,%f,%f,%f"), startPt.x, startPt.y, startPt.z, endPt.x, endPt.y, endPt.z);
				pair<map<CString, AcDbObjectId>::iterator, bool> bRet = m_MapInfo.insert(make_pair(strTmp, objId));
				if (!bRet.second)
				{
					m_MulData.insert(make_pair(strTmp, objId));
				}
				m_PlineVec.push_back(objId);
				

				//////////////////////////////////////////////////////////////////////////
				vector<AcDbObjectId> plineVec;
				plineVec.push_back(objId);
				pair<map<CString, vector<AcDbObjectId> >::iterator, bool> bRe = m_dataInfo.insert(make_pair(strStart, plineVec));
				if (!bRe.second)
				{
					bRe.first->second.push_back(objId);
				}

				bRe = m_dataInfo.insert(make_pair(strEnd, plineVec));
				if (!bRe.second)
				{
					bRe.first->second.push_back(objId);
				}
			}
		}
		pEnt->close();
	}
	acedSSFree(ssname);
	return true;
}

bool CDistinguishData::collectData()
{
	COperatePL pl;
	for(vector<AcDbObjectId>::size_type ix=0; ix<m_PlineVec.size(); ++ix)
	{
		for(vector<AcDbObjectId>::size_type iy=ix+1; iy<m_PlineVec.size(); ++iy)
		{
			AcDbObjectId line1,line2;
			line1 = m_PlineVec.at(ix);
			line2 = m_PlineVec.at(iy);
			if (pl.JudgeTheSame(line1, line2))
			{
				CString strStart,strEnd,strTmp;
				AcGePoint3d startPt,endPt;
				startPt = pl.startPt(line2);
				endPt = pl.endPt(line2);
				/*strStart.Format(_T("%f,%f,%f"),  startPt.x, startPt.y, startPt.z);
				strEnd.Format(_T("%f,%f,%f"), endPt.x, endPt.y, endPt.z);*/
				strTmp.Format(_T("%f,%f,%f,%f,%f,%f,%f"), startPt.x, startPt.y, startPt.z, endPt.x, endPt.y, endPt.z);
				//strTmp = strStart + strEnd;
				m_MulData.insert(std::make_pair(strTmp, line2));
			}
			/*else if (pl.CompareTwoPline(line1, line2) == -1)
			{
				CString strStart,strEnd,strTmp;
				AcGePoint3d startPt,endPt;
				startPt = pl.startPt(line1);
				endPt = pl.endPt(line1);
				MyTransFunc::ptToStr(startPt, strStart);
				MyTransFunc::ptToStr(endPt, strEnd);
				strTmp = strStart + strEnd;
				m_MulData.insert(std::make_pair(strTmp, line1));
			}*/
		}
	}


	for (map<CString, AcDbObjectId>::iterator iter = m_MulData.begin();
		iter != m_MulData.end();
		++iter)
	{
		AcDbObjectId objId = iter->second;
		MyEditEntity::openEntChangeColor(objId, 1);
	}
	if (m_MulData.size() > 0)
	{
		return false;
	}
	return true;
}

bool CDistinguishData::doData()
{
	//int i=1;
	for (vector<AcDbObjectId>::iterator itr = m_PlineVec.begin();
	itr != m_PlineVec.end();
	++itr)
	{
		AcDbObjectId objId = *itr;
		COperatePL pl;
		objId = pl.trimbycircle(objId);
		objId = MyEditEntity::openEntChangeLayer(objId, CGWDesingUtils::getGlobalPipeLayerId());
		/*MyEditEntity::OpenObjAppendIntToXdata(objId, GD_NODE, i);
		i++;*/
	}

	for (map<CString, vector<AcDbObjectId> >::iterator iter = m_dataInfo.begin();
		iter != m_dataInfo.end();
		++iter)
	{
		CString strTmp = iter->first;
		AcGePoint3d tmpPt = stringToPt(strTmp);
		vector<AcDbObjectId> vec = iter->second;
		drawXh(tmpPt, vec);
	}

	
	return true;
}

AcDbObjectId CDistinguishData::drawPipe(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId startId, AcDbObjectId endId)
{
	/*AcDbObjectId layId = MySymble::CreateNewLayer(_T("abcd"), 1);
	CGasPipe* pPipe = new CGasPipe;
	pPipe->setStartPt(startPt);
	pPipe->setStartId(startId);
	pPipe->setEndId(endId);
	pPipe->setEndPt(endPt);
	pPipe->setLayer(layId);
	MyBaseUtils::addToCurrentSpaceAndClose(pPipe);
	return pPipe->objectId();*/
	return AcDbObjectId::kNull;
}


AcDbObjectId CDistinguishData::drawXh(AcGePoint3d basePt, vector<AcDbObjectId> info)
{
	COperatePL PL;
	CSerialNo* pNo = new CSerialNo;
	pNo->setBasePt(basePt);
	//////////////////////////////////////////////////////////////////////////
	for (vector<AcDbObjectId>::iterator iter = info.begin();
		iter != info.end();
		++iter)
	{
		AcDbObjectId objId = *iter;
		pNo->addObjId(objId);

		AcGePoint3d startPt,endPt;
		startPt = PL.startPt(objId);
		endPt = PL.endPt(objId);
		double dDist1 = acutDistance(asDblArray(startPt), asDblArray(basePt));
		double dDist2 = acutDistance(asDblArray(endPt), asDblArray(basePt));
		double dRadius = CGWDesingUtils::getGlobalRadius();
		CString strNo = pNo->strText();//文字
		if (abs(dDist1 - dRadius) < GeTol)
		{
			MyEditEntity::OpenObjAppendStrToXdata(objId, START_ENT, strNo);
		}
		else
		{
			MyEditEntity::OpenObjAppendStrToXdata(objId, END_ENT, strNo);
		}
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pNo);

	CString strNo = CGWDesingUtils::getCurNum();
	int nCount = MyTransFunc::StringToInt(strNo);
	nCount++;
	CString strTmp;
	strTmp.Format(_T("%d"), nCount);
	CGWDesingUtils::SetCurNum(strTmp);
	return pNo->objectId();
}

AcGePoint3d CDistinguishData::stringToPt(CString strValue)
{
	CString strFirst,strMid,strEnd;
	strFirst = MyParserString::SubString(strValue, _T(","), 0);
	strMid = MyParserString::SubString(strValue, _T(","), 1);
	strEnd = MyParserString::SubString(strValue, _T(","), 2);
	AcGePoint3d pt;
	pt.x = MyTransFunc::StringToDouble(strFirst);
	pt.y = MyTransFunc::StringToDouble(strMid);
	pt.z = MyTransFunc::StringToDouble(strEnd);
	return pt;
}
