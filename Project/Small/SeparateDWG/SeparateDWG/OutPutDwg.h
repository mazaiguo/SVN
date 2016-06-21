#pragma once
#include <vector>
#include "FrameInfo.h"
using namespace std;

class COutPutDwg
{
public:
	COutPutDwg(void);
	~COutPutDwg(void);
	bool doIt();

private:
	bool sel();
	bool OutPutDwg(AcGePoint3d minPt, AcGePoint3d maxPt, CString strDwgFile);


	//º∆À„
	int JudgeAreaInOtherArea(AcGePoint3d curMinPt,AcGePoint3d curMaxPt,AcGePoint3d preMinPt,AcGePoint3d preMaxPt);
	int JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts);
	BOOL JudgePointInArea(AcGePoint3d insertPt, AcGePoint3d minPt, AcGePoint3d maxPt);
	bool getText(FrameInfo* pInfo);
	bool getWidthAndHeight(FrameInfo* pInfo);
	bool GetOutPutPath();
	void ZOOMWINDOW(AcGePoint3d minPt, AcGePoint3d maxPt);

private:
	vector<FrameInfo*> m_frames;
	CString m_strFile;
	CString m_strWidth;
	CString m_strHeight;
	CString m_strPath;
};
