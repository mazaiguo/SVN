#include "StdAfx.h"
#include "Arc.h"

CArc::CArc(void)
{

}

CArc::~CArc(void)
{
}

CArc::CArc(const AcGePoint3d& center, double radius, double startAngle, double endAngle, AcDbObjectId objId)
{
	m_centerPt = center;
	m_dRadius = radius;
	m_dStartAngle = startAngle;
	m_dEndAngle = endAngle;
	m_objId = objId;
}

void CArc::setCenter(AcGePoint3d cenPt)
{
	m_centerPt = cenPt;
}

void CArc::setRadius(double dRadius)
{
	m_dRadius = dRadius;
}

void CArc::setStartAngle(double dStartAng)
{
	m_dStartAngle = dStartAng;
}

void CArc::setEndAngle(double dEndAng)
{
	m_dEndAngle = dEndAng;
}

void CArc::setObjId(AcDbObjectId objId)
{	
	m_objId = objId;
}

AcGePoint3d CArc::center()
{
	return m_centerPt;
}

double CArc::radius()
{
	return m_dRadius;
}

double CArc::startAng()
{
	return m_dStartAngle;
}

double CArc::endAng()
{
	return m_dEndAngle;
}

AcDbObjectId CArc::objId()
{
	return m_objId;
}

bool CompareTwoEnt(CArc line1, CArc line2)
{
	AcGeTol geTol;
	geTol.setEqualPoint(GeTol);
	if ((line1.center().isEqualTo(line2.center(), geTol)))
	{
		if (line1.startAng() - line2.startAng() >= 0)
		{
			if (line1.endAng() - line2.endAng() <= 0)
			{
				//line2°üº¬line1
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (line1.endAng() - line2.endAng() <= 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
