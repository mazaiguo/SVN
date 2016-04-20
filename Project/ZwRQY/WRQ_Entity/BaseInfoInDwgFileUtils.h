#pragma once
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API CBaseInfoInDwgFileUtils
{
public:
	CBaseInfoInDwgFileUtils(void);
	~CBaseInfoInDwgFileUtils(void);

	//操作base里的数据
	static void setNumCount(CString strCount);
	static CString getNumCount();

	static void SetCurNum(CString strCount);
	static CString getCurNum();

};

