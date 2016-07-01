#include "StdAfx.h"
#include "GWDesingUtils.h"
#include "CBaseDataForGwDesign.h"
#include "MySymble.h"

CGWDesingUtils::CGWDesingUtils(void)
{
}

CGWDesingUtils::~CGWDesingUtils(void)
{
}

void CGWDesingUtils::setNumCount(CString nValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForGwDesign::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
			newRec->setNumCount(nValue);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				//MyBaseUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseDataForGwDesign* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setNumCount(nValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

CString CGWDesingUtils::getNumCount()
{
	CString nBlkRefCount = _T("1");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForGwDesign* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->NumCount();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseDataForGwDesign* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->NumCount();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		setNumCount(_T("1"));
	}
	return nBlkRefCount;
}

void CGWDesingUtils::SetCurNum(CString nValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForGwDesign::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
			newRec->setCurNum(nValue);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				//MyBaseUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseDataForGwDesign* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setCurNum(nValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

CString CGWDesingUtils::getCurNum()
{
	CString nBlkRefCount = _T("1");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForGwDesign* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->CurNum();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseDataForGwDesign* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->CurNum();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetCurNum(_T("1"));
	}
	return nBlkRefCount;
}

void CGWDesingUtils::SetGlobalScale(double dValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForGwDesign::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
			newRec->setGlobalScale(dValue);
			es = testDict->setAt(_T("BASE"), newRec, StyleId);
			if (es == Acad::eOk)
			{
				newRec->close();
			}
			else
			{
				//MyBaseUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseDataForGwDesign* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setGlobalScale(dValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CGWDesingUtils::getGlobalScale()
{
	double nBlkRefCount = 1.0;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForGwDesign* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->globalScale();
				newRec->close();
			}
			else 
			{
				//ArxDbgUtils::rxErrorAlert(es);
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
		else
		{
			CBaseDataForGwDesign* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->globalScale();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetGlobalScale(1.0);
	}
	return nBlkRefCount;
}


double CGWDesingUtils::getGlobalRadius()
{
	return getGlobalScale()*3;
}

double CGWDesingUtils::getGlobalTextHeight()
{
	return getGlobalScale()*4;
}

AcDbObjectId CGWDesingUtils::getGlobalTextStyle()
{
	AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 3.0);
	return textStyleId;
}

AcDbObjectId CGWDesingUtils::getGlobalGdLayerId()
{
	AcDbObjectId layerId = MySymble::CreateNewLayer(_T("È¼Æø¹ÜµÀ"), 1, TRUE);
	return layerId;
}	

//void CGWDesingUtils::SetJdNum(CString nValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setJdNum(nValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setJdNum(nValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//CString CGWDesingUtils::getJdNum()
//{
//	CString nBlkRefCount = _T("1");
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->JdNum();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->JdNum();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetJdNum(_T("1"));
//	}
//	return nBlkRefCount;
//}
//
//void CGWDesingUtils::SetStartZH(double dValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setStartZH(dValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setStartZH(dValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//double CGWDesingUtils::getStartZH()
//{
//	double nBlkRefCount = 500;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->startZH();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->startZH();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetStartZH(0);
//	}
//	return nBlkRefCount;
//}
//
//void CGWDesingUtils::SetXScale(double dValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setXScale(dValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setXScale(dValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//double CGWDesingUtils::getXScale()
//{
//	double nBlkRefCount = 500;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->XScale();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->XScale();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetXScale(500);
//	}
//	return nBlkRefCount;
//}
//
//void CGWDesingUtils::SetYScale(double dValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setYScale(dValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setYScale(dValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//double CGWDesingUtils::getYScale()
//{
//	double nBlkRefCount = 100;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->YScale();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->YScale();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetYScale(100);
//	}
//	return nBlkRefCount;
//}
//
//
//void CGWDesingUtils::SetcreateBc(bool bValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setcreateBc(bValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setcreateBc(bValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//bool CGWDesingUtils::getcreateBc()
//{
//	bool nBlkRefCount = false;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->createBc();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->createBc();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetcreateBc(false);
//	}
//	return nBlkRefCount;
//}
//
//
//void CGWDesingUtils::SetcreateJiedian( bool bValue )
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setcreateJieidan(bValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setcreateJieidan(bValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//bool CGWDesingUtils::getcreateJiedian()
//{
//	bool nBlkRefCount = false;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->createJiedian();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->createJiedian();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetcreateJiedian(false);
//	}
//	return nBlkRefCount;
//}
//
//void CGWDesingUtils::SetbasePt(AcGePoint3d bValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setbasePt(bValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setbasePt(bValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//AcGePoint3d CGWDesingUtils::getbasePt()
//{
//	AcGePoint3d nBlkRefCount;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->basePt();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->basePt();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetbasePt(AcGePoint3d(0, 0, 0));
//	}
//	return nBlkRefCount;
//}
//
//void CGWDesingUtils::SetMinElavation(double dValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setMinElavation(dValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setMinElavation(dValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//double CGWDesingUtils::getMinElavation()
//{
//	double nBlkRefCount = 0;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->minElavation();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->minElavation();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetMinElavation(0);
//	}
//	return nBlkRefCount;
//}
//
//
//void CGWDesingUtils::SetMaxElavation(double dValue)
//{
//	AcDbObjectId StyleId;
//
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
//		CBaseDataForGwDesign::dictName(), true,
//		acdbHostApplicationServices()->workingDatabase());
//	if (testDict) 
//	{
//		Acad::ErrorStatus es;
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
//			newRec->setMaxElavation(dValue);
//			es = testDict->setAt(_T("BASE"), newRec, StyleId);
//			if (es == Acad::eOk)
//			{
//				newRec->close();
//			}
//			else
//			{
//				//MyBaseUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
//			{
//				testDict->close();
//				return;
//			}
//			newRec->setMaxElavation(dValue);
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else 
//	{
//		return;
//	}
//}
//
//double CGWDesingUtils::getMaxElavation()
//{
//	double nBlkRefCount = 0;
//	AcDbObjectId StyleId;
//	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForGwDesign::dictName(), acdbHostApplicationServices()->workingDatabase());
//	if (testDict)
//	{
//		Acad::ErrorStatus es;
//
//		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//
//			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
//			if (es == Acad::eOk) 
//			{
//				nBlkRefCount = newRec->maxElavation();
//				newRec->close();
//			}
//			else 
//			{
//				//ArxDbgUtils::rxErrorAlert(es);
//				delete newRec;
//				StyleId = AcDbObjectId::kNull;
//			}
//		}
//		else
//		{
//			CBaseDataForGwDesign* newRec = NULL;
//			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
//			{
//				testDict->close();
//				return nBlkRefCount;
//			}
//			nBlkRefCount = newRec->maxElavation();
//			newRec->close();
//		}
//		testDict->close();
//	}
//	else
//	{
//		SetMaxElavation(1);
//	}
//	return nBlkRefCount;
//}
