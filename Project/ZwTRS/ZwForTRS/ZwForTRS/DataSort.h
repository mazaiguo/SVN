#pragma once
#include "DataBaseInfo.h"
#include "FileFormat.h"
#include <map>
#include <vector>
#include <utility>
using namespace std;
//数据排序
class CDataSort
{
public:
	CDataSort(void);
	~CDataSort(void);
	//设置数据
	bool setData(map<CString, pair<CDataBaseInfo*, int> > Data, LPCTSTR str);	

	//返回结果
	//map<CString, map<CString, DataVec> > getData();
	map<CString, DataVec> getData();
	//返回第二个数据
	map<CString, vector<CString> > getStrData();
	
	//合并行数的数量
	int getCombineCount() const;
	//起始位置
	int getStart() const;
private:
	//从databaseinfo中根据关键字获取数据
	//@param 第一个参数记录databaseinfo中的数据
	//@param 第二个参数记录ini文件中关键字的数据和位置
	//@param 第三个参数记录数量
	//@param 第四个参数记录流程图编号
	DataVec getDataByKey(CDataBaseInfo* pDb, DataVec vecKey, int nCount, LPCTSTR strChartNum);
	//对取出来的数据进行排序
	//@param 第一个参数记录流程图编号
	//@param 第二个参数记录关键字数据
	void SortDataByString(LPCTSTR strChartNum);
private:
	//文件规则
	CFileFormat m_FormatFile;
	//文件路径
	//CString m_strFile;
	////@param 第一个参数记录流程图编号
	////@param vector中第一个参数记录关键字
	////@param vector中第二个参数记录关键字数据的位置
	map<CString, DataVec> m_Data;
	//@param 第一个参数记录流程图编号的前面字符
	//@param 第二个参数map中第一个参数记录流程图编号
	//@param 第二个参数map中的第二个参数记录vector数据
	//@param vector中第一个参数记录关键字
	//@param vector中第二个参数记录关键字数据的位置
	//map<CString, map<CString, DataVec> > m_RetData;
	//记录按字符序号排序的数据
	//@param 第一个参数cstring记录流程图编号的前缀
	//@param 第二个参数记录同一前缀下数据所有的数据
	map<CString, vector<CString> > m_StrData;
};

////输出Excel的数据行
//class COutCell
//{
//public:
//	COutCell(void);
//	~COutCell(void);
//	//获取数据
//	LPCTSTR getKey1();
//	LPCTSTR getKey2();
//	LPCTSTR getKey3();
//	LPCTSTR getKey4();
//	LPCTSTR getKey5();
//	LPCTSTR getKey6();
//
//	//设置数据
//	void setKey1(LPCTSTR str);
//	void setKey2(LPCTSTR str);
//	void setKey3(LPCTSTR str);
//	void setKey4(LPCTSTR str);
//	void setKey5(LPCTSTR str);
//	void setKey6(LPCTSTR str);
//
//
//private:
//	//关键字
//	CString m_strKey1;
//	CString m_strKey2;
//	CString m_strKey3;
//	CString m_strKey4;
//	CString m_strKey5;
//	CString m_strKey6;
//};