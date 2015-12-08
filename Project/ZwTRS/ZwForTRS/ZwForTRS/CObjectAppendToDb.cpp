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
//----- CObjectAppendToDb.cpp : Implementation of CObjectAppendToDb
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CObjectAppendToDb.h"
#include "MyEditEntity.h"
#include "ArxDbgXdata.h"

extern CObjectAppendToDb* m_dbReactor;
//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(CObjectAppendToDb, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
CObjectAppendToDb::CObjectAppendToDb (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
CObjectAppendToDb::~CObjectAppendToDb () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CObjectAppendToDb::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void CObjectAppendToDb::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}

AcDbDatabase *CObjectAppendToDb::Subject () const {
	return (mpDatabase) ;
}

bool CObjectAppendToDb::IsAttached () const {
	return (mpDatabase != NULL) ;
}

// -----------------------------------------------------------------------------
void CObjectAppendToDb::objectAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	//if (dbObj->isKindOf(AcDbEntity::desc()))
	//{
	//	AcDbEntity* pEnt = AcDbEntity::cast(dbObj);
	//	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	//	{
	//		AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
	//		CString strXdata;
	//		ArxDbgAppXdataList  xdata(pBlkRef);
	//		ArxDbgAppXdataListIterator iter(xdata);
	//		if (xdata.isEmpty())
	//		{				
	//			return;
	//		}

	//		int nIndex = 0;
	//		for (;!iter.done(); iter.next())
	//		{
	//			if (!_tcsicmp(ZRXDBG_TYPE, iter.item()->getAppName()))
	//			{
	//				break;
	//			}
	//			else
	//			{
	//				nIndex++;
	//			}
	//		}
	//		ArxDbgAppXdata* xdPtr = xdata.at(nIndex);
	//		bool bIsOk = xdPtr->getString(XDT_XG_CSTRING, strXdata);
	//		if (!bIsOk)
	//		{
	//			return;
	//		}
	//		else
	//		{
	//			AcDbSortentsTable *pSt = MyEditEntity::get_sortents_table_of( pBlkRef->ownerId());
	//			//pEnt->close();
	//			if (NULL == pSt)
	//				return;	
	//			AcDbObjectIdArray entity_array;
	//			entity_array.append(pBlkRef->objectId());
	//			Acad::ErrorStatus es;
	//			es = pSt->moveToTop(entity_array);//移到底部
	//			pSt->close();
	//		}
	//	}
	//	//else
	//	//{
	//	//	//MyTransFunc::DuiXiangPaiXu(dbObj->objectId(), true);	
	//	//	AcDbSortentsTable *pSt = MyEditEntity::get_sortents_table_of( pEnt->ownerId());
	//	//	//pEnt->close();
	//	//	if (NULL == pSt)
	//	//		return;	
	//	//	AcDbObjectIdArray entity_array;
	//	//	entity_array.append(pEnt->objectId());
	//	//	Acad::ErrorStatus es;
	//	//	es = pSt->moveToBottom(entity_array);//移到底部
	//	//	pSt->close();
	//	//	return;
	//	//}
	//}
	
	AcDbDatabaseReactor::objectAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CObjectAppendToDb::objectUnAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectUnAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CObjectAppendToDb::objectReAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectReAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CObjectAppendToDb::objectOpenedForModify(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectOpenedForModify (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CObjectAppendToDb::objectModified(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectModified (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CObjectAppendToDb::objectErased(const AcDbDatabase * dwg, const AcDbObject * dbObj, Adesk::Boolean pErased)
{
	AcDbDatabaseReactor::objectErased (dwg, dbObj, pErased) ;
}


void CObjectAppendToDb::databaseConstructed(AcDbDatabase* db)
{
	if (m_dbReactor)
		db->addReactor(m_dbReactor);
}

void CObjectAppendToDb::databaseDestroyed(AcDbDatabase* db)
{
	if (m_dbReactor)
		db->removeReactor(m_dbReactor);
}

