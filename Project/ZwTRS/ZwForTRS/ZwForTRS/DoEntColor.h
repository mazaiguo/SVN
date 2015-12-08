#pragma once

class CDoEntColor
{
public:
	CDoEntColor(void);
	~CDoEntColor(void);
	
	bool SetEntFrame(AcGePoint3d minPt, AcGePoint3d maxPt);
	bool ReStoreFrame();

private:
	void DrawPlineLine(AcGePoint3dArray ptArr);
	AcDbObjectId m_PreId;
	double m_dWidth;
	int m_nColor;
};
