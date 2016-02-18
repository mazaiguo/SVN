#include "StdAfx.h"
#include "DrawBiaochi.h"
#include "DrawDMXProcess.h"
#include "ZdmDataInfo.h"

CDrawBiaochi::CDrawBiaochi(void)
{
	m_dXScale = 500.0;
	m_dYScale = 100.0;
	acedSSAdd(NULL, NULL, m_ssname);
}

CDrawBiaochi::~CDrawBiaochi(void)
{
	acedSSFree(m_ssname);
}

bool CDrawBiaochi::Draw()
{
	if (!startDraw())
	{
		return false;
	}
	
	if (!DrawNext())
	{
		return false;
	}
		
	return true;
}


bool CDrawBiaochi::DrawNext()
{
	//////////////////////////////////////////////////////////////////////////
	//下一步绘制纵断面
	bool bContinued = true;
	while(bContinued)
	{	
		DrawDMXProcess zdm;
		if (!zdm.Draw())
		{
			bContinued = false;
		}	
	}
	return true;
}

//绘制标尺
AcDbObjectId CDrawBiaochi:: DrawBiaoChi()
{
	int nCount = m_nEndBG - m_nStartBG;
	CString strText;
	int nCurCount = 0;
	AcGePoint3d pt;
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectIdArray objIds;
	objIds.removeAll();
	AcDbObjectId groupId = AcDbObjectId::kNull;
	for (int i=0; i<nCount; i++)
	{
		nCurCount = m_nStartBG + i;
		strText.Format(_T("%d"), nCurCount);
		acutPolar(asDblArray(m_basePt), PI/2, i*m_dYRatio, asDblArray(pt));
		groupId = DrawTextAndBC(pt, strText, true&i);
		objIds.append(groupId);
	}
	objId = MyDrawEntity::MakeGroup(objIds, true);
	return objId;
}

bool CDrawBiaochi::startDraw()
{
	if (!GetXScale())
	{
		return false;
	}

	if (!GetYScale())
	{
		return false;
	}
	if (!GetStartBG())
	{
		return false;
	}
	if (!GetEndBG())
	{
		return false;
	}

	InitData();
	AcDbObjectId id1,id2;
	AcDbObjectIdArray objIds;
	id1 = DrawBiaoChi();
	if (!id1.isValid())
	{
		return false;
	}
	objIds.append(id1);
	id2	= DrawBiaoge();
	if (!id2.isValid())
	{
		return false;
	}
	objIds.append(id2);

	AcDbObjectId groupId = MyDrawEntity::MakeGroup(objIds, false);
	int nRet = MyDrag::Draggen(_T("\n指定插入点:"), m_ssname, asDblArray(m_basePt), asDblArray(m_basePt), _T(""), -1);
	if (nRet == -1)
	{
		acedSSFree(m_ssname);
		return false;
	}

	SaveDataToDwg();
	return true;
}

//绘制图框
AcDbObjectId CDrawBiaochi::DrawBiaoge()
{
	CString strFileName = gGlobal.GetAppPath() + _T("bc\\zdmmq.dwg");
	AcDbObjectId blkRecId = MyDrawEntity::GetBlkRef(strFileName);
	AcDbBlockReference* pBlkRef = new AcDbBlockReference(m_basePt, blkRecId);
	pBlkRef->setScaleFactors(AcGeScale3d(m_dXRatio/2, m_dYRatio/10, 1.0));
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef);
	AcDbObjectId blkRefId = pBlkRef->objectId();
	ads_name blkName;
	acdbGetAdsName(blkName, blkRefId);
	acedSSAdd(blkName, m_ssname, m_ssname);

	return blkRefId;
}

//交互提供数据支撑 
//获取X比例数据
bool CDrawBiaochi::GetXScale()
{
	int nRet = acedGetReal(_T("\nX方向比例<500>："), &m_dXScale);
	if (nRet == RTNORM)
	{
		return true;
	}
	else if (nRet == RTNONE)
	{
		m_dXScale = 500;
	}
	else
	{
		return false;
	}
	return true;
}
//获取Y比例数据
bool CDrawBiaochi::GetYScale()
{
	int nRet = acedGetReal(_T("\nY方向比例<100>："), &m_dYScale);
	if (nRet == RTNORM)
	{
		return true;
	}
	else if (nRet == RTNONE)
	{
		m_dYScale = 100;
	}
	else
	{
		return false;
	}
	return true;
}
//获取起始标高
bool CDrawBiaochi::GetStartBG()
{
	acedInitGet(1 , NULL);
	int nRet = acedGetInt(_T("\n标尺的起始标高<m>："), &m_nStartBG);
	if (nRet == RTNORM)
	{
		//return true;
	}
	else if (nRet == RTNONE)
	{
		m_nStartBG = 0;
	}
	else
	{
		return false;
	}
	CUtils::SetMinElavation(m_nStartBG);
	return true;
}
//获取终止标高数据
bool CDrawBiaochi::GetEndBG()
{
	acedInitGet(1 , NULL);
	bool bIsCancel = false;
	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetInt(_T("\n标尺的终点标高<m>："), &m_nEndBG);
		if (nRet == RTNORM)
		{
			//return true;
			if (m_nEndBG < m_nStartBG)
			{
				AfxMessageBox(_T("终止标高不能比起始标高低，请重新输入。"));
				bRet = false;
			}
			else
			{
				bRet = true;
			}
		}
		else if (nRet == RTNONE)
		{
			m_nEndBG = 0;
			if (m_nEndBG < m_nStartBG)
			{
				AfxMessageBox(_T("终止标高不能比起始标高低，请重新输入。"));
				bRet = false;
			}
			else
			{
				bRet = true;
			}
		}
		else
		{
			bRet = true;
			bIsCancel = true;
		}
	}
	if (bIsCancel)
	{
		return false;
	}
	CUtils::SetMaxElavation(m_nEndBG);
	return true;
}


bool CDrawBiaochi::InitData()
{
	m_dYRatio = 1000.0/m_dYScale;
	m_dXRatio = 1000.0/m_dXScale;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//具体的绘制过程
AcDbObjectId CDrawBiaochi::DrawTextAndBC(AcGePoint3d pt, CString strText, bool bFull)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId textId = AcDbObjectId::kNull;
	AcDbObjectId blkRefId = AcDbObjectId::kNull;
	
	AcDbObjectIdArray objIds;
	objIds.removeAll();

	ads_name textName;
	ads_name blkName;
	AcGePoint3d textPt;
	acutPolar(asDblArray(pt), PI, m_dXRatio*1.5, asDblArray(textPt));

	textId = MyDrawEntity::DrawText(textPt, strText, 3.0*m_dYRatio/10, AcDbObjectId::kNull, AcDb::kTextRight);
	objIds.append(textId);

	acdbGetAdsName(textName, textId);
	acedSSAdd(textName, m_ssname, m_ssname);

	CString strFileName;
	if (bFull)
	{
		strFileName = gGlobal.GetAppPath() + _T("bc\\ZDMB1.dwg");
	}
	else
	{
		strFileName = gGlobal.GetAppPath() + _T("bc\\ZDMB2.dwg");
	}

	AcDbObjectId blkRecId = MyDrawEntity::GetBlkRef(strFileName);

	AcDbBlockReference* pBlkRef = new AcDbBlockReference(pt, blkRecId);
	pBlkRef->setScaleFactors(AcGeScale3d(m_dXRatio/2, m_dYRatio*10, 1.0));
	MyBaseUtils::addToCurrentSpaceAndClose(pBlkRef);
	blkRefId = pBlkRef->objectId();
	acdbGetAdsName(blkName, blkRefId);
	acedSSAdd(blkName, m_ssname, m_ssname);
	objIds.append(blkRefId);

	objId = MyDrawEntity::MakeGroup(objIds, true);
	return objId;
}


void CDrawBiaochi::AppendIdToSS(AcDbObjectId objId)
{
	if (objId.isNull())
	{
		return;
	}
	ads_name ename;
	ads_name_clear(ename);
	acdbGetAdsName(ename, objId);
	acedSSAdd(ename, m_ssname, m_ssname);
}

void CDrawBiaochi::SaveDataToDwg()
{
	CUtils::SetXScale(m_dXScale);
	CUtils::SetYScale(m_dYScale);
	CUtils::SetbasePt(m_basePt);
	CUtils::SetcreateBc(true);
}
