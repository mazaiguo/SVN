#pragma once

#include "ZwmToolKitExtLib.h"
#include "zwm.h"
#include "DataBaseInfo.h"
#include <map>
#include <vector>
#include <utility>
//using namespace std;
//操作bom表
class COperateBom
{
public:
	COperateBom(void);
	~COperateBom(void);
	//////////////////////////////////////////////////////////////////////////
	bool DoIt();

private:
	//选择实体
	bool SelectEnt();
	//将数据添加到bom
	bool AddDataToBom();
	//获取Bom表并将bom表的数据清空
	bool GetBom();
	//设置Bom表数据
	bool SetBom(CDataBaseInfo* pData, int nCount);
	//设置bom表单行数据
	ZwcadmBomRow SetBomCell();
	//param objId为块参照id
	void GetEntInfoById(AcDbObjectId objId);

private:
	ZwcadmApp m_appZwm;
	ZwcadmDb* m_pZwmDb;
	ZwcadmBom* m_pBom;
	//第一个参数记录流程图编号，第二个参数记录块里的信息及数量
	map<CString, pair<CDataBaseInfo*, int> > m_Data;
};
