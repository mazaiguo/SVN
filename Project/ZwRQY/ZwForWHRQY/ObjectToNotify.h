

//-----------------------------------------------------------------------------
//----- ObjectToNotify.h : Declaration of the CObjectToNotify
//-----------------------------------------------------------------------------
#pragma once

#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
class ZDM_DLL_API CObjectToNotify : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(CObjectToNotify) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CObjectToNotify () ;
	virtual ~CObjectToNotify () ;

	void link(AcDbObjectId objId, 
		AcDbObjectId blkId = AcDbObjectId::kNull)
	{
		m_ObjId = objId;
		m_blkId = blkId;
	}
	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const ;
	virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) ;

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

	enum 
	{
		kDxfLabel       = 1,
		kDxfScale		= 140,
		kDxfRotate		= 141,
		kDxfBasePt		= 10,	
		kDxfNameText	= 331,
		kDxfNoText		= 332,
		kDxfText		= 333,
		kDxfStandard	= 334,
		kDxfDeFine		= 335,
		kDxfBlk			= 336,
		kDxfLine		= 337,
	};
private:
	AcDbObjectId m_ObjId;
	AcDbObjectId m_blkId;//记录仪表的块
} ;

