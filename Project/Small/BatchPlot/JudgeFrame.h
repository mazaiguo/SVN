#pragma once
#include "StdAfx.h"
#include "FrameInfo.h"
#include "vector"

class CJudgeFrame
{
public:
	CJudgeFrame(void);
	virtual ~CJudgeFrame(void);
	
	std::vector<FrameInfo*> GetDataFromDwg();//ѡ��ģʽ
	
	void SetLayerName(LPCTSTR strLayerName);//���ù��������е�ͼ����
	void SetBlockName(LPCTSTR strBlockName);//���ù��������еĿ���
	void SetMode(const int nMode);//����ѡ��ģʽ
	void SetMultiDoc(const int nMultiDoc);
private:
	BOOL GetDataByLayerName();

	BOOL GetDataByBlockName();

	void GetDataBySanxian();

	BOOL SelectMode(const resbuf* filter);

	void SelectSpecail(const resbuf* filter);
	
	//����
	int JudgeAreaInOtherArea(AcGePoint2d curMinPt,AcGePoint2d curMaxPt,AcGePoint2d preMinPt,AcGePoint2d preMaxPt);
	int JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts);
	BOOL JudgePointInArea(AcGePoint2d insertPt, AcGePoint2d minPt, AcGePoint2d maxPt);


private:
	CString m_strLayerName;
	CString m_strBlockName;
	std::vector<FrameInfo*> m_frames;
	int m_nMode;//ѡ��ͼ���ģʽ
	int m_nMultiDoc;//���ĵ�ģʽ

};
