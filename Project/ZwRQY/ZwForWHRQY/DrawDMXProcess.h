#pragma once
#include "ZdmDataInfo.h"

//���ƻ��Ƶ����ߵ�����

#ifdef WRQ_ZDM_MODULE //�ڴ�������DLL�Ĺ������Ȱ� 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API DrawDMXProcess
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
	//�Ƿ�ڵ�
	bool GetIsJiedian();
	//
	bool GetSJDmHeight();

	bool GetXzDmHeight();

	//�Ƿ����¶�
	bool GetIsPd();

	//��ȡ�¶ȸ߶�
	bool GetPdHeight();

	bool GetSJDmHeightS();

	bool GetXzDmHeightS();

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

	AcDbObjectId setlectEnt(CString strPromPt);
	
	//ʵ�彻��
	bool EntInteraction();


private:
	double m_dXScale;//x����
	double m_dYScale;//y����
	bool   m_bDrawJiedian;//�Ƿ���ƽڵ�
	double m_dZhuanghao;//׮��
	CString m_strJiedian;
	double m_dSJDmHeight;//��Ƶ����
	double m_dXzDmHeight;//��״�����

	double m_dDesignDmxS;//�ڶ�����Ƶ�����
	double m_dRealDmxS;//�ڶ���ʵ�ʵ�����
	AcGePoint3d m_basePt;
	int m_nCout;//��ǰ����
	CZdmDataInfo* m_pZdmInfo;
	double m_dminElavation;
	double m_dmaxElavation;
	bool m_bHasBugle;//�Ƿ����¶�
	//AcDbObjectIdArray m_GroupIds;

};
