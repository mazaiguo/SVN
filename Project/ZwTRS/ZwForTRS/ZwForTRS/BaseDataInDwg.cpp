
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "BaseDataInDwg.h"

     // MDI safe statics
Adesk::Int16    CBaseDataInDwg::m_version = 0;
LPCTSTR			CBaseDataInDwg::m_dictName = _T("GRID_DATA_IN_DWG");

ACRX_DXF_DEFINE_MEMBERS(CBaseDataInDwg, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        0, BASE_DATA_IN_DWG, ZwForTRSApp)

CBaseDataInDwg::CBaseDataInDwg()
: m_label(_T("BASE"))
{
	m_startPt.set(0,0,0);
	m_endPt.set(100,100,0);
	m_dHengxiang = 20;
	m_dZongxiang = 20;
	m_nBlkRefCount = 1;
}



CBaseDataInDwg::~CBaseDataInDwg()
{
}



LPCTSTR
CBaseDataInDwg::label() const
{
    assertReadEnabled();
    return m_label;
}


Acad::ErrorStatus
CBaseDataInDwg::setLabel(LPCTSTR newLabel)
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

int
CBaseDataInDwg::blkRefCount() const
{
	assertReadEnabled();
	return m_nBlkRefCount;
}


Acad::ErrorStatus
CBaseDataInDwg::setblkRefCount(int nCount)
{
	assertWriteEnabled();
	m_nBlkRefCount = nCount;

	return Acad::eOk;
}



AcGePoint3d			CBaseDataInDwg::startPt() const
{
	assertReadEnabled();
	return m_startPt;
}

Acad::ErrorStatus	CBaseDataInDwg::setStartPt(AcGePoint3d startPt)
{
	assertWriteEnabled();
	m_startPt = startPt;
	return Acad::eOk;
}


AcGePoint3d			CBaseDataInDwg::endPt() const
{
	assertReadEnabled();
	return m_endPt;
}

Acad::ErrorStatus	CBaseDataInDwg::setEndPt(AcGePoint3d endPt)
{
	assertWriteEnabled();
	m_endPt = endPt;
	return Acad::eOk;
}

double				CBaseDataInDwg::hengxiang() const
{
	assertReadEnabled();
	return m_dHengxiang;
}

Acad::ErrorStatus	CBaseDataInDwg::setHengxiang(double dHx)
{
	assertWriteEnabled();
	m_dHengxiang = dHx;
	return Acad::eOk;
}

double				CBaseDataInDwg::zongxiang() const
{
	assertReadEnabled();
	return m_dZongxiang;
}

Acad::ErrorStatus	CBaseDataInDwg::setZongxiang(double dZx)
{
	assertWriteEnabled();
	m_dZongxiang = dZx;
	return Acad::eOk;
}

Acad::ErrorStatus
CBaseDataInDwg::dwgInFields(AcDbDwgFiler* filer)
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
	filer->readItem(&m_nBlkRefCount);
	filer->readItem(&m_startPt);
	filer->readItem(&m_endPt);
	filer->readItem(&m_dHengxiang);
	filer->readItem(&m_dZongxiang);


    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseDataInDwg::dwgOutFields(AcDbDwgFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk)
        return es;
	
	assertReadEnabled();

    filer->writeItem(m_version);

    filer->writeItem(static_cast<const TCHAR*>(m_label));
	filer->writeItem(m_nBlkRefCount);
	filer->writeItem(m_startPt);
	filer->writeItem(m_endPt);
	filer->writeItem(m_dHengxiang);
	filer->writeItem(m_dZongxiang);

    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseDataInDwg::dxfInFields(AcDbDxfFiler* filer)
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
		else if (rb.restype == kDxfBlkRefCount)
		{
			setblkRefCount(rb.resval.rint);
		}
		else if (rb.restype == kDxfStartPt)
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
		}
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
CBaseDataInDwg::dxfOutFields(AcDbDxfFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfOutFields(filer)) != Acad::eOk)
        return es;

	assertReadEnabled();

    filer->writeItem(AcDb::kDxfSubclass, this->desc()->name());

    filer->writeItem(kDxfLabel, static_cast<const TCHAR*>(m_label));
	filer->writeItem(kDxfBlkRefCount, m_nBlkRefCount);
	filer->writeItem(kDxfStartPt, m_startPt);
	filer->writeItem(kDxfEndPt, m_endPt);
	filer->writeItem(kDxfHengxiang, m_dHengxiang);
	filer->writeItem(kDxfZongxiang, m_dZongxiang);
    return filer->filerStatus();
}


AcDbObjectId
CBaseDataInDwg::getStandardStyle(AcDbDatabase* db, bool makeIfNotThere)
{
    Acad::ErrorStatus es;
    AcDbObjectId styleId;
    AcDbDictionary* dict;

    const TCHAR* standardStrPtr = acdbGetReservedString(AcDb::kStandard, false);
    ASSERT(standardStrPtr != NULL);
    
	dict = ArxDbgUtils::openDictionaryForRead(m_dictName, db);
    if (dict) 
	{
        es = dict->getAt(standardStrPtr, styleId);
        dict->close();

        if (es == Acad::eOk)
            return styleId;

        if (makeIfNotThere == false)
            return AcDbObjectId::kNull;
    }

	dict = ArxDbgUtils::openDictionaryForWrite(m_dictName, true, db);
    if (dict) 
	{
        CBaseDataInDwg* newRec = new CBaseDataInDwg;
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



