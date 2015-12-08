//-----------------------------------------------------------------------------
//----- CAppEditorReactor.h : Declaration of the CAppEditorReactor
//-----------------------------------------------------------------------------
#pragma once

#ifndef CAPPEDITORREACTOR_H
#define CAPPEDITORREACTOR_H

//-----------------------------------------------------------------------------
#include "aced.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ CAppEditorReactor : public AcEditorReactor {

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	CAppEditorReactor (const bool autoInitAndRelease =true) ;
	virtual ~CAppEditorReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// -----------------------------------------------------------------------------
	virtual void databaseConstructed(AcDbDatabase * param2);
	// -----------------------------------------------------------------------------
	virtual void databaseToBeDestroyed(AcDbDatabase * param2);
} ;

#endif
//#ifdef ZWFORTRS_MODULE
//ACDB_REGISTER_OBJECT_ENTRY_AUTO(CAppEditorReactor)
//#endif

