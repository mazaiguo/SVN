#pragma once

class CLine
{
public:
	CLine(void);
	CLine(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId objId);
	~CLine(void);

	/*BOOL operator<(const CLine& tmpLine) const;
	BOOL operator==(const CLine& tmpLine) const;*/
	void setStartPt(AcGePoint3d pt);
	void setEndPt(AcGePoint3d pt);
	void setObjectId(AcDbObjectId objId);

	AcGePoint3d startPt();
	AcGePoint3d endPt();
	AcDbObjectId objId();
	CString  handleStr();

	double dist() const;
	double direction() const;
	AcGeVector3d vector() const;
private:
	AcGePoint3d m_startPt;
	AcGePoint3d m_endPt;
	AcDbObjectId m_objId;
};


//************************************
// Method:    CompareTwoLine
// FullName:  CompareTwoLine
// Access:    public static 
// Returns:   bool
// Qualifier: 如果是一样的返回true，不是一样的返回false
// 容许在一定误差范围内
// Parameter: CLine line1
// Parameter: CLine line2
//************************************
bool CompareTwoEnt(CLine line1, CLine line2);

