#pragma once
//�����ϰ���Ļ���
#include "ZdmDataInfo.h"
class CDrawObstacle
{
public:
	CDrawObstacle(void);
	CDrawObstacle(int nIndex, CString strName);
	~CDrawObstacle(void);
	//************************************
	// Method:    draw
	// FullName:  CDrawObstacle::draw
	// Access:    public 
	// Returns:   bool
	// Qualifier:	
	//************************************
	bool doIt();
	

	CString getName();
	void setName(LPCTSTR strName);
	
	int getIndex();
	void setIndex(int nIndex);

private:
	bool getZhuanghao();
	//************************************
	// Method:    gethasBase
	// FullName:  CDrawObstacle::gethasBase
	// Access:    private 
	// Returns:   bool
	// Qualifier: ѯ���Ƿ��л���
	//************************************
	bool gethasBase();
	//************************************
	// Method:    getTopOrBottom
	// FullName:  CDrawObstacle::getTopOrBottom
	// Access:    private 
	// Returns:   bool
	// Qualifier: ѯ���ǹܶ����ǹܵ�
	//************************************
	bool getTopOrBottom();
	bool GetGuanDing();
	bool GetGuandi();
	//////////////////////////////////////////////////////////////////////////
	bool drawCircle();
	bool drawRectangle();
	bool drawOther();

	
	//////////////////////////////////////////////////////////////////////////
	bool verifyValue(double dValue);
	bool insertRef();
	bool drawText();
	bool drawCirlceOrEllipse();
	void makeGroup();
	bool GetUseBase();
	bool getHeightOrWidth();
	bool getRecHeight();
	bool getRecWidth();
	void drawRec(double dHeigth, double dWidth);
	//bool GetBaseHeight();
	//bool GetBaseWidth();
	enum type
	{
		circleType,
		rectangleType,
		otherType,
	};
private:
	CString m_strName;//���ⲿ������������
	int m_nIndex;//����
	CZdmDataInfo m_zdmdata;
	double m_dMin;//��С��Χ
	double m_dMax;//���Χ
	double m_dBase;//��������
	double m_dHeight;//����λ��
	double m_dPipeDiameter;//�ܾ�
	double m_dBaseHeight;
	//double m_dBaseWidth;
	bool m_bIsGdingType;//�ܵ����ͣ��ܶ���ܵ�
	AcDbObjectIdArray m_idArrs;
	CString m_strText;//��������
};

//////////////////////////////////////////////////////////////////////////
/*
	1.drawcircle
	2.drawreactangle
	3.drawother

	1.�������ܶ����ܵף�����
*/
//////////////////////////////////////////////////////////////////////////