//-----------------------------------------------------------------------------
//----- CBaseEventReactor.h : Declaration of the CBaseEventReactor
//-----------------------------------------------------------------------------
#pragma once

#ifdef WRQ_REACTOR_MOUDLE
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
#include "rxevent.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ CBaseEventReactor : public AcRxEventReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	CBaseEventReactor (const bool autoInitAndRelease =true) ;
	virtual ~CBaseEventReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcRxEvent *Subject () const ;
	virtual bool IsAttached () const ;

	// -----------------------------------------------------------------------------
	virtual void beginDeepClone(AcDbDatabase * pTo, AcDbIdMapping & param3);
	// -----------------------------------------------------------------------------
	virtual void beginDeepCloneXlation(AcDbIdMapping & param2, Acad::ErrorStatus * param3);
	// -----------------------------------------------------------------------------
	virtual void abortDeepClone(AcDbIdMapping & param2);
	// -----------------------------------------------------------------------------
	virtual void endDeepClone(AcDbIdMapping & param2);
	// -----------------------------------------------------------------------------
	virtual void dwgFileOpened(AcDbDatabase * param2, ACHAR * fileName);
	// -----------------------------------------------------------------------------
	virtual void initialDwgFileOpenComplete(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void databaseConstructed(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void databaseToBeDestroyed(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void beginSave(AcDbDatabase * param2, const ACHAR * pIntendedName);
	// -----------------------------------------------------------------------------
	virtual void saveComplete(AcDbDatabase * param2, const ACHAR * pActualName);
	// -----------------------------------------------------------------------------
	virtual void abortSave(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void partialOpenNotice(AcDbDatabase * pDb);
	// -----------------------------------------------------------------------------
	virtual void beginDxfIn(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void abortDxfIn(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void dxfInComplete(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void beginDxfOut(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void abortDxfOut(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void dxfOutComplete(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void beginInsert(AcDbDatabase * pTo, const ACHAR * pBlockName, AcDbDatabase * pFrom);
	virtual void beginInsert(void);
	// -----------------------------------------------------------------------------
	virtual void otherInsert(AcDbDatabase * pTo, AcDbIdMapping & idMap, AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void abortInsert(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void endInsert(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void wblockNotice(AcDbDatabase * pDb);
	// -----------------------------------------------------------------------------
	virtual void beginWblock(AcDbDatabase * pTo, AcDbDatabase * pFrom, const AcGePoint3d *& insertionPoint);
	virtual void beginWblock(void);
	// -----------------------------------------------------------------------------
	virtual void otherWblock(AcDbDatabase * pTo, AcDbIdMapping & param3, AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void abortWblock(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void endWblock(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void beginWblockObjects(AcDbDatabase * param2, AcDbIdMapping & param3);
	// -----------------------------------------------------------------------------
	virtual void beginAttach(AcDbDatabase * pTo, const ACHAR * param3, AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void otherAttach(AcDbDatabase * pTo, AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void abortAttach(AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void endAttach(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void redirected(AcDbObjectId newId, AcDbObjectId oldId);
	// -----------------------------------------------------------------------------
	virtual void comandeered(AcDbDatabase * pTo, AcDbObjectId id, AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void beginRestore(AcDbDatabase * pTo, const ACHAR * param3, AcDbDatabase * pFrom);
	// -----------------------------------------------------------------------------
	virtual void abortRestore(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void endRestore(AcDbDatabase * pTo);
	// -----------------------------------------------------------------------------
	virtual void xrefSubCommandStart(AcDbDatabase * pHostDb, AcXrefSubCommand op, const AcArray<AcDbObjectId> & ids, const ACHAR *const * btrNames, const ACHAR *const * paths, bool & vetoOp);
	// -----------------------------------------------------------------------------
	virtual void xrefSubCommandAborted(AcDbDatabase * pHostDb, AcXrefSubCommand op, const AcArray<AcDbObjectId> & ids, const ACHAR *const * btrNames, const ACHAR *const * paths);
	// -----------------------------------------------------------------------------
	virtual void preXrefLockFile(AcDbDatabase * pDb, AcDbObjectId btrId);
	// -----------------------------------------------------------------------------
	static void rxInit(void);
	// -----------------------------------------------------------------------------
	static AcRxEventReactor * cast(const AcRxObject * inPtr);
	// -----------------------------------------------------------------------------
	static AcRxClass * desc(void);
	// -----------------------------------------------------------------------------
	virtual AcRxClass * isA(void) const;
} ;

