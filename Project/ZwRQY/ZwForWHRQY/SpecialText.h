//-----------------------------------------------------------------------------
//----- SpecialText.h : Declaration of the CSpecialText
//-----------------------------------------------------------------------------
#pragma once

#ifdef ZWFORWHRQY_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbents.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP CSpecialText : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(CSpecialText) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	CSpecialText () ;
	virtual ~CSpecialText () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	enum
	{
		kMirroredInX     = 0x02,
		kMirroredInY     = 0x04,
		kGenerationMask  = 0x06,
		kAlignPtNotValid = 0x80
	};
	////»ù´¡º¯Êý
	AcGePoint3d         pos() const;
	Acad::ErrorStatus   setPos(const AcGePoint3d&);


	//double              oblique() const;
	//Acad::ErrorStatus   setOblique(double);

	//double              rotation() const; 
	//Acad::ErrorStatus   setRotation(double);

	double              heig() const; 
	Acad::ErrorStatus   setHeig(double);

	//double              widthFactor() const; 
	//Acad::ErrorStatus   setWidthFactor(double);

	CString              txtString();
	Acad::ErrorStatus   setTxtString(CString);

	/*AcDbObjectId        textStyle() const;
	Acad::ErrorStatus   setTextStyle(AcDbObjectId);*/


	//----- AcDbEntity protocols
	//- Graphics protocol
#ifdef ARX
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents & extents) const;
#else
	virtual Adesk::Boolean worldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus getGeomExtents(AcDbExtents & extents) const;
#endif



private:
	double            m_dHeight;
	//double            m_dAngle;           // << Annot Scale dependent
	//double            m_dWidthFactor;
	//double            m_dOblique;
	CString          m_strText;
	AcGePoint3d       m_position;         // << Annot Scale dependent
	// -----------------------------------------------------------------------------
} ;

//#ifdef ZWFORWHRQY_MODULE
//ACDB_REGISTER_OBJECT_ENTRY_AUTO(CSpecialText)
//#endif
