
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "CBaseInfoInDwgFile.h"

     // MDI safe statics
Adesk::Int16    CBaseInfoInDwgFile::m_version = 0;
LPCTSTR			CBaseInfoInDwgFile::m_dictName = _T("ZW_BASE_INFO_IN_DWG_FILE_FOR_WRQ");

ACRX_DXF_DEFINE_MEMBERS(CBaseInfoInDwgFile, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        0, BASE_DATA_IN_DWG, ZwForTRSApp)

CBaseInfoInDwgFile::CBaseInfoInDwgFile()
: m_label(_T("BASE"))
{
	/*m_startPt.set(0,0,0);
	m_endPt.set(100,100,0);
	m_dHengxiang = 20;
	m_dZongxiang = 20;*/
	//m_nNumCount = 1;
	//m_nCurNum = 1;
}



CBaseInfoInDwgFile::~CBaseInfoInDwgFile()
{
}



LPCTSTR
CBaseInfoInDwgFile::label() const
{
    assertReadEnabled();
    return m_label;
}


Acad::ErrorStatus
CBaseInfoInDwgFile::setLabel(LPCTSTR newLabel)
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

CString CBaseInfoInDwgFile::NumCount() 
{
	assertReadEnabled();
	return m_strNumCount;
}


Acad::ErrorStatus CBaseInfoInDwgFile::setNumCount(CString nCount)
{
	assertWriteEnabled();
	m_strNumCount = nCount;

	return Acad::eOk;
}

CString CBaseInfoInDwgFile::CurNum() 
{
	assertReadEnabled();
	return m_strCurNum;
}


Acad::ErrorStatus CBaseInfoInDwgFile::setCurNum(CString nCount)
{
	assertWriteEnabled();
	m_strCurNum = nCount;

	return Acad::eOk;
}



Acad::ErrorStatus
CBaseInfoInDwgFile::dwgInFields(AcDbDwgFiler* filer)
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
	//filer->readItem(&m_nNumCount);
	//filer->readItem(&m_nCurNum);
	/*filer->readItem(&m_startPt);
	filer->readItem(&m_endPt);
	filer->readItem(&m_dHengxiang);
	filer->readItem(&m_dZongxiang);*/


    return filer->filerStatus();
}


Acad::ErrorStatus
CBaseInfoInDwgFile::dwgOutFields(AcDbDwgFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk)
        return es;
	
	assertReadEnabled();

    filer->writeItem(m_version);

    filer->writeItem(static_cast<const TCHAR*>(m_label));
	filer->writeItem(static_cast<const TCHAR*>(m_strNumCount));
	filer->writeItem(static_cast<const TCHAR*>(m_strCurNum));
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
CBaseInfoInDwgFile::dxfInFields(AcDbDxfFiler* filer)
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
CBaseInfoInDwgFile::dxfOutFields(AcDbDxfFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfOutFields(filer)) != Acad::eOk)
        return es;

	assertReadEnabled();

    filer->writeItem(AcDb::kDxfSubclass, this->desc()->name());

    filer->writeItem(kDxfLabel, static_cast<const TCHAR*>(m_label));
	filer->writeItem(kDxfNumCount, m_strNumCount);
	filer->writeItem(kDxfCurNum, m_strCurNum);
	/*filer->writeItem(kDxfStartPt, m_startPt);
	filer->writeItem(kDxfEndPt, m_endPt);
	filer->writeItem(kDxfHengxiang, m_dHengxiang);
	filer->writeItem(kDxfZongxiang, m_dZongxiang);*/
    return filer->filerStatus();
}


AcDbObjectId
CBaseInfoInDwgFile::getStandardStyle(AcDbDatabase* db, bool makeIfNotThere)
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
        CBaseInfoInDwgFile* newRec = new CBaseInfoInDwgFile;
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



