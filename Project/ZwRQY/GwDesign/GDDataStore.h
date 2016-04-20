#pragma once
//识别的管道数据存储
#include <vector>
#include <map>
#include "Line.h"
#include "Arc.h"
using namespace std;

class CGDDataStore
{
public:
	CGDDataStore(void);
	~CGDDataStore(void);

	vector<CLine> getLineData();
	vector<CArc> getArcData();

private:
	bool doRepeatData();

private:
	vector<CLine> m_lineVec;
	vector<CArc> m_arcVec;

};
