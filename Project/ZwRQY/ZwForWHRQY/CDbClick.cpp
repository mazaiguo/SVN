//-----------------------------------------------------------------------------
//----- CDbClick.cpp : Implementation of CDbClick
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CDbClick.h"
#include "MyBase.h"
//-----------------------------------------------------------------------------
CDbClick::CDbClick (const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acedEditor )
			acedEditor->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CDbClick::~CDbClick () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CDbClick::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CDbClick::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *CDbClick::Subject () const {
	return (acedEditor) ;
}

bool CDbClick::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

// -----------------------------------------------------------------------------
void CDbClick::beginDwgOpen(ACHAR * filename)
{
	AcEditorReactor::beginDwgOpen (filename) ;
}

// -----------------------------------------------------------------------------
void CDbClick::endDwgOpen(const ACHAR * filename, AcDbDatabase * pDb)
{
	AcEditorReactor::endDwgOpen (filename, pDb) ;
}

// -----------------------------------------------------------------------------
void CDbClick::dwgFileOpened(AcDbDatabase * param2, ACHAR * fileName)
{
	AcEditorReactor::dwgFileOpened (param2, fileName) ;
}

// -----------------------------------------------------------------------------
void CDbClick::databaseConstructed(AcDbDatabase * param2)
{
	AcEditorReactor::databaseConstructed (param2) ;
}

// -----------------------------------------------------------------------------
void CDbClick::databaseToBeDestroyed(AcDbDatabase * param2)
{
	AcEditorReactor::databaseToBeDestroyed (param2) ;
}

// -----------------------------------------------------------------------------
void CDbClick::beginSave(AcDbDatabase * param2, const ACHAR * pIntendedName)
{
	AcEditorReactor::beginSave (param2, pIntendedName) ;
}

// -----------------------------------------------------------------------------
void CDbClick::saveComplete(AcDbDatabase * param2, const ACHAR * pActualName)
{
	AcEditorReactor::saveComplete (param2, pActualName) ;
}

// -----------------------------------------------------------------------------
void CDbClick::abortSave(AcDbDatabase * param2)
{
	AcEditorReactor::abortSave (param2) ;
}

// -----------------------------------------------------------------------------
void CDbClick::lispWillStart(const ACHAR * firstLine)
{
	AcEditorReactor::lispWillStart (firstLine) ;
}

// -----------------------------------------------------------------------------
void CDbClick::lispEnded(void)
{
	AcEditorReactor::lispEnded () ;
}

// -----------------------------------------------------------------------------
void CDbClick::lispCancelled(void)
{
	AcEditorReactor::lispCancelled () ;
}

// -----------------------------------------------------------------------------
void CDbClick::beginDoubleClick(const AcGePoint3d & clickPoint)
{
	//AcEditorReactor::beginDoubleClick (clickPoint) ;
	acDocManager->lockDocument(curDoc(),AcAp::kWrite,NULL,NULL,true); 
	ads_name ssname,ename;
	AcDbObjectId objId;
	AcDbObject* pObj = NULL;
	int nResult = acedSSGet(NULL, asDblArray(clickPoint), NULL, NULL, ssname);
	if (nResult == RTNORM)
	{
		long len;
		acedSSLength(ssname, &len);
		if (len > 1L)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 
			return;
		}
		
		acedSSName(ssname, 0, ename);
		Acad::ErrorStatus es = acdbGetObjectId(objId, ename);


		if (acdbOpenObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 
			return;
		}
		AcDbObjectIdArray hardPointerIds, softPointerIds, hardOwnershipIds, softOwnershipIds;
		ArxDbgReferenceFiler filer;
		pObj->dwgOut(&filer);
		pObj->close();
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		hardPointerIds = filer.m_hardPointerIds;
		softPointerIds = filer.m_softPointerIds;
		hardOwnershipIds = filer.m_hardOwnershipIds;
		softOwnershipIds = filer.m_softOwnershipIds;
		if (softPointerIds.length() < 1)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 
			return;
		}

		AcDbObjectId entId = softPointerIds.at(0);
		AcDbGroup* pGroup = NULL;
		if (acdbOpenObject(pGroup, entId, AcDb::kForRead) != Acad::eOk)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 
			return;
		}
		TCHAR* name;
		pGroup->getName(name);
		CString strName = name;
		int nFind = strName.Find(BC_DICT);
		//如果找到就要弹出对话框进行下一步的处理操作
		if (nFind >= 0)
		{
			AfxMessageBox(_T("取到有用的数据!"));
		}

		acutDelString(name);
	}
	acedSSFree(ssname);
	acDocManager->unlockDocument(curDoc()); 
}

// -----------------------------------------------------------------------------
void CDbClick::beginRightClick(const AcGePoint3d & clickPoint)
{
	AcEditorReactor::beginRightClick (clickPoint) ;
}
