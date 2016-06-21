#pragma once
typedef enum
{
	//AcGeLineSeg2d
} LinesRelation;

class CLine
{
public:
	CLine(void);
	CLine(AcGePoint2d startPoint, AcGePoint2d endPoint);
	CLine(AcGePoint3d startPoint, AcGePoint3d endPoint);
	virtual ~CLine(void);
public:
	AcGePoint2d startPoint(void);
	AcGePoint2d endPoint(void);
	AcGePoint2d minPoint(void);
	AcGePoint2d maxPoint(void);
	double length(void);
	double distanceTo(AcGePoint2d point, const AcGeTol& tol = AcGeContext::gTol);
	Acad::ErrorStatus transformBy(const AcGeMatrix2d& xform);
	int IsEndpoint(AcGePoint2d point, const AcGeTol& tol = AcGeContext::gTol);
	bool isPerpendicularTo(CLine* line, const AcGeTol& tol = AcGeContext::gTol);
	bool isConnectedTo(CLine* line, const AcGeTol& tol = AcGeContext::gTol);
	bool isHorizontal(void);
	bool isVertical(void);
	bool operator == (const CLine& line) const;
	bool operator != (const CLine& line) const;
    bool isEqualTo   (const CLine& line, const AcGeTol& tol = AcGeContext::gTol) const;
private:
	AcGePoint2d m_startPoint;
	AcGePoint2d m_endPoint;
};

