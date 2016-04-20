//-----------------------------------------------------------------------------
//----- CBaseDbReactor.cpp : Implementation of CBaseDbReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CBaseDbReactor.h"
#include "SerialNo.h"
#include "BaseInfoInDwgFileUtils.h"
//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(CBaseDbReactor, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
CBaseDbReactor::CBaseDbReactor (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
CBaseDbReactor::~CBaseDbReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CBaseDbReactor::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void CBaseDbReactor::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}

AcDbDatabase *CBaseDbReactor::Subject () const {
	return (mpDatabase) ;
}

bool CBaseDbReactor::IsAttached () const {
	return (mpDatabase != NULL) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::proxyResurrectionCompleted(const AcDbDatabase * dwg, const ACHAR * appname, AcDbObjectIdArray & objects)
{
	AcDbDatabaseReactor::proxyResurrectionCompleted (dwg, appname, objects) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::goodbye(const AcDbDatabase * dwg)
{
	AcDbDatabaseReactor::goodbye (dwg) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::headerSysVarWillChange(const AcDbDatabase * dwg, const ACHAR * name)
{
	AcDbDatabaseReactor::headerSysVarWillChange (dwg, name) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::headerSysVarChanged(const AcDbDatabase * dwg, const ACHAR * name, Adesk::Boolean bSuccess)
{
	AcDbDatabaseReactor::headerSysVarChanged (dwg, name, bSuccess) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectUnAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectUnAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectReAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectReAppended (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectOpenedForModify(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectOpenedForModify (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectModified(const AcDbDatabase * dwg, const AcDbObject * dbObj)
{
	AcDbDatabaseReactor::objectModified (dwg, dbObj) ;
}

// -----------------------------------------------------------------------------
void CBaseDbReactor::objectErased(const AcDbDatabase * dwg, const AcDbObject * dbObj, Adesk::Boolean pErased)
{
	if (pErased)
	{
		/*CString str, str2;
		MyTransFunc::objToHandleStr(dbObj, str);
		str2.Format(_T("<DB REACTOR: %p>"), dwg);

		acutPrintf(_T("\n%-15s : <%-18s: %s, %s> "), str2, _T("Erased"),
			MyTransFunc::objToClassStr(dbObj), str);*/
		if (dbObj->isKindOf(CSerialNo::desc()))
		{
			//记录最下的号
			CString strCurNum = CBaseInfoInDwgFileUtils::getCurNum();
			CString strNumCount = CBaseInfoInDwgFileUtils::getNumCount();
			int nCurNum = MyTransFunc::StringToInt(strCurNum);
			int nNum =  MyTransFunc::StringToInt(strNumCount);
			nNum--;
			CSerialNo* pNo = CSerialNo::cast(dbObj);
			CString strText = pNo->strText();
			int nCur = MyTransFunc::StringToInt(strText);
			if (nCur < nCurNum)
			{
				nCurNum = nCur;	
				strCurNum.Format(_T("%d"), nCurNum);
				CBaseInfoInDwgFileUtils::SetCurNum(strCurNum);
			}
			strNumCount.Format(_T("%d"), nNum);
			CBaseInfoInDwgFileUtils::setNumCount(strNumCount);
		}
	}
	AcDbDatabaseReactor::objectErased (dwg, dbObj, pErased) ;
}

// -----------------------------------------------------------------------------
inline AcDbDatabaseReactor * CBaseDbReactor::cast(void)
{
	return NULL;
}
