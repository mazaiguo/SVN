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
	

	bool del();

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
	bool verifyIntersect();
	void modCurZh();
	void editGroupName(CString strPreName, CString strGroupName);
	map<int, CString> getAllData(int nCount = 0);
	void selAllJCGroup(int index, bool bIsIncreased = true);
	map<int, int> selEnt();
	void editData(map<int, int> info);
	bool JudgeDir(AcGePoint3d tmpPt, AcGePoint3d minPt, AcGePoint3d maxPt);
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
	CZdmDataInfo m_nextZdm;
	CZdmDataInfo m_preZdm;
	CString m_strCount;

	AcDbObjectId m_objId;
	double m_dMin;//��С��Χ
	double m_dMax;//���Χ
	double m_dYBase;//��������
	double m_dXBase;
	double m_dHeight;//����λ��
	double m_dPipeDiameter;//�ܾ�
	double m_dBaseHeight;
	double m_dXScale;
	double m_dYScale;
	bool m_bIsModGd;
	double m_dFlag;//��ʶ
	//double m_dBaseWidth;
	bool m_bIsGdingType;//�ܵ����ͣ��ܶ���ܵ�
	AcDbObjectIdArray m_idArrs;
	CString m_strText;//��������
	AcGePoint3d m_guandiPt;
	AcGePoint3d m_guandiTopPt;

	AcGePoint3d m_SpecialPt;
};

//////////////////////////////////////////////////////////////////////////
/*
	1.drawcircle
	2.drawreactangle
	3.drawother

	1.�������ܶ����ܵף�����
*/
//////////////////////////////////////////////////////////////////////////