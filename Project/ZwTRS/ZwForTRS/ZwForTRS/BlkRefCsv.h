#pragma once

#include "BlkInfo.h"
class CBlkRefCsv
{
public:
	CBlkRefCsv(void);
	~CBlkRefCsv();

	//读取文件
	bool Read();
	bool Write();

	//
	CString GetFileName() const;
	void SetFileName(LPCTSTR strFileName);
	bool IsFileExit();

	//处理数据
	CDataCell GetData() const;
	void SetData(CDataCell pCell);

	//
	CDataBaseInfo SetDataCell(CDataCell pCell);
	CDataCell GetDataCellByDB(CDataBaseInfo dbInfo);

	//设置特殊的行
	void SetSpecialString(CDataBaseInfo dbInfo);

private:
	CDataCell m_pCell;
	CString m_strFileName;//文件名
};
