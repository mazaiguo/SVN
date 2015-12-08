// (C) Copyright 2002-2007 by Autodesk, Inc. 
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

//-----------------------------------------------------------------------------
//----- CAppEditorReactor.cpp : Implementation of CAppEditorReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CAppEditorReactor.h"
#include "CObjectAppendToDb.h"

//ACRX_NO_CONS_DEFINE_MEMBERS(CAppEditorReactor, AcEditorReactor);

//-----------------------------------------------------------------------------
CAppEditorReactor::CAppEditorReactor (const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acedEditor )
			acedEditor->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CAppEditorReactor::~CAppEditorReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CAppEditorReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CAppEditorReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *CAppEditorReactor::Subject () const {
	return (acedEditor) ;
}

bool CAppEditorReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

// -----------------------------------------------------------------------------
void CAppEditorReactor::databaseConstructed(AcDbDatabase * param2)
{
	CObjectAppendToDb::databaseConstructed(param2);
	AcEditorReactor::databaseConstructed (param2) ;
}

// -----------------------------------------------------------------------------
void CAppEditorReactor::databaseToBeDestroyed(AcDbDatabase * param2)
{
	CObjectAppendToDb::databaseDestroyed(param2);
	AcEditorReactor::databaseToBeDestroyed (param2) ;
}
