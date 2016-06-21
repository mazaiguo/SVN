#include "StdAfx.h"
#include "Line.h"


CLine::CLine(void)
	: m_startPoint(0, 0)
	, m_endPoint(0, 0)
{
}


CLine::CLine(AcGePoint2d startPoint, AcGePoint2d endPoint)
	: m_startPoint(startPoint)
	, m_endPoint(endPoint)
{
}


CLine::CLine(AcGePoint3d startPoint, AcGePoint3d endPoint)
	: m_startPoint(startPoint.x, startPoint.y)
	, m_endPoint(endPoint.x, endPoint.y)
{
}


CLine::~CLine(void)
{
}


bool CLine::operator == (const CLine& line) const
{
	return this->isEqualTo(line);
}


bool CLine::operator != (const CLine& line) const
{
	return !this->isEqualTo(line);
}


bool CLine::isEqualTo   (const CLine& line, const AcGeTol& tol/* = AcGeContext::gTol*/) const
{
	return m_startPoint.isEqualTo(line.m_startPoint, tol) && m_endPoint.isEqualTo(line.m_endPoint, tol);
}


Acad::ErrorStatus CLine::transformBy(const AcGeMatrix2d& xform)
{
	m_startPoint.transformBy(xform);
	m_endPoint.transformBy(xform);
	return Acad::eOk;
}


AcGePoint2d CLine::startPoint(void)
{
	return m_startPoint;
}


AcGePoint2d CLine::endPoint(void)
{
	return m_endPoint;
}


AcGePoint2d CLine::minPoint(void)
{
	AcGePoint2d minPt;
	if (m_startPoint.x < m_endPoint.x)
		minPt.x = m_startPoint.x;
	else
		minPt.x = m_endPoint.x;
	if (m_startPoint.y < m_endPoint.y)
		minPt.y = m_startPoint.y;
	else
		minPt.y = m_endPoint.y;
	return minPt;
}


AcGePoint2d CLine::maxPoint(void)
{
	AcGePoint2d maxPt;
	if (m_startPoint.x < m_endPoint.x)
		maxPt.x = m_endPoint.x;
	else
		maxPt.x = m_startPoint.x;
	if (m_startPoint.y < m_endPoint.y)
		maxPt.y = m_endPoint.y;
	else
		maxPt.y = m_startPoint.y;
	return maxPt;
}


double CLine::length(void)
{
	return (m_endPoint - m_startPoint).length();
}


double CLine::distanceTo(AcGePoint2d point, const AcGeTol& tol/* = AcGeContext::gTol*/)
{
	AcGeLineSeg2d line(startPoint(), endPoint());
	return line.distanceTo(point, tol);
}


int CLine::IsEndpoint(AcGePoint2d point, const AcGeTol& tol/* = AcGeContext::gTol*/)
{
	if (m_startPoint.isEqualTo(point, tol))
		return 1;
	if (m_endPoint.isEqualTo(point, tol))
		return 2;
	return 0;
}

bool CLine::isPerpendicularTo(CLine* line, const AcGeTol& tol/* = AcGeContext::gTol*/)
{
	AcGeVector2d vec1, vec2;
	vec1 = startPoint() - endPoint();
	vec2 = line->startPoint() - line->endPoint();
	bool bIsRet = abs(vec1.x*vec2.x +vec1.y+vec2.y)<GeTol;
	return bIsRet;
}

bool CLine::isConnectedTo(CLine* line, const AcGeTol& tol/* = AcGeContext::gTol*/)
{
	bool bo1 = IsEndpoint(line->startPoint(), tol);
	bool bo2 = IsEndpoint(line->endPoint(), tol);
	bool bRet = bo1 || bo2;
	return bRet;
}

bool CLine::isHorizontal(void)
{
	return abs(m_startPoint.y - m_endPoint.y) < 0.01;
}

bool CLine::isVertical(void)
{
	return abs(m_startPoint.x - m_endPoint.x) < 0.01;
}
