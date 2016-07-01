#include "StdAfx.h"
#include "SerialNo.h"
#include "GWDesingUtils.h"
//#include "Utils.h"
//#include "DoSerialNo.h"
//#include "dbwipe.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CSerialNo::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CSerialNo, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, 序号,
ZWFORWHRQAPP
|Product Desc:     序号 mazaiguo@zwcad.com
|Company:          ZwSoft
|WEB Address:      http://www.zwcad.com
)

//-----------------------------------------------------------------------------
CSerialNo::CSerialNo () : AcDbEntity () 
{
	m_basePt.set(0, 0, 0);//插入点
	m_dRadius = CGWDesingUtils::getGlobalRadius();//圆半径，默认为30
	m_dTextHeight = CGWDesingUtils::getGlobalTextHeight();//字高，默认为40
	m_TextId = AcDbObjectId::kNull;//字体样式
	m_LayerId = AcDbObjectId::kNull;//图层名
	m_strText = CGWDesingUtils::getNumCount();//文字	
	int nCount = MyTransFunc::StringToInt(m_strText);
	nCount++;
	CString strTmp;
	strTmp.Format(_T("%d"), nCount);
	CGWDesingUtils::setNumCount(strTmp);
}

CSerialNo::~CSerialNo ()
{

}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CSerialNo::dwgOutFields (AcDbDwgFiler *pFiler) const 
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CSerialNo::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	es = pFiler->writePoint3d(m_basePt);
	es = pFiler->writeDouble(m_dRadius);
	es = pFiler->writeDouble(m_dTextHeight);
	es = pFiler->writeHardPointerId(m_TextId);
	es = pFiler->writeHardPointerId(m_LayerId);
	pFiler->writeString((AcString)m_strText);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CSerialNo::dwgInFields (AcDbDwgFiler *pFiler)
{
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CSerialNo::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CSerialNo::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	es = pFiler->readPoint3d(&m_basePt);
	es = pFiler->readDouble(&m_dRadius);
	es = pFiler->readDouble(&m_dTextHeight);
	AcDbHardPointerId tmpId = AcDbHardPointerId::kNull;
	es = pFiler->readHardPointerId(&tmpId);
	m_TextId = tmpId;
	es = pFiler->readHardPointerId(&tmpId);
	m_LayerId = tmpId;
	TCHAR* pStrTextFirst = NULL;
	pFiler->readString(&pStrTextFirst);
	m_strText = pStrTextFirst;
	acutDelString(pStrTextFirst);

	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus CSerialNo::dxfOutFields (AcDbDxfFiler *pFiler) const
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("CSerialNo")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, CSerialNo::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....


	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CSerialNo::dxfInFields (AcDbDxfFiler *pFiler) 
{
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("CSerialNo")) )
		return (pFiler->filerStatus ()) ;
	//----- Object version number needs to be read first
	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 ) {
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}
	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;
	if ( version > CSerialNo::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CSerialNo::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk ) {
		switch ( rb.restype ) {
			//----- Read params by looking at their DXF code (example below)
			//case AcDb::kDxfXCoord:
			//	if ( version == 1 )
			//		cen3d =asPnt3d (rb.resval.rpoint) ;
			//	else 
			//		cen2d =asPnt2d (rb.resval.rpoint) ;
			//	break ;
			//.....

			default:
				//----- An unrecognized group. Push it back so that the subclass can read it again.
				pFiler->pushBackItem () ;
				es =Acad::eEndOfFile ;
				break ;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;

	return (pFiler->filerStatus ()) ;
}

//- SubXXX() methods (self notification)
Acad::ErrorStatus CSerialNo::subOpen (AcDb::OpenMode mode)
{
	return (AcDbEntity::subOpen (mode)) ;
}

Acad::ErrorStatus CSerialNo::subErase (Adesk::Boolean erasing)
{
	//记录最下的号
	//CString strCurNum = CUtils::getCurNum();
	//CString strNumCount = CUtils::getNumCount();
	//int nCurNum = MyTransFunc::StringToInt(strCurNum);
	//int nNum =  MyTransFunc::StringToInt(strNumCount);
	//nNum--;

	//int nCur = MyTransFunc::StringToInt(m_strText);
	//if (nCur < nCurNum)
	//{
	//	nCurNum = nCur;	
	//	strCurNum.Format(_T("%d"), nCurNum);
	//	CUtils::SetCurNum(strCurNum);
	//}
	//strNumCount.Format(_T("%d"), nNum);
	//CUtils::setNumCount(strNumCount);

	////CString strCurNum = CUtils::getCurNum();
	////CString strNumCount = CUtils::getNumCount();
	//if (strNumCount.CompareNoCase(strCurNum) != 0)
	//{
	//	CString strTmp;
	//	strTmp.Format(_T("第%s个实体被删除,是否对实体进行重排"), m_strText);
	//	if (MyBaseUtils::yesNoAlert(strTmp) == IDYES)
	//	{
	//		CDoSerialNo doNo;
	//		doNo.sortNo();
	//	}		
	//}	
	return (AcDbEntity::subErase (erasing)) ;
}

Acad::ErrorStatus CSerialNo::subCancel () 
{
	return (AcDbEntity::subCancel ()) ;
}

Acad::ErrorStatus CSerialNo::subClose ()
{
	return (AcDbEntity::subClose ()) ;
}

//- Persistent reactor callbacks
void CSerialNo::openedForModify (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::openedForModify (pDbObj) ;
}

void CSerialNo::cancelled (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::cancelled (pDbObj) ;
}

void CSerialNo::objectClosed (const AcDbObjectId objId) 
{
	assertReadEnabled () ;
	AcDbEntity::objectClosed (objId) ;
}

void CSerialNo::goodbye (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::goodbye (pDbObj) ;
}

void CSerialNo::copied (const AcDbObject *pDbObj, const AcDbObject *pNewObj)
{
	assertReadEnabled () ;
	AcDbEntity::copied (pDbObj, pNewObj) ;
}

void CSerialNo::erased (const AcDbObject *pDbObj, Adesk::Boolean bErasing) 
{
	assertReadEnabled () ;
	AcDbEntity::erased (pDbObj, bErasing) ;
}

void CSerialNo::modified (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::modified (pDbObj) ;
}

void CSerialNo::modifiedGraphics (const AcDbEntity *pDbEnt)
{
	assertReadEnabled () ;
	AcDbEntity::modifiedGraphics (pDbEnt) ;
}

void CSerialNo::modifiedXData (const AcDbObject *pDbObj) 
{
	assertReadEnabled () ;
	AcDbEntity::modifiedXData (pDbObj) ;
}

void CSerialNo::subObjModified (const AcDbObject *pMainbObj, const AcDbObject *pSubObj)
{
	assertReadEnabled () ;
	AcDbEntity::subObjModified (pMainbObj, pSubObj) ;
}

void CSerialNo::modifyUndone (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::modifyUndone (pDbObj) ;
}

void CSerialNo::reappended (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::reappended (pDbObj) ;
}

void CSerialNo::unappended (const AcDbObject *pDbObj) 
{
	assertReadEnabled () ;
	AcDbEntity::unappended (pDbObj) ;
}

//----- deepClone
#ifdef ARX
	Acad::ErrorStatus CSerialNo::subDeepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const 
	{
		assertReadEnabled () ;
		return (AcDbEntity::subDeepClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//----- wblockClone
	Acad::ErrorStatus CSerialNo::subWblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const
	{
		assertReadEnabled () ;
		return (AcDbEntity::subWblockClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//-----------------------------------------------------------------------------
	//----- AcDbEntity protocols
	Adesk::Boolean CSerialNo::subWorldDraw (AcGiWorldDraw *mode) 
	{
		assertReadEnabled () ;

		//------ Returning Adesk::kFalse here will force viewportDraw() call
		//////////////////////////////////////////////////////////////////////////
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();

		//CreateWipeout();

		Acad::ErrorStatus es;
		AcDbCircle pCircle;
		pCircle.setDatabaseDefaults(db);
		pCircle.setCenter(m_basePt);
		pCircle.setLayer(m_LayerId);
		pCircle.setRadius(m_dRadius);
		pCircle.worldDraw(mode);
		//绘制文字
		AcDbMText pText;
		pText.setDatabaseDefaults(db);
		pText.setAttachment(AcDbMText::kMiddleCenter);
		pText.setLocation(m_basePt);
		pText.setContents(m_strText);
		if (!m_LayerId.isNull())
		{
			pText.setLayer(m_LayerId);
		}
		if (!m_TextId.isNull())
		{
			pText.setTextStyle(m_TextId);
		}
		pText.setTextHeight(m_dTextHeight);
		pText.worldDraw(mode);
		return Adesk::kTrue;
	}

	void CSerialNo::subViewportDraw (AcGiViewportDraw *mode) 
	{
		assertReadEnabled () ;
		AcDbEntity::subViewportDraw (mode) ;
	}

	Adesk::UInt32 CSerialNo::subViewportDrawLogicalFlags (AcGiViewportDraw *vd)
	{
		assertReadEnabled () ;
		return (AcDbEntity::subViewportDrawLogicalFlags (vd)) ;
	}

	Adesk::UInt32 CSerialNo::subSetAttributes (AcGiDrawableTraits *traits) 
	{
		assertReadEnabled () ;
		return (AcDbEntity::subSetAttributes (traits)) ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CSerialNo::subTransformBy(const AcGeMatrix3d & xform)
	{
		assertWriteEnabled();
		//Acad::ErrorStatus retCode =AcDbEntity::subTransformBy (xform) ;
		//return (retCode) ;
		m_basePt.transformBy(xform);
		return Acad::eOk;
	}

	//- Osnap points protocol
	Acad::ErrorStatus CSerialNo::subGetOsnapPoints (
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds) const
	{
		assertReadEnabled () ;
		snapPoints.append(m_basePt);
		return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
	}

	Acad::ErrorStatus CSerialNo::subGetOsnapPoints (
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds,
		const AcGeMatrix3d& insertionMat) const
	{
		assertReadEnabled () ;
		snapPoints.append(m_basePt);
		return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
	}

	//- Grip points protocol
	Acad::ErrorStatus CSerialNo::subGetGripPoints (
		AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
		) const 
	{
		assertReadEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new getGripPoints() method below (which is the default implementation)
		gripPoints.append(m_basePt);

		return Acad::eOk ;
	}

	Acad::ErrorStatus CSerialNo::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset)
	{
		assertWriteEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new moveGripPointsAt() method below (which is the default implementation)
		m_basePt += offset;

		return Acad::eOk ;
	}

	Acad::ErrorStatus CSerialNo::subGetGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags
		) const 
	{
		assertReadEnabled () ;

		//----- If you return eNotImplemented here, that will force AutoCAD to call
		//----- the older getGripPoints() implementation. The call below may return
		//----- eNotImplemented depending of your base class.
		return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
	}

	Acad::ErrorStatus CSerialNo::subMoveGripPointsAt (
		const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
		const int bitflags
		) 
	{
		assertWriteEnabled () ;

		//----- If you return eNotImplemented here, that will force AutoCAD to call
		//----- the older getGripPoints() implementation. The call below may return
		//----- eNotImplemented depending of your base class.
		return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
	}

	// -----------------------------------------------------------------------------
	void CSerialNo::subList(void) const
	{
		AcDbEntity::subList () ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CSerialNo::subExplode(AcDbVoidPtrArray & entitySet) const
	{
		//Acad::ErrorStatus retCode =AcDbEntity::subExplode (entitySet) ;
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();


		Acad::ErrorStatus es;
		AcDbCircle* pCircle = new AcDbCircle;
		pCircle->setDatabaseDefaults(db);
		pCircle->setCenter(m_basePt);
		pCircle->setLayer(m_LayerId);
		pCircle->setRadius(m_dRadius);
		entitySet.append(pCircle);
		//绘制文字
		AcDbMText* pText = new AcDbMText;
		pText->setDatabaseDefaults(db);
		pText->setAttachment(AcDbMText::kMiddleCenter);
		pText->setLocation(m_basePt);
		pText->setContents(m_strText);
		pText->setDatabaseDefaults(db);
		pText->setLayer(m_LayerId);
		pText->setTextStyle(m_TextId);
		pText->setTextHeight(m_dTextHeight);
		entitySet.append(pText);

		return Acad::eOk ;
	}

#else

	Acad::ErrorStatus CSerialNo::deepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const 
	{
		assertReadEnabled () ;
		return (AcDbEntity::deepClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//----- wblockClone
	Acad::ErrorStatus CSerialNo::wblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const
	{
		assertReadEnabled () ;
		return (AcDbEntity::wblockClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//-----------------------------------------------------------------------------
	//----- AcDbEntity protocols
	Adesk::Boolean CSerialNo::worldDraw (AcGiWorldDraw *mode) 
	{
		assertReadEnabled () ;

		//------ Returning Adesk::kFalse here will force viewportDraw() call
		//////////////////////////////////////////////////////////////////////////
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();

		Acad::ErrorStatus es;
		AcDbCircle pCircle;
		pCircle.setDatabaseDefaults(db);
		pCircle.setCenter(m_basePt);
		pCircle.setLayer(m_LayerId);
		pCircle.setRadius(m_dRadius);
		pCircle.worldDraw(mode);
		//绘制文字
		AcDbMText pText;
		pText.setDatabaseDefaults(db);
		pText.setAttachment(AcDbMText::kMiddleCenter);
		pText.setLocation(m_basePt);
		pText.setContents(m_strText);
		pText.setDatabaseDefaults(db);
		if (!m_LayerId.isNull())
		{
			pText.setLayer(m_LayerId);
		}
		if (!m_TextId.isNull())
		{
			pText.setTextStyle(m_TextId);
		}
		pText.setTextHeight(m_dTextHeight);
		pText.worldDraw(mode);

		//return (Adesk::kFalse) ;
		return Adesk::kTrue;
	}

	void CSerialNo::viewportDraw (AcGiViewportDraw *mode) 
	{
		assertReadEnabled () ;
		AcDbEntity::viewportDraw (mode) ;
	}

	Adesk::UInt32 CSerialNo::viewportDrawLogicalFlags (AcGiViewportDraw *vd)
	{
		assertReadEnabled () ;
		return (AcDbEntity::viewportDrawLogicalFlags (vd)) ;
	}

	Adesk::UInt32 CSerialNo::setAttributes (AcGiDrawableTraits *traits) 
	{
		assertReadEnabled () ;
		return (AcDbEntity::setAttributes (traits)) ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CSerialNo::transformBy(const AcGeMatrix3d & xform)
	{
		assertWriteEnabled();
		m_basePt.transformBy(xform);
		return Acad::eOk ;
	}

	//- Osnap points protocol
	Acad::ErrorStatus CSerialNo::getOsnapPoints (
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds) const
	{
		assertReadEnabled () ;
		snapPoints.append(m_basePt);
		return (AcDbEntity::getOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
	}

	Acad::ErrorStatus CSerialNo::getOsnapPoints (
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds,
		const AcGeMatrix3d& insertionMat) const
	{
		assertReadEnabled () ;
		snapPoints.append(m_basePt);
		return (AcDbEntity::getOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
	}

	//- Grip points protocol
	Acad::ErrorStatus CSerialNo::getGripPoints (
		AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
		) const 
	{
		assertReadEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new getGripPoints() method below (which is the default implementation)
		gripPoints.append(m_basePt);

		return Acad::eOk ;
	}

	Acad::ErrorStatus CSerialNo::moveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset)
	{
		assertWriteEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new moveGripPointsAt() method below (which is the default implementation)
		m_basePt += offset;

		return Acad::eOk ;
	}

	Acad::ErrorStatus CSerialNo::getGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags
		) const 
	{
		assertReadEnabled () ;

		//----- If you return eNotImplemented here, that will force AutoCAD to call
		//----- the older getGripPoints() implementation. The call below may return
		//----- eNotImplemented depending of your base class.
		return (AcDbEntity::getGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
	}

	Acad::ErrorStatus CSerialNo::moveGripPointsAt (
		const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
		const int bitflags
		) 
	{
		assertWriteEnabled () ;

		//----- If you return eNotImplemented here, that will force AutoCAD to call
		//----- the older getGripPoints() implementation. The call below may return
		//----- eNotImplemented depending of your base class.
		return (AcDbEntity::moveGripPointsAt (gripAppData, offset, bitflags)) ;
	}

	// -----------------------------------------------------------------------------
	void CSerialNo::list(void) const
	{
		AcDbEntity::list () ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CSerialNo::explode(AcDbVoidPtrArray & entitySet) const
	{
		//Acad::ErrorStatus retCode =AcDbEntity::subExplode (entitySet) ;
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();

		Acad::ErrorStatus es;
		AcDbCircle* pCircle = new AcDbCircle;
		pCircle->setDatabaseDefaults(db);
		pCircle->setCenter(m_basePt);
		pCircle->setLayer(m_LayerId);
		pCircle->setRadius(m_dRadius);
		entitySet.append(pCircle);
		//绘制文字
		AcDbMText* pText = new AcDbMText;
		pText->setDatabaseDefaults(db);
		pText->setAttachment(AcDbMText::kMiddleCenter);
		pText->setLocation(m_basePt);
		pText->setContents(m_strText);
		pText->setDatabaseDefaults(db);
		pText->setLayer(m_LayerId);
		pText->setTextStyle(m_TextId);
		pText->setTextHeight(m_dTextHeight);
		entitySet.append(pText);

		return Acad::eOk ;
	}
#endif



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//函数
AcGePoint3d CSerialNo::basePt()
{
	assertReadEnabled () ;
	return m_basePt;
}
double CSerialNo::radius()
{
	assertReadEnabled () ;
	return m_dRadius;
}
double CSerialNo::textHeight()
{
	assertReadEnabled () ;
	return m_dTextHeight;
}
AcDbObjectId CSerialNo::textId()
{
	assertReadEnabled () ;
	return m_TextId;
}
AcDbObjectId CSerialNo::layerId()
{
	assertReadEnabled () ;
	return m_LayerId;
}
CString CSerialNo::strText()
{
	assertReadEnabled () ;
	return m_strText;
}

void CSerialNo::setBasePt(AcGePoint3d basePt)
{
	assertWriteEnabled () ;
	m_basePt = basePt;
}
void CSerialNo::setRadius(double dRadius)
{
	assertWriteEnabled () ;
	m_dRadius = dRadius;
}
void CSerialNo::setTextHeight(double dTextHeight)
{
	assertWriteEnabled () ;
	m_dTextHeight = dTextHeight;
}
void CSerialNo::setTextId(AcDbObjectId textId)
{
	assertWriteEnabled () ;
	m_TextId = textId;
}
void CSerialNo::setLayerId(AcDbObjectId layerId)
{
	assertWriteEnabled () ;
	m_LayerId = layerId;
}
void CSerialNo::setstrText(CString strText)
{
	assertWriteEnabled () ;
	m_strText = strText;
}


int	 CSerialNo::No() const
{
	assertReadEnabled();
	int nNo = MyTransFunc::StringToInt(m_strText);
	return nNo;
}

Acad::ErrorStatus CSerialNo::addObjId(AcDbObjectId objId)
{
	assertWriteEnabled();
	if (m_IdArrs.find(objId) < 0)
	{
		m_IdArrs.append(objId);
	}
	return Acad::eOk;
}

int CSerialNo::size()
{
	assertReadEnabled();
	m_nSize = m_IdArrs.length();
	return m_nSize;
}

Acad::ErrorStatus CSerialNo::removeId(AcDbObjectId objId)
{
	assertWriteEnabled();
	int nFind = m_IdArrs.find(objId);
	if (nFind > 0)
	{
		m_IdArrs.remove(objId);
	}
	return Acad::eOk;
}

////创建WipeOut对象
//Acad::ErrorStatus CSerialNo::CreateWipeout () const
//{
//	Acad::ErrorStatus es;
//#ifndef ZRX_2014
//	if (RTNORM != acedArxLoad(_T("AcWipeoutObj18.dbx"))) return Acad::eNotImplementedYet;
//	es = AcDbWipeout::createImageDefinition();
//	if (es != Acad::eOk)
//	{
//		return es;
//	}
//#else
//	if (RTNORM != acedArxLoad(_T("ZrxDraw.zrx")))
//	{
//		return Acad::eNotImplementedYet;
//	}
//#endif	
//	AcGePoint3dArray point3ds;
//	GetPointArr(point3ds);
//
//	if (!point3ds.at(0).isEqualTo(point3ds.last()))
//	{
//		point3ds.append(point3ds.at(0));
//	}
//	AcDbWipeout *pWipeout = new AcDbWipeout;
//	if(pWipeout == NULL) 
//		return Acad::eNotImplementedYet;
//	pWipeout->setDatabaseDefaults();
//	AcGePoint3d originPnt(AcGePoint3d::kOrigin);
//	AcGeVector3d Udirection(1,0,0);
//	AcGeVector3d Vdirection(0,-1,0);
//	pWipeout->setOrientation(originPnt,Udirection, Vdirection); 
//
//	pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent,Adesk::kTrue);
//	pWipeout->setDisplayOpt(AcDbRasterImage::kShow, false);
//	es = pWipeout->setClipBoundaryToWholeImage();
//	AcGeMatrix3d PixelToModel,ModelToPixel;
//	pWipeout->getPixelToModelTransform(PixelToModel);
//	ModelToPixel=PixelToModel.invert();
//	for(int i=0;i< point3ds.length();i++)
//	{
//		point3ds.at(i).transformBy(ModelToPixel);
//	}   
//	AcGePoint2dArray point2ds = MyTransFunc::My3d22d(point3ds);
//	es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly,point2ds);
//	AcGePoint2d pt2d;
//	double scale;
//	AcDbObjectId id;
//	pWipeout->append(id);
//	//wipeoutId = pWipeout->objectId();
//	pWipeout->close();
//	return Acad::eOk;
//}
//
//void CSerialNo::GetPointArr(AcGePoint3dArray& point3d) const
//{
//	point3d.removeAll();
//	AcGePoint3d minPt,maxPt,leftPt,rightPt;
//	minPt.x = m_basePt.x - m_dRadius;
//	minPt.y = m_basePt.y - m_dRadius;
//	minPt.z = maxPt.z = leftPt.z = rightPt.z = 0;
//
//	maxPt.x = m_basePt.x + m_dRadius;
//	maxPt.y = m_basePt.y + m_dRadius;
//
//	leftPt.x = m_basePt.x - m_dRadius;
//	leftPt.y = m_basePt.y + m_dRadius;
//
//	rightPt.x = m_basePt.x + m_dRadius;
//	rightPt.y = m_basePt.y - m_dRadius;
//
//	point3d.append(minPt);
//	point3d.append(leftPt);
//	point3d.append(maxPt);
//	point3d.append(rightPt);
//	point3d.append(minPt);
//
//}
//
