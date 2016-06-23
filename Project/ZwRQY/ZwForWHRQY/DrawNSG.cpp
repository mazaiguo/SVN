#include "StdAfx.h"
#include "DrawNSG.h"
#include "GWDesingUtils.h"
#include "ZDMUtils.h"
#include "ZdmDataInfo.h"
#include "BcUtils.h"
#include "BlkInfo.h"

CDrawNSG::CDrawNSG(void)
{
}

CDrawNSG::~CDrawNSG(void)
{
}

bool CDrawNSG::draw()
{
	if (!doIt())
	{
		return false;
	}
	return true;
}

bool CDrawNSG::del()
{
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
		int nFind = strGroupName.Find(NSG_DICT);
		if (nFind >= 0)
		{
			MyEditEntity::EraseEntByGroupName(strGroupName);
		}
	}
	return true;
}

bool CDrawNSG::drawlineAndText()
{
	AcGePoint3d midPt,endPt;
	int nRet = acedGetPoint(asDblArray(m_blkInsert), _T("\n��ָ���յ�"), asDblArray(midPt));
	if (nRet != RTNORM)
	{
		return false;
	}
	AcDbObjectId textStyleId = CGWDesingUtils::getGlobalTextStyle();

	AcGePoint3d textPt1, textPt2;
	acutPolar(asDblArray(midPt), PI/2, 1, asDblArray(textPt1));
	acutPolar(asDblArray(midPt), 3*PI/2, 1, asDblArray(textPt2));
	AcDbObjectId textId1 = MyDrawEntity::DrawText(textPt1, m_strNo, 3.0, textStyleId, AcDb::kTextLeft);
	double dLen1 = MyEditEntity::OpenObjAndGetLength(textId1);
	AcDbObjectId textId2 = MyDrawEntity::DrawText(textPt2, m_strZF, 3.0, textStyleId, AcDb::kTextLeft, AcDb::kTextTop);
	double dLen2 = MyEditEntity::OpenObjAndGetLength(textId2);
	double dLen;
	if (dLen1 > dLen2)
	{
		dLen = dLen1;
	}
	else
	{
		dLen = dLen2;
	}
	acutPolar(asDblArray(midPt), 0, dLen, asDblArray(endPt));

	AcGePoint3dArray points;
	points.append(m_blkInsert);
	points.append(midPt);
	points.append(endPt);
	AcDbObjectId plineId = MyDrawEntity::DrawPlineByPoints(points);
	plineId = MyEditEntity::openEntChangeColor(plineId, 2);
	m_idArrs.append(plineId);
	m_idArrs.append(textId1);
	m_idArrs.append(textId2);
	return true;
}

bool CDrawNSG::doIt()
{
	if (!getZhuanghao())
	{
		return false;
	}

	if (!GetDescription())
	{
		return false;
	}

	if (!GetZuoFa())
	{
		return false;
	}

	if (!insert())
	{
		return false;
	}
	
	drawlineAndText();

	makeGroup();

	return true;
}

void CDrawNSG::makeGroup()
{
	int nBlkRefCount = 0;
	MyBaseUtils::GetVar(_T("USERI2"), &nBlkRefCount);
	CString strName;
	strName.Format(_T("%s%d"), NSG_DICT, nBlkRefCount);
	MyDrawEntity::MakeGroup(m_idArrs, false, strName);
	nBlkRefCount++;
	MyBaseUtils::SetVar(_T("USERI2"), nBlkRefCount);
}

bool CDrawNSG::getZhuanghao()
{
	CString strPrompt;
	int nRet = acedGetReal(_T("\nָ������׮��ֵ<m>:"), &m_dZhuanghao);
	if (nRet != RTNORM)
	{
		return false;
	}
	bool bIsExisted = false;
	CString strCur = CurNumPosition(m_dZhuanghao, bIsExisted);
	if (!bIsExisted)
	{
		AfxMessageBox(_T("������û��׮�ŵĵط�������ˮ�ף����²���׮��"));
		return false;
	}
	return true;
}

bool CDrawNSG::GetDescription()
{
	TCHAR tempBuf[133];

	int nRet = acedGetString(1, _T("\n�������ͺ�<DN300>:"),tempBuf);
	if (nRet == RTNORM)
	{
		CString strTmp = tempBuf;
		if (strTmp.IsEmpty())
		{
			m_strNo = _T("DN300������ˮ��");
		}
		else
		{
			m_strNo.Format(_T("%s������ˮ��"), strTmp);
		}
	}
	else if (nRet == RTNONE)
	{
		m_strNo = _T("DN300������ˮ��");
	}
	else
	{
		return false;
	}
	
	return true;
}

bool CDrawNSG::insert()
{
	bool bIsExisted = false;
	CString strCur = CurNumPosition(m_dZhuanghao, bIsExisted);
	CString strLabel = BC_DICT + strCur;
	double dXScale = 1000/(CDMXUtils::getXScale());
	double dYScale = 1000/(CDMXUtils::getYScale());
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	AcGePoint3d tmpPt,insertPt;
	acutPolar(asDblArray(basePt), 0, 20 + m_dZhuanghao*dXScale, asDblArray(tmpPt));
	CBcUtils bc;
	CZdmDataInfo zdm;
	bIsExisted = bc.get(strLabel, zdm);	

	double dRotate = 0.0;
	if (!bIsExisted)//���û���ҵ���˵����strCur��strCur+1֮��
	{
		AfxMessageBox(_T("������û��׮�ŵĵط�������ˮ�ף����²���׮��"));
		return false;
	}
	else
	{
		acutPolar(asDblArray(tmpPt), PI/2, (zdm.getRealDmx() - CDMXUtils::getMinElavation())*dYScale, asDblArray(m_blkInsert));
	}
	acutPolar(asDblArray(m_blkInsert), 3*PI/2, 5, asDblArray(m_blkInsert));//���Ӿ���
	acutPolar(asDblArray(tmpPt), PI/2, (zdm.getGuanDi() - CDMXUtils::getMinElavation())*dYScale, asDblArray(insertPt));
	insertGdBlk(insertPt);
	insertUp(m_blkInsert, dRotate);
	return true;
}

CString CDrawNSG::CurNumPosition(double dValue, bool& bIsExisted)
{
	CString strCur = _T("0");
	double dZhuanghao;
	CBcUtils bcUtils;
	map<CString, CZdmDataInfo> data = bcUtils.getAllData();

	for (map<CString, CZdmDataInfo>::iterator iter = data.begin();
		iter != data.end();
		++iter)
	{
		CZdmDataInfo pData = iter->second;
		dZhuanghao = pData.getcurData();
		//��׮�ű���ʵֵ��ʱ��˵��λ�þ�������ط�
		if (dZhuanghao > dValue)
		{
			strCur = pData.getCount();
			break;
		}
		if (abs(dZhuanghao - dValue) < GeTol)
		{
			bIsExisted = true;
			strCur = pData.getCount();
			break;
		}
	}
	return strCur;
}

bool CDrawNSG::insertGdBlk(AcGePoint3d insertPt)
{
	CBlkBaseInFo blkInfo;
	CString strPath = MyBaseUtils::GetAppPath() + _T("zdm\\nsg\\nsg.dwg");
	blkInfo.SetBlkName(_T("NSG"));
	blkInfo.SetFileName(strPath);
	blkInfo.SetInsertPt(insertPt);
	//blkInfo.SetInsertPt();
	CBlkInsert blkInsert;
	blkInsert.SetBlkInfo(blkInfo);
	blkInsert.SetDrawWP(false);
	blkInsert.SetModified(true);
	if (blkInsert.Insert()) 
	{
		AcDbObjectId blkId = blkInsert.GetObjectId();
		m_idArrs.append(blkId);
	}
	else
	{
		return false;
	}
	return true;
}

bool CDrawNSG::insertUp(AcGePoint3d insertPt, double dRotate)
{
	CBlkBaseInFo blkInfo;
	CString strPath = MyBaseUtils::GetAppPath() + _T("zdm\\nsg\\nsg-up.dwg");
	blkInfo.SetBlkName(_T("NSG-UP"));
	blkInfo.SetFileName(strPath);
	blkInfo.SetInsertPt(insertPt);
	blkInfo.SetRotate(dRotate);
	//blkInfo.SetInsertPt();
	CBlkInsert blkInsert;
	blkInsert.SetBlkInfo(blkInfo);
	blkInsert.SetModified(true);
	blkInsert.SetDrawWP(false);
	if (blkInsert.Insert())
	{
		AcDbObjectId blkId = blkInsert.GetObjectId();
		m_idArrs.append(blkId);
		return true;
	}
	else
	{
		return false;
	}
}

bool CDrawNSG::GetZuoFa()
{
	TCHAR tempBuf[133];

	int nRet = acedGetString(1, _T("\n������ͨ��ͼ����<����>��"),tempBuf);
	if (nRet == RTNORM)
	{
		CString strTmp = tempBuf;
		if (strTmp.IsEmpty())
		{
			m_strZF = _T("����");
		}
		else
		{
			m_strZF.Format(_T("����\"%s\""), strTmp);
		}
	}
	else if (nRet == RTNONE)
	{
		m_strZF = _T("����");
	}
	else
	{
		return false;
	}

	return true;
}
