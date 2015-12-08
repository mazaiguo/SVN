

//-----------------------------------------------------------------------------
//----- ObjectToNotify.cpp : Implementation of CObjectToNotify
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ObjectToNotify.h"
#include "ArxDbgXdata.h"
#include "MyEditEntity.h"
#include "dbwipe.h"
#include "MyTransFunc.h"
//-----------------------------------------------------------------------------
Adesk::UInt32 CObjectToNotify::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CObjectToNotify, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, OBJECTTONOTIFY,
ZWFORTRSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
CObjectToNotify::CObjectToNotify () : AcDbObject () {
}

CObjectToNotify::~CObjectToNotify () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CObjectToNotify::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CObjectToNotify::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem((AcDbSoftPointerId&)m_ObjId);
	pFiler->writeItem((AcDbSoftPointerId&)m_TextId1);
	pFiler->writeItem((AcDbSoftPointerId&)m_TextId2);
	pFiler->writeItem((AcDbSoftPointerId&)m_TextId3);
	pFiler->writeItem((AcDbSoftPointerId&)m_TextId4);
	pFiler->writeItem((AcDbSoftPointerId&)m_TextId5);
	pFiler->writeItem((AcDbSoftPointerId&)m_blkId);
	pFiler->writeItem((AcDbSoftPointerId&)m_lineId);
	//pFiler->writeItem(m_basePt);
	//pFiler->writeItem(m_dScale);
	//pFiler->writeItem(m_dRotate);
	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CObjectToNotify::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CObjectToNotify::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CObjectToNotify::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	pFiler->readItem((AcDbSoftPointerId*) &m_ObjId);
	pFiler->readItem((AcDbSoftPointerId*)&m_TextId1);
	pFiler->readItem((AcDbSoftPointerId*)&m_TextId2);
	pFiler->readItem((AcDbSoftPointerId*)&m_TextId3);
	pFiler->readItem((AcDbSoftPointerId*)&m_TextId4);
	pFiler->readItem((AcDbSoftPointerId*)&m_TextId5);
	pFiler->readItem((AcDbSoftPointerId*)&m_blkId);
	pFiler->readItem((AcDbSoftPointerId*)&m_lineId);
	//pFiler->readItem(&m_basePt);
	//pFiler->readItem(&m_dScale);
	//pFiler->readItem(&m_dRotate);
	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus CObjectToNotify::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("CObjectToNotify")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, CObjectToNotify::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem(AcDb::kDxfSoftPointerId, m_ObjId);
	pFiler->writeItem(CObjectToNotify::kDxfText, m_TextId1);
	pFiler->writeItem(CObjectToNotify::kDxfNameText, m_TextId2);
	pFiler->writeItem(CObjectToNotify::kDxfNoText, m_TextId3);
	pFiler->writeItem(CObjectToNotify::kDxfStandard, m_TextId4);
	pFiler->writeItem(CObjectToNotify::kDxfDeFine, m_TextId5);
	pFiler->writeItem(CObjectToNotify::kDxfBlk, m_blkId);
	pFiler->writeItem(CObjectToNotify::kDxfLine, m_lineId);
	//pFiler->writeItem(CObjectToNotify::kDxfBasePt, m_basePt);
	//pFiler->writeItem(CObjectToNotify::kDxfScale, m_dScale);
	//pFiler->writeItem(CObjectToNotify::kDxfRotate, m_dRotate);
	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CObjectToNotify::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("CObjectToNotify")) )
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
	if ( version > CObjectToNotify::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CObjectToNotify::kCurrentVersionNumber )
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
			case AcDb::kDxfSoftPointerId:
				acdbGetObjectId(m_ObjId, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfText:
				acdbGetObjectId(m_TextId1, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfNameText:
				acdbGetObjectId(m_TextId2, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfNoText:
				acdbGetObjectId(m_TextId3, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfStandard:
				acdbGetObjectId(m_TextId4, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfDeFine:
				acdbGetObjectId(m_TextId5, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfBlk:
				acdbGetObjectId(m_blkId, rb.resval.rlname);
				break;
			case CObjectToNotify::kDxfLine:
				acdbGetObjectId(m_lineId, rb.resval.rlname);
				break;
		/*	case CObjectToNotify::kDxfScale:
				m_dScale = rb.resval.rreal;
				break;
			case CObjectToNotify::kDxfBasePt:
				m_basePt.x = rb.resval.rpoint[0];
				m_basePt.y = rb.resval.rpoint[1];
				m_basePt.z = rb.resval.rpoint[2];
				break;
			case CObjectToNotify::kDxfRotate:
				m_dRotate = rb.resval.rreal;
				break;*/
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

//- Persistent reactor callbacks
void CObjectToNotify::openedForModify (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::openedForModify (pDbObj) ;
}

void CObjectToNotify::cancelled (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::cancelled (pDbObj) ;
}

void CObjectToNotify::objectClosed (const AcDbObjectId objId) {
	assertReadEnabled () ;
	AcDbObject::objectClosed (objId) ;
}

void CObjectToNotify::goodbye (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::goodbye (pDbObj) ;
}

void CObjectToNotify::copied (const AcDbObject *pDbObj, const AcDbObject *pNewObj) {
	assertReadEnabled () ;
	AcDbObject::copied (pDbObj, pNewObj) ;
}

void CObjectToNotify::erased (const AcDbObject *pDbObj, Adesk::Boolean bErasing) 
{
	assertReadEnabled () ;
	AcDbEntity* pBlkRef = AcDbEntity::cast(pDbObj);
	if (!pBlkRef)
	{
		return;
	}
	CString strXdata;
	ArxDbgAppXdataList  xdata(pBlkRef);
	ArxDbgAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		return;
	}

	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(ZRXDBG_TYPE, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	ArxDbgAppXdata* xdPtr = xdata.at(nIndex);
	bool bIsOk = xdPtr->getString(XDT_XG_CSTRING, strXdata);
	if (!bIsOk)
	{
		return;
	}
	AcDbWipeout* pWipeout = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pWipeout, m_ObjId, AcDb::kForWrite) != Acad::eOk)
	{
		return;
	}
	pWipeout->erase(bErasing);
	pWipeout->close();
	//删除文字
	//AcDbText* pText = NULL;
	AcDbEntity* pEnt = NULL;
	if (!m_TextId1.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_TextId1, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}
	
	if (!m_TextId2.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_TextId2, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}
	
	if (!m_TextId3.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_TextId3, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}
	
	if (!m_TextId4.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_TextId4, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}
	
	if (!m_TextId5.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_TextId5, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}
	

	if (!m_blkId.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_blkId, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}

	if (!m_lineId.isNull())
	{
		if (acdbOpenAcDbEntity((AcDbEntity*&)pEnt, m_lineId, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		pEnt->erase();
		pEnt->close();
	}
	AcDbObject::erased (pDbObj, bErasing) ;
}

void CObjectToNotify::modified (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbEntity* pBlkRef = AcDbEntity::cast(pDbObj);
	if (!pBlkRef)
	{
		return;
	}
	CString strXdata;
	ArxDbgAppXdataList  xdata(pBlkRef);
	ArxDbgAppXdataListIterator iter(xdata);
	if (xdata.isEmpty())
	{
		return;
	}

	int nIndex = 0;
	for (;!iter.done(); iter.next())
	{
		if (!_tcsicmp(ZRXDBG_TYPE, iter.item()->getAppName()))
		{
			break;
		}
		else
		{
			nIndex++;
		}
	}
	ArxDbgAppXdata* xdPtr = xdata.at(nIndex);
	bool bIsOk = xdPtr->getString(XDT_XG_CSTRING, strXdata);
	if (!bIsOk)
	{
		return;
	}
	AcDbWipeout* pWipeout = NULL;
	Acad::ErrorStatus es;
	AcDbExtents exts;
	pBlkRef->getGeomExtents(exts);
	AcGePoint3d minPt,maxPt;
	minPt = exts.minPoint();
	maxPt = exts.maxPoint();

	AcGePoint3dArray ptArr;
	ptArr.removeAll();
	ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(minPt, maxPt);
	if (acdbOpenObject(pWipeout, m_ObjId,
		AcDb::kForWrite) == Acad::eOk)
	{	
		//es = pWipeout->erase();
		/////////////////////////////////////////
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
		for(int i=0;i< ptArr.length();i++)
		{
			ptArr.at(i).transformBy(ModelToPixel);
		}   
		AcGePoint2dArray point2ds = MyTransFunc::My3d22d(ptArr);
		es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly,point2ds);
		AcGePoint2d pt2d;
		double scale;
		AcDbObjectId id;
		pWipeout->append(id);
		pWipeout->close();
	}
	//添加文字的处理
	AcDbObject::modified (pDbObj) ;
}

void CObjectToNotify::modifiedGraphics (const AcDbEntity *pDbEnt) {
	assertReadEnabled () ;
	AcDbObject::modifiedGraphics (pDbEnt) ;
}

void CObjectToNotify::modifiedXData (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::modifiedXData (pDbObj) ;
}

void CObjectToNotify::subObjModified (const AcDbObject *pMainbObj, const AcDbObject *pSubObj) {
	assertReadEnabled () ;
	AcDbObject::subObjModified (pMainbObj, pSubObj) ;
}

void CObjectToNotify::modifyUndone (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::modifyUndone (pDbObj) ;
}

void CObjectToNotify::reappended (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::reappended (pDbObj) ;
}

void CObjectToNotify::unappended (const AcDbObject *pDbObj) {
	assertReadEnabled () ;
	AcDbObject::unappended (pDbObj) ;
}

