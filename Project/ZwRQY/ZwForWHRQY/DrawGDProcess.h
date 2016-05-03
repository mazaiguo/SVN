#pragma once
#include "ZdmDataInfo.h"
//控制绘制地面线的流程

#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CDrawGDProcess
{
public:
	CDrawGDProcess(void);
	~CDrawGDProcess(void);

//绘制管道线
	bool Draw();

	////************************************
	//// Method:    Insert
	//// FullName:  DrawDMXProcess::Insert
	//// Access:    public 
	//// Returns:   bool
	//// Qualifier: 在现有的图形上插入数据
	////************************************
	//bool Insert();


	////************************************
	//// Method:    Del
	//// FullName:  DrawDMXProcess::Del
	//// Access:    public 
	//// Returns:   bool
	//// Qualifier:在现有的图形上删除数据
	////************************************
	//bool Del();


	////************************************
	//// Method:    Mod
	//// FullName:  DrawDMXProcess::Mod
	//// Access:    public 
	//// Returns:   bool
	//// Qualifier:在现有图形上编辑数据
	////************************************
	//bool Mod();

private:
	//起始处输入管道管径、管底深和挖深，
	//从第二点开始可选择输入坡度计算距离及绘制相应的线，
	//或是输入管底深和挖深，通过两点的管底深和挖深计算坡度及绘制相应的线，
	//或是根据重大障碍物的垂直净距（交互输入）绘制相应的线
	//交互相关

	//获取管径
	bool GetPipeDiameter();

	bool GetKeyWord();


	//获取管底
	bool GetGuanDi();
	//
	bool GetWaShen();

	bool GetPodu();

	//是否有坡度
	bool GetVertical();


	
	
	//************************************
	// Method:    CurNumPositon
	// FullName:  DrawDMXProcess::CurNumPositon
	// Access:    private 
	// Returns:   cstring
	// Qualifier: 计算当前桩号所在的位置
	// Parameter: double dValue
	//************************************
	CString CurNumPosition(double dValue);


	//************************************
	// Method:    CurNumPosition
	// FullName:  DrawDMXProcess::CurNumPosition
	// Access:    private 
	// Returns:   CString
	// Qualifier:计算当前group所在位置
	// Parameter: CString strlabel
	//************************************
	CString CurNumPosition(CString strlabel);

	AcDbObjectId setlectEnt(CString strPromPt);
	
	//实体交互
	bool EntInteraction();


private:
	double m_dXScale;//x比例
	double m_dYScale;//y比例
	bool   m_bDrawJiedian;//是否绘制节点
	double m_dPipeDiameter;//管径
	CString m_strJiedian;
	double m_dGuandi;//管底深
	double m_dWashen;//挖深
	double m_dRealDmx;//实际地面线
	double m_dPodu;

	double m_dDesignDmxS;//第二个设计地面线
	double m_dRealDmxS;//第二个实际地面线
	AcGePoint3d m_basePt;
	int m_nCout;//当前计数
	CZdmDataInfo m_pZdmInfo;
	double m_dminElavation;
	double m_dmaxElavation;
	bool m_bHasBugle;//是否有坡度
};
