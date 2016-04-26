#pragma once
#include "ZDMUtils.h"
class CDrawBiaochi
{
public:
	CDrawBiaochi(void);
	~CDrawBiaochi(void);

	//设置基点
	bool setBasePt(AcGePoint3d basePt);
	//获取基点
	AcGePoint3d getBasePt() const;

	bool Draw();

	bool DrawNext();

private:
	//初始绘制
	bool startDraw();
	//绘制标尺
	AcDbObjectId DrawBiaoChi();
	//绘制图框
	AcDbObjectId DrawBiaoge();
	
	//交互提供数据支撑 
	//获取X比例数据
	bool GetXScale();
	//获取Y比例数据
	bool GetYScale();
	//获取起始标高
	bool GetStartBG();
	//获取终止标高数据
	bool GetEndBG();

	bool GetStartZhuanghao();

	//数据初始设置
	bool InitData();
	//将数据存入DWG文件中保存
	void SaveDataToDwg();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//具体的绘制过程
	AcDbObjectId DrawTextAndBC(AcGePoint3d pt, CString strText, bool bFull = true);


	//将数据加入选择集中
	void AppendIdToSS(AcDbObjectId objId);
	

private:
	AcGePoint3d m_basePt;//插入点
	bool		m_bNeedWH;//是否需要位号
	ads_name	m_ssname;//记录选择集数据
	double		m_dXScale;//xscale
	double		m_dYScale;//yscale
	int			m_nStartBG;//起始标高
	int			m_nEndBG;//终止标高
	double		m_dstartZhuanghao;//起始桩号
	
	//绘制基础数据
	double		m_dXRatio;
	double		m_dYRatio;
};
