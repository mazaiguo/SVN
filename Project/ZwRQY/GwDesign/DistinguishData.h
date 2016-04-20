#pragma once
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//分辨数据
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include "GDDataStore.h"
#include "Line.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
class CDistinguishData
{
public:
	CDistinguishData(void);
	~CDistinguishData(void);
	
	bool doIt();

private:
	//************************************
	// Method:    selectEnt
	// FullName:  CDistinguishData::selectEnt
	// Access:    private 
	// Returns:   bool
	// Qualifier:交互选择实体
	//************************************
	bool selectEnt();
	
	//************************************
	// Method:    collectData
	// FullName:  CDistinguishData::collectData
	// Access:    private 
	// Returns:   bool
	// Qualifier:收集数据
	//************************************
	bool collectData();

	//************************************
	// Method:    doData
	// FullName:  CDistinguishData::doData
	// Access:    private 
	// Returns:   bool
	// Qualifier: 处理数据
	//************************************
	bool doData();


private:
	CGDDataStore* m_pDataStore;
	vector<CLine> m_lineVec;
};
