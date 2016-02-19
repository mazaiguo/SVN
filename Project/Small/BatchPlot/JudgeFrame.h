#pragma once
#include "StdAfx.h"
#include "FrameInfo.h"
#include "vector"

class CJudgeFrame
{
public:
	CJudgeFrame(void);
	virtual ~CJudgeFrame(void);
	
	std::vector<FrameInfo*> GetDataFromDwg();//选择模式
	
	void SetLayerName(LPCTSTR strLayerName);//设置过滤条件中的图层名
	void SetBlockName(LPCTSTR strBlockName);//设置过滤条件中的块名
	void SetMode(const int nMode);//设置选择模式
	void SetMultiDoc(const int nMultiDoc);
private:
	BOOL GetDataByLayerName();

	BOOL GetDataByBlockName();

	void GetDataBySanxian();

	BOOL SelectMode(const resbuf* filter);

	void SelectSpecail(const resbuf* filter);
	
	//计算
	int JudgeAreaInOtherArea(AcGePoint2d curMinPt,AcGePoint2d curMaxPt,AcGePoint2d preMinPt,AcGePoint2d preMaxPt);
	int JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts);
	BOOL JudgePointInArea(AcGePoint2d insertPt, AcGePoint2d minPt, AcGePoint2d maxPt);


private:
	CString m_strLayerName;
	CString m_strBlockName;
	std::vector<FrameInfo*> m_frames;
	int m_nMode;//选择图框的模式
	int m_nMultiDoc;//多文档模式

};
