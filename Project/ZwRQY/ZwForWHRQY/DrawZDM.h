#pragma once
//绘制纵断面图形
#include "ZdmDataInfo.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//绘制纵断面的具体流程
#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CDrawZDM
{
public:
	CDrawZDM(void);
	~CDrawZDM(void);
	CDrawZDM(CZdmDataInfo* pZDM);
	//处理编组
	AcDbObjectId add();

	void setData(CZdmDataInfo* pZDM);
	
	AcDbObjectId insert();
	//删除桩号
	bool del(CString strGroupName);
	//编辑数据
	bool mod(CString strGroupName);
	//绘制管道
	void setDrawGd(bool bIsDrawGd);
private:
	//初始数据获取
	bool initdata();
	//
	bool DrawLine(bool bIsDeFault = true);
	//绘制地面线相关的文字
	bool DrawDMText();

	bool DrawNextDMText();
	//绘制管道相关的文字
	bool DrawXMText();
	//绘制节点文字
	bool DrawJdText(AcGePoint3d basePt, AcGePoint3d TopPt, AcGePoint3d endPt);

	//************************************
	// Method:    DrawSMXLine
	// FullName:  CDrawZDM::DrawSMXLine
	// Access:    private 
	// Returns:   bool
	// Qualifier: 根据现状水面线数据及设计水面线数据绘制两条水面线
	//************************************
	bool DrawSMXLine(bool bIsDeFault = true);

	//************************************
	// Method:    AddObjToDict
	// FullName:  CDrawZDM::AddObjToDict
	// Access:    private 
	// Returns:   bool
	// Qualifier: 先找strlabel所在的group，然后打开往group中添加实体id
	// Parameter: CString strLabel
	// Parameter: AcDbObjectId objId
	//************************************
	bool AddObjToDict(CString strLabel, AcDbObjectId objId);

	bool ModifyDictbyInt(int nCur, bool bIsAdded = true);

	//************************************
	// Method:    ChangeDictName
	// FullName:  CDrawZDM::ChangeDictName
	// Access:    private 
	// Returns:   bool
	// Qualifier:将原始strPreGroupName改为strGroupName
	// Parameter: CString strGroupName
	// Parameter: CString strPreGroupName
	// Parameter: int nCount
	// Parameter: bool bIsDeFault
	//************************************
	bool ChangeDictName(CString strGroupName, CString strPreGroupName, int nCount, bool bIsDeFault = true);
	bool EditDict(int nCur);
private:
	CZdmDataInfo m_pZDM;
	CZdmDataInfo m_preData;
	AcDbObjectIdArray m_idArrs;
	double m_dLen;
	double m_dWidth;
	double m_dXScale;
	double m_dYScale;
	bool m_bIsDel;//是否删除
	bool m_bIsDrawGD;//是否绘制管道
	AcGePoint3d m_basePt;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//绘制管道
class CDrawGd
{
public:
	CDrawGd(void);
	~CDrawGd(void);
	bool add(CZdmDataInfo pZDM);
	bool del(CZdmDataInfo pZDM);
	bool insert(CZdmDataInfo pZDM);
	bool mod(CZdmDataInfo pZDM);

private:
	//初始数据获取
	bool initdata();
	//绘制燃气管道
	bool drawGd();
	//绘制管道示意图
	bool drawGdflat(AcGePoint3d pretmpPt, AcGePoint3d tmpPt);
	//绘制管底，挖深文字
	bool drawText(AcGePoint3d basePt);
	//绘制坡度，距离文字及坡度线
	bool drawTextAndLine(AcGePoint3d pretmpPt, AcGePoint3d tmpPt);

	//绘制椭圆或者圆
	bool drawCirlceOrEllipse();
private:
	CZdmDataInfo m_pZDM;
	CZdmDataInfo m_preData;
	AcDbObjectIdArray m_idArrs;
	double m_dLen;
	double m_dWidth;
	double m_dXScale;
	double m_dYScale;
	bool m_bIsDel;//是否删除
	bool m_bIsDrawGD;//是否绘制管道
	int m_nCount;
	AcGePoint3d m_basePt;
};