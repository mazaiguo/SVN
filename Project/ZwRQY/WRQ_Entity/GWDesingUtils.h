#pragma once
#ifdef WRQ_ENTITY_MODULE //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
class WRQ_ENTITY_DLL CGWDesingUtils
{
public:
	CGWDesingUtils(void);
	~CGWDesingUtils(void);

	//操作base里的数据,将数据存储在图纸中
	static void setNumCount(CString strCount);
	static CString getNumCount();

	static void SetCurNum(CString strCount);
	static CString getCurNum();

	static void SetGlobalScale(double dStartZH);
	static double getGlobalScale();

	//为全局变量创建的函数
	static double getGlobalRadius();
	static double getGlobalTextHeight();
	static AcDbObjectId getGlobalTextStyle();
	static AcDbObjectId getGlobalGdLayerId();
	
	
};

