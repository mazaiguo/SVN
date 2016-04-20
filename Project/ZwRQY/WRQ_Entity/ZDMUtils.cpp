#include "StdAfx.h"
#include "ZDMUtils.h"
#include "CBaseDataForZdDwg.h"

CDMXUtils::CDMXUtils(void)
{
}

CDMXUtils::~CDMXUtils(void)
{
}

void CDMXUtils::setNumCount(CString nValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
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
			CBaseDataForZdDwg* newRec = NULL;
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

CString CDMXUtils::getNumCount()
{
	CString nBlkRefCount = _T("1");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

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
			CBaseDataForZdDwg* newRec = NULL;
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

void CDMXUtils::SetCurNum(CString nValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
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
			CBaseDataForZdDwg* newRec = NULL;
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

CString CDMXUtils::getCurNum()
{
	CString nBlkRefCount = _T("1");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

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
			CBaseDataForZdDwg* newRec = NULL;
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




void CDMXUtils::SetJdNum(CString nValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setJdNum(nValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setJdNum(nValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

CString CDMXUtils::getJdNum()
{
	CString nBlkRefCount = _T("1");
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->JdNum();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->JdNum();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetJdNum(_T("1"));
	}
	return nBlkRefCount;
}

void CDMXUtils::SetXScale(double dValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setXScale(dValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setXScale(dValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CDMXUtils::getXScale()
{
	double nBlkRefCount = 500;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->XScale();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->XScale();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetXScale(500);
	}
	return nBlkRefCount;
}

void CDMXUtils::SetYScale(double dValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setYScale(dValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setYScale(dValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CDMXUtils::getYScale()
{
	double nBlkRefCount = 100;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->YScale();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->YScale();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetYScale(100);
	}
	return nBlkRefCount;
}


void CDMXUtils::SetcreateBc(bool bValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setcreateBc(bValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setcreateBc(bValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

bool CDMXUtils::getcreateBc()
{
	bool nBlkRefCount = false;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->createBc();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->createBc();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetcreateBc(false);
	}
	return nBlkRefCount;
}


void CDMXUtils::SetcreateJiedian( bool bValue )
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setcreateJieidan(bValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setcreateJieidan(bValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

bool CDMXUtils::getcreateJiedian()
{
	bool nBlkRefCount = false;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->createJiedian();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->createJiedian();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetcreateJiedian(true);
	}
	return nBlkRefCount;
}

void CDMXUtils::SetbasePt(AcGePoint3d bValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setbasePt(bValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setbasePt(bValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

AcGePoint3d CDMXUtils::getbasePt()
{
	AcGePoint3d nBlkRefCount;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->basePt();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->basePt();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetbasePt(AcGePoint3d(0, 0, 0));
	}
	return nBlkRefCount;
}

void CDMXUtils::SetMinElavation(double dValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setMinElavation(dValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setMinElavation(dValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CDMXUtils::getMinElavation()
{
	double nBlkRefCount = 0;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->minElavation();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->minElavation();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetMinElavation(0);
	}
	return nBlkRefCount;
}


void CDMXUtils::SetMaxElavation(double dValue)
{
	AcDbObjectId StyleId;

	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBaseDataForZdDwg::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	if (testDict) 
	{
		Acad::ErrorStatus es;
		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = new CBaseDataForZdDwg;
			newRec->setMaxElavation(dValue);
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForWrite) != Acad::eOk)
			{
				testDict->close();
				return;
			}
			newRec->setMaxElavation(dValue);
			newRec->close();
		}
		testDict->close();
	}
	else 
	{
		return;
	}
}

double CDMXUtils::getMaxElavation()
{
	double nBlkRefCount = 0;
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(CBaseDataForZdDwg::dictName(), acdbHostApplicationServices()->workingDatabase());
	if (testDict)
	{
		Acad::ErrorStatus es;

		if (testDict->getAt(_T("BASE"), StyleId) != Acad::eOk)
		{
			CBaseDataForZdDwg* newRec = NULL;

			es = testDict->getAt(_T("BASE"), (AcDbObject *&)newRec, AcDb::kForRead);
			if (es == Acad::eOk) 
			{
				nBlkRefCount = newRec->maxElavation();
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
			CBaseDataForZdDwg* newRec = NULL;
			if (acdbOpenAcDbObject((AcDbObject*&)newRec, StyleId, AcDb::kForRead) != Acad::eOk)
			{
				testDict->close();
				return nBlkRefCount;
			}
			nBlkRefCount = newRec->maxElavation();
			newRec->close();
		}
		testDict->close();
	}
	else
	{
		SetMaxElavation(1);
	}
	return nBlkRefCount;
}
