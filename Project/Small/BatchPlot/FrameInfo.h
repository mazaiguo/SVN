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
	void GetRegion(double& dWidth, double& dHeight);//��ȡ��ǰ�Ŀ����߶�

	double GetFrameScale();//��ȡͼ��ı���
	bool GetFrameDirection();//��ȡͼ��ķ��򣬺��������
	CString GetDwgFrameSize();//��ȡͼ��ֽ��
	CString GetDwgFrame();//��ȡͼ��
	AcGePoint2d GetminPoint();//��ȡ��С��
	AcGePoint2d GetmaxPoint();//��ȡ����
	CString GetFileName();//��ȡ�ļ���

	void SetDwgFrame(LPCTSTR);//����ͼ��
	void SetFrameScale(const double);//���ñ���
	void SetDwgFrameSize(LPCTSTR);//����ͼ��ֽ��
	//CString CalcDwgFramebyScale(const double dScale);//ͨ��ͼֽ��С�ͱ����õ�ͼ��

	void SetFrameDirection(const bool);//���÷���

	void SetFileName(LPCTSTR);
	double m_dScale; // ͼֽ����
	bool m_bIsHorizontal;//�Ƿ�ˮƽ
	CString m_strDwgFrameSize;//ͼ��ֽ��
	CString m_strDwgFrame;//ͼ��
	CString m_strFileName;//ͼֽ��
};

