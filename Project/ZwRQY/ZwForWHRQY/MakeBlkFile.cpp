#include "StdAfx.h"
#include "MakeBlkFile.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////////////////����BLK�ļ�////////////////////////////////////////
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
	return bRet;
}

void CMakeBlkFile::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

bool CMakeBlkFile::GetBasePt()
{
	int nRet = acedGetPoint(NULL, _T("\n��ѡ������"), asDblArray(m_insertPt));
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
	acTransactionManagerPtr()->flushGraphics();//ˢ��
	acedUpdateDisplay();
	return bRet;
}

