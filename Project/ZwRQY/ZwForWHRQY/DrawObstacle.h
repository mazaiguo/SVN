#pragma once
//处理障碍物的绘制
#include "ZdmDataInfo.h"
class CDrawObstacle
{
public:
	CDrawObstacle(void);
	CDrawObstacle(int nIndex, CString strName);
	~CDrawObstacle(void);
	//************************************
	// Method:    draw
	// FullName:  CDrawObstacle::draw
	// Access:    public 
	// Returns:   bool
	// Qualifier:	
	//************************************
	bool doIt();
	

	bool del();

	CString getName();
	void setName(LPCTSTR strName);
	
	int getIndex();
	void setIndex(int nIndex);

private:
	bool getZhuanghao();
	//************************************
	// Method:    gethasBase
	// FullName:  CDrawObstacle::gethasBase
	// Access:    private 
	// Returns:   bool
	// Qualifier: 询问是否有基础
	//************************************
	bool gethasBase();
	//************************************
	// Method:    getTopOrBottom
	// FullName:  CDrawObstacle::getTopOrBottom
	// Access:    private 
	// Returns:   bool
	// Qualifier: 询问是管顶还是管底
	//************************************
	bool getTopOrBottom();
	bool GetGuanDing();
	bool GetGuandi();
	//////////////////////////////////////////////////////////////////////////
	bool drawCircle();
	bool drawRectangle();
	bool drawOther();

	
	//////////////////////////////////////////////////////////////////////////
	bool verifyValue(double dValue);
	bool insertRef();
	bool drawText();
	bool drawCirlceOrEllipse();
	void makeGroup();
	bool GetUseBase();
	bool getHeightOrWidth();
	bool getRecHeight();
	bool getRecWidth();
	void drawRec(double dHeigth, double dWidth);
	bool verifyIntersect();
	void modCurZh();
	void editGroupName(CString strPreName, CString strGroupName);
	map<int, CString> getAllData(int nCount = 0);
	void selAllJCGroup(int index, bool bIsIncreased = true);
	map<int, int> selEnt();
	void editData(map<int, int> info);
	bool JudgeDir(AcGePoint3d tmpPt, AcGePoint3d minPt, AcGePoint3d maxPt);
	//bool GetBaseHeight();
	//bool GetBaseWidth();
	enum type
	{
		circleType,
		rectangleType,
		otherType,
	};
private:
	CString m_strName;//从外部传进来的名称
	int m_nIndex;//序列
	CZdmDataInfo m_zdmdata;
	CZdmDataInfo m_nextZdm;
	CZdmDataInfo m_preZdm;
	CString m_strCount;

	AcDbObjectId m_objId;
	double m_dMin;//最小范围
	double m_dMax;//最大范围
	double m_dYBase;//基础数据
	double m_dXBase;
	double m_dHeight;//插入位置
	double m_dPipeDiameter;//管径
	double m_dBaseHeight;
	double m_dXScale;
	double m_dYScale;
	bool m_bIsModGd;
	double m_dFlag;//标识
	//double m_dBaseWidth;
	bool m_bIsGdingType;//管道类型，管顶或管底
	AcDbObjectIdArray m_idArrs;
	CString m_strText;//文字内容
	AcGePoint3d m_guandiPt;
	AcGePoint3d m_guandiTopPt;

	AcGePoint3d m_SpecialPt;
};

//////////////////////////////////////////////////////////////////////////
/*
	1.drawcircle
	2.drawreactangle
	3.drawother

	1.交互，管顶，管底，基础
*/
//////////////////////////////////////////////////////////////////////////