#pragma once
#include "ZdmDataInfo.h"

//控制绘制地面线的流程

#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API DrawDMXProcess
{
public:
	DrawDMXProcess(void);
	~DrawDMXProcess(void);
	//绘制地面线及表格线
	bool Draw();

	//************************************
	// Method:    Insert
	// FullName:  DrawDMXProcess::Insert
	// Access:    public 
	// Returns:   bool
	// Qualifier: 在现有的图形上插入数据
	//************************************
	bool Insert();


	//************************************
	// Method:    Del
	// FullName:  DrawDMXProcess::Del
	// Access:    public 
	// Returns:   bool
	// Qualifier:在现有的图形上删除数据
	//************************************
	bool Del();


	//************************************
	// Method:    Mod
	// FullName:  DrawDMXProcess::Mod
	// Access:    public 
	// Returns:   bool
	// Qualifier:在现有图形上编辑数据
	//************************************
	bool Mod();

private:
	//交互相关
	int GetZhuanghao();
	//是否节点
	bool GetIsJiedian();
	//
	bool GetSJDmHeight();

	bool GetXzDmHeight();

	//是否有坡度
	bool GetIsPd();

	//获取坡度高度
	bool GetPdHeight();

	bool GetSJDmHeightS();

	bool GetXzDmHeightS();

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
	double m_dZhuanghao;//桩号
	CString m_strJiedian;
	double m_dSJDmHeight;//设计地面高
	double m_dXzDmHeight;//现状地面高

	double m_dDesignDmxS;//第二个设计地面线
	double m_dRealDmxS;//第二个实际地面线
	AcGePoint3d m_basePt;
	int m_nCout;//当前计数
	CZdmDataInfo* m_pZdmInfo;
	double m_dminElavation;
	double m_dmaxElavation;
	bool m_bHasBugle;//是否有坡度
	//AcDbObjectIdArray m_GroupIds;

};
