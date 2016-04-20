// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- CBaseEdReactor.cpp : Implementation of CBaseEdReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CBaseEdReactor.h"
//#include "CBaseInContextReactor.h"
//extern vector<CCmdButton*> m_VecButtons;
ads_name g_ssname;
//-----------------------------------------------------------------------------
CBaseEdReactor::CBaseEdReactor (const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acedEditor )
			acedEditor->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
	/*long sslen;
	acedSSLength(g_ssname, &sslen);
	if (sslen < 1)
	{
		acedSSAdd(NULL, NULL, g_ssname);
	}*/
}

//-----------------------------------------------------------------------------
CBaseEdReactor::~CBaseEdReactor () {
	Detach () ;
	//acedSSFree(g_ssname);
}

//-----------------------------------------------------------------------------
void CBaseEdReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CBaseEdReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *CBaseEdReactor::Subject () const {
	return (acedEditor) ;
}

bool CBaseEdReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::unknownCommand(const ACHAR * cmdStr, AcDbVoidPtrArray * al)
{
	AcEditorReactor::unknownCommand (cmdStr, al) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::commandWillStart(const ACHAR * cmdStr)
{
	AcEditorReactor::commandWillStart (cmdStr) ;

	
	/*resbuf *gSet=NULL, *pset=NULL;
	int nRet = acedSSGetFirst(&gSet, &pset);
	if (pset)
	{
		if (pset->restype==RTPICKS)
		{
			ads_name ename;
			ename[0] = pset->resval.rlname[0];
			ename[1] = pset->resval.rlname[1];
			acedSSAdd(ename, g_ssname, g_ssname);
		}
	}*/
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::commandEnded(const ACHAR * cmdStr)
{
	//AcEditorReactor::commandEnded (cmdStr) ;
	if (!acdbHostApplicationServices()->workingDatabase())
		return;
	CString strCmd = cmdStr;
	if ((strCmd.CompareNoCase(_T("XJDMX")) == 0) || (strCmd.CompareNoCase(_T("_XJDMX")) == 0))
	{
		
	}
	//if (m_VecButtons.size() > 0)
	//{
	//	for (vector<CCmdButton*>::iterator iter = m_VecButtons.begin();
	//		iter != m_VecButtons.end();
	//		++iter)
	//	{
	//		delete *iter;
	//	}
	//	m_VecButtons.clear();
	//}
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::commandCancelled(const ACHAR * cmdStr)
{
	if (!acdbHostApplicationServices()->workingDatabase())
		return;
	/*if (m_VecButtons.size() > 0)
	{
		for (vector<CCmdButton*>::iterator iter = m_VecButtons.begin();
			iter != m_VecButtons.end();
			++iter)
		{
			delete *iter;
		}
		m_VecButtons.clear();
	}*/
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::commandFailed(const ACHAR * cmdStr)
{
	if (!acdbHostApplicationServices()->workingDatabase())
		return;
	/*if (m_VecButtons.size() > 0)
	{
		for (vector<CCmdButton*>::iterator iter = m_VecButtons.begin();
			iter != m_VecButtons.end();
			++iter)
		{
			delete *iter;
		}
		m_VecButtons.clear();
	}*/
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginClose(AcDbDatabase * pDwg)
{
	AcEditorReactor::beginClose (pDwg) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginDxfIn(AcDbDatabase * param2)
{
	AcEditorReactor::beginDxfIn (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortDxfIn(AcDbDatabase * param2)
{
	AcEditorReactor::abortDxfIn (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::dxfInComplete(AcDbDatabase * param2)
{
	AcEditorReactor::dxfInComplete (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginDxfOut(AcDbDatabase * param2)
{
	AcEditorReactor::beginDxfOut (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortDxfOut(AcDbDatabase * param2)
{
	AcEditorReactor::abortDxfOut (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::dxfOutComplete(AcDbDatabase * param2)
{
	AcEditorReactor::dxfOutComplete (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginDwgOpen(ACHAR * filename)
{
	AcEditorReactor::beginDwgOpen (filename) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::endDwgOpen(const ACHAR * filename, AcDbDatabase * pDb)
{
	AcEditorReactor::endDwgOpen (filename, pDb) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::initialDwgFileOpenComplete(AcDbDatabase * pDwg)
{
	AcEditorReactor::initialDwgFileOpenComplete (pDwg) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::dwgFileOpened(AcDbDatabase * param2, ACHAR * fileName)
{
	AcEditorReactor::dwgFileOpened (param2, fileName) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::databaseConstructed(AcDbDatabase * param2)
{
	AcEditorReactor::databaseConstructed (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::databaseToBeDestroyed(AcDbDatabase * param2)
{
	AcEditorReactor::databaseToBeDestroyed (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginSave(AcDbDatabase * param2, const ACHAR * pIntendedName)
{
	AcEditorReactor::beginSave (param2, pIntendedName) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::saveComplete(AcDbDatabase * param2, const ACHAR * pActualName)
{
	AcEditorReactor::saveComplete (param2, pActualName) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortSave(AcDbDatabase * param2)
{
	AcEditorReactor::abortSave (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginDeepClone(AcDbDatabase * pTo, AcDbIdMapping & param3)
{
	AcEditorReactor::beginDeepClone (pTo, param3) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginDeepCloneXlation(AcDbIdMapping & param2, Acad::ErrorStatus * param3)
{
	AcEditorReactor::beginDeepCloneXlation (param2, param3) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortDeepClone(AcDbIdMapping & param2)
{
	AcEditorReactor::abortDeepClone (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::endDeepClone(AcDbIdMapping & param2)
{
	AcEditorReactor::endDeepClone (param2) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginInsert(AcDbDatabase * pTo, const ACHAR * pBlockName, AcDbDatabase * pFrom)
{
	AcEditorReactor::beginInsert (pTo, pBlockName, pFrom) ;
}

void CBaseEdReactor::beginInsert(void)
{
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::otherInsert(AcDbDatabase * pTo, AcDbIdMapping & idMap, AcDbDatabase * pFrom)
{
	AcEditorReactor::otherInsert (pTo, idMap, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortInsert(AcDbDatabase * pTo)
{
	AcEditorReactor::abortInsert (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::endInsert(AcDbDatabase * pTo)
{
	AcEditorReactor::endInsert (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::lispWillStart(const ACHAR * firstLine)
{
	AcEditorReactor::lispWillStart (firstLine) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::lispEnded(void)
{
	AcEditorReactor::lispEnded () ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::lispCancelled(void)
{
	AcEditorReactor::lispCancelled () ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::pickfirstModified(void)
{
	AcEditorReactor::pickfirstModified () ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::layoutSwitched(const ACHAR * newLayoutName)
{
	AcEditorReactor::layoutSwitched (newLayoutName) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::modelessOperationWillStart(const ACHAR * contextStr)
{
	AcEditorReactor::modelessOperationWillStart (contextStr) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::modelessOperationEnded(const ACHAR * contextStr)
{
	AcEditorReactor::modelessOperationEnded (contextStr) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginDoubleClick(const AcGePoint3d & clickPoint)
{
	AcEditorReactor::beginDoubleClick (clickPoint) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginRightClick(const AcGePoint3d & clickPoint)
{
	AcEditorReactor::beginRightClick (clickPoint) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::partialOpenNotice(AcDbDatabase * pDb)
{
	AcEditorReactor::partialOpenNotice (pDb) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::objectsLazyLoaded(const AcDbObjectIdArray & idArray)
{
	AcEditorReactor::objectsLazyLoaded (idArray) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginQuit(void)
{
	AcEditorReactor::beginQuit () ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::quitAborted(void)
{
	AcEditorReactor::quitAborted () ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::quitWillStart(void)
{
	AcEditorReactor::quitWillStart () ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::sysVarChanged(const ACHAR * varName, Adesk::Boolean success)
{
	AcEditorReactor::sysVarChanged (varName, success) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::sysVarWillChange(const ACHAR * varName)
{
	AcEditorReactor::sysVarWillChange (varName) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::toolbarBitmapSizeWillChange(Adesk::Boolean bLarge)
{
	AcEditorReactor::toolbarBitmapSizeWillChange (bLarge) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::toolbarBitmapSizeChanged(Adesk::Boolean bLarge)
{
	AcEditorReactor::toolbarBitmapSizeChanged (bLarge) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandAuto(int activity, Adesk::Boolean state)
{
	AcEditorReactor::undoSubcommandAuto (activity, state) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandControl(int activity, int option)
{
	AcEditorReactor::undoSubcommandControl (activity, option) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandBegin(int activity)
{
	AcEditorReactor::undoSubcommandBegin (activity) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandEnd(int activity)
{
	AcEditorReactor::undoSubcommandEnd (activity) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandMark(int activity)
{
	AcEditorReactor::undoSubcommandMark (activity) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandBack(int activity)
{
	AcEditorReactor::undoSubcommandBack (activity) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::undoSubcommandNumber(int activity, int num)
{
	AcEditorReactor::undoSubcommandNumber (activity, num) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::wblockNotice(AcDbDatabase * pDb)
{
	AcEditorReactor::wblockNotice (pDb) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginWblock(AcDbDatabase * pTo, AcDbDatabase * pFrom, const AcGePoint3d *& insertionPoint)
{
	AcEditorReactor::beginWblock (pTo, pFrom, insertionPoint) ;
}

void CBaseEdReactor::beginWblock(void)
{
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::otherWblock(AcDbDatabase * pTo, AcDbIdMapping & param3, AcDbDatabase * pFrom)
{
	AcEditorReactor::otherWblock (pTo, param3, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortWblock(AcDbDatabase * pTo)
{
	AcEditorReactor::abortWblock (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::endWblock(AcDbDatabase * pTo)
{
	AcEditorReactor::endWblock (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginWblockObjects(AcDbDatabase * pFrom, AcDbIdMapping & param3)
{
	AcEditorReactor::beginWblockObjects (pFrom, param3) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::docFrameMovedOrResized(Adesk::LongPtr hwndDocFrame, bool bMoved)
{
	AcEditorReactor::docFrameMovedOrResized (hwndDocFrame, bMoved) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::mainFrameMovedOrResized(Adesk::LongPtr hwndMainFrame, bool bMoved)
{
	AcEditorReactor::mainFrameMovedOrResized (hwndMainFrame, bMoved) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandBindItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId)
{
	AcEditorReactor::xrefSubcommandBindItem (pHost, activity, blockId) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandAttachItem(AcDbDatabase * pHost, int activity, const ACHAR * pPath)
{
	AcEditorReactor::xrefSubcommandAttachItem (pHost, activity, pPath) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandOverlayItem(AcDbDatabase * pHost, int activity, const ACHAR * pPath)
{
	AcEditorReactor::xrefSubcommandOverlayItem (pHost, activity, pPath) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandDetachItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId)
{
	AcEditorReactor::xrefSubcommandDetachItem (pHost, activity, blockId) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandPathItem(int activity, AcDbObjectId blockId, const ACHAR * pNewPath)
{
	AcEditorReactor::xrefSubcommandPathItem (activity, blockId, pNewPath) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandReloadItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId)
{
	AcEditorReactor::xrefSubcommandReloadItem (pHost, activity, blockId) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::xrefSubcommandUnloadItem(AcDbDatabase * pHost, int activity, AcDbObjectId blockId)
{
	AcEditorReactor::xrefSubcommandUnloadItem (pHost, activity, blockId) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginAttach(AcDbDatabase * pTo, const ACHAR * param3, AcDbDatabase * pFrom)
{
	AcEditorReactor::beginAttach (pTo, param3, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::otherAttach(AcDbDatabase * pTo, AcDbDatabase * pFrom)
{
	AcEditorReactor::otherAttach (pTo, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortAttach(AcDbDatabase * pFrom)
{
	AcEditorReactor::abortAttach (pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::endAttach(AcDbDatabase * pTo)
{
	AcEditorReactor::endAttach (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::redirected(AcDbObjectId newId, AcDbObjectId oldId)
{
	AcEditorReactor::redirected (newId, oldId) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::comandeered(AcDbDatabase * pTo, AcDbObjectId id, AcDbDatabase * pFrom)
{
	AcEditorReactor::comandeered (pTo, id, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::beginRestore(AcDbDatabase * pTo, const ACHAR * param3, AcDbDatabase * pFrom)
{
	AcEditorReactor::beginRestore (pTo, param3, pFrom) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::abortRestore(AcDbDatabase * pTo)
{
	AcEditorReactor::abortRestore (pTo) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::endRestore(AcDbDatabase * pTo)
{
	AcEditorReactor::endRestore (pTo) ;
}


// -----------------------------------------------------------------------------
void CBaseEdReactor::cmdIUnkModified(const ACHAR * strCommand)
{
	AcEditorReactor::cmdIUnkModified (strCommand) ;
}

// -----------------------------------------------------------------------------
Acad::ErrorStatus CBaseEdReactor::xrefSubCommandStart(AcXrefSubCommand subcmd, const AcDbObjectIdArray & btrIds, const ACHAR *const * btrNames, const ACHAR *const * paths)
{
	Acad::ErrorStatus retCode =AcEditorReactor::xrefSubCommandStart (subcmd, btrIds, btrNames, paths) ;
	return (retCode) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::preXrefLockFile(AcDbObjectId btrId)
{
	AcEditorReactor::preXrefLockFile (btrId) ;
}

// -----------------------------------------------------------------------------
void CBaseEdReactor::viewChanged(void)
{
	AcEditorReactor::viewChanged () ;
}

// -----------------------------------------------------------------------------
Acad::ErrorStatus CBaseEdReactor::veto(void)
{
	Acad::ErrorStatus retCode =AcEditorReactor::veto () ;
	return (retCode) ;
}

// -----------------------------------------------------------------------------
//void CBaseEdReactor::rxInit(void)
//{
//	AcEditorReactor::rxInit () ;
//}
//
//// -----------------------------------------------------------------------------
//inline AcEditorReactor * CBaseEdReactor::cast(const AcRxObject * inPtr)
//{
//	AcEditorReactor * retCode =AcEditorReactor::cast (inPtr) ;
//	return (retCode) ;
//}
//
//// -----------------------------------------------------------------------------
//AcRxClass * CBaseEdReactor::desc(void)
//{
//	AcRxClass * retCode =AcEditorReactor::desc () ;
//	return (retCode) ;
//}
//
//// -----------------------------------------------------------------------------
//AcRxClass * CBaseEdReactor::isA(void) const
//{
//	AcRxClass * retCode =AcEditorReactor::isA () ;
//	return (retCode) ;
//}
