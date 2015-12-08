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

#ifndef CGDDATAINDWG_H
#define CGDDATAINDWG_H

class CGDDataInDWG : public AcDbObject 
{

public:
                CGDDataInDWG();
    virtual     ~CGDDataInDWG();

    ACRX_DECLARE_MEMBERS(CGDDataInDWG);

        // access to data members

    LPCTSTR             Description() const;
    Acad::ErrorStatus   setDescription(LPCTSTR newLabel);
	
	LPCTSTR             LayerColor() const;
	Acad::ErrorStatus   setLayerColor(LPCTSTR newLabel);

	LPCTSTR             LineType() const;
	Acad::ErrorStatus   setLineType(LPCTSTR newLabel);

	LPCTSTR             LineWeight() const;
	Acad::ErrorStatus   setLineWeight(LPCTSTR newLabel);

	bool				DeFault() const;
	Acad::ErrorStatus   setDeFault(bool newLabel);

        // overridden from AcDbObject
    virtual Acad::ErrorStatus   dwgInFields(AcDbDwgFiler* filer);
    virtual Acad::ErrorStatus   dwgOutFields(AcDbDwgFiler* filer) const;
    virtual Acad::ErrorStatus   dxfInFields(AcDbDxfFiler* filer);
    virtual Acad::ErrorStatus   dxfOutFields(AcDbDxfFiler* filer) const;

private:
    // data members
	CString m_strDescription;//说明
	CString m_strLayerColor;//颜色
	CString m_strLineType;//线型
	CString m_strLineweight;//线宽
    //AcDbHardPointerId   m_DieId;//记录die id
	bool	m_bDeFault;
    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;

        // Dxf Codes
    enum 
	{
        kDxfDescription       = 300,
        kDxfLayerColor		  = 301,
		kDxfLineType		  = 302,
		kDxfLineWeight		  = 303,
		kDXFDeFault			  = 290,
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
