#include "StdAfx.h"
#include "BlkInfo.h"
#include "dbwipe.h"
#include "MyTransFunc.h"
#include "MyDrawEntity.h"
#include "MyEditEntity.h"
#include "BlkJig.h"
#include "Global.h"
#include "MyDrag.h"
#include "ObjectToNotify.h"

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
	m_blkId = AcDbObjectId::kNull;
	
}

CBlkInsert::CBlkInsert(CBlkBaseInFo blkInfo)
{
	m_bDrawWP = true;
	m_Info = blkInfo;
	m_bIsModified = false;
	//////////////////////////////////////////////////////////////////////////
	m_blkId = AcDbObjectId::kNull;
}

CBlkInsert::~CBlkInsert(void)
{
	m_bDrawWP = true;
}

void CBlkInsert::SetBlkInfo(CBlkBaseInFo blkInfo)
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
	if (m_bDrawWP)
	{
		bRet = DoEntData();
		if (!bRet)
		{
			return false;
		}
	}
	//AddDataToBlk();
	setImageFrameOff();
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

//void CBlkInsert::AddDataToBlk()
//{
//
//}

bool CBlkInsert::DoEntData()
{
	AcGePoint3dArray ptArr;
	ptArr.removeAll();
	ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(m_Exts.minPoint(), m_Exts.maxPoint());
	//AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId wipeoutId = AcDbObjectId::kNull;
	if (CreateWipeout(wipeoutId, ptArr)!= Acad::eOk)
	{
		return false;
	}

	MyTransFunc::DuiXiangPaiXu(wipeoutId, true);	
	MyTransFunc::DuiXiangPaiXu(m_blockId, true);

	assocWipeOut(m_blockId, wipeoutId);
	return true;
}

//主要操作数据的函数
bool CBlkInsert::InsertBlk()
{
	AcDbObjectId LayerId;
	
	AcDbObjectId objId = MyDrawEntity::GetBlkRef(m_Info.GetFileName());
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

//创建WipeOut对象
Acad::ErrorStatus CBlkInsert::CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds)
{
	Acad::ErrorStatus es;
#ifdef ARX
	if (RTNORM != acedArxLoad(_T("AcWipeoutObj18.dbx"))) return Acad::eNotImplementedYet;
	es = AcDbWipeout::createImageDefinition();
	if (es != Acad::eOk)
	{
		return es;
	}
#else
	if (RTNORM != acedArxLoad(_T("ZrxDraw.zrx")))
	{
		return Acad::eNotImplementedYet;
	}
#endif	
	if (!point3ds.at(0).isEqualTo(point3ds.last()))
	{
		point3ds.append(point3ds.at(0));
	}
	AcDbWipeout *pWipeout = new AcDbWipeout;
	if(pWipeout == NULL) return Acad::eNotImplementedYet;
	pWipeout->setDatabaseDefaults();
	AcGePoint3d originPnt(AcGePoint3d::kOrigin);
	AcGeVector3d Udirection(1,0,0);
	AcGeVector3d Vdirection(0,-1,0);
	pWipeout->setOrientation(originPnt,Udirection, Vdirection); 

	pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent,Adesk::kTrue);
	pWipeout->setDisplayOpt(AcDbRasterImage::kShow, false);
	es = pWipeout->setClipBoundaryToWholeImage();
	AcGeMatrix3d PixelToModel,ModelToPixel;
	pWipeout->getPixelToModelTransform(PixelToModel);
	ModelToPixel=PixelToModel.invert();
	for(int i=0;i< point3ds.length();i++)
	{
		point3ds.at(i).transformBy(ModelToPixel);
	}   
	AcGePoint2dArray point2ds = MyTransFunc::My3d22d(point3ds);
	es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly,point2ds);
	AcGePoint2d pt2d;
	double scale;
	AcDbObjectId id;
	pWipeout->append(id);
	wipeoutId = pWipeout->objectId();
	pWipeout->close();
	return Acad::eOk;
}


void CBlkInsert::assocWipeOut(AcDbObjectId blkId, AcDbObjectId wipeoutId)
{
	//处理blkref
	AcDbBlockReference* pBlkRef = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, blkId, AcDb::kForWrite) != Acad::eOk)
	{
		return;
	}
	AcGePoint3d basePt = pBlkRef->position();
	double dScale = pBlkRef->scaleFactors().sx;
	double dRotate = pBlkRef->rotation();


	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//反应器操作
	AcDbDictionary *pNamedObj;
	AcDbDictionary *pNameList;    
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	pDb->getNamedObjectsDictionary(pNamedObj,
		AcDb::kForWrite);
	if (pNamedObj->getAt(_T("ZW_DICT_WIPEOUT"),
		(AcDbObject*&)pNameList, AcDb::kForWrite)
		== Acad::eKeyNotFound)
	{
		pNameList = new AcDbDictionary;
		AcDbObjectId DictId;
		pNamedObj->setAt(_T("ZW_DICT_WIPEOUT"), pNameList, DictId);
	}
	pNamedObj->close();

	// Create the AsdkObjectToNotify for lineA
	//
	CObjectToNotify *pObj = new CObjectToNotify();
	pObj->link(wipeoutId, m_blkId);

	AcDbObjectId objId;
	int nBlkRefCount = 0;
	MyBaseUtils::GetVar(_T("USERI1"), &nBlkRefCount);
	CString strName;
	strName.Format(_T("OBJECT_TO_NOTIFY_%d"), nBlkRefCount);
	if ((pNameList->getAt(strName, objId))
		== Acad::eKeyNotFound)
	{
		pNameList->setAt(strName, pObj, objId);
		pObj->close();
	} 
	else 
	{
		delete pObj;
	}
	pNameList->close();

	// Set up persistent reactor link between line a
	// and AsdkObjectToNotify
	//
	pBlkRef->addPersistentReactor(objId);
	pBlkRef->close();
	nBlkRefCount++;

	MyBaseUtils::SetVar(_T("USERI1"), nBlkRefCount);
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

void CBlkInsert::setImageFrameOff()
{
	acedCommand(RTSTR, _T("wipeout"), RTSTR, _T("F"), RTSTR, _T("Off"), RTNONE);
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
	m_insertPt.set(0, 0, 0);
	m_scale.set(1.0, 1.0, 1.0);
	m_dRotate = 0.0;
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

