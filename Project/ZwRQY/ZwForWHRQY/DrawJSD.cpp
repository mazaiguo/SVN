#include "StdAfx.h"
#include "DrawJSD.h"
#include "BcUtils.h"
#include "ZdmDataInfo.h"
#include "GWDesingUtils.h"


CDrawJSD::CDrawJSD(void)
{
}

CDrawJSD::~CDrawJSD(void)
{

}

bool CDrawJSD::draw()
{
	if (!doIt())
	{
		return false;
	}
	return true;
}

bool CDrawJSD::del()
{
	/*AcDbObjectId objId = AcDbObjectId::kNull;

	ads_name ename;
	AcGePoint3d pt;
	int nRet = acedEntSel(_T("\n��ѡ��Ҫɾ���ľ�ʾ����"), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	 
	acdbGetObjectId(objId, ename);	
	int nBlkRefCount = 0;

	CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
	if (strGroupName.Find(JSD_DICT) != -1)
	{
		MyEditEntity::EraseEntByGroupName(strGroupName);
	}
	else
	{
		AfxMessageBox(_T("��ѡ��ʾ��"));
	}*/
	ads_name ssname;
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	int nRet = acedSSGet(NULL, NULL, NULL, NULL, ssname);
	if (nRet != RTNORM)
	{
		return false;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	map<CString, CString> infoMap;
	CString strGroupName;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
		int nFind = strGroupName.Find(JSD_DICT);
		if (nFind >= 0)
		{
			MyEditEntity::EraseEntByGroupName(strGroupName);
		}
	}
	acedSSFree(ssname);
	return true;
}

AcGePoint3dArray CDrawJSD::getData()
{
	AcGePoint3dArray ptArr;
	ptArr.removeAll();
	CBcUtils bc;
	map<int, CZdmDataInfo> mapInfo = bc.getAllData();
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	for (map<int, CZdmDataInfo>::iterator iter = mapInfo.begin();
		iter != mapInfo.end(); ++iter)
	{
		CZdmDataInfo zdm = iter->second;
		AcGePoint3d startPt;

		acutPolar(asDblArray(basePt), 0, 20 + (zdm.getcurData() - CDMXUtils::startzh())*CDMXUtils::globalXScale(), asDblArray(startPt));
		acutPolar(asDblArray(startPt), PI/2, (zdm.getRealDmx() - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale() - 5, asDblArray(startPt));//����ƫ��5
		ptArr.append(startPt);
	}
	return ptArr;
}

bool CDrawJSD::drawlineAndText()
{
	int nOsmode = 512;
	MyBaseUtils::GetVar(_T("OSMODE"), &nOsmode);
	if (nOsmode != 512)
	{
		MyBaseUtils::SetVar(_T("OSMODE"), 512);
	}
	
		
	AcDbObjectIdArray objIdArr;
	objIdArr.removeAll();

	AcGePoint3d startPt, midPt,endPt,txtPt, tmpPt;
	bool bIsContinued = true;
	while (bIsContinued)
	{
		int nRet = acedGetPoint(NULL, _T("\n��ָ�����뾯ʾ����λ�ã�"), asDblArray(startPt));
		if (nRet != RTNORM)
		{
			bIsContinued = false;
			break;
		}
		
		nRet = acedGetPoint(asDblArray(startPt), _T("\n��ָ���յ�"), asDblArray(endPt));
		if (nRet != RTNORM)
		{
			bIsContinued = false;
			break;
		}

		AcDbObjectId textId = AcDbObjectId::kNull;
		AcDbObjectId plineId = AcDbObjectId::kNull;
		AcDbObjectId textStyleId = CGWDesingUtils::getGlobalTextStyle();
		AcGePoint3dArray points;
		double dLen;
		AcGePoint3d textPt;
		acutPolar(asDblArray(endPt), PI/2, 1, asDblArray(textPt));
		CString strTxt;
		TCHAR tempBuf[133];
		nRet = acedGetString(1, _T("\n����������<ȼ���ܵ���ʾ��>:"), tempBuf);
		if (nRet == RTNONE)
		{
			strTxt = _T("ȼ���ܵ���ʾ��");
		}
		else if (nRet == RTNORM)
		{
			strTxt = tempBuf;
			if (strTxt.IsEmpty())
			{
				strTxt = _T("ȼ���ܵ���ʾ��");
			}
		}
		else
		{
			bIsContinued = false;
			break;
		}
		textId = MyDrawEntity::DrawText(textPt, strTxt, 3.0, textStyleId, AcDb::kTextLeft);
		dLen = MyEditEntity::OpenObjAndGetLength(textId);
		acutPolar(asDblArray(endPt), 0, dLen, asDblArray(tmpPt));
		points.append(startPt);
		points.append(endPt);
		points.append(tmpPt);
		plineId = MyDrawEntity::DrawPlineByPoints(points);
		m_idArrs.append(textId);
		m_idArrs.append(plineId);
	}
	

	MyBaseUtils::SetVar(_T("OSMODE"), nOsmode);
	return true;
}

bool CDrawJSD::doIt()
{
	AcGePoint3dArray ptArr = getData();
	if (ptArr.length() < 1)
	{
		return false;
	}
	AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(ptArr);
	m_idArrs.append(plineId);

	
	if (!drawlineAndText())
	{
		return false;
	}
	makeGroup();
	return true;
}

void CDrawJSD::makeGroup()
{
	//throw std::logic_error("The method or operation is not implemented.");
	int nBlkRefCount = 0;
	MyBaseUtils::GetVar(_T("USERI2"), &nBlkRefCount);
	CString strName;
	strName.Format(_T("%s%d"), JSD_DICT, nBlkRefCount);
	MyDrawEntity::MakeGroup(m_idArrs, false, strName);
	nBlkRefCount++;
	MyBaseUtils::SetVar(_T("USERI2"), nBlkRefCount);
}
