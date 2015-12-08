// (C) Copyright 2005 by Autodesk, Inc. 
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

//-----------------------------------------------------------------------------
//- AcDbBlockReferenceJig.h
#pragma once

//-----------------------------------------------------------------------------
class ZsdkBlockreferenceJig : public AcEdJig
{
public:
	ZsdkBlockreferenceJig(const AcGePoint3d&, AcDbObjectId& objId, double dScale, double dRotate = 0.0);
	~ZsdkBlockreferenceJig();
	AcEdJig::DragStatus doIt();
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const;

public:
	AcGePoint3d m_CenterPt;
	double m_dRotation;
	double m_tmpdRotate;
	AcGePoint3d m_RotatePt;
	AcGeScale3d m_Scale;
	int m_nPromptCounter;
private:
	AcDbBlockReference *m_pBlockreference;
	
	AcDbObjectId m_blockId;//¼ÇÂ¼acdbblockreferenceµÄid
};
