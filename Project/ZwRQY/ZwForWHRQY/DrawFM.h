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

	
private:
	AcDbObjectIdArray m_idArrs;

};
