#pragma once
//-----------------------------------------------------------------------------
#include "dbmain.h"
#ifdef WRQ_ENTITY_MODULE //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
class WRQ_ENTITY_DLL CSerialNo : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(CSerialNo) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CSerialNo () ;
	virtual ~CSerialNo () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const ;
	virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) ;

	//- SubXXX() methods (self notification)
	virtual Acad::ErrorStatus subOpen (AcDb::OpenMode mode) ;
	virtual Acad::ErrorStatus subErase (Adesk::Boolean erasing) ;
	virtual Acad::ErrorStatus subCancel () ;
	virtual Acad::ErrorStatus subClose () ;
	

	//- Persistent reactor callbacks
	virtual void openedForModify (const AcDbObject *pDbObj) ;
	virtual void cancelled (const AcDbObject *pDbObj) ;
	virtual void objectClosed (const AcDbObjectId objId) ;
	virtual void goodbye (const AcDbObject *pDbObj) ;
	virtual void copied (const AcDbObject *pDbObj, const AcDbObject *pNewObj) ;
	virtual void erased (const AcDbObject *pDbObj, Adesk::Boolean bErasing =true) ;
	virtual void modified (const AcDbObject *pDbObj) ;
	virtual void modifiedGraphics (const AcDbEntity *pDbEnt) ;
	virtual void modifiedXData (const AcDbObject *pDbObj) ;
	virtual void subObjModified (const AcDbObject *pMainbObj, const AcDbObject *pSubObj) ;
	virtual void modifyUndone (const AcDbObject *pDbObj) ;
	virtual void reappended (const AcDbObject *pDbObj) ;
	virtual void unappended (const AcDbObject *pDbObj) ;
#ifdef ARX
	//----- deepClone
	virtual Acad::ErrorStatus subDeepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary =true) const ;

	//----- wblockClone
	virtual Acad::ErrorStatus subWblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary =true) const ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual void subViewportDraw (AcGiViewportDraw *mode) ;
	virtual Adesk::UInt32 subViewportDrawLogicalFlags (AcGiViewportDraw *vd) ;
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);


	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus   subGetOsnapPoints(
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds) const;
	virtual Acad::ErrorStatus   subGetOsnapPoints(
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds,
		const AcGeMatrix3d& insertionMat) const;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus subGetGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;

	// -----------------------------------------------------------------------------
	virtual void subList(void) const;
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus subExplode(AcDbVoidPtrArray & entitySet) const;

#else
	//----- deepClone
	virtual Acad::ErrorStatus deepClone (AcDbObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary =true) const ;

	//----- wblockClone
	virtual Acad::ErrorStatus wblockClone (AcRxObject *pOwnerObject, AcDbObject *&pClonedObject, AcDbIdMapping &idMap, Adesk::Boolean isPrimary =true) const ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual void viewportDraw (AcGiViewportDraw *mode) ;
	virtual Adesk::UInt32 viewportDrawLogicalFlags (AcGiViewportDraw *vd) ;
	virtual Adesk::Boolean worldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 setAttributes (AcGiDrawableTraits *traits) ;
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus transformBy(const AcGeMatrix3d & xform);


	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus   getOsnapPoints(
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds) const;
	virtual Acad::ErrorStatus   getOsnapPoints(
		AcDb::OsnapMode     osnapMode,
		Adesk::GsMarker     gsSelectionMark,
		const AcGePoint3d&  pickPoint,
		const AcGePoint3d&  lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray&   snapPoints,
		AcDbIntArray &   geomIds,
		const AcGeMatrix3d& insertionMat) const;

	//- Grip points protocol
	virtual Acad::ErrorStatus getGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus moveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus getGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags) const ;
	virtual Acad::ErrorStatus moveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;

	// -----------------------------------------------------------------------------
	virtual void list(void) const;
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus explode(AcDbVoidPtrArray & entitySet) const;

#endif

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//函数
	AcGePoint3d basePt();
	double radius();
	double textHeight();
	AcDbObjectId textId();
	AcDbObjectId layerId();
	CString strText();

	void setBasePt(AcGePoint3d basePt);
	void setRadius(double dRadius);
	void setTextHeight(double dTextHeight);
	void setTextId(AcDbObjectId textId);
	void setLayerId(AcDbObjectId layerId);
	void setstrText(CString strText);

	//返回no
	int No() const;
	
	Acad::ErrorStatus addObjId(AcDbObjectId objId);
	int				  size();
	Acad::ErrorStatus removeId(AcDbObjectId objId);
private:
	AcGePoint3d m_basePt;//插入点
	double m_dRadius;//圆半径，默认为30
	double m_dTextHeight;//字高，默认为40
	AcDbObjectId m_TextId;//字体样式
	AcDbObjectId m_LayerId;//图层名
	AcDbObjectIdArray m_IdArrs;
	int m_nSize;//记录id的数量
	CString	m_strText;//文字
protected:
} ;
	
