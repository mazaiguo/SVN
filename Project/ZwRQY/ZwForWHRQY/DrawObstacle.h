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
	double m_dMin;//最小范围
	double m_dMax;//最大范围
	double m_dBase;//基础数据
	double m_dHeight;//插入位置
	double m_dPipeDiameter;//管径
	bool m_bIsGdType;//管道类型，管顶或管底
	AcDbObjectIdArray m_idArrs;
	CString m_strText;//文字内容
};

//////////////////////////////////////////////////////////////////////////
/*
	1.drawcircle
	2.drawreactangle
	3.drawother

	1.交互，管顶，管底，基础
*/
//////////////////////////////////////////////////////////////////////////