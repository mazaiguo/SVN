#pragma once
#include "ZDMUtils.h"
#include "ZdmDataInfo.h"
#include "CBiaochiForRQY.h"
//控制绘制地面线的流程
class DrawDMXProcess
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
	bool GetStartZhuanghao();
	//是否节点
	bool GetIsJiedian();
	//
	bool GetSJDmHeight();

	bool GetXzDmHeight();

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
	// Method:    EraseEntFromDict
	// FullName:  DrawDMXProcess::EraseEntFromDict
	// Access:    private 
	// Returns:   bool
	// Qualifier: 操作dictionary删除实体
	// Parameter: AcDbObjectId groupId
	//************************************
	bool EraseEntFromDict(CString strGroupName);

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

private:
	double m_dXScale;//x比例
	double m_dYScale;//y比例
	bool   m_bDrawJiedian;//是否绘制节点
	double m_dZhuanghao;//桩号
	double m_dstartZhuanghao;//其实桩号
	CString m_strJiedian;
	double m_dSJDmHeight;//设计地面高
	double m_dXzDmHeight;//现状地面高
	AcGePoint3d m_basePt;
	int m_nCout;//当前计数
	CZdmDataInfo* m_pZdmInfo;
	double m_dminElavation;
	double m_dmaxElavation;
	//AcDbObjectIdArray m_GroupIds;

};
