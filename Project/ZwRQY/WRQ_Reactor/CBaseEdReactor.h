//-----------------------------------------------------------------------------
//----- CBaseEdReactor.h : Declaration of the CBaseEdReactor
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
#include "aced.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ CBaseEdReactor : public AcEditorReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	CBaseEdReactor (const bool autoInitAndRelease =true) ;
	virtual ~CBaseEdReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// -----------------------------------------------------------------------------
	virtual void unknownCommand(const ACHAR * cmdStr, AcDbVoidPtrArray * al);
	// -----------------------------------------------------------------------------
	virtual void commandWillStart(const ACHAR * cmdStr);
	// -----------------------------------------------------------------------------
	virtual void commandEnded(const ACHAR * cmdStr);
	// -----------------------------------------------------------------------------
	virtual void commandCancelled(const ACHAR * cmdStr);
	// -----------------------------------------------------------------------------
	virtual void commandFailed(const ACHAR * cmdStr);
	// -----------------------------------------------------------------------------
	virtual void beginClose(AcDbDatabase * pDwg);
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
	virtual void beginDwgOpen(ACHAR * filename);
	// -----------------------------------------------------------------------------
	virtual void endDwgOpen(const ACHAR * filename, AcDbDatabase * pDb);
	// -----------------------------------------------------------------------------
	virtual void initialDwgFileOpenComplete(AcDbDatabase * pDwg);
	// -----------------------------------------------------------------------------
	virtual void dwgFileOpened(AcDbDatabase * param2, ACHAR * fileName);
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
	virtual void beginDeepClone(AcDbDatabase * pTo, AcDbIdMapping & param3);
	// -----------------------------------------------------------------------------
	virtual void beginDeepCloneXlation(AcDbIdMapping & param2, Acad::ErrorStatus * param3);
	// -----------------------------------------------------------------------------
	virtual void abortDeepClone(AcDbIdMapping & param2);
	// -----------------------------------------------------------------------------
	virtual void endDeepClone(AcDbIdMapping & param2);
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
	virtual void lispWillStart(const ACHAR * firstLine);
	// -----------------------------------------------------------------------------
	virtual void lispEnded(void);
	// -----------------------------------------------------------------------------
	virtual void lispCancelled(void);
	// -----------------------------------------------------------------------------
	virtual void pickfirstModified(void);
	// -----------------------------------------------------------------------------
	virtual void layoutSwitched(const ACHAR * newLayoutName);
	// -----------------------------------------------------------------------------
	virtual void modelessOperationWillStart(const ACHAR * contextStr);
	// -----------------------------------------------------------------------------
	virtual void modelessOperationEnded(const ACHAR * contextStr);
	// -----------------------------------------------------------------------------
	virtual void beginDoubleClick(const AcGePoint3d & clickPoint);
	// -----------------------------------------------------------------------------
	virtual void beginRightClick(const AcGePoint3d & clickPoint);
	// -----------------------------------------------------------------------------
	virtual void partialOpenNotice(AcDbDatabase * pDb);
	// -----------------------------------------------------------------------------
	virtual void objectsLazyLoaded(const AcDbObjectIdArray & idArray);
	// -----------------------------------------------------------------------------
	virtual void beginQuit(void);
	// -----------------------------------------------------------------------------
	virtual void quitAborted(void);
	// -----------------------------------------------------------------------------
	virtual void quitWillStart(void);
	// -----------------------------------------------------------------------------
	virtual void sysVarChanged(const ACHAR * varName, Adesk::Boolean success);
	// -----------------------------------------------------------------------------
	virtual void sysVarWillChange(const ACHAR * varName);
	// -----------------------------------------------------------------------------
	virtual void toolbarBitmapSizeWillChange(Adesk::Boolean bLarge);
	// -----------------------------------------------------------------------------
	virtual void toolbarBitmapSizeChanged(Adesk::Boolean bLarge);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandAuto(int activity, Adesk::Boolean state);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandControl(int activity, int option);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandBegin(int activity);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandEnd(int activity);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandMark(int activity);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandBack(int activity);
	// -----------------------------------------------------------------------------
	virtual void undoSubcommandNumber(int activity, int num);
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
	virtual void beginWblockObjects(AcDbDatabase * pFrom, AcDbIdMapping & param3);
	// -----------------------------------------------------------------------------
	virtual void docFrameMovedOrResized(Adesk::LongPtr hwndDocFrame, bool bMoved);
	// -----------------------------------------------------------------------------
	virtual void mainFrameMovedOrResized(Adesk::LongPtr hwndMainFrame, bool bMoved);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandBindItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandAttachItem(AcDbDatabase * pHost, int activity, const ACHAR * pPath);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandOverlayItem(AcDbDatabase * pHost, int activity, const ACHAR * pPath);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandDetachItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandPathItem(int activity, AcDbObjectId blockId, const ACHAR * pNewPath);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandReloadItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId);
	// -----------------------------------------------------------------------------
	virtual void xrefSubcommandUnloadItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId);
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
	// -----------------------------------------------------------------------------
	virtual void cmdIUnkModified(const ACHAR * strCommand);
	// -----------------------------------------------------------------------------
	virtual Acad::ErrorStatus xrefSubCommandStart(AcXrefSubCommand subcmd, const AcDbObjectIdArray & btrIds, const ACHAR *const * btrNames, const ACHAR *const * paths);
	// -----------------------------------------------------------------------------
	virtual void preXrefLockFile(AcDbObjectId btrId);
	// -----------------------------------------------------------------------------
	virtual void viewChanged(void);
protected:
	// -----------------------------------------------------------------------------
	Acad::ErrorStatus veto(void);
//public:
//	// -----------------------------------------------------------------------------
//	static void rxInit(void);
//	// -----------------------------------------------------------------------------
//	static AcEditorReactor * cast(const AcRxObject * inPtr);
//	// -----------------------------------------------------------------------------
//	static AcRxClass * desc(void);
//	// -----------------------------------------------------------------------------
//	virtual AcRxClass * isA(void) const;
} ;
