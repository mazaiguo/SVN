#pragma once
#ifdef WRQ_ENTITY_MODULE //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
class WRQ_ENTITY_DLL CGWDesingUtils
{
public:
	CGWDesingUtils(void);
	~CGWDesingUtils(void);

	//����base�������,�����ݴ洢��ͼֽ��
	static void setNumCount(CString strCount);
	static CString getNumCount();

	static void SetCurNum(CString strCount);
	static CString getCurNum();

	static void SetGlobalScale(double dStartZH);
	static double getGlobalScale();

	//Ϊȫ�ֱ��������ĺ���
	static double getGlobalRadius();
	static double getGlobalTextHeight();
	static AcDbObjectId getGlobalTextStyle();
	static AcDbObjectId getGlobalGdLayerId();
	
	
};

