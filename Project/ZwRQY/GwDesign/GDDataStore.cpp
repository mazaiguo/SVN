#include "StdAfx.h"
#include "GDDataStore.h"

CGDDataStore::CGDDataStore(void)
{
}

CGDDataStore::~CGDDataStore(void)
{
}

vector<CLine> CGDDataStore::getLineData()
{
	return m_lineVec;
}

vector<CArc> CGDDataStore::getArcData()
{	
	return m_arcVec;
}

bool CGDDataStore::doRepeatData()
{
	for (vector<CLine>::iterator iter = m_lineVec.begin();
		iter != m_lineVec.end();
		++iter)
	{

	}

	return true;
}
