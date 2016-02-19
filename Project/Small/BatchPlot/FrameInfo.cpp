#include "StdAfx.h"
#include "FrameInfo.h"
#include "Utility.h"


FrameInfo::FrameInfo(void)
	: m_minPoint(0, 0)
	, m_maxPoint(0, 0)
	, m_dScale(1.0)
	, m_bIsHorizontal(true)
{
}


FrameInfo::~FrameInfo(void)
{
}

AcGePoint2d FrameInfo::centerPoint(void)
{
	return m_minPoint + (m_maxPoint - m_minPoint) * 0.5;
}

double FrameInfo::width(void)
{
	return m_maxPoint.x - m_minPoint.x;
}

double FrameInfo::height(void)
{
	return m_maxPoint.y - m_minPoint.y;
}

bool FrameInfo::IsContain(FrameInfo* frame)
{
	return frame->m_minPoint.x >= m_minPoint.x && frame->m_minPoint.y >= m_minPoint.y
		&& frame->m_maxPoint.x <= m_maxPoint.x && frame->m_maxPoint.y <= m_maxPoint.y;
}

void FrameInfo::CreateRect(void)
{
	Utility util;
	util.CreateLine(m_minPoint, AcGePoint2d(m_maxPoint.x, m_minPoint.y));
	util.CreateLine(AcGePoint2d(m_maxPoint.x, m_minPoint.y), m_maxPoint);
	util.CreateLine(m_maxPoint, AcGePoint2d(m_minPoint.x, m_maxPoint.y));
	util.CreateLine(AcGePoint2d(m_minPoint.x, m_maxPoint.y), m_minPoint);
	// X
	util.CreateLine(m_minPoint, m_maxPoint);
	util.CreateLine(AcGePoint2d(m_maxPoint.x, m_minPoint.y), AcGePoint2d(m_minPoint.x, m_maxPoint.y));
}


#define My2d23d(pt) AcGePoint3d(pt.x,pt.y,0.0)
bool FrameInfo::HighLightEntity()
{
	Utility util;
	ads_name ename;
	ads_name_clear(ename);
	AcDbObjectId objId;
	ads_name ssName;
	ads_name_clear(ssName);
	acedSSAdd(NULL, NULL, ssName);

	AcDbObjectId lineId1;
	AcDbLine* pLine1 = new AcDbLine;
	Acad::ErrorStatus es;
	es = pLine1->setStartPoint(My2d23d(m_minPoint));
	es = pLine1->setEndPoint(My2d23d(m_maxPoint));
	es = pLine1->setColorIndex(1);


	if ( util.AddToModelSpace(pLine1,lineId1)!=Acad::eOk )
	{
		delete pLine1;
		return false;
	}
	es = pLine1->highlight();	
	pLine1->close();

	ads_name en1;
	if (acdbGetAdsName(en1, lineId1)!=Acad::eOk)
	{
		return false;
	}
	if (acedSSAdd(en1, ssName, ssName)!=RTNORM)
	{
		return false;
	}

	AcDbObjectId lineId2;
	AcDbLine* pLine2 = new AcDbLine;
	pLine2->setStartPoint(AcGePoint3d(m_minPoint.x, m_maxPoint.y, 0));
	pLine2->setEndPoint(AcGePoint3d(m_maxPoint.x, m_minPoint.y, 0));
	pLine2->setColorIndex(1);

	if ( util.AddToModelSpace(pLine2,lineId2)!=Acad::eOk )
	{
		delete pLine2;
		return false;
	}
	es = pLine2->highlight();	
	pLine2->close();

	ads_name en2;
	if (acdbGetAdsName(en2, lineId2)!=Acad::eOk)
	{
		return false;
	}
	if (acedSSAdd(en2, ssName, ssName)!=RTNORM)
	{
		return false;
	}

	AcDbObjectId tId;
	AcDbPolyline* pPoly=new AcDbPolyline;
	AcGePoint2d minPt2d,maxPt2d;
	minPt2d.set(m_minPoint.x, m_minPoint.y);
	maxPt2d.set(m_maxPoint.x, m_maxPoint.y);
	pPoly->addVertexAt(0,minPt2d);
	pPoly->addVertexAt(1,AcGePoint2d(m_minPoint.x, m_maxPoint.y));
	pPoly->addVertexAt(2,maxPt2d);
	pPoly->addVertexAt(3,AcGePoint2d(m_maxPoint.x, m_minPoint.y));
	pPoly->addVertexAt(4, minPt2d);
	pPoly->setColorIndex(1);
	if (util.AddToModelSpace(pPoly,tId)!=Acad::eOk )
	{
		delete pPoly;
		return false;
	}	
	pPoly->close();

	ads_name en3;
	if (acdbGetAdsName(en3, tId)!=Acad::eOk)
	{
		return false;
	}
	if (acedSSAdd(en3, ssName, ssName)!=RTNORM)
	{
		return false;
	}

	TCHAR tmpName[132];
	acedGetString(0, _T("图中红色交叉框区即为选中的图框<返回>:"), tmpName);

	long len1;
	acedSSLength(ssName, &len1);
	if (len1<=0)
	{
		return false;
	}
	else
	{
		AcDbEntity* pEnt1 = NULL;
		for (int i=0; i<len1; i++)
		{
			ads_name en;
			AcDbObjectId oId;
			acedSSName(ssName, i, en);
			acdbGetObjectId(oId, en);
			if (acdbOpenAcDbEntity(pEnt1, oId, AcDb::kForWrite)!=Acad::eOk)
			{
				pEnt1->close();
				continue;
			}
			pEnt1->erase();
			pEnt1->close();
		}
	}
	acedSSFree(ssName);

	return true;
}


void FrameInfo::GetRegion(double& dWidth, double& dHeight)
{
	dWidth = abs(m_maxPoint.y - m_minPoint.y);
	dHeight = abs(m_maxPoint.x - m_minPoint.x);
	return;
}



double FrameInfo::GetFrameScale()//获取图框的比例
{
	return m_dScale;
}

bool FrameInfo::GetFrameDirection()
{
	return m_bIsHorizontal;
}


CString FrameInfo::GetDwgFrameSize()
{
	return m_strDwgFrameSize;
}

CString FrameInfo::GetDwgFrame()
{
	return m_strDwgFrame;
}
AcGePoint2d FrameInfo::GetminPoint()
{
	return m_minPoint;
}

AcGePoint2d FrameInfo::GetmaxPoint()
{
	return m_maxPoint;
}

CString FrameInfo::GetFileName()
{
	return m_strFileName;
}

void FrameInfo::SetFrameDirection(const bool bIsHorizontal)
{
	m_bIsHorizontal = bIsHorizontal;
}

void FrameInfo::SetFrameScale(const double dScale)
{
	m_dScale = dScale;
}
//void FrameInfo::SetDwgFrame(CString strFrameName)
//{
//	m_strDwgFrame = strFrameName;
//}

void FrameInfo::SetDwgFrameSize(LPCTSTR strDwgFrameSize)
{
	m_strDwgFrameSize = strDwgFrameSize;
}

void FrameInfo::SetDwgFrame(LPCTSTR strDwgFrame)
{
	m_strDwgFrame = strDwgFrame;
}

void FrameInfo::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}