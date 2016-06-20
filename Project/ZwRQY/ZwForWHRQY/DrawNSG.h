#pragma once
//»æÖÆÄýË®¸×
class CDrawNSG
{
public:
	CDrawNSG(void);
	~CDrawNSG(void);
	bool draw();
	bool del();
private:
	AcGePoint3dArray getData();
	bool drawlineAndText();
	bool doIt();
	void makeGroup();
	bool getZhuanghao();
	bool GetDescription();
	bool insert();
	CString CurNumPosition(double m_dZhuanghao, bool& bIsExisted);
	bool insertGdBlk(AcGePoint3d insertPt);
	bool insertUp(AcGePoint3d m_blkInsert, double dRotate);
private:
	AcDbObjectIdArray m_idArrs; 
	double m_dZhuanghao;
	CString m_strNo;
	AcGePoint3d m_blkInsert;
};
