//
// (C) Copyright 1998-1999 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
//

#ifndef CBASEDATAINDWG_H
#define CBASEDATAINDWG_H
#include "ArxDbgUtils.h"

class CBaseDataInDwg : public AcDbObject 
{

public:
                CBaseDataInDwg();
    virtual     ~CBaseDataInDwg();

    ACRX_DECLARE_MEMBERS(CBaseDataInDwg);

        // access to data members

    LPCTSTR             label() const;
    Acad::ErrorStatus   setLabel(LPCTSTR newLabel);

	int					blkRefCount() const;
	Acad::ErrorStatus	setblkRefCount(int nCount);
	
	AcGePoint3d			startPt() const;
	Acad::ErrorStatus	setStartPt(AcGePoint3d startPt);


	AcGePoint3d			endPt() const;
	Acad::ErrorStatus	setEndPt(AcGePoint3d endPt);

	double				hengxiang() const;
	Acad::ErrorStatus	setHengxiang(double dHx);

	double				zongxiang() const;
	Acad::ErrorStatus	setZongxiang(double dZx);
        // overridden from AcDbObject
    virtual Acad::ErrorStatus   dwgInFields(AcDbDwgFiler* filer);
    virtual Acad::ErrorStatus   dwgOutFields(AcDbDwgFiler* filer) const;
    virtual Acad::ErrorStatus   dxfInFields(AcDbDxfFiler* filer);
    virtual Acad::ErrorStatus   dxfOutFields(AcDbDxfFiler* filer) const;

private:
    // data members
    CString             m_label;
	int					m_nBlkRefCount;
    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;

	AcGePoint3d			m_startPt;
	AcGePoint3d			m_endPt;
	double				m_dHengxiang;
	double				m_dZongxiang;

        // Dxf Codes
    enum 
	{
        kDxfLabel       = 1,
		kDxfBlkRefCount = 90,//90~99 32位整数值
		kDxfHengxiang	= 140,
		kDxfZongxiang	= 141,
		kDxfStartPt		= 10,
		kDxfEndPt		= 11,
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
