//-----------------------------------------------------------------------------
//----- CBaseEventReactor.cpp : Implementation of CBaseEventReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CBaseEventReactor.h"

//-----------------------------------------------------------------------------
CBaseEventReactor::CBaseEventReactor (const bool autoInitAndRelease) : AcRxEventReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acrxEvent )
			acrxEvent->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CBaseEventReactor::~CBaseEventReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CBaseEventReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acrxEvent ) {
			acrxEvent->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CBaseEventReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acrxEvent ) {
			acrxEvent->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcRxEvent *CBaseEventReactor::Subject () const {
	return (acrxEvent) ;
}

bool CBaseEventReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginDeepClone(AcDbDatabase * pTo, AcDbIdMapping & param3)
{
	AcRxEventReactor::beginDeepClone (pTo, param3) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginDeepCloneXlation(AcDbIdMapping & param2, Acad::ErrorStatus * param3)
{
	AcRxEventReactor::beginDeepCloneXlation (param2, param3) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortDeepClone(AcDbIdMapping & param2)
{
	AcRxEventReactor::abortDeepClone (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::endDeepClone(AcDbIdMapping & param2)
{
	AcRxEventReactor::endDeepClone (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::dwgFileOpened(AcDbDatabase * param2, ACHAR * fileName)
{
	AcRxEventReactor::dwgFileOpened (param2, fileName) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::initialDwgFileOpenComplete(AcDbDatabase * param2)
{
	AcRxEventReactor::initialDwgFileOpenComplete (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::databaseConstructed(AcDbDatabase * param2)
{
	AcRxEventReactor::databaseConstructed (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::databaseToBeDestroyed(AcDbDatabase * param2)
{
	AcRxEventReactor::databaseToBeDestroyed (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginSave(AcDbDatabase * param2, const ACHAR * pIntendedName)
{
	AcRxEventReactor::beginSave (param2, pIntendedName) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::saveComplete(AcDbDatabase * param2, const ACHAR * pActualName)
{
	AcRxEventReactor::saveComplete (param2, pActualName) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortSave(AcDbDatabase * param2)
{
	AcRxEventReactor::abortSave (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::partialOpenNotice(AcDbDatabase * pDb)
{
	AcRxEventReactor::partialOpenNotice (pDb) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginDxfIn(AcDbDatabase * param2)
{
	AcRxEventReactor::beginDxfIn (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortDxfIn(AcDbDatabase * param2)
{
	AcRxEventReactor::abortDxfIn (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::dxfInComplete(AcDbDatabase * param2)
{
	AcRxEventReactor::dxfInComplete (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginDxfOut(AcDbDatabase * param2)
{
	AcRxEventReactor::beginDxfOut (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortDxfOut(AcDbDatabase * param2)
{
	AcRxEventReactor::abortDxfOut (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::dxfOutComplete(AcDbDatabase * param2)
{
	AcRxEventReactor::dxfOutComplete (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginInsert(AcDbDatabase * pTo, const ACHAR * pBlockName, AcDbDatabase * pFrom)
{
	AcRxEventReactor::beginInsert (pTo, pBlockName, pFrom) ;
}

void CBaseEventReactor::beginInsert(void)
{
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::otherInsert(AcDbDatabase * pTo, AcDbIdMapping & idMap, AcDbDatabase * pFrom)
{
	AcRxEventReactor::otherInsert (pTo, idMap, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortInsert(AcDbDatabase * pTo)
{
	AcRxEventReactor::abortInsert (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::endInsert(AcDbDatabase * pTo)
{
	AcRxEventReactor::endInsert (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::wblockNotice(AcDbDatabase * pDb)
{
	AcRxEventReactor::wblockNotice (pDb) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginWblock(AcDbDatabase * pTo, AcDbDatabase * pFrom, const AcGePoint3d *& insertionPoint)
{
	AcRxEventReactor::beginWblock (pTo, pFrom, insertionPoint) ;
}

void CBaseEventReactor::beginWblock(void)
{
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::otherWblock(AcDbDatabase * pTo, AcDbIdMapping & param3, AcDbDatabase * pFrom)
{
	AcRxEventReactor::otherWblock (pTo, param3, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortWblock(AcDbDatabase * pTo)
{
	AcRxEventReactor::abortWblock (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::endWblock(AcDbDatabase * pTo)
{
	AcRxEventReactor::endWblock (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginWblockObjects(AcDbDatabase * param2, AcDbIdMapping & param3)
{
	AcRxEventReactor::beginWblockObjects (param2, param3) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginAttach(AcDbDatabase * pTo, const ACHAR * param3, AcDbDatabase * pFrom)
{
	AcRxEventReactor::beginAttach (pTo, param3, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::otherAttach(AcDbDatabase * pTo, AcDbDatabase * pFrom)
{
	AcRxEventReactor::otherAttach (pTo, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortAttach(AcDbDatabase * pFrom)
{
	AcRxEventReactor::abortAttach (pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::endAttach(AcDbDatabase * pTo)
{
	AcRxEventReactor::endAttach (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::redirected(AcDbObjectId newId, AcDbObjectId oldId)
{
	AcRxEventReactor::redirected (newId, oldId) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::comandeered(AcDbDatabase * pTo, AcDbObjectId id, AcDbDatabase * pFrom)
{
	AcRxEventReactor::comandeered (pTo, id, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::beginRestore(AcDbDatabase * pTo, const ACHAR * param3, AcDbDatabase * pFrom)
{
	AcRxEventReactor::beginRestore (pTo, param3, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::abortRestore(AcDbDatabase * pTo)
{
	AcRxEventReactor::abortRestore (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::endRestore(AcDbDatabase * pTo)
{
	AcRxEventReactor::endRestore (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::xrefSubCommandStart(AcDbDatabase * pHostDb, AcXrefSubCommand op, const AcArray<AcDbObjectId> & ids, const ACHAR *const * btrNames, const ACHAR *const * paths, bool & vetoOp)
{
	AcRxEventReactor::xrefSubCommandStart (pHostDb, op, ids, btrNames, paths, vetoOp) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::xrefSubCommandAborted(AcDbDatabase * pHostDb, AcXrefSubCommand op, const AcArray<AcDbObjectId> & ids, const ACHAR *const * btrNames, const ACHAR *const * paths)
{
	AcRxEventReactor::xrefSubCommandAborted (pHostDb, op, ids, btrNames, paths) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::preXrefLockFile(AcDbDatabase * pDb, AcDbObjectId btrId)
{
	AcRxEventReactor::preXrefLockFile (pDb, btrId) ;
}

// -----------------------------------------------------------------------------
void CBaseEventReactor::rxInit(void)
{
	AcRxEventReactor::rxInit () ;
}

// -----------------------------------------------------------------------------
inline AcRxEventReactor * CBaseEventReactor::cast(const AcRxObject * inPtr)
{
	AcRxEventReactor * retCode =AcRxEventReactor::cast (inPtr) ;
	return (retCode) ;
}

// -----------------------------------------------------------------------------
AcRxClass * CBaseEventReactor::desc(void)
{
	AcRxClass * retCode =AcRxEventReactor::desc () ;
	return (retCode) ;
}

// -----------------------------------------------------------------------------
AcRxClass * CBaseEventReactor::isA(void) const
{
	AcRxClass * retCode =AcRxEventReactor::isA () ;
	return (retCode) ;
}
