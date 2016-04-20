#pragma once

class CArc
{
public:
	CArc(void);
	~CArc(void);
	CArc( const AcGePoint3d& center, 
		double radius, 
		double startAngle, 
		double endAngle,
		AcDbObjectId objId);

	void setCenter(AcGePoint3d cenPt);
	void setRadius(double dRadius);
	void setStartAngle(double dStartAng);
	void setEndAngle(double dEndAng);
	void setObjId(AcDbObjectId objId);

	AcGePoint3d center();
	double radius();
	double startAng();
	double endAng();
	AcDbObjectId objId();


private:
	AcGePoint3d m_centerPt;
	double m_dRadius;
	double m_dStartAngle;
	double m_dEndAngle;
	AcDbObjectId m_objId;

};

//************************************
// Method:    CompareTwoLine
// FullName:  CompareTwoLine
// Access:    public static 
// Returns:   bool
// Qualifier: �����һ���ķ���true������һ���ķ���false
// ������һ����Χ��
// Parameter: CLine line1
// Parameter: CLine line2
//************************************
bool CompareTwoEnt(CArc line1, CArc line2);