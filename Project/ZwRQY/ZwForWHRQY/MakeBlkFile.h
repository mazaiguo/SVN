#pragma once
//#include "DataBaseInfo.h"
//#include "BlkRefCsv.h"

class CMakeBlkFile//制作blkFile
{
public:
	CMakeBlkFile(void);
	CMakeBlkFile(LPCTSTR strFileName);
	~CMakeBlkFile();
	bool Save();
	void SetFileName(LPCTSTR strFileName);

private:
	bool GetBasePt();
	bool SelectEnt();//选择实体
	bool SaveToFile();

private:
	AcGePoint3d m_insertPt;
	CString m_strFileName;//文件名
	AcDbObjectIdArray m_objIdArrs;//id集

};

