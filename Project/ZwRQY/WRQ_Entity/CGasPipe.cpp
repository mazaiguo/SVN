#include "StdAfx.h"
#include "CGasPipe.h"
#include "GWDesingUtils.h"
//#include "Utils.h"
//#include "DoGasPipe.h"
//#include "dbwipe.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CGasPipe::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CGasPipe, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, 燃气管道,
ZWFORWHRQAPP
|Product Desc:     燃气管道 mazaiguo@zwcad.com
|Company:          ZwSoft
|WEB Address:      http://www.zwcad.com
)

//-----------------------------------------------------------------------------
CGasPipe::CGasPipe () : AcDbEntity () 
{
	m_dRadius = CGWDesingUtils::getGlobalRadius();//圆半径，默认为30
	m_dTextHeight = CGWDesingUtils::getGlobalTextHeight();//字高，默认为40
	m_TextId = AcDbObjectId::kNull;//字体样式
	m_LayerId = AcDbObjectId::kNull;//图层名
}

CGasPipe::~CGasPipe ()
{

}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CGasPipe::dwgOutFields (AcDbDwgFiler *pFiler) const 
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CGasPipe::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	es = pFiler->writeUInt32(m_nSize);
	for (vector<PlineInfo>::const_iterator iter = m_dataInfo.begin();
		iter != m_dataInfo.end();
		++iter)
	{	
		PlineInfo pInfo = *iter;
		pFiler->writeUInt16(pInfo.nIndex);
		pFiler->writePoint3d(m_ptArr.at(pInfo.nIndex));
		pFiler->writeDouble(pInfo.dBulge);
		pFiler->writeDouble(pInfo.dStartWidth);
		pFiler->writeDouble(pInfo.dEndWidth);
		pFiler->writeBool(pInfo.bIsClosed);
	}
	es = pFiler->writeHardPointerId(m_TextId);
	es = pFiler->writeHardPointerId(m_LayerId);
	es = pFiler->writeHardPointerId(m_startId);
	es = pFiler->writeHardPointerId(m_endId);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CGasPipe::dwgInFields (AcDbDwgFiler *pFiler)
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
	if ( version > CGasPipe::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CGasPipe::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	es = pFiler->readUInt32(&m_nSize);
	Adesk::UInt16 nIndex;
	AcGePoint3d insertPt;
	double dBulge;
	double dStartWidth;
	double dEndWidth;
	bool bIsClosed;	
	PlineInfo pInfo;
	m_ptArr.removeAll();
	for (int i=0; i<m_nSize; i++)
	{
		pFiler->readUInt16(&nIndex);
		pFiler->readPoint3d(&insertPt);
		pFiler->readDouble(&dBulge);
		pFiler->readDouble(&dStartWidth);
		pFiler->readDouble(&dEndWidth);
		pFiler->readBool(&bIsClosed);
		pInfo.nIndex = nIndex;
		pInfo.insertPt = insertPt;
		pInfo.dBulge = dBulge;
		pInfo.dStartWidth = dStartWidth;
		pInfo.dEndWidth = dEndWidth;
		pInfo.bIsClosed = bIsClosed;
		m_dataInfo.push_back(pInfo);
		m_ptArr.append(insertPt);
	}
	AcDbHardPointerId tmpId = AcDbHardPointerId::kNull;
	es = pFiler->readHardPointerId(&tmpId);
	m_TextId = tmpId;
	es = pFiler->readHardPointerId(&tmpId);
	m_LayerId = tmpId;
	es = pFiler->readHardPointerId(&tmpId);
	m_startId = tmpId;
	es = pFiler->readHardPointerId(&tmpId);
	m_endId = tmpId;
	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus CGasPipe::dxfOutFields (AcDbDxfFiler *pFiler) const
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("CGasPipe")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, CGasPipe::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....


	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CGasPipe::dxfInFields (AcDbDxfFiler *pFiler) 
{
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("CGasPipe")) )
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
	if ( version > CGasPipe::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CGasPipe::kCurrentVersionNumber )
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
Acad::ErrorStatus CGasPipe::subOpen (AcDb::OpenMode mode)
{
	return (AcDbEntity::subOpen (mode)) ;
}

Acad::ErrorStatus CGasPipe::subErase (Adesk::Boolean erasing)
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
	//		CDoGasPipe doNo;
	//		doNo.sortNo();
	//	}		
	//}	
	return (AcDbEntity::subErase (erasing)) ;
}

Acad::ErrorStatus CGasPipe::subCancel () 
{
	return (AcDbEntity::subCancel ()) ;
}

Acad::ErrorStatus CGasPipe::subClose ()
{
	return (AcDbEntity::subClose ()) ;
}

//- Persistent reactor callbacks
void CGasPipe::openedForModify (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::openedForModify (pDbObj) ;
}

void CGasPipe::cancelled (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::cancelled (pDbObj) ;
}

void CGasPipe::objectClosed (const AcDbObjectId objId) 
{
	assertReadEnabled () ;
	AcDbEntity::objectClosed (objId) ;
}

void CGasPipe::goodbye (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::goodbye (pDbObj) ;
}

void CGasPipe::copied (const AcDbObject *pDbObj, const AcDbObject *pNewObj)
{
	assertReadEnabled () ;
	AcDbEntity::copied (pDbObj, pNewObj) ;
}

void CGasPipe::erased (const AcDbObject *pDbObj, Adesk::Boolean bErasing) 
{
	assertReadEnabled () ;
	AcDbEntity::erased (pDbObj, bErasing) ;
}

void CGasPipe::modified (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::modified (pDbObj) ;
}

void CGasPipe::modifiedGraphics (const AcDbEntity *pDbEnt)
{
	assertReadEnabled () ;
	AcDbEntity::modifiedGraphics (pDbEnt) ;
}

void CGasPipe::modifiedXData (const AcDbObject *pDbObj) 
{
	assertReadEnabled () ;
	AcDbEntity::modifiedXData (pDbObj) ;
}

void CGasPipe::subObjModified (const AcDbObject *pMainbObj, const AcDbObject *pSubObj)
{
	assertReadEnabled () ;
	AcDbEntity::subObjModified (pMainbObj, pSubObj) ;
}

void CGasPipe::modifyUndone (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::modifyUndone (pDbObj) ;
}

void CGasPipe::reappended (const AcDbObject *pDbObj)
{
	assertReadEnabled () ;
	AcDbEntity::reappended (pDbObj) ;
}

void CGasPipe::unappended (const AcDbObject *pDbObj) 
{
	assertReadEnabled () ;
	AcDbEntity::unappended (pDbObj) ;
}

//----- deepClone
#ifdef ARX
	Acad::ErrorStatus CGasPipe::subDeepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const 
	{
		assertReadEnabled () ;
		return (AcDbEntity::subDeepClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//----- wblockClone
	Acad::ErrorStatus CGasPipe::subWblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const
	{
		assertReadEnabled () ;
		return (AcDbEntity::subWblockClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//-----------------------------------------------------------------------------
	//----- AcDbEntity protocols
	Adesk::Boolean CGasPipe::subWorldDraw (AcGiWorldDraw *mode) 
	{
		assertReadEnabled () ;

		//------ Returning Adesk::kFalse here will force viewportDraw() call
		//////////////////////////////////////////////////////////////////////////
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();

		//CreateWipeout();
		AcDbPolyline pLine;
		pLine.setDatabaseDefaults(db);
		if (!m_LayerId.isNull())
		{
			pLine.setLayer(m_LayerId);
		}

		int i=0;
		pLine.setClosed(m_dataInfo.at(0).bIsClosed);
		AcGePoint3d endPt = m_dataInfo.at(m_dataInfo.size() - 1).insertPt;
		
		for (vector<PlineInfo>::iterator iter = m_dataInfo.begin();
			iter != m_dataInfo.end();
			++iter)
		{
			PlineInfo info = *iter;
			pLine.addVertexAt(i, info.insertPt.convert2d(AcGePlane::kXYPlane), info.dBulge, info.dStartWidth, info.dEndWidth);
			i++;
		}
		pLine.worldDraw(mode);
		
		//double dDist = 
		return Adesk::kTrue;
	}

	void CGasPipe::subViewportDraw (AcGiViewportDraw *mode) 
	{
		assertReadEnabled () ;
		AcDbEntity::subViewportDraw (mode) ;
	}

	Adesk::UInt32 CGasPipe::subViewportDrawLogicalFlags (AcGiViewportDraw *vd)
	{
		assertReadEnabled () ;
		return (AcDbEntity::subViewportDrawLogicalFlags (vd)) ;
	}

	Adesk::UInt32 CGasPipe::subSetAttributes (AcGiDrawableTraits *traits) 
	{
		assertReadEnabled () ;
		return (AcDbEntity::subSetAttributes (traits)) ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CGasPipe::subTransformBy(const AcGeMatrix3d & xform)
	{
		assertWriteEnabled();
		//Acad::ErrorStatus retCode =AcDbEntity::subTransformBy (xform) ;
		//return (retCode) ;
// 		m_startPt.transformBy(xform);
// 		m_endPt.transformBy(xform);
		/*for (vector<PlineInfo>::iterator iter = m_dataInfo.begin();
			iter != m_dataInfo.end();
			++iter)
		{
			PlineInfo info = *iter;
			AcGePoint3d pt = info.insertPt;
			pt = pt.transformBy(xform);
			info.insertPt = pt;
		}*/
		for (int i=0; i<m_nSize; i++)
		{
			m_ptArr.at(i).transformBy(xform);
		}
		return Acad::eOk;
	}

	//- Osnap points protocol
	Acad::ErrorStatus CGasPipe::subGetOsnapPoints (
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds) const
	{
		assertReadEnabled () ;
		AcGeVector3d viewDir(viewXform(Z, 0), viewXform(Z, 1),
			viewXform(Z, 2));
		//AcGeLineSeg3d lnsg(m_startPt, m_endPt);
		//AcGePolyline3d 
		AcGePoint3d pt;
		if (osnapMode == AcDb::kOsModeEnd)
		{
			for (vector<PlineInfo>::const_iterator iter = m_dataInfo.begin();
				iter != m_dataInfo.end();
				++iter)
			{
				PlineInfo info = *iter;
				AcGePoint3d pt = info.insertPt;
				snapPoints.append(pt);
			}
		}
		else if (osnapMode == AcDb::kOsModeNear)
		{
			//pt = lnsg.projClosestPointTo(pickPoint, viewDir);
			//snapPoints.append(pt);
		}
		return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
	}

	Acad::ErrorStatus CGasPipe::subGetOsnapPoints (
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
		AcGeVector3d viewDir(viewXform(Z, 0), viewXform(Z, 1),
			viewXform(Z, 2));
		//AcGeLineSeg3d lnsg(m_startPt, m_endPt);
		AcGePoint3d pt;
		if (osnapMode == AcDb::kOsModeEnd)
		{
			for (vector<PlineInfo>::const_iterator iter = m_dataInfo.begin();
				iter != m_dataInfo.end();
				++iter)
			{
				PlineInfo info = *iter;
				AcGePoint3d pt = info.insertPt;
				snapPoints.append(pt);
			}
		}
		else if (osnapMode == AcDb::kOsModeNear)
		{
			/*pt = lnsg.projClosestPointTo(pickPoint, viewDir);
			snapPoints.append(pt);*/
		}
		return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
	}

	//- Grip points protocol
	Acad::ErrorStatus CGasPipe::subGetGripPoints (
		AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
		) const 
	{
		assertReadEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new getGripPoints() method below (which is the default implementation)
	/*	gripPoints.append(m_startPt);
		gripPoints.append(m_endPt);*/
		for (vector<PlineInfo>::const_iterator iter = m_dataInfo.begin();
			iter != m_dataInfo.end();
			++iter)
		{
			PlineInfo info = *iter;
			AcGePoint3d pt = info.insertPt;
			gripPoints.append(pt);
		}
		return Acad::eOk ;
	}

	Acad::ErrorStatus CGasPipe::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset)
	{
		assertWriteEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new moveGripPointsAt() method below (which is the default implementation)
		/*m_startPt += offset;
		m_endPt += offset;*/
		for (vector<PlineInfo>::iterator iter = m_dataInfo.begin();
			iter != m_dataInfo.end();
			++iter)
		{
			PlineInfo info = *iter;
			AcGePoint3d pt = info.insertPt;
			pt += offset;
			info.insertPt = pt;
		}
		return Acad::eOk ;
	}

	Acad::ErrorStatus CGasPipe::subGetGripPoints (
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

	Acad::ErrorStatus CGasPipe::subMoveGripPointsAt (
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
	void CGasPipe::subList(void) const
	{
		AcDbEntity::subList () ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CGasPipe::subExplode(AcDbVoidPtrArray & entitySet) const
	{
		//Acad::ErrorStatus retCode =AcDbEntity::subExplode (entitySet) ;
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();


		Acad::ErrorStatus es;
	/*	AcDbLine* pLine = new AcDbLine;
		pLine->setDatabaseDefaults(db);
		pLine->setStartPoint(m_startPt);
		pLine->setEndPoint(m_endPt);
		pLine->setLayer(m_LayerId);
		entitySet.append(pLine);*/
		return Acad::eOk ;
	}

#else

	Acad::ErrorStatus CGasPipe::deepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const 
	{
		assertReadEnabled () ;
		return (AcDbEntity::deepClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//----- wblockClone
	Acad::ErrorStatus CGasPipe::wblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary) const
	{
		assertReadEnabled () ;
		return (AcDbEntity::wblockClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
	}

	//-----------------------------------------------------------------------------
	//----- AcDbEntity protocols
	Adesk::Boolean CGasPipe::worldDraw (AcGiWorldDraw *mode) 
	{
		assertReadEnabled () ;

		//------ Returning Adesk::kFalse here will force viewportDraw() call
		//////////////////////////////////////////////////////////////////////////
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();

		//CreateWipeout();
		AcDbLine pline;
		pline.setDatabaseDefaults(db);
		if (!m_LayerId.isNull())
		{
			pline.setLayer(m_LayerId);
		}
		AcGePoint3d startPt,endPt;
		AcGeVector3d vec = m_endPt - m_startPt;
		double dAng = vec.angleOnPlane(AcGePlane::kXYPlane);
		acutPolar(asDblArray(m_startPt), dAng, m_dRadius, asDblArray(startPt));
		acutPolar(asDblArray(m_endPt), dAng + PI, m_dRadius, asDblArray(endPt));
		pline.setStartPoint(startPt);
		pline.setEndPoint(endPt);
		pline.worldDraw(mode);
		return Adesk::kTrue;
	}

	void CGasPipe::viewportDraw (AcGiViewportDraw *mode) 
	{
		assertReadEnabled () ;
		AcDbEntity::viewportDraw (mode) ;
	}

	Adesk::UInt32 CGasPipe::viewportDrawLogicalFlags (AcGiViewportDraw *vd)
	{
		assertReadEnabled () ;
		return (AcDbEntity::viewportDrawLogicalFlags (vd)) ;
	}

	Adesk::UInt32 CGasPipe::setAttributes (AcGiDrawableTraits *traits) 
	{
		assertReadEnabled () ;
		return (AcDbEntity::setAttributes (traits)) ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CGasPipe::transformBy(const AcGeMatrix3d & xform)
	{
		assertWriteEnabled();
		m_startPt.transformBy(xform);
		m_endPt.transformBy(xform);
		return Acad::eOk ;
	}

	//- Osnap points protocol
	Acad::ErrorStatus CGasPipe::getOsnapPoints (
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds) const
	{
		assertReadEnabled () ;
		snapPoints.append(m_startPt);
		snapPoints.append(m_endPt);
		return (AcDbEntity::getOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
	}

	Acad::ErrorStatus CGasPipe::getOsnapPoints (
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
		snapPoints.append(m_startPt);
		snapPoints.append(m_endPt);
		return (AcDbEntity::getOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
	}

	//- Grip points protocol
	Acad::ErrorStatus CGasPipe::getGripPoints (
		AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
		) const 
	{
		assertReadEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new getGripPoints() method below (which is the default implementation)
		gripPoints.append(m_startPt);
		gripPoints.append(m_endPt);

		return Acad::eOk ;
	}

	Acad::ErrorStatus CGasPipe::moveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset)
	{
		assertWriteEnabled () ;
		//----- This method is never called unless you return eNotImplemented 
		//----- from the new moveGripPointsAt() method below (which is the default implementation)
		m_startPt += offset;
		m_endPt += offset;
		return Acad::eOk ;
	}

	Acad::ErrorStatus CGasPipe::getGripPoints (
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

	Acad::ErrorStatus CGasPipe::moveGripPointsAt (
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
	void CGasPipe::list(void) const
	{
		AcDbEntity::list () ;
	}

	// -----------------------------------------------------------------------------
	Acad::ErrorStatus CGasPipe::explode(AcDbVoidPtrArray & entitySet) const
	{
		//Acad::ErrorStatus retCode =AcDbEntity::subExplode (entitySet) ;
		AcDbDatabase* db = database();
		if (db == NULL)
			db = acdbHostApplicationServices()->workingDatabase();

		Acad::ErrorStatus es;
		AcDbLine* pLine = new AcDbLine;
		pLine->setDatabaseDefaults(db);
		pLine->setStartPoint(m_startPt);
		pLine->setEndPoint(m_endPt);
		pLine->setLayer(m_LayerId);
		entitySet.append(pLine);

		return Acad::eOk ;
	}
#endif



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//函数
AcDbObjectId CGasPipe::textId()
{
	assertReadEnabled () ;
	return m_TextId;
}
AcDbObjectId CGasPipe::layerId()
{
	assertReadEnabled () ;
	return m_LayerId;
}

AcDbObjectId CGasPipe::startId()
{
	assertReadEnabled();
	return m_startId;
}

AcDbObjectId CGasPipe::endId()
{	
	assertReadEnabled();
	return m_endId;
}

vector<PlineInfo> CGasPipe::getData()
{
	assertReadEnabled();
	return m_dataInfo;
}

void CGasPipe::setData(vector<PlineInfo> info)
{
	assertWriteEnabled();
	m_dataInfo.clear();
	m_dataInfo.insert(m_dataInfo.end(), info.begin(), info.end());
	
	m_nSize = m_dataInfo.size();
	for (int i=0; i<m_nSize; i++)
	{
		m_ptArr.append(m_dataInfo.at(i).insertPt);
	}
}

void CGasPipe::setTextId(AcDbObjectId textId)
{
	assertWriteEnabled () ;
	m_TextId = textId;
}
void CGasPipe::setLayerId(AcDbObjectId layerId)
{
	assertWriteEnabled () ;
	m_LayerId = layerId;
}

void CGasPipe::setStartId(AcDbObjectId startId)
{
	assertWriteEnabled();
	m_startId = startId;
}

void CGasPipe::setEndId(AcDbObjectId endId)
{
	assertWriteEnabled();
	m_endId = endId;
}

double CGasPipe::length()
{
	assertReadEnabled();
	/*double dLength = acutDistance(asDblArray(m_startPt), asDblArray(m_endPt))/CGWDesingUtils::getGlobalScale();
	return dLength;*/
	return 0.0;
	/*AcGePolyline3d pline(m_ptArr);
	double dLen;
	pline.endParam();*/
}

////创建WipeOut对象
//Acad::ErrorStatus CGasPipe::CreateWipeout () const
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
//void CGasPipe::GetPointArr(AcGePoint3dArray& point3d) const
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

