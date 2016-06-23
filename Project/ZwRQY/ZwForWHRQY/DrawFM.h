#pragma once

//////////////////////////////////////////////////////////////////////////
//绘制阀门
//阀门所在图层FM-RQ
class CDrawFM
{
public:
	CDrawFM(void);
	~CDrawFM(void);

	bool doIt();
	bool del();
	
private:
	//获取桩号
	bool GetZhuanghao();

	//************************************
	// Method:    GetDescription
	// FullName:  CDrawFM::GetDescription
	// Access:    private 
	// Returns:   bool
	// Qualifier：获取说明文字
	//************************************
	bool GetDescription();

	Acad::ErrorStatus CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds);
	
	bool insertGdBlk(AcGePoint3d insertPt);
	
	CString CurNumPosition( double dValue, bool& bIsExisted);
	bool insert();
	void insertUp(AcGePoint3d blkInsert, double dRotate);
	void drawLineAndText();
	bool GetZuoFa();
private:
	AcDbObjectIdArray m_idArrs;
	AcDbObjectId m_layerId;
	CString m_strNo;
	double m_dZhuanghao;
	AcGePoint3d m_blkInsert;
	CString m_strZF;
	double m_dScale;
};
