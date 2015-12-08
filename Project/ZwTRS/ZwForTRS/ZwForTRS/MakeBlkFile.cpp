#include "StdAfx.h"
#include "MakeBlkFile.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////////////////创建BLK文件////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CMakeBlkFile::CMakeBlkFile(void)
{
	m_objIdArrs.removeAll();
}

CMakeBlkFile::CMakeBlkFile(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
	m_objIdArrs.removeAll();
}


CMakeBlkFile::~CMakeBlkFile()
{
	m_objIdArrs.removeAll();
}

bool CMakeBlkFile::Save()
{
	bool bRet = GetBasePt();
	if (!bRet)
	{
		return false;
	}
	bRet = SelectEnt();
	if (!bRet)
	{
		return false;
	}
	bRet = SaveToFile();
	if (!bRet)
	{
		return bRet;
	}
	if (!m_baseInfo.GetFNumber().IsEmpty())
	{
		CDataCell pCell;

		pCell = m_blkFile.GetDataCellByDB(m_baseInfo);
		m_blkFile.SetData(pCell);
		CString strCsvFile;
		CString strTmp = m_strFileName.Left(m_strFileName.GetLength() - 4);
		strCsvFile = strTmp + _T(".txt");
		m_blkFile.SetFileName(strCsvFile);
		m_blkFile.Write();
	}
	return bRet;
}

void CMakeBlkFile::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

void CMakeBlkFile::SetDBInfo(CDataBaseInfo dbInfo)
{
	m_baseInfo = dbInfo;
}

bool CMakeBlkFile::GetBasePt()
{
	int nRet = acedGetPoint(NULL, _T("\n请选择插入点"), asDblArray(m_insertPt));
	if (nRet != RTNORM)
	{
		return false;
	}
	return true;
}

bool CMakeBlkFile::SelectEnt()
{
	ads_name ssname,ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	int nRet = acedSSGet(NULL, NULL, NULL, NULL, ssname);
	if (nRet != RTNORM)
	{
		return false;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		m_objIdArrs.append(objId);
	}
	acedSSFree(ssname);
	return true;
}

bool CMakeBlkFile::SaveToFile()
{
	bool bRet = true;
	AcDbDatabase *pDwg = NULL; 
	Acad::ErrorStatus es;

	es = acdbHostApplicationServices()->workingDatabase()->wblock(pDwg, m_objIdArrs, m_insertPt/*, AcDb::kDrcIgnore*/);
	if (es == Acad::eOk)
	{
		AcDb::AcDbDwgVersion dwgVer = acdbHostApplicationServices()->workingDatabase()->originalFileVersion();
		es = pDwg->saveAs(m_strFileName,FALSE,dwgVer,0);
		if (es!=Acad::eOk)
		{
			bRet = false;
		}
	}
	if (pDwg != NULL)
	{
		delete pDwg;
		pDwg = NULL;
	}

	acTransactionManagerPtr()->queueForGraphicsFlush();
	acTransactionManagerPtr()->flushGraphics();//刷新
	acedUpdateDisplay();
	return bRet;
}

