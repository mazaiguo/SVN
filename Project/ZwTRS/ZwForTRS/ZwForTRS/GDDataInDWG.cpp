
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "GDDataInDWG.h"
#include "ArxDbgUtils.h"



     // MDI safe statics
Adesk::Int16    CGDDataInDWG::m_version = 0;
LPCTSTR			CGDDataInDWG::m_dictName = _T("GD_DATA_IN_DWG");

ACRX_DXF_DEFINE_MEMBERS(CGDDataInDWG, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        0, GD_DATA_IN_DWG, ZwForTRSApp)

CGDDataInDWG::CGDDataInDWG()

{
	m_strDescription = _T("Ö÷¹ÜµÀ");
	m_strLayerColor = _T("1");
	m_strLineType = _T("ByLayer");
	m_strLineweight = _T("kLnWtByLwDefault");
	m_bDeFault = true;
}



CGDDataInDWG::~CGDDataInDWG()
{
}


LPCTSTR             CGDDataInDWG::Description() const
{
	assertReadEnabled();
	return m_strDescription;
}

Acad::ErrorStatus   CGDDataInDWG::setDescription(LPCTSTR newLabel)
{
	if (newLabel == NULL) 
	{
		if (m_strDescription.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strDescription.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(newLabel);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strDescription.Compare(newLabel))
	{
		assertWriteEnabled();
		m_strDescription = newLabel;
	}

	return Acad::eOk;
}

LPCTSTR             CGDDataInDWG::LayerColor() const
{
	assertReadEnabled();
	return m_strLayerColor;
}

Acad::ErrorStatus   CGDDataInDWG::setLayerColor(LPCTSTR newLabel)
{
	if (newLabel == NULL) 
	{
		if (m_strLayerColor.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strLayerColor.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(newLabel);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strLayerColor.Compare(newLabel))
	{
		assertWriteEnabled();
		m_strLayerColor = newLabel;
	}

	return Acad::eOk;
}

LPCTSTR             CGDDataInDWG::LineType() const
{
	assertReadEnabled();
	return m_strLineType;
}

Acad::ErrorStatus   CGDDataInDWG::setLineType(LPCTSTR newLabel)
{
	if (newLabel == NULL) 
	{
		if (m_strLineType.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strLineType.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(newLabel);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strLineType.Compare(newLabel))
	{
		assertWriteEnabled();
		m_strLineType = newLabel;
	}

	return Acad::eOk;
}

LPCTSTR             CGDDataInDWG::LineWeight() const
{
	assertReadEnabled();
	return m_strLineweight;
}

Acad::ErrorStatus   CGDDataInDWG::setLineWeight(LPCTSTR newLabel)
{
	if (newLabel == NULL) 
	{
		if (m_strLineweight.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strLineweight.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(newLabel);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strLineweight.Compare(newLabel))
	{
		assertWriteEnabled();
		m_strLineweight = newLabel;
	}

	return Acad::eOk;
}

bool CGDDataInDWG::DeFault() const
{
	assertReadEnabled();
	return m_bDeFault;
}

Acad::ErrorStatus CGDDataInDWG::setDeFault(bool newLabel)
{
	assertWriteEnabled();
	m_bDeFault = newLabel;
	return Acad::eOk;
}

Acad::ErrorStatus
CGDDataInDWG::dwgInFields(AcDbDwgFiler* filer)
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
    m_strDescription = tmpStr;
    acutDelString(tmpStr);

	filer->readItem(&tmpStr);
	m_strLayerColor = tmpStr;
	acutDelString(tmpStr);

	filer->readItem(&tmpStr);
	m_strLineType = tmpStr;
	acutDelString(tmpStr);

	filer->readItem(&tmpStr);
	m_strLineweight = tmpStr;
	acutDelString(tmpStr);

	filer->readItem(&m_bDeFault);
    return filer->filerStatus();
}


Acad::ErrorStatus
CGDDataInDWG::dwgOutFields(AcDbDwgFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk)
        return es;
	
	assertReadEnabled();

    filer->writeItem(m_version);

    filer->writeItem(static_cast<const TCHAR*>(m_strDescription));
	filer->writeItem(static_cast<const TCHAR*>(m_strLayerColor));
	filer->writeItem(static_cast<const TCHAR*>(m_strLineType));
	filer->writeItem(static_cast<const TCHAR*>(m_strLineweight));
	filer->writeItem(m_bDeFault);
    return filer->filerStatus();
}


Acad::ErrorStatus
CGDDataInDWG::dxfInFields(AcDbDxfFiler* filer)
{
	//return AcDbObject::dxfInFields(filer);
    Acad::ErrorStatus es;
    if (((es = AcDbObject::dxfInFields(filer)) != Acad::eOk) ||
        (filer->atSubclassData(this->desc()->name()) != Adesk::kTrue))
        return filer->filerStatus();

	assertWriteEnabled();

	struct resbuf rb;
	es = Acad::eOk;

    while ((es == Acad::eOk) && ((es = filer->readResBuf(&rb)) == Acad::eOk)) 
	{
        if (rb.restype == kDxfDescription)
            setDescription(rb.resval.rstring);        // NOTE: normally have to free string from resbuf... not in this case, DXF Filer does
        else if (rb.restype == kDxfLayerColor)
		{
           setLayerColor(rb.resval.rstring);
        }
		else if (rb.restype == kDxfLineType)
		{
			setLineType(rb.resval.rstring);
		}
		else if (rb.restype == kDxfLineWeight)
		{
			setLineWeight(rb.resval.rstring);
		}
		else if (rb.restype == kDXFDeFault)
		{
			setDeFault(rb.resval.rint);
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
CGDDataInDWG::dxfOutFields(AcDbDxfFiler* filer) const
{
	//return AcDbObject::dxfOutFields(filer);
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfOutFields(filer)) != Acad::eOk)
        return es;

	assertReadEnabled();

    filer->writeItem(AcDb::kDxfSubclass, this->desc()->name());

    filer->writeItem(kDxfDescription, static_cast<const TCHAR*>(m_strDescription));
	filer->writeItem(kDxfLayerColor, static_cast<const TCHAR*>(m_strLayerColor));
	filer->writeItem(kDxfLineType, static_cast<const TCHAR*>(m_strLineType));
	filer->writeItem(kDxfLineWeight, static_cast<const TCHAR*>(m_strLineweight));
	filer->writeItem(kDXFDeFault, m_bDeFault);
    return filer->filerStatus();
}

AcDbObjectId
CGDDataInDWG::getStandardStyle(AcDbDatabase* db, bool makeIfNotThere)
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

        // wasn't already there, so we need to make it
    dict = ArxDbgUtils::openDictionaryForWrite(m_dictName, true, db);
    if (dict) 
	{
        CGDDataInDWG* newRec = new CGDDataInDWG;
        es = dict->setAt(standardStrPtr, newRec, styleId);
        dict->close();
        if (es == Acad::eOk) 
		{
            newRec->close();
            return styleId;
        }
        else 
		{
            acutPrintf(_T("\nERROR: Could not add new  (%s)"), ArxDbgUtils::rxErrorStr(es));
            delete newRec;
        }
    }

    return AcDbObjectId::kNull;
}



