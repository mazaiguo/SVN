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
};

//////////////////////////////////////////////////////////////////////////
/*
	1.drawcircle
	2.drawreactangle
	3.drawother

	1.交互，管顶，管底，基础
*/
//////////////////////////////////////////////////////////////////////////