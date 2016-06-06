

//-----------------------------------------------------------------------------
//----- ObjectToNotify.h : Declaration of the CObjectToNotify
//-----------------------------------------------------------------------------
#pragma once

#ifdef WRQ_ZDM_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP CObjectToNotify : public AcDbObject {

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
	AcDbObjectId m_blkId;//��¼�Ǳ�Ŀ�
} ;

#ifdef WRQ_ZDM_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CObjectToNotify)
#endif
