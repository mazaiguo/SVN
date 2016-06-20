#pragma once
#include <map>
using namespace  std;
class CDrawJSD
{
public:
	CDrawJSD(void);
	~CDrawJSD(void);
	bool draw();
	bool del();
private:
	AcGePoint3dArray getData();
	bool drawlineAndText();
	bool doIt();
	void makeGroup();
private:
	AcDbObjectIdArray m_idArrs; 
};
