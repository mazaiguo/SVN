#pragma once
#include <vector>
using namespace std;
#include "DataBaseInfo.h"
class CDataCell
{
public: 
	CDataCell(void);
	~CDataCell();

	CString GetFItemId() const;
	CString GetFName() const;
	CString GetF105() const;
	CString GetF106() const;
	CString GetF107() const;
	CString GetFNumber() const;
	CString GetFErpCls() const;
	CString GetFMaund() const;

	void SetFItemId(CString str);
	void SetFName(CString str);
	void SetF105(CString str);
	void SetF106(CString str);
	void SetF107(CString str);
	void SetFNumber(CString str);
	void SetFErpCls(CString str);
	void SetFMaund(CString str);

	CString SetFullString() const;//获取一串完整的字符串

private:
	CString m_FItemId;
	CString m_FName;
	CString m_F105;
	CString m_F106;
	CString m_F107;
	CString m_FNumber;
	CString m_FErpCls;
	CString m_FMaund;
};

class COperateCsv
{
public:
	COperateCsv(void);
	~COperateCsv();
	
	//读取文件
	bool Read();
	bool Write();

	//
	CString GetFileName() const;
	void SetFileName(LPCTSTR strFileName);
	bool IsFileExit();

	//处理数据
	vector<CDataCell> GetData() const;
	void SetData(vector<CDataCell>);
	void SetSingleData(CDataCell pCell);
	void clear();

	map<CString, CString> GetStandardMap() const;//执行标准对应F_106在t_ICItemCustom表中
	map<CString, CString> GetCailiaoMap() const;//材料对应的F_105在t_ICItemCustom中
private:
	vector<CDataCell> m_Data;
	CString m_strFileName;//文件名
	map<CString, CString> m_F105Map;//
	map<CString, CString> m_F106Map;//
};

