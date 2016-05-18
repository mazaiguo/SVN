
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "CBaseDataForGwDesign.h"

     // MDI safe statics
Adesk::Int16    CBaseDataForGwDesign::m_version = 0;
LPCTSTR			CBaseDataForGwDesign::m_dictName = _T("ZW_BASE_GWDESING_FOR_RQY");

ACRX_DXF_DEFINE_MEMBERS(CBaseDataForGwDesign, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        0, BASE_GWDESIGN_FOR_RQY, ZwForTRSApp)

CBaseDataForGwDesign::CBaseDataForGwDesign()
: m_label(_T("BASE"))
{
	/*m_dXScale = 500;
	m_dYScale = 100;
	m_bDrawBc = true;
	m_bDrawJiedian = false;*/
	m_strCurNum = _T("1");
	m_strNumCount = _T("1");
	m_dGlobalScale = 1.0;
	/*m_strJdNum = _T("1");
	m_dStartZH = 0;*/
}



CBaseDataForGwDesign::~CBaseDataForGwDesign()
{
	/*m_dXScale = 500;
	m_dYScale = 100;
	m_bDrawBc = true;
	m_bDrawJiedian = false;*/
	m_strCurNum = _T("1");
	m_strNumCount = _T("1");
	/*m_strJdNum = _T("1");
	m_dStartZH = 0;*/
}



LPCTSTR
CBaseDataForGwDesign::label() const
{
    assertReadEnabled();
    return m_label;
}


Acad::ErrorStatus
CBaseDataForGwDesign::setLabel(LPCTSTR newLabel)
{
        // we allow a null string label, so check for NULL
        // and turn it into NullString
    if (newLabel == NULL) 
	{
        if (m_label.IsEmpty() == FALSE)
		{   // make sure it will actually change
            assertWriteEnabled();
            m_label.Empty();
        }
        return Acad::eOk;
    }

        // don't allow to be longer than 255 for DXF simplicity
    int len = _tcslen(newLabel);
    if (len > 255)
        return Acad::eStringTooLong;

    if (m_label.Compare(newLabel))
	{
        assertWriteEnabled();
        m_label = newLabel;
    }

    return Acad::eOk;
}

CString CBaseDataForGwDesign::NumCount() 
{
	assertReadEnabled();
	return m_strNumCount;
}


Acad::ErrorStatus CBaseDataForGwDesign::setNumCount(CString nCount)
{
	assertWriteEnabled();
	m_strNumCount = nCount;

	return Acad::eOk;
}

CString CBaseDataForGwDesign::CurNum() 
{
	assertReadEnabled();
	return m_strCurNum;
}


Acad::ErrorStatus CBaseDataForGwDesign::setCurNum(CString nCount)
{
	assertWriteEnabled();
	m_strCurNum = nCount;

	return Acad::eOk;
}

double CBaseDataForGwDesign::globalScale() const
{
	assertReadEnabled();
	return m_dGlobalScale;
}

Acad::ErrorStatus CBaseDataForGwDesign::setGlobalScale(double dHx)
{
	assertWriteEnabled();
	m_dGlobalScale = dHx;

	return Acad::eOk;
}
//CString CBaseDataForGwDesign::JdNum()
//{
//	assertReadEnabled();
//	return m_strJdNum;
//}
//
//Acad::ErrorStatus CBaseDataForGwDesign::setJdNum(CString nCount)
//{
//	assertWriteEnabled();
//	m_strJdNum = nCount;
//
//	return Acad::eOk;
//}
//
//double CBaseDataForGwDesign::startZH() const
//{
//	assertReadEnabled();
//	return m_dStartZH;
//}
//
//Acad::ErrorStatus CBaseDataForGwDesign::setStartZH(double dHx)
//{
//	assertWriteEnabled();
//	m_dStartZH = dHx;
//
//	return Acad::eOk;
//}
//
//double CBaseDataForGwDesign::XScale() const
//{
//	assertReadEnabled();
//	return m_dXScale;
//}
//
//
//Acad::ErrorStatus CBaseDataForGwDesign::setXScale(double dCount)
//{
//	assertWriteEnabled();
//	m_dXScale = dCount;
//
//	return Acad::eOk;
//}
//
//double CBaseDataForGwDesign::YScale() const
//{
//	assertReadEnabled();
//	return m_dYScale;
//}
//
//
//Acad::ErrorStatus CBaseDataForGwDesign::setYScale(double dCount)
//{
//	assertWriteEnabled();
//	m_dYScale = dCount;
//
//	return Acad::eOk;
//}
//
//double CBaseDataForGwDesign::minElavation() const
//{
//	assertReadEnabled();
//	return m_dminElavation;
//}
//
//Acad::ErrorStatus CBaseDataForGwDesign::setMinElavation( double dElavation )
//{
//	assertWriteEnabled();
//	m_dminElavation = dElavation;
//	return Acad::eOk;
//}
//
//double CBaseDataForGwDesign::maxElavation() const
//{
//	assertReadEnabled();
//	return m_dmaxElavation;
//}
//
//Acad::ErrorStatus CBaseDataForGwDesign::setMaxElavation( double dElavation )
//{
//	assertWriteEnabled();
//	m_dmaxElavation = dElavation;
//	return Acad::eOk;
//}
//
//bool CBaseDataForGwDesign::createBc() const
//{
//	assertReadEnabled();
//	return m_bDrawBc;
//}
//
//
//Acad::ErrorStatus CBaseDataForGwDesign::setcreateBc(bool bDrawBc)
//{
//	assertWriteEnabled();
//	m_bDrawBc = bDrawBc;
//
//	return Acad::eOk;
//}
//
//
//bool CBaseDataForGwDesign::createJiedian() const
//{
//	assertReadEnabled();
//	return m_bDrawJiedian;
//}
//
//Acad::ErrorStatus CBaseDataForGwDesign::setcreateJieidan( bool bDrawBc )
//{
//	assertWriteEnabled();
//	m_bDrawJiedian = bDrawBc;
//
//	return Acad::eOk;
//}
//
//AcGePoint3d			CBaseDataForGwDesign::basePt() const
//{
//	assertReadEnabled();
//	return m_basePt;
//}
//
//Acad::ErrorStatus	CBaseDataForGwDesign::setbasePt(AcGePoint3d startPt)
//{
//	assertWriteEnabled();
//	m_basePt = startPt;
//
//	return Acad::eOk;
//}


Acad::ErrorStatus
CBaseDataForGwDesign::dwgInFields(AcDbDwgFiler* filer)
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgInFields(filer)) != Acad::eOk)
        return es;
	assertWriteEnabled();
    Adesk::Int16 version;
    filer->readItem(&version);
    if (version > m_version)
        return Acad::eMakeMeProxy;

    TCHAR* tmpStr = NULL;    // must explicitly set to NULL or readItem() crashes!

    filer->readItem(&tmpStr);
    m_label = tmpStr;
    acutDelString(tmpStr);
	filer->readItem(&tmpStr);
	m_strNumCount = tmpStr;
	acutDelString(tmpStr);
	filer->readItem(&tmpStr);
	m_strCurNum = tmpStr;
	acutDelString(tmpStr);

	filer->readItem(&m_dGlobalScale);
	/*filer->readItem(&tmpStr);
	m_strJdNum = tmpStr;
	acutDelString(tmpStr);

	filer->readItem(&m_dXScale);
	filer->readItem(&m_dYScale);
	filer->readItem(&m_dminElavation);
	filer->readItem(&m_dmaxElavation);
	filer->readItem(&m_bDrawBc);
	filer->readItem(&m_bDrawJiedian);
	filer->readItem(&m_basePt);
	filer->readItem(&m_dStartZH);*/
	//filer->readItem(&m_nNumCount);
	//filer->readItem(&m_nCurNum);
	/*filer->readItem(&m_startPt);
	filer->readItem(&m_endPt);
	filer->readItem(&m_dHengxiang);
	filer->readItem(&m_dZongxiang);*/


    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseDataForGwDesign::dwgOutFields(AcDbDwgFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk)
        return es;
	
	assertReadEnabled();

    filer->writeItem(m_version);

    filer->writeItem(static_cast<const TCHAR*>(m_label));
	filer->writeItem(static_cast<const TCHAR*>(m_strNumCount));
	filer->writeItem(static_cast<const TCHAR*>(m_strCurNum));
	filer->writeItem(m_dGlobalScale);
	/*filer->writeItem(static_cast<const TCHAR*>(m_strJdNum));
	filer->writeItem(m_dXScale);
	filer->writeItem(m_dYScale);
	filer->writeItem(m_dminElavation);
	filer->writeItem(m_dmaxElavation);
	filer->writeItem(m_bDrawBc);
	filer->writeItem(m_bDrawJiedian);
	filer->writeItem(m_basePt);
	filer->writeItem(m_dStartZH);*/
	/*filer->writeInt32(m_strNumCount);
	filer->writeInt32(m_strCurNum);*/
	/*filer->writeItem(m_nNumCount);
	filer->writeItem(m_nCurNum);*/
	/*filer->writeItem(m_startPt);
	filer->writeItem(m_endPt);
	filer->writeItem(m_dHengxiang);
	filer->writeItem(m_dZongxiang);*/

    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseDataForGwDesign::dxfInFields(AcDbDxfFiler* filer)
{
    Acad::ErrorStatus es;
    if (((es = AcDbObject::dxfInFields(filer)) != Acad::eOk) ||
        (filer->atSubclassData(this->desc()->name()) != Adesk::kTrue))
        return filer->filerStatus();

	assertWriteEnabled();

	struct resbuf rb;
	es = Acad::eOk;

    while ((es == Acad::eOk) && ((es = filer->readResBuf(&rb)) == Acad::eOk)) 
	{
        if (rb.restype == kDxfLabel)
            setLabel(rb.resval.rstring);        // NOTE: normally have to free string from resbuf... not in this case, DXF Filer does
		else if (rb.restype == kDxfNumCount)
		{
			setNumCount(rb.resval.rstring);
		}
		else if (rb.restype == kDxfCurNum)
		{
			setCurNum(rb.resval.rstring);
		}
		else if (rb.restype == kDxfGlobalScale)
		{
			setGlobalScale(rb.resval.rreal);
		}
		/*else if (rb.restype == kDxfJdNum)
		{
			setJdNum(rb.resval.rstring);
		}
		else if (rb.restype == kDxfXScale)
		{
			setXScale(rb.resval.rreal);
		}
		else if (rb.restype == kDxfYScale)
		{
			setYScale(rb.resval.rreal);
		}
		else if (rb.restype == kDxfminElavation)
		{
			setMinElavation(rb.resval.rreal);
		}
		else if (rb.restype == kDxfmaxElavation)
		{
			setMaxElavation(rb.resval.rreal);
		}
		else if (rb.restype == kDxfDrawBc)
		{
			setcreateBc(rb.resval.rint);
		}
		else if (rb.restype == kDxfDrawJiedian)
		{
			setcreateJieidan(rb.resval.rint);
		}
		else if (rb.restype == kDxfBasePt)
		{
			setbasePt(asPnt3d(rb.resval.rpoint));
		}
		else if (rb.restype == kDxfStartZH)
		{
			setStartZH(rb.resval.rreal);
		}*/
		/*else if (rb.restype == kDxfStartPt)
		{
           setStartPt(asPnt3d(rb.resval.rpoint));
        }
		else if (rb.restype == kDxfEndPt)
		{
			setEndPt(asPnt3d(rb.resval.rpoint));
		}
		else if (rb.restype == kDxfHengxiang)
		{
			setHengxiang(rb.resval.rreal);
		}
		else if (rb.restype == kDxfZongxiang)
		{
			setZongxiang(rb.resval.rreal);
		}*/
        else 
		{
            filer->pushBackItem();
            es = Acad::eEndOfFile;
        }
    }

    if (es != Acad::eEndOfFile)
        return Acad::eInvalidResBuf;

    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseDataForGwDesign::dxfOutFields(AcDbDxfFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfOutFields(filer)) != Acad::eOk)
        return es;

	assertReadEnabled();

    filer->writeItem(AcDb::kDxfSubclass, this->desc()->name());

    filer->writeItem(kDxfLabel, static_cast<const TCHAR*>(m_label));
	filer->writeItem(kDxfNumCount, m_strNumCount);
	filer->writeItem(kDxfCurNum, m_strCurNum);
	filer->writeItem(kDxfGlobalScale, m_dGlobalScale);
	/*filer->writeItem(kDxfJdNum, m_strJdNum);
	filer->writeItem(kDxfXScale, m_dXScale);
	filer->writeItem(kDxfYScale, m_dYScale);
	filer->writeItem(kDxfminElavation, m_dminElavation);
	filer->writeItem(kDxfmaxElavation, m_dmaxElavation);
	filer->writeItem(kDxfDrawBc, m_bDrawBc);
	filer->writeItem(kDxfDrawJiedian, m_bDrawJiedian);
	filer->writeItem(kDxfBasePt, m_basePt);
	filer->writeItem(kDxfStartZH, m_dStartZH);*/
	/*filer->writeItem(kDxfStartPt, m_startPt);
	filer->writeItem(kDxfEndPt, m_endPt);
	filer->writeItem(kDxfHengxiang, m_dHengxiang);
	filer->writeItem(kDxfZongxiang, m_dZongxiang);*/
    return filer->filerStatus();
}


AcDbObjectId
CBaseDataForGwDesign::getStandardStyle(AcDbDatabase* db, bool makeIfNotThere)
{
    Acad::ErrorStatus es;
    AcDbObjectId styleId;
    AcDbDictionary* dict;

    const TCHAR* standardStrPtr = acdbGetReservedString(AcDb::kStandard, false);
    ASSERT(standardStrPtr != NULL);
    
	dict = MyBaseUtils::openDictionaryForRead(m_dictName, db);
    if (dict) 
	{
        es = dict->getAt(standardStrPtr, styleId);
        dict->close();

        if (es == Acad::eOk)
            return styleId;

        if (makeIfNotThere == false)
            return AcDbObjectId::kNull;
    }

	dict = MyBaseUtils::openDictionaryForWrite(m_dictName, true, db);
    if (dict) 
	{
        CBaseDataForGwDesign* newRec = new CBaseDataForGwDesign;
        es = dict->setAt(standardStrPtr, newRec, styleId);
        dict->close();
        if (es == Acad::eOk) 
		{
            newRec->close();
            return styleId;
        }
        else 
		{
            delete newRec;
        }
    }

    return AcDbObjectId::kNull;
}



