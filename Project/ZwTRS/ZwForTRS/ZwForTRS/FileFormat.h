#pragma once
#include "IniFile.h"
#include <map>
#include <vector>
#include <utility>
using namespace std;

typedef vector<pair<CString,int> > DataVec;

//文件规则
class CFileFormat
{
public:
	CFileFormat(void);
	~CFileFormat(void);
	//取数据
	int getStart() const;
	int getCombine() const;
	/*int getKey1() const;
	int getKey2() const;
	int getKey3() const;
	int getKey4() const;
	int getKey5() const;
	int getKey6() const;

	LPCTSTR getKey1();
	LPCTSTR getKey2();
	LPCTSTR getKey3();
	LPCTSTR getKey4();
	LPCTSTR getKey5();
	LPCTSTR getKey6();*/
	LPCTSTR getData();
	//获取关键字的名字和位置
	DataVec GetKeyData();
	////设置数据
	//void setStart(int );
	//void setCombine(int );
	//void setKey1(int );
	//void setKey2(int );
	//void setKey3(int );
	//void setKey4(int );
	//void setKey5(int );
	//void setKey6(int );
	//			 
	//void setKey1(LPCTSTR );
	//void setKey2(LPCTSTR );
	//void setKey3(LPCTSTR );
	//void setKey4(LPCTSTR );
	//void setKey5(LPCTSTR );
	//void setKey6(LPCTSTR );

	//从文件中读取数据
	//@param strFileName为文件路径
	//从strfilename中读取数据
	bool Read(LPCTSTR strFileName);

	//解析数据
	//@param strInput为导入的字符串，该字符串用“、”隔开，使得字符串中包含int和string
	//@param nOut为导出的int数据
	//@param strOut为导出的数据
	bool GetDataFromString(CString strInput/*, int& nOut, CString& strOut*/);
private:
	//第一列必然是序号
	//总共六列
	//第四列数据不明
	int m_nCombine;//合并行数
	int m_nStart;//开始位置
	int m_nKey1;//关键字1的位置
	//CString m_strKey1;//关键字1的数据
	//int m_nKey2;//关键字2的位置
	//CString m_strKey2;//关键字2的数据
	//int m_nKey3;
	//CString m_strKey3;
	//int m_nKey4;
	//CString m_strKey4;
	//int m_nKey5;
	//CString m_strKey5;
	//int m_nKey6;
	//CString m_strKey6;
	CIniFile m_IniFile;
	CString m_strData;//数据序列
	//@param pair中的第一个参数记录关键字
	//@param pair中的第二个参数记录关键字的位置
	DataVec m_Data;
	//@param 第一个参数记录关键字数据
	//map<CString, CString> m_DataMap;
};


