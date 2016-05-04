#pragma once
//�����ݶ���ͼ��
#include "ZdmDataInfo.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//�����ݶ���ľ�������
#ifdef WRQ_ZDM_MODULE //�ڴ�������DLL�Ĺ������Ȱ� 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CDrawZDM
{
public:
	CDrawZDM(void);
	~CDrawZDM(void);
	CDrawZDM(CZdmDataInfo* pZDM);
	//�������
	AcDbObjectId add();

	void setData(CZdmDataInfo* pZDM);
	
	AcDbObjectId insert();
	//ɾ��׮��
	bool del(CString strGroupName);
	//�༭����
	bool mod(CString strGroupName);
	//���ƹܵ�
	void setDrawGd(bool bIsDrawGd);
private:
	//��ʼ���ݻ�ȡ
	bool initdata();
	//
	bool DrawLine(bool bIsDeFault = true);
	//���Ƶ�������ص�����
	bool DrawDMText();

	bool DrawNextDMText();
	//���ƹܵ���ص�����
	bool DrawXMText();
	//���ƽڵ�����
	bool DrawJdText(AcGePoint3d basePt, AcGePoint3d TopPt, AcGePoint3d endPt);

	//************************************
	// Method:    DrawSMXLine
	// FullName:  CDrawZDM::DrawSMXLine
	// Access:    private 
	// Returns:   bool
	// Qualifier: ������״ˮ�������ݼ����ˮ�������ݻ�������ˮ����
	//************************************
	bool DrawSMXLine(bool bIsDeFault = true);

	//************************************
	// Method:    AddObjToDict
	// FullName:  CDrawZDM::AddObjToDict
	// Access:    private 
	// Returns:   bool
	// Qualifier: ����strlabel���ڵ�group��Ȼ�����group�����ʵ��id
	// Parameter: CString strLabel
	// Parameter: AcDbObjectId objId
	//************************************
	bool AddObjToDict(CString strLabel, AcDbObjectId objId);

	bool ModifyDictbyInt(int nCur, bool bIsAdded = true);

	//************************************
	// Method:    ChangeDictName
	// FullName:  CDrawZDM::ChangeDictName
	// Access:    private 
	// Returns:   bool
	// Qualifier:��ԭʼstrPreGroupName��ΪstrGroupName
	// Parameter: CString strGroupName
	// Parameter: CString strPreGroupName
	// Parameter: int nCount
	// Parameter: bool bIsDeFault
	//************************************
	bool ChangeDictName(CString strGroupName, CString strPreGroupName, int nCount, bool bIsDeFault = true);
	bool EditDict(int nCur);
private:
	CZdmDataInfo m_pZDM;
	CZdmDataInfo m_preData;
	AcDbObjectIdArray m_idArrs;
	double m_dLen;
	double m_dWidth;
	double m_dXScale;
	double m_dYScale;
	bool m_bIsDel;//�Ƿ�ɾ��
	bool m_bIsDrawGD;//�Ƿ���ƹܵ�
	AcGePoint3d m_basePt;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���ƹܵ�
class CDrawGd
{
public:
	CDrawGd(void);
	~CDrawGd(void);
	bool add(CZdmDataInfo pZDM);
	bool del(CZdmDataInfo pZDM);
	bool insert(CZdmDataInfo pZDM);
	bool mod(CZdmDataInfo pZDM);

private:
	//��ʼ���ݻ�ȡ
	bool initdata();
	//����ȼ���ܵ�
	bool drawGd();
	//���ƹܵ�ʾ��ͼ
	bool drawGdflat(AcGePoint3d pretmpPt, AcGePoint3d tmpPt);
	//���ƹܵף���������
	bool drawText(AcGePoint3d basePt);
	//�����¶ȣ��������ּ��¶���
	bool drawTextAndLine(AcGePoint3d pretmpPt, AcGePoint3d tmpPt);

	//������Բ����Բ
	bool drawCirlceOrEllipse();
private:
	CZdmDataInfo m_pZDM;
	CZdmDataInfo m_preData;
	AcDbObjectIdArray m_idArrs;
	double m_dLen;
	double m_dWidth;
	double m_dXScale;
	double m_dYScale;
	bool m_bIsDel;//�Ƿ�ɾ��
	bool m_bIsDrawGD;//�Ƿ���ƹܵ�
	int m_nCount;
	AcGePoint3d m_basePt;
};