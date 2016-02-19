#pragma once
#include <list>


class FrameInfo
{
public:
	FrameInfo(void);
	virtual ~FrameInfo(void);
public:
	AcGePoint2d m_minPoint;
	AcGePoint2d m_maxPoint;
	AcGePoint2d centerPoint(void);
	double width(void);
	double height(void);
	bool IsContain(FrameInfo* frame);
	void CreateRect(void);
public:
	bool HighLightEntity();
	void GetRegion(double& dWidth, double& dHeight);//获取当前的宽度与高度

	double GetFrameScale();//获取图框的比例
	bool GetFrameDirection();//获取图框的方向，横向或纵向
	CString GetDwgFrameSize();//获取图幅纸张
	CString GetDwgFrame();//获取图幅
	AcGePoint2d GetminPoint();//获取最小点
	AcGePoint2d GetmaxPoint();//获取最大点
	CString GetFileName();//获取文件名

	void SetDwgFrame(LPCTSTR);//设置图幅
	void SetFrameScale(const double);//设置比例
	void SetDwgFrameSize(LPCTSTR);//设置图幅纸张
	//CString CalcDwgFramebyScale(const double dScale);//通过图纸大小和比例得到图幅

	void SetFrameDirection(const bool);//设置方向

	void SetFileName(LPCTSTR);
	double m_dScale; // 图纸比例
	bool m_bIsHorizontal;//是否水平
	CString m_strDwgFrameSize;//图幅纸张
	CString m_strDwgFrame;//图幅
	CString m_strFileName;//图纸名
};

