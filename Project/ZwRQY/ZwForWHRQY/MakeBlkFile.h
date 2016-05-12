#pragma once
//#include "DataBaseInfo.h"
//#include "BlkRefCsv.h"

class CMakeBlkFile//����blkFile
{
public:
	CMakeBlkFile(void);
	CMakeBlkFile(LPCTSTR strFileName);
	~CMakeBlkFile();
	bool Save();
	void SetFileName(LPCTSTR strFileName);

private:
	bool GetBasePt();
	bool SelectEnt();//ѡ��ʵ��
	bool SaveToFile();

private:
	AcGePoint3d m_insertPt;
	CString m_strFileName;//�ļ���
	AcDbObjectIdArray m_objIdArrs;//id��

};

