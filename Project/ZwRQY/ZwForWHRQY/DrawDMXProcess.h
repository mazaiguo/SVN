#pragma once
#include "ZDMUtils.h"
#include "ZdmDataInfo.h"
#include "CBiaochiForRQY.h"
//���ƻ��Ƶ����ߵ�����
class DrawDMXProcess
{
public:
	DrawDMXProcess(void);
	~DrawDMXProcess(void);
	//���Ƶ����߼������
	bool Draw();

	//************************************
	// Method:    Insert
	// FullName:  DrawDMXProcess::Insert
	// Access:    public 
	// Returns:   bool
	// Qualifier: �����е�ͼ���ϲ�������
	//************************************
	bool Insert();


	//************************************
	// Method:    Del
	// FullName:  DrawDMXProcess::Del
	// Access:    public 
	// Returns:   bool
	// Qualifier:�����е�ͼ����ɾ������
	//************************************
	bool Del();


	//************************************
	// Method:    Mod
	// FullName:  DrawDMXProcess::Mod
	// Access:    public 
	// Returns:   bool
	// Qualifier:������ͼ���ϱ༭����
	//************************************
	bool Mod();

private:
	//�������
	int GetZhuanghao();
	bool GetStartZhuanghao();
	//�Ƿ�ڵ�
	bool GetIsJiedian();
	//
	bool GetSJDmHeight();

	bool GetXzDmHeight();

	//��֤�Ƿ���ϵ����ߵĸ߶�
	bool verifyHeight(double dHeight);
	//����ʵ�岢��ϳ�һ����
	

	//************************************
	// Method:    doUndo
	// FullName:  DrawDMXProcess::doUndo
	// Access:    private 
	// Returns:   bool
	// Qualifier: ����undo��صĶ���
	//************************************
	bool doUndo();
	
	//************************************
	// Method:    EraseEntFromDict
	// FullName:  DrawDMXProcess::EraseEntFromDict
	// Access:    private 
	// Returns:   bool
	// Qualifier: ����dictionaryɾ��ʵ��
	// Parameter: AcDbObjectId groupId
	//************************************
	bool EraseEntFromDict(CString strGroupName);

	//************************************
	// Method:    CurNumPositon
	// FullName:  DrawDMXProcess::CurNumPositon
	// Access:    private 
	// Returns:   cstring
	// Qualifier: ���㵱ǰ׮�����ڵ�λ��
	// Parameter: double dValue
	//************************************
	CString CurNumPosition(double dValue);


	//************************************
	// Method:    CurNumPosition
	// FullName:  DrawDMXProcess::CurNumPosition
	// Access:    private 
	// Returns:   CString
	// Qualifier:���㵱ǰgroup����λ��
	// Parameter: CString strlabel
	//************************************
	CString CurNumPosition(CString strlabel);

private:
	double m_dXScale;//x����
	double m_dYScale;//y����
	bool   m_bDrawJiedian;//�Ƿ���ƽڵ�
	double m_dZhuanghao;//׮��
	double m_dstartZhuanghao;//��ʵ׮��
	CString m_strJiedian;
	double m_dSJDmHeight;//��Ƶ����
	double m_dXzDmHeight;//��״�����
	AcGePoint3d m_basePt;
	int m_nCout;//��ǰ����
	CZdmDataInfo* m_pZdmInfo;
	double m_dminElavation;
	double m_dmaxElavation;
	//AcDbObjectIdArray m_GroupIds;

};
