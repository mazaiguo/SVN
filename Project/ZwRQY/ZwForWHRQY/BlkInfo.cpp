#include "StdAfx.h"
#include "BlkInfo.h"
#include "dbwipe.h"
#include "MyTransFunc.h"
#include "MyDrawEntity.h"
#include "MyEditEntity.h"
#include "BlkJig.h"
#include "Global.h"
#include "MyDrag.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CBlkInfo::CBlkInfo(void)
{	
	m_insertPt.set(0, 0, 0);
	m_scale.set(1.0, 1.0, 1.0);
	m_dRotate = 0.0;
}

CBlkInfo::CBlkInfo(const CBlkInfo& scl)
{
	*this = scl;
}


CBlkInfo::~CBlkInfo(void)
{
}

//基本数据
CString CBlkInfo::GetBlkType() const
{
	return m_strBlkType;
}

AcGePoint3d CBlkInfo::GetInsertPt() const
{
	return m_insertPt;
}

AcGeScale3d CBlkInfo::GetScale() const
{
	return m_scale;
}

double CBlkInfo::GetRotate() const
{
	return m_dRotate;
}

LPCTSTR CBlkInfo::GetBlkName() const
{
	return m_strBlkName;
}

LPCTSTR CBlkInfo::GetFilePath() const
{
	return m_strFileName;
}
//基本数据
void CBlkInfo::SetBlkType(LPCTSTR str)
{
	m_strBlkType = str;
}
void CBlkInfo::SetInsertPt(AcGePoint3d pt)
{
	m_insertPt = pt;
}

void CBlkInfo::SetScale(AcGeScale3d sc)
{
	m_scale = sc;
}

void CBlkInfo::SetRotate(double dRotate)
{
	m_dRotate = dRotate;
}

void CBlkInfo::SetBlkName(LPCTSTR strBlkName)
{
	m_strBlkName = strBlkName;
}

void CBlkInfo::SetFilePath(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

CBlkInfo&   CBlkInfo::operator = (const CBlkInfo& scl)
{
	m_strBlkType = scl.GetBlkType();
	//块的基本信息
	m_insertPt = scl.GetInsertPt();//插入点
	m_scale = scl.GetScale();//比例
	m_dRotate = scl.GetRotate();//旋转
	m_strBlkName = scl.GetBlkName();
	m_strFileName = scl.GetFilePath();
	return *this;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////插入块及在块上做区域覆盖/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
CBlkInsert::CBlkInsert(void)
{
	m_bDrawWP = true;
	m_blockId = AcDbObjectId::kNull;
	m_bIsModified = false;

	//////////////////////////////////////////////////////////////////////////
	m_strDefine = _T("");
	m_strStandard = _T("");
	m_nPosition = 2;
	m_textId2 = AcDbObjectId::kNull;
	m_textId3 = AcDbObjectId::kNull;
	m_textId4 = AcDbObjectId::kNull;
	m_textId5 = AcDbObjectId::kNull;
	m_blkId = AcDbObjectId::kNull;
	m_lineId = AcDbObjectId::kNull;
	
}

CBlkInsert::CBlkInsert(CBlkInfo blkInfo/*, CDataBaseInfo dataInfo*/)
{
	m_bDrawWP = true;
	m_Info = blkInfo;
	//m_DataInfo = dataInfo;
	m_bIsModified = false;
	//////////////////////////////////////////////////////////////////////////
	m_strDefine = _T("");
	m_strStandard = _T("");
	m_nPosition = 2;
	m_textId2 = AcDbObjectId::kNull;
	m_textId3 = AcDbObjectId::kNull;
	m_textId4 = AcDbObjectId::kNull;
	m_textId5 = AcDbObjectId::kNull;
	m_blkId = AcDbObjectId::kNull;
	m_lineId = AcDbObjectId::kNull;
}

CBlkInsert::~CBlkInsert(void)
{
	m_bDrawWP = true;
}

void CBlkInsert::SetBlkInfo(CBlkInfo blkInfo)
{
	m_Info = blkInfo;
}

bool CBlkInsert::Insert()
{
	bool bRet = InsertBlk();
	if (!bRet)
	{
		return false;
	}
	/*if (m_bDrawWP)
	{
		bRet = DoEntData();
		if (!bRet)
		{
			return false;
		}
	}*/
	//AddDataToBlk();
	//setImageFrameOff();
	return true;
}

void CBlkInsert::SetDrawWP(bool bDrawWp)
{
	m_bDrawWP = bDrawWp;
}

void CBlkInsert::SetModified(bool bModified)
{
	m_bIsModified = bModified;
}



bool CBlkInsert::InsertBlk()
{
	AcDbObjectId LayerId;
	AcDbObjectId objId = MyDrawEntity::GetBlkRef(m_Info.GetFilePath());
	if (objId.isNull())
	{
		return false;
	}
	if (!m_bIsModified)
	{
		//jig效果不好，换用dragen
		AcGePoint3d basePt;
		basePt.set(0, 0,0 );
		ads_name ssname,ename;
		acedSSAdd(NULL, NULL, ssname);
		AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_Info.GetInsertPt(), objId) ; 
		pBlkRef->setScaleFactors(m_Info.GetScale());
		pBlkRef->getGeomExtents(m_Exts);
		pBlkRef->setPosition(basePt);
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		m_blockId = pBlkRef->objectId();
		acdbGetAdsName(ename, m_blockId);
		acedSSAdd(ename, ssname, ssname);

		int nRet = MyDrag::Draggen(_T("\n指定插入点:"), ssname, asDblArray(basePt), asDblArray(basePt), _T(""), -1);
		if (nRet == -1)
		{
			acedSSFree(ssname);
			return false;
		}
		AcGeMatrix3d mat;
		mat.setToIdentity();
		AcGeVector3d vec;
		vec = basePt.asVector();
		mat.setToTranslation(vec);
		m_Exts.transformBy(mat);
		acedSSFree(ssname);
		m_insertPt = basePt;
	}
	else
	{
		AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_Info.GetInsertPt(), objId) ; 
		pBlkRef->setScaleFactors(m_Info.GetScale());
		pBlkRef->getGeomExtents(m_Exts);
		m_insertPt = pBlkRef->position();
		MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		m_blockId = pBlkRef->objectId();
		ads_name ssname;
	}

	return true;
}


void CBlkInsert::SetName(LPCTSTR strName)
{
	m_strName = strName;
}

void CBlkInsert::SetNo(LPCTSTR strNo)
{
	m_strNo = strNo;
}

void CBlkInsert::SetText(LPCTSTR strText)
{
	m_strText = strText;
}

void CBlkInsert::SetZongxiang(BOOL bZongxiang)
{
	m_bZongxiang = bZongxiang;
}

void CBlkInsert::SetDefine(LPCTSTR strDefine)
{
	m_strDefine = strDefine;
}

void CBlkInsert::SetStandard(LPCTSTR strStandard)
{
	m_strStandard = strStandard;
}

void CBlkInsert::SetPosition(int nPosition)
{
	m_nPosition = nPosition;
}


void CBlkInsert::SetDrawText(bool bIsDrawText)
{
	m_bIsDrawText = bIsDrawText;
}

AcDbExtents CBlkInsert::GetObjExts()
{
	return m_Exts;
}

AcDbObjectId CBlkInsert::GetObjectId()
{
	return m_blockId;
}

AcGePoint3d CBlkInsert::GetPosition()
{
	return m_insertPt;
}


void CBlkInsert::SetTmpPoint(AcGePoint3d pt)
{
	m_tmpPoint = pt;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CBlkBaseInFo::CBlkBaseInFo(void)
{
	m_insertPt.set(0, 0, 0);
	m_scale.set(1.0, 1.0, 1.0);
	m_dRotate = 0.0;
}

CBlkBaseInFo::~CBlkBaseInFo()
{

}

AcGePoint3d CBlkBaseInFo::GetInsertPt() const
{
	return m_insertPt;
}

AcGeScale3d CBlkBaseInFo::GetScale() const
{
	return m_scale;
}

double CBlkBaseInFo::GetRotate() const
{
	return m_dRotate;
}

CString CBlkBaseInFo::GetBlkName() const
{
	return m_strBlkName;
}

CString CBlkBaseInFo::GetFileName() const
{
	return m_strFileName;
}


void CBlkBaseInFo::SetInsertPt(AcGePoint3d insertPt)
{
	m_insertPt = insertPt;
}

void CBlkBaseInFo::SetScale(AcGeScale3d sc)
{
	m_scale = sc;
}

void CBlkBaseInFo::SetRotate(double dRotate)
{
	m_dRotate = dRotate;
}

void CBlkBaseInFo::SetBlkName(LPCTSTR strBlkName)
{
	m_strBlkName = strBlkName;
}

void CBlkBaseInFo::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

CBlkBaseInFo&   CBlkBaseInFo::operator = (const CBlkBaseInFo& scl)
{
	//块的基本信息
	m_insertPt = scl.GetInsertPt();//插入点
	m_scale = scl.GetScale();//比例
	m_dRotate = scl.GetRotate();//旋转
	m_strBlkName = scl.GetBlkName();
	m_strFileName = scl.GetFileName();
	return *this;
}

