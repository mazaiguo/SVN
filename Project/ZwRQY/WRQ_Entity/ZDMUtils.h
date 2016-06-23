#pragma once
#ifdef WRQ_ENTITY_MODULE //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
class WRQ_ENTITY_DLL CDMXUtils
{
public:
	CDMXUtils(void);
	~CDMXUtils(void);

	//����base�������,�����ݴ洢��ͼֽ��
	static void setNumCount(CString strCount);
	static CString getNumCount();

	static void SetCurNum(CString strCount);
	static CString getCurNum();

	static void SetJdNum(CString strCount);
	static CString getJdNum();

	static void SetJcNum(CString strCount);
	static CString getJcNum();

	static void SetStartZH(CString dStartZH);
	static CString getStartZH();
	static double  startzh();

	static void SetInitZH(double dStartZH);
	static double getInitZh();

	static void SetXScale(double dXScale);
	static double getXScale();

	static void SetYScale(double dYScale);
	static double getYScale();

	static void SetMinElavation(double dElavation);
	static double getMinElavation();

	static void SetMaxElavation(double dElavation);
	static double getMaxElavation();

	static void SetcreateBc(bool bDrawBc);
	static bool getcreateBc();

	static void SetcreateJiedian(bool bDrawBc);
	static bool getcreateJiedian();

	static void SetcreateGw(bool bDrawBc);
	static bool getcreateGw();

	static void SetbasePt(AcGePoint3d basePt);
	static AcGePoint3d getbasePt();


	//////////////////////////////////////////////////////////////////////////
	//ȫ�ֵ���
	static double globalXScale();
	static double globalYScale();
};

