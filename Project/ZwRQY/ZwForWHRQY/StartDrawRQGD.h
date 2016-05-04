#pragma once
#ifdef WRQ_ZDM_MODULE //�ڴ�������DLL�Ĺ������Ȱ� 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
#include "ZdmDataInfo.h"
class ZDM_DLL_API CStartDrawRQGD
{
public:
	CStartDrawRQGD(void);
	~CStartDrawRQGD(void);
	bool start();
	//�����µĵ�����
	bool newLine();
	//��ԭ�е������ϼ���
	bool oldLine();
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

private:
	bool Draw();

	bool DrawNext();

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
private:
	bool m_bIsNode;//�Ƿ��нڵ�
	AcGePoint3d m_basePt;//����

	int m_nCout;
	double m_dXScale;//x����
	double m_dYScale;//y����
	bool   m_bDrawJiedian;//�Ƿ���ƽڵ�
	double m_dPipeDiameter;//�ܾ�
	CString m_strJiedian;
	double m_dGuandi;//�ܵ���
	double m_dWashen;//����
	double m_dRealDmx;//ʵ�ʵ�����
	double m_dPodu;
	CZdmDataInfo m_pZdmInfo;
};
