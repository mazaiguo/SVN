#pragma once
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//分辨数据
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include "Line.h"
#include "COperatePL.h"
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

	//识别完成后获得所有的序号与管道的id


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


	AcDbObjectId drawPipe(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId startId, AcDbObjectId endId);
	AcDbObjectId drawXh(AcGePoint3d basePt, vector<AcDbObjectId> info);

	AcGePoint3d stringToPt(CString strValue);
private:
	vector<AcDbObjectId> m_PlineVec;
	map<CString, AcDbObjectId> m_MulData;
	map<CString, AcDbObjectId> m_MapInfo;
	map<CString, vector<AcDbObjectId> > m_dataInfo;
};
