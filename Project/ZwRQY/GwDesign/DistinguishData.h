#pragma once
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//�ֱ�����
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
	// Qualifier:����ѡ��ʵ��
	//************************************
	bool selectEnt();
	
	//************************************
	// Method:    collectData
	// FullName:  CDistinguishData::collectData
	// Access:    private 
	// Returns:   bool
	// Qualifier:�ռ�����
	//************************************
	bool collectData();

	//************************************
	// Method:    doData
	// FullName:  CDistinguishData::doData
	// Access:    private 
	// Returns:   bool
	// Qualifier: ��������
	//************************************
	bool doData();


private:
	CGDDataStore* m_pDataStore;
	vector<CLine> m_lineVec;
};
