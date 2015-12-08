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
//

//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AcDbBlockReferenceJig.h"
#include "MyTransFunc.h"
ZsdkBlockreferenceJig::ZsdkBlockreferenceJig(
	const AcGePoint3d& pt, AcDbObjectId& objId, double dScale, double dRotate)
	: m_CenterPt(pt), m_nPromptCounter(0)
{	
	m_blockId = objId;	
	m_Scale.set(dScale, dScale, dScale);
	m_tmpdRotate = dRotate;
}

ZsdkBlockreferenceJig::~ZsdkBlockreferenceJig()
{
}

AcEdJig::DragStatus ZsdkBlockreferenceJig::doIt()
{
	m_pBlockreference = new AcDbBlockReference;
	m_pBlockreference->setDatabaseDefaults();
	m_pBlockreference->setBlockTableRecord(m_blockId);
	//m_pBlockreference->setPosition(m_CenterPt);
	m_pBlockreference->setScaleFactors(m_Scale);

	CString strPrompt;
	setDispPrompt(_T("\n请输入插入点: "));
	AcEdJig::DragStatus stat = drag();
	m_nPromptCounter++;
	strPrompt.Format(_T("\n请输入旋转角度：<%.1f>"), m_tmpdRotate);
	stat = drag();
	setDispPrompt(strPrompt);
	
	if (stat==AcEdJig::kNormal)
	{
		append();
	}
	else if (stat == AcEdJig::kNull)
	{
		m_tmpdRotate = MyTransFunc::RotationToCurve(m_tmpdRotate);
		m_pBlockreference->setRotation(m_tmpdRotate);
		m_dRotation = m_tmpdRotate;
		append();
	}
	else
	{
		delete m_pBlockreference;
		return AcEdJig::kCancel;
	}
	return AcEdJig::kNormal;
}


AcEdJig::DragStatus ZsdkBlockreferenceJig::sampler()
{
	DragStatus stat;

	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNullResponseAccepted ));
		
		//AcGePoint3d rotatePt;	
		//stat = acquirePoint(m_RotatePt,m_CenterPt);
		/*if (rotatePt != m_RotatePt)
		{
			rotatePt = m_RotatePt;
		}*/
	if (m_nPromptCounter == 0)
	{
		static	AcGePoint3d basePt;
		stat = acquirePoint(m_CenterPt);
		if (m_CenterPt != basePt)
		{
			basePt = m_CenterPt;
		}
	}
	else if (m_nPromptCounter == 1)
	{
		static double dRotate;
		stat = acquireAngle(m_dRotation, m_CenterPt);		
		if (dRotate != m_dRotation)
		{
			dRotate = m_dRotation;
		}
		else if (stat == AcEdJig::kNormal)
		{
			return AcEdJig::kNoChange;
		}
	}
		
		
	//}
	return stat;
}

// This function is called to update the entity based on the
// input values.
//
Adesk::Boolean ZsdkBlockreferenceJig::update()
{
	//m_dRotation = acutAngle(asDblArray(m_CenterPt), asDblArray(m_RotatePt)); 
	// Now update the Blockreference with the latest setting.
	if (m_nPromptCounter == 0)
	{
		m_pBlockreference->setPosition(m_CenterPt);
	}
	else if (m_nPromptCounter == 1)
	{
		m_pBlockreference->setRotation(m_dRotation);
	}


	m_pBlockreference->close();
	return Adesk::kTrue;
}

// This function must be implemented to return a pointer to
// the entity being manipulated by the jig.
//
AcDbEntity* ZsdkBlockreferenceJig::entity() const
{
	return m_pBlockreference;
}


