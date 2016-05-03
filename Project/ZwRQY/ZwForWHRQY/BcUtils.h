#pragma once
#include "ZdmDataInfo.h"
#include <utility>
#include <map>
using namespace std;

//typedef map<CString, CBiaochiForRQY> map<CString, CBiaochiForRQY>;
//////////////////////////////////////////////////////////////////////////
//类CBcUtils处理标尺相关的数据
//1.删除单条数据
//2.插入单条数据
//3.修改单条数据
//4.
//编写时间：2016年3月31日16:50:15
#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CBcUtils
{
public:
	CBcUtils(void);
	~CBcUtils(void);
	//增加dic里的数据
	//@param strLabel是名称
	//@param CZdmDataInfo是具体数据
	bool add(LPCTSTR strLabel, CZdmDataInfo pData);

	////增加dic里的数据
	////@param strLabel是名称
	////@param CZdmDataInfo是具体数据
	//bool add(LPCTSTR strLabel, CBiaochiForRQY* pData);
	//插入dic里的数据
	//@param strLabel是名称
	//@param CZdmDataInfo是具体数据
	bool insert(LPCTSTR strLabel, CZdmDataInfo pData);
	//修改dic里的数据
	//@param strLabel是名称
	//@param CZdmDataInfo是具体数据
	bool modify(LPCTSTR strLabel, CZdmDataInfo pData);
	//删除dic里strLabel对应的数据
	bool del(LPCTSTR strLabel);
	//通过strLabel的名称得到存储在该关键字下的对象数据
	//@param strLabel为关键字
	bool get(LPCTSTR strLabel, CZdmDataInfo&);
	//获取数量
	//@param
	//@return 返回dictionary中的数量
	int getCount();

	//bool OperateDic(CZdmDataInfo pData, )
	//将dwg文件中所有CBiaochiForRQY::m_dictName = _T("ZW_BIAOCHI_DATA_IN_DWG")中数据取出来
	map<CString, CZdmDataInfo> getAllData();
private:
	
	//将数据设置到dwg文件中
	void addAll();
	//删除ZW_BIAOCHI_DATA_IN_DWG中的所有数据
	void delAll();

private:
	map<CString, CZdmDataInfo> m_Data;
};


