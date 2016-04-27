#pragma once
#include "ZDMUtils.h"
class CDrawBiaochi
{
public:
	CDrawBiaochi(void);
	~CDrawBiaochi(void);

	//���û���
	bool setBasePt(AcGePoint3d basePt);
	//��ȡ����
	AcGePoint3d getBasePt() const;

	bool Draw();

	bool DrawNext();

private:
	//��ʼ����
	bool startDraw();
	//���Ʊ��
	AcDbObjectId DrawBiaoChi();
	//����ͼ��
	AcDbObjectId DrawBiaoge();
	
	//�����ṩ����֧�� 
	//��ȡX��������
	bool GetXScale();
	//��ȡY��������
	bool GetYScale();
	//��ȡ��ʼ���
	bool GetStartBG();
	//��ȡ��ֹ�������
	bool GetEndBG();

	bool GetStartZhuanghao();

	//���ݳ�ʼ����
	bool InitData();
	//�����ݴ���DWG�ļ��б���
	void SaveDataToDwg();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//����Ļ��ƹ���
	AcDbObjectId DrawTextAndBC(AcGePoint3d pt, CString strText, bool bFull = true);


	//�����ݼ���ѡ����
	void AppendIdToSS(AcDbObjectId objId);
	

private:
	AcGePoint3d m_basePt;//�����
	bool		m_bNeedWH;//�Ƿ���Ҫλ��
	ads_name	m_ssname;//��¼ѡ������
	double		m_dXScale;//xscale
	double		m_dYScale;//yscale
	int			m_nStartBG;//��ʼ���
	int			m_nEndBG;//��ֹ���
	double		m_dstartZhuanghao;//��ʼ׮��
	
	//���ƻ�������
	double		m_dXRatio;
	double		m_dYRatio;
};
