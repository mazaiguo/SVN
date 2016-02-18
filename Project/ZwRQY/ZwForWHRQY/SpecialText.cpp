//-----------------------------------------------------------------------------
//----- SpecialText.cpp : Implementation of CSpecialText
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "SpecialText.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CSpecialText::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CSpecialText, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, SPECIALTEXT,
ZWFORWHRQYAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
CSpecialText::CSpecialText () : AcDbEntity () {
}

CSpecialText::~CSpecialText () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CSpecialText::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CSpecialText::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem(static_cast<const TCHAR*>(m_strText));
	pFiler->writeItem(m_dHeight);
	pFiler->writeItem(m_position);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CSpecialText::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CSpecialText::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CSpecialText::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	TCHAR* tmpStr = NULL; 
	pFiler->readItem(&tmpStr);
	m_strText = tmpStr;
	acutDelString(tmpStr);
	pFiler->readItem(&m_dHeight);
	pFiler->readItem(&m_position);

	return (pFiler->filerStatus ()) ;
}

AcGePoint3d CSpecialText::pos() const
{
	assertReadEnabled();
	return m_position;
}

Acad::ErrorStatus CSpecialText::setPos(const AcGePoint3d& pt)
{
	assertWriteEnabled();
	m_position = pt;
	return Acad::eOk;
}


double CSpecialText::heig() const
{
	assertReadEnabled();
	return m_dHeight;
}

Acad::ErrorStatus CSpecialText::setHeig(double dHeight)
{
	assertWriteEnabled();
	m_dHeight = dHeight;
	return Acad::eOk;
}


CString CSpecialText::txtString()
{
	assertReadEnabled();
	return m_strText;
}

Acad::ErrorStatus CSpecialText::setTxtString(CString strText)
{
	assertWriteEnabled();
	m_strText = strText;
	return Acad::eOk;
}

//AcDbObjectId CSpecialText::textStyle() const
//{
//
//}
//
//Acad::ErrorStatus CSpecialText::setTextStyle(AcDbObjectId)
//{

//}

//-----------------------------------------------------------------------------
//----- AcDbText protocols
#ifdef  ARX
Adesk::Boolean CSpecialText::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	AcDbText pText;
	AcDbDatabase* db = database();
	if (db == NULL)
		db = acdbHostApplicationServices()->workingDatabase();
	pText.subSetDatabaseDefaults(db);
	pText.setPosition(m_position);
	pText.setTextString(m_strText);
	pText.setHeight(m_dHeight);
	//pText.setWidthFactor(m_dWidthFactor);

	pText.worldDraw(mode);
	
	return (AcDbEntity::subWorldDraw (mode)) ;
}


// -----------------------------------------------------------------------------
Acad::ErrorStatus CSpecialText::subGetGeomExtents(AcDbExtents & extents) const
{
	//Acad::ErrorStatus retCode =AcDbEntity::subGetGeomExtents (extents) ;
	//return (retCode) ;
	AcGePoint3d maxPt,minPt;
	minPt = m_position;
	maxPt.x = m_position.x + 100;
	maxPt.y = m_position.y + 100;
 	maxPt.z = m_position.z;
	extents.set(minPt, maxPt);
	return Acad::eOk;
}
#else 

Adesk::Boolean CSpecialText::worldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	AcDbText pText;
	AcDbDatabase* db = database();
	if (db == NULL)
		db = acdbHostApplicationServices()->workingDatabase();
	pText.subSetDatabaseDefaults(db);
	pText.setPosition(m_position);
	pText.setTextString(m_strText);
	pText.setHeight(m_dHeight);
	//pText.setWidthFactor(m_dWidthFactor);

	pText.worldDraw(mode);

	return (AcDbEntity::worldDraw (mode)) ;
}


// -----------------------------------------------------------------------------
Acad::ErrorStatus CSpecialText::getGeomExtents(AcDbExtents & extents) const
{
	//Acad::ErrorStatus retCode =AcDbEntity::subGetGeomExtents (extents) ;
	//return (retCode) ;
	AcGePoint3d maxPt,minPt;
	minPt = m_position;
	maxPt.x = m_position.x + 100;
	maxPt.y = m_position.y + 100;
	maxPt.z = m_position.z;
	extents.set(minPt, maxPt);
	return Acad::eOk;
}
#endif