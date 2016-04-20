#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API CDMXUtils
{
public:
	CDMXUtils(void);
	~CDMXUtils(void);

	//操作base里的数据,将数据存储在图纸中
	static void setNumCount(CString strCount);
	static CString getNumCount();

	static void SetCurNum(CString strCount);
	static CString getCurNum();

	static void SetJdNum(CString strCount);
	static CString getJdNum();

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

	static void SetbasePt(AcGePoint3d basePt);
	static AcGePoint3d getbasePt();
};

