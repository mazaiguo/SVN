#include "StdAfx.h"
#include "Line.h"

CLine::CLine(void)
{

}

CLine::CLine(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId objId)
{
	m_startPt = startPt;
	m_endPt = endPt;
	m_objId = objId;
}

CLine::~CLine(void)
{

}

//BOOL CLine::operator<(const CLine& tmpLine) const
//{
//	return (this->startPt().x < tmpLine.startPt().x)&&(this->startPt().y < tmpLine.startPt().y);
//}
//
//BOOL CLine::operator==(const CLine& tmpLine) const
//{
//	return CompareTwoEnt(*this, tmpLine);
//}

void CLine::setStartPt(AcGePoint3d pt)
{
	m_startPt = pt;
}

void CLine::setEndPt(AcGePoint3d pt)
{
	m_endPt = pt;
}

void CLine::setObjectId(AcDbObjectId objId)
{	
	m_objId = objId;
}

AcGePoint3d CLine::startPt()
{
	return m_startPt;
}

AcGePoint3d CLine::endPt()
{
	return m_endPt;
}

AcDbObjectId CLine::objId()
{
	return m_objId;
}

CString CLine::handleStr()
{
	AcDbHandle dHandle;
	dHandle = m_objId.handle();
	CString strHandle;
	TCHAR pName[256];
	dHandle.getIntoAsciiBuffer(pName);
	strHandle = pName;
	return strHandle;
}

double CLine::dist() const
{
	double dDist = acutDistance(asDblArray(m_startPt), asDblArray(m_endPt));
	return dDist;
}

double CLine::direction() const
{
	AcGeVector3d vDir = m_endPt - m_startPt;
	vDir = vDir.normalize();
	double dDir = vDir.angleOnPlane(AcGePlane::kXYPlane);
	return dDir;
}

AcGeVector3d CLine::vector() const
{
	AcGeVector3d vDir = m_endPt - m_startPt;
	vDir = vDir.normalize();
	return vDir;
}

bool CompareTwoEnt(CLine line1, CLine line2)
{
	/*double dDist1 = line1.dist();
	double dDist2 = line2.dist();
	double dDir1 = line1.direction();
	double dDir2 = line2.direction();

	if (abs(dDir1 - dDir2) > GeTol)
	{
		return false;
	}
	AcGePoint3d startPt1 = line1.startPt();
	AcGePoint3d startPt2 = line2.startPt();
	AcGePoint3d endPt1 = line1.endPt();
	AcGePoint3d endPt2 = line2.endPt();
	AcGeVector3d vec1,vec2;
	vec1 = startPt2 - startPt1;
	vec2 = endPt1 - endPt2;
	vec1 = vec1.normalize();
	vec2 = vec2.normalize();
	AcGeTol geTol;
	geTol.setEqualPoint(GeTol);
	if (startPt1.isEqualTo(startPt2, geTol))
	{
		if (line1.vector().isEqualTo(vec2, geTol))
		{
			return true;
		}
	}
	else if (endPt1.isEqualTo(endPt2, geTol))
	{
		if (line1.vector().isEqualTo(vec1, geTol))
		{
			return true;
		}
	}
	if (vec1.isEqualTo(vec2, geTol))
	{
		return true;
	}
	else
	{
		return false;
	}

	if (abs(dDist1 - dDist2) < GeTol)
	{
		return true;
	}
	return false;*/
	AcGeLine3d geLine1,geLine2;
	AcGePoint3d startPt1,endPt1,startPt2,endPt2;
	startPt1 = line1.startPt();
	endPt1 = line1.endPt();
	startPt2 = line2.startPt();
	endPt2 = line2.endPt();
	geLine1.set(startPt1, endPt2);
	AcGeTol geTol;
	geTol.setEqualPoint(GeTol);
	Adesk::Boolean bRet1,bRet2;
	bRet1 = geLine1.isOn(startPt2, geTol);
	bRet2 = geLine1.isOn(endPt2, geTol);
	return bRet1&&bRet2;
}


