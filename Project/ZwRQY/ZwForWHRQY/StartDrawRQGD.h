#pragma once
#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
#include "ZdmDataInfo.h"
class ZDM_DLL_API CStartDrawRQGD
{
public:
	CStartDrawRQGD(void);
	~CStartDrawRQGD(void);
	bool start();
	//创建新的地面线
	bool newLine();
	//在原有的数据上继续
	bool oldLine();
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	bool Draw();

	bool DrawNext();

	//验证是否符合地面线的高度
	bool verifyHeight(double dHeight);
	//绘制实体并组合成一个组
	

	//************************************
	// Method:    doUndo
	// FullName:  DrawDMXProcess::doUndo
	// Access:    private 
	// Returns:   bool
	// Qualifier: 处理undo相关的东西
	//************************************
	bool doUndo();
private:
	bool m_bIsNode;//是否有节点
	AcGePoint3d m_basePt;//基点

	int m_nCout;
	double m_dXScale;//x比例
	double m_dYScale;//y比例
	bool   m_bDrawJiedian;//是否绘制节点
	double m_dPipeDiameter;//管径
	CString m_strJiedian;
	double m_dGuandi;//管底深
	double m_dWashen;//挖深
	double m_dRealDmx;//实际地面线
	double m_dPodu;
	CZdmDataInfo m_pZdmInfo;
};
