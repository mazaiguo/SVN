#include "StdAfx.h"
#include "DoEntColor.h"
#include "global.h"
#include "ArxDbgUtils.h"

CDoEntColor::CDoEntColor(void)
{
	m_dWidth = gGlobal.GetIniValue(_T("查找图块"), _T("宽度"), 0);
	m_PreId = AcDbObjectId::kNull;
	m_nColor = (int)gGlobal.GetIniValue(_T("查找图块"), _T("颜色索引"), 0);
}

CDoEntColor::~CDoEntColor(void)
{
	ReStoreFrame();
}
bool CDoEntColor::SetEntFrame(AcGePoint3d minPt, AcGePoint3d maxPt)
{
	acDocManager->lockDocument(curDoc(),AcAp::kWrite,NULL,NULL,true);
	ReStoreFrame();
	AcGePoint3d LeftTopPt,RightDownPt;

	LeftTopPt.x = minPt.x;
	LeftTopPt.y = maxPt.y;
	LeftTopPt.z = RightDownPt.z = 0;

	RightDownPt.x = maxPt.x;
	RightDownPt.y = minPt.y;
	AcGePoint3dArray ptArr;
	ptArr.removeAll();
	ptArr.append(minPt);
	ptArr.append(LeftTopPt);
	ptArr.append(maxPt);
	ptArr.append(RightDownPt);

	//绘制直线
	DrawPlineLine(ptArr);


	acDocManager->sendStringToExecute(curDoc(), _T(".regen "));
	acDocManager->unlockDocument(curDoc()); 
	return true;
}

bool CDoEntColor::ReStoreFrame()
{
	if (m_PreId.isNull())
	{
		return false;
	}
	acDocManager->lockDocument(curDoc(),AcAp::kWrite,NULL,NULL,true);
	AcDbObjectId objId = AcDbObjectId::kNull;
	
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, m_PreId, AcDb::kForWrite) != Acad::eOk)
	{
		return false;
	}
	pEnt->erase();
	pEnt->close();

	acDocManager->sendStringToExecute(curDoc(), _T(".regen "));
	acDocManager->unlockDocument(curDoc()); 
	return true;
}


void CDoEntColor::DrawPlineLine(AcGePoint3dArray ptArr)
{
	if (ptArr.length() < 1)
	{
		return;
	}

	AcDbPolyline* pLine = new AcDbPolyline;
	AcGePoint3d tmpPt;
	for (int i=0; i<ptArr.length(); i++)
	{
		tmpPt = ptArr.at(i);
		pLine->addVertexAt(i, tmpPt.convert2d(AcGePlane::kXYPlane));
	}
	pLine->setConstantWidth(m_dWidth);
	pLine->setColorIndex(m_nColor);
	pLine->setClosed(Adesk::kTrue);
	ArxDbgUtils::addToCurrentSpaceAndClose(pLine);

	m_PreId = pLine->objectId();
}
