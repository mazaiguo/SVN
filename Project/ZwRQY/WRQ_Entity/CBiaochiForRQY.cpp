
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "CBiaochiForRQY.h"

     // MDI safe statics
Adesk::Int16    CBiaochiForRQY::m_version = 0;
LPCTSTR			CBiaochiForRQY::m_dictName = _T("ZW_BIAOCHI_DATA_IN_DWG");

ACRX_DXF_DEFINE_MEMBERS(CBiaochiForRQY, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        0, BASE_DATA_IN_DWG, ZwForTRSApp)

CBiaochiForRQY::CBiaochiForRQY()
//: m_label(_T("BASE"))
{
	m_bHasBulge = false;
	m_strCount = _T("1");//数量	
	m_label =  BC_DICT + m_strCount;
	//////////////////////////////////////////////////////////////////////////
	m_dDesignDmx = 0.0;//设计地面线
	m_dRealDmx = 0.0;//实际地面线
	m_strJiedian = _T("");//节点
	m_dcurData = 0.0;//当前桩号数据
	m_dGuanDi = 0.0;//管底
	m_dWaShen = 0.0;//挖深
	m_dPoDu = 0.0;//坡度
	m_dJuli = 0.0;//距离
	//////////////////////////////////////////////////////////////////////////
	//在同一处凸起
	//m_bHasBulge;//是否有凸起
	m_dDesignDmxS = 0.0;//第二个设计地面线
	m_dRealDmxS = 0.0;//第二个实际地面线
	m_dJiedianS = 350;
}



CBiaochiForRQY::~CBiaochiForRQY()
{
	//m_bHasBulge = false;
}



LPCTSTR
CBiaochiForRQY::label() const
{
    assertReadEnabled();
    return m_label;
}


Acad::ErrorStatus
CBiaochiForRQY::setLabel(LPCTSTR newLabel)
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


CString CBiaochiForRQY::getCount() const
{
	assertReadEnabled();
	return m_strCount;
}

double CBiaochiForRQY::getDesignDmx() const
{
	assertReadEnabled();
	return m_dDesignDmx;
}

double CBiaochiForRQY::getRealDmx() const
{
	assertReadEnabled();
	return m_dRealDmx;
}

LPCTSTR CBiaochiForRQY::getJiedian() const
{
	assertReadEnabled();
	return m_strJiedian;
}

double CBiaochiForRQY::getGuanDi() const
{
	assertReadEnabled();
	return m_dGuanDi;
}

double CBiaochiForRQY::getWaShen() const
{
	assertReadEnabled();
	return m_dWaShen;
}

double CBiaochiForRQY::getPoDu() const
{
	assertReadEnabled();
	return m_dPoDu;
}

double CBiaochiForRQY::getJuli() const
{
	assertReadEnabled();
	return m_dJuli;
}

double CBiaochiForRQY::getcurData() const
{
	assertReadEnabled();
	return m_dcurData;
}

bool CBiaochiForRQY::getHasBulge() const
{
	assertReadEnabled();
	return m_bHasBulge;
}

double CBiaochiForRQY::getDesingDmxS() const
{
	assertReadEnabled();
	return m_dDesignDmxS;
}

double CBiaochiForRQY::getRealDmxS() const
{
	assertReadEnabled();
	return m_dRealDmxS;
}

double CBiaochiForRQY::getPipeDiameter() const
{
	assertReadEnabled();
	return m_dJiedianS;
}

Acad::ErrorStatus CBiaochiForRQY::setCount(LPCTSTR newLabel )
{
	if (newLabel == NULL) 
	{
		if (m_strCount.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strCount.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(newLabel);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strCount.Compare(newLabel))
	{
		assertWriteEnabled();
		m_strCount = newLabel;
	}

	return Acad::eOk;
}


Acad::ErrorStatus CBiaochiForRQY::setDesignDmx( double strText )
{
	assertWriteEnabled();
	m_dDesignDmx = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setRealDmx( double strText )
{
	assertWriteEnabled();
	m_dRealDmx = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setJiedian( LPCTSTR newLabel )
{
	if (newLabel == NULL) 
	{
		if (m_strJiedian.IsEmpty() == FALSE)
		{   // make sure it will actually change
			assertWriteEnabled();
			m_strJiedian.Empty();
		}
		return Acad::eOk;
	}

	// don't allow to be longer than 255 for DXF simplicity
	int len = _tcslen(newLabel);
	if (len > 255)
		return Acad::eStringTooLong;

	if (m_strJiedian.Compare(newLabel))
	{
		assertWriteEnabled();
		m_strJiedian = newLabel;
	}
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setGuanDi( double strText )
{
	assertWriteEnabled();
	m_dGuanDi = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setWaShen( double strText )
{
	assertWriteEnabled();
	m_dWaShen = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setPoDu( double strPoDu )
{
	assertWriteEnabled();
	m_dPoDu = strPoDu;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setJuli( double strJuli )
{
	assertWriteEnabled();
	m_dJuli = strJuli;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setcurData( double dcurData )
{
	assertWriteEnabled();
	m_dcurData = dcurData;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setHasBulge( bool bHasBulge )
{
	assertWriteEnabled();
	m_bHasBulge = bHasBulge;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setDesingDmxS( double strText )
{
	assertWriteEnabled();
	m_dDesignDmxS = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setRealDmxS( double strText )
{
	assertWriteEnabled();
	m_dRealDmxS = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CBiaochiForRQY::setPipeDiameter( double strText )
{
	assertWriteEnabled();
	m_dJiedianS = strText;
	return Acad::eOk;
}

//AcDbObjectId CBiaochiForRQY::groupId() const
//{
//	assertReadEnabled();
//	return m_GroupId;
//}
//
//Acad::ErrorStatus CBiaochiForRQY::setGroupId( const AcDbObjectId& styleId )
//{
//	if (styleId == AcDbObjectId::kNull)
//		return Acad::eNullObjectId;
//
//	if (styleId != m_GroupId) {  // don't force notifications if nothing has changed
//		assertWriteEnabled();
//		m_GroupId = styleId;
//	}
//
//	return Acad::eOk;
//}

Acad::ErrorStatus
CBiaochiForRQY::dwgInFields(AcDbDwgFiler* filer)
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
	m_strCount = tmpStr;
	acutDelString(tmpStr);
	filer->readItem(&tmpStr);
	m_strJiedian = tmpStr;
	acutDelString(tmpStr);

	filer->readItem(&m_dDesignDmx);
	filer->readItem(&m_dRealDmx);
	filer->readItem(&m_dcurData);
	filer->readItem(&m_dGuanDi);
	filer->readItem(&m_dWaShen);
	filer->readItem(&m_dPoDu);
	filer->readItem(&m_dJuli);
	filer->readItem(&m_bHasBulge);
	filer->readItem(&m_dDesignDmxS);
	filer->readItem(&m_dRealDmxS);
	filer->readItem(&m_dJiedianS);

	//filer->readItem(&m_GroupId);
    return filer->filerStatus();
}


Acad::ErrorStatus
CBiaochiForRQY::dwgOutFields(AcDbDwgFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk)
        return es;
	
	assertReadEnabled();

    filer->writeItem(m_version);

    filer->writeItem(static_cast<const TCHAR*>(m_label));
	filer->writeItem(static_cast<const TCHAR*>(m_strCount));
	filer->writeItem(static_cast<const TCHAR*>(m_strJiedian));
	filer->writeItem(m_dDesignDmx);
	filer->writeItem(m_dRealDmx);
	filer->writeItem(m_dcurData);
	filer->writeItem(m_dGuanDi);
	filer->writeItem(m_dWaShen);
	filer->writeItem(m_dPoDu);
	filer->writeItem(m_dJuli);
	filer->writeItem(m_bHasBulge);
	filer->writeItem(m_dDesignDmxS);
	filer->writeItem(m_dRealDmxS);
	filer->writeItem(m_dJiedianS);

	//filer->writeItem(m_GroupId);
    return filer->filerStatus();
}


Acad::ErrorStatus
CBiaochiForRQY::dxfInFields(AcDbDxfFiler* filer)
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
			setCount(rb.resval.rstring);
		}
		else if (rb.restype == kDxfJieDian)
		{
			setJiedian(rb.resval.rstring);
		}
		else if (rb.restype == kDxfDesignDmx)
		{
			setDesignDmx(rb.resval.rreal);
		}
		else if (rb.restype == kDxfRealDmx)
		{
			setRealDmx(rb.resval.rreal);
		}
		else if (rb.restype == kDxfCurData)
		{
			setcurData(rb.resval.rreal);
		}
		else if (rb.restype == kDxfGuandi)
		{
			setGuanDi(rb.resval.rreal);
		}
		else if (rb.restype == kDxfWashen)
		{
			setWaShen(rb.resval.rint);
		}
		else if (rb.restype == kDxfPodu)
		{
			setPoDu(rb.resval.rreal);
		}
		else if (rb.restype == kDxfJuli)
		{
			setJuli(rb.resval.rreal);
		}
		else if (rb.restype == kDxfDesignDmxS)
		{
			setDesingDmxS(rb.resval.rreal);
		}
		else if (rb.restype == kDxfRealDmxS)
		{
			setRealDmxS(rb.resval.rreal);
		}
		else if (rb.restype == kDxfJieDianS)
		{
			setPipeDiameter(rb.resval.rreal);
		}
		else if (rb.restype == kDxfHasBulge)
		{
			setHasBulge(rb.resval.rint);
		}
		/*else if (rb.restype == kDxfGroupId)
		{
			AcDbObjectId tmpId;
			tmpId.setFromOldId(rb.resval.mnLongPtr);
			setGroupId(tmpId);
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
CBiaochiForRQY::dxfOutFields(AcDbDxfFiler* filer) const
{
    Acad::ErrorStatus es;
    if ((es = AcDbObject::dxfOutFields(filer)) != Acad::eOk)
        return es;

	assertReadEnabled();

    filer->writeItem(AcDb::kDxfSubclass, this->desc()->name());

    filer->writeItem(kDxfLabel, static_cast<const TCHAR*>(m_label));
	filer->writeItem(kDxfNumCount, static_cast<const TCHAR*>(m_strCount));
	filer->writeItem(kDxfJieDian, static_cast<const TCHAR*>(m_strJiedian));
	filer->writeItem(kDxfDesignDmx, m_dDesignDmx);
	filer->writeItem(kDxfRealDmx, m_dRealDmx);
	filer->writeItem(kDxfCurData, m_dcurData);
	filer->writeItem(kDxfGuandi, m_dGuanDi);
	filer->writeItem(kDxfWashen, m_dWaShen);
	filer->writeItem(kDxfPodu, m_dPoDu);
	filer->writeItem(kDxfJuli, m_dJuli);
	filer->writeItem(kDxfDesignDmxS, m_dDesignDmxS);
	filer->writeItem(kDxfRealDmxS, m_dRealDmxS);
	filer->writeItem(kDxfJieDianS, m_dJiedianS);
	filer->writeItem(kDxfHasBulge, m_bHasBulge);

	//filer->writeItem(kDxfGroupId, m_GroupId);
    return filer->filerStatus();
}


AcDbObjectId
CBiaochiForRQY::getStandardStyle(AcDbDatabase* db, bool makeIfNotThere)
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
        CBiaochiForRQY* newRec = new CBiaochiForRQY;
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



