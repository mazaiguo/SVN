//-----------------------------------------------------------------------------
//----- CDbClick.h : Declaration of the CDbClick
//-----------------------------------------------------------------------------
#pragma once

#ifdef ZWFORWHRQY_MODULE
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
class /*DLLIMPEXP*/ CDbClick : public AcEditorReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	CDbClick (const bool autoInitAndRelease =true) ;
	virtual ~CDbClick () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// -----------------------------------------------------------------------------
	virtual void beginDwgOpen(ACHAR * filename);
	// -----------------------------------------------------------------------------
	virtual void endDwgOpen(const ACHAR * filename, AcDbDatabase * pDb);
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
	virtual void lispWillStart(const ACHAR * firstLine);
	// -----------------------------------------------------------------------------
	virtual void lispEnded(void);
	// -----------------------------------------------------------------------------
	virtual void lispCancelled(void);
	// -----------------------------------------------------------------------------
	virtual void beginDoubleClick(const AcGePoint3d & clickPoint);
	// -----------------------------------------------------------------------------
	virtual void beginRightClick(const AcGePoint3d & clickPoint);
} ;
