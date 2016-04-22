#pragma once
//�����ݶ���ͼ��
#include "ZDMUtils.h"
#include "BcUtils.h"
#include "ZdmDataInfo.h"
#include "CBiaochiForRQY.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//�����ݶ���ľ�������
class CDrawZDM
{
public:
	CDrawZDM(void);
	~CDrawZDM(void);
	CDrawZDM(CZdmDataInfo* pZDM);
	//�������
	AcDbObjectId Draw();

	void setData(CZdmDataInfo* pZDM);
	
	AcDbObjectId insert();
	//ɾ��׮��
	bool del(CString strGroupName);
	//�༭����
	bool mod(CString strGroupName);
private:
	//��ʼ���ݻ�ȡ
	bool initdata();
	//
	bool DrawLine(bool bIsDeFault = true);
	//���Ƶ�������ص�����
	bool DrawDMText();
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

private:
	CZdmDataInfo m_pZDM;
	CZdmDataInfo m_preData;
	AcDbObjectIdArray m_idArrs;
	double m_dLen;
	double m_dWidth;
	double m_dXScale;
	double m_dYScale;
};
