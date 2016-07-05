#include "StdAfx.h"
#include "PipeUtils.h"
#include "COperatePL.h"
#include "GWDesingUtils.h"

CPipeUtils::CPipeUtils(void)
{
	m_PipeInfo.clear();
}

CPipeUtils::~CPipeUtils(void)
{
	m_PipeInfo.clear();
}

vector<AcDbObjectId> CPipeUtils::getAllPipe()
{
	resbuf *rb = acutBuildList( -4,_T("<and"),RTDXF0,_T("LWPOLYLINE"), 8, _T("燃气管道"), -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, rb, ssname);
	acutRelRb(rb);
	if (nRet != RTNORM)
	{
		return m_PipeInfo;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	if (sslen < 1)
	{
		return m_PipeInfo;
	}
	AcDbEntity* pEnt = NULL;
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		m_PipeInfo.push_back(objId);
	}

	acedSSFree(ssname);
	return m_PipeInfo;
}

bool CPipeUtils::ShowGuanduanText()
{
	bool bRet = doGuandaoXdata(true);
	return bRet;
}

bool CPipeUtils::doGuandaoXdata(bool bIsShow /*= false*/)
{
	//////////////////////////////////////////////////////////////////////////
	//先删除原有的
	resbuf *rb = acutBuildList( -4,_T("<and"),RTDXF0,_T("TEXT"), 8, _T("管段文字"), -4,_T("and>"), RTNONE);
	ads_name ssname;
	int nRet = acedSSGet(_T("X"), NULL, NULL, rb, ssname);
	acutRelRb(rb);
	if (nRet == RTNORM)
	{
		long sslen;
		acedSSLength(ssname, &sslen);
		MyEditEntity::EraseSSName(ssname);
		acedSSFree(ssname);
	}
	//////////////////////////////////////////////////////////////////////////
	getAllPipe();
	AcGePoint3d startPt, endPt,cenPt;
	COperatePL PL;
	AcDbObjectId layerId = CGWDesingUtils::getGlobalGdLayerId();
	AcDbObjectId textId = CGWDesingUtils::getGlobalTextStyle();
	AcDbObjectId entId = AcDbObjectId::kNull;
	int i=1;
	for (vector<AcDbObjectId>::iterator iter = m_PipeInfo.begin();
		iter != m_PipeInfo.end();
		++iter)
	{
		AcDbObjectId objId = *iter;
		objId = MyEditEntity::openEntChangeLayer(objId, CGWDesingUtils::getGlobalPipeLayerId());
		MyEditEntity::OpenObjAppendIntToXdata(objId, GD_NODE, i);
		startPt = PL.startPt(objId);
		endPt = PL.endPt(objId);
		cenPt = MyTransFunc::MyMidPoint(startPt, endPt);
		CString strNo;
		strNo.Format(_T("管段%d"), i);
		entId = MyDrawEntity::DrawText(cenPt, strNo, 3.0*CGWDesingUtils::getGlobalScale(), textId, AcDb::kTextCenter, AcDb::kTextBottom);
		entId = MyEditEntity::openEntChangeLayer(entId, layerId);
		i++;
	}
	
	return true;
}

