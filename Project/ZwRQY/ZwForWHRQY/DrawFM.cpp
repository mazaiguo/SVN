#include "StdAfx.h"
#include "DrawFM.h"
#include "dbwipe.h"
#include "BlkInfo.h"
#include "BcUtils.h"
#include "GWDesingUtils.h"

CDrawFM::CDrawFM(void)
{
	m_layerId = MySymble::CreateNewLayer(_T("FM-RQ"), 1);
}

CDrawFM::~CDrawFM(void)
{

}


bool CDrawFM::doIt()
{
	if (!GetZhuanghao())
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


	drawLineAndText();

	int nBlkRefCount = 0;
	MyBaseUtils::GetVar(_T("USERI2"), &nBlkRefCount);
	CString strName;
	strName.Format(_T("%s%d"), FM_DICT, nBlkRefCount);
	MyDrawEntity::MakeGroup(m_idArrs, false, strName);
	nBlkRefCount++;
	MyBaseUtils::SetVar(_T("USERI2"), nBlkRefCount);
	return true;
}

bool CDrawFM::del()
{
	//AcDbObjectId objId = AcDbObjectId::kNull;

	//ads_name ename;
	//AcGePoint3d pt;
	//int nRet = acedEntSel(_T("\n请选择要删除的阀门："), ename, asDblArray(pt));
	//if (nRet != RTNORM)
	//{
	//	return false;
	//}
	//acdbGetObjectId(objId, ename);
	//CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
	//if (strGroupName.Find(FM_DICT) != -1)
	//{
	//	MyEditEntity::EraseEntByGroupName(strGroupName);
	//}
	//else
	//{
	//	AfxMessageBox(_T("请选择阀门"));
	//}

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
		int nFind = strGroupName.Find(FM_DICT);
		if (nFind >= 0)
		{
			MyEditEntity::EraseEntByGroupName(strGroupName);
		}
	}
	return true;
}

bool CDrawFM::GetZhuanghao()
{
	CString strPrompt;
	//strPrompt.Format(_T("\n指定插入桩号值<m> <%.2f>:"), m_dZhuanghao);
	
	int nRet = acedGetReal(_T("\n指定插入桩号值<m>:"), &m_dZhuanghao);
	if (nRet != RTNORM)
	{
		return false;
	}
	return true;
}

bool CDrawFM::GetDescription()
{
	TCHAR tempBuf[133];

	int nRet = acedGetString(1, _T("\n请输入编号:"),tempBuf);
	if (nRet != RTNORM)
	{
		return false;
	}
	m_strNo = tempBuf;
	
	return true;
}

//创建WipeOut对象
Acad::ErrorStatus CDrawFM::CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds)
{
	Acad::ErrorStatus es;
#ifdef ARX
	if (RTNORM != acedArxLoad(_T("AcWipeoutObj18.dbx"))) return Acad::eNotImplementedYet;
	es = AcDbWipeout::createImageDefinition();
	if (es != Acad::eOk)
	{
		return es;
	}
#else
	if (RTNORM != acedArxLoad(_T("ZrxDraw.zrx")))
	{
		return Acad::eNotImplementedYet;
	}
#endif	
	if (!point3ds.at(0).isEqualTo(point3ds.last()))
	{
		point3ds.append(point3ds.at(0));
	}
	AcDbWipeout *pWipeout = new AcDbWipeout;
	if(pWipeout == NULL) return Acad::eNotImplementedYet;
	pWipeout->setDatabaseDefaults();
	AcGePoint3d originPnt(AcGePoint3d::kOrigin);
	AcGeVector3d Udirection(1,0,0);
	AcGeVector3d Vdirection(0,-1,0);
	pWipeout->setOrientation(originPnt,Udirection, Vdirection); 

	pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent,Adesk::kTrue);
	pWipeout->setDisplayOpt(AcDbRasterImage::kShow, false);
	es = pWipeout->setClipBoundaryToWholeImage();
	AcGeMatrix3d PixelToModel,ModelToPixel;
	pWipeout->getPixelToModelTransform(PixelToModel);
	ModelToPixel=PixelToModel.invert();
	for(int i=0;i< point3ds.length();i++)
	{
		point3ds.at(i).transformBy(ModelToPixel);
	}   
	AcGePoint2dArray point2ds = MyTransFunc::My3d22d(point3ds);
	es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly,point2ds);
	AcGePoint2d pt2d;
	double scale;
	AcDbObjectId id;
	pWipeout->append(id);
	wipeoutId = pWipeout->objectId();
	pWipeout->close();
	return Acad::eOk;
}

bool CDrawFM::insertGdBlk(AcGePoint3d insertPt)
{
	CBlkBaseInFo blkInfo;
	CString strPath = MyBaseUtils::GetAppPath() + _T("zdm\\fm\\RFM1.dwg");
	blkInfo.SetBlkName(_T("RFM1"));
	blkInfo.SetFileName(strPath);
	blkInfo.SetInsertPt(insertPt);
	//blkInfo.SetInsertPt();
	CBlkInsert blkInsert;
	blkInsert.SetBlkInfo(blkInfo);
	blkInsert.SetModified(true);
	if (blkInsert.Insert()) 
	{
		AcDbObjectId blkId = blkInsert.GetObjectId();
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		m_idArrs.append(blkId);
	}
	else
	{
		return false;
	}
	return true;
}
CString CDrawFM::CurNumPosition( double dValue, bool& bIsExisted)
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
		//当桩号比真实值大时，说明位置就在这个地方
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

bool CDrawFM::insert()
{
	bool bIsExisted = false;
	CString strCur = CurNumPosition(m_dZhuanghao, bIsExisted);
	CString strLabel = BC_DICT + strCur;
	double dXScale = 1000/(CDMXUtils::getXScale());
	double dYScale = 1000/(CDMXUtils::getYScale());
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	AcGePoint3d tmpPt,insertPt;
	acutPolar(asDblArray(basePt), 0, 20 + m_dZhuanghao*dXScale, asDblArray(tmpPt));
	acutPolar(asDblArray(tmpPt), 3*PI/2, 91, asDblArray(insertPt));
	CBcUtils bc;
	CZdmDataInfo zdm;
	bc.get(strLabel, zdm);
	double dRotate = 0.0;
	double dRadius = (dYScale*zdm.getPipeDiameter())/2000;
	if (!bIsExisted)//如果没有找到，说明在strCur与strCur+1之间
	{
		CBcUtils nextBc;
		CZdmDataInfo preZdm;
		int nCount = MyTransFunc::StringToInt(strCur);
		nCount--;
		strCur.Format(_T("%d"), nCount);
		CString strNext = BC_DICT + strCur;
		bc.get(strNext, preZdm);
		double dDist1,dDist2,dDist3,dDist4;
		AcGePoint3d startPt,endPt;
		acutPolar(asDblArray(basePt), 0, 20 + (preZdm.getcurData() - CDMXUtils::startzh())*dXScale, asDblArray(startPt));
		acutPolar(asDblArray(startPt), PI/2, (preZdm.getGuanDi() - CDMXUtils::getMinElavation())*dYScale, asDblArray(startPt));
		acutPolar(asDblArray(basePt), 0, 20 + (zdm.getcurData() - CDMXUtils::startzh())*dXScale, asDblArray(endPt));
		acutPolar(asDblArray(endPt), PI/2, (zdm.getGuanDi() - CDMXUtils::getMinElavation())*dYScale, asDblArray(endPt));
		
		AcGePoint3d rayPt = tmpPt;
		rayPt.y += 1000*dYScale;
		AcGeLine3d line1(startPt, endPt);
		AcGeLine3d line2(rayPt, tmpPt);
		line1.intersectWith(line2, m_blkInsert);
	
		AcGeVector3d vec;
		vec.set((zdm.getcurData() - preZdm.getcurData())*dXScale, (zdm.getGuanDi() - preZdm.getGuanDi())*dYScale, 0);
		dRotate = vec.angleOnPlane(AcGePlane::kXYPlane);
	}
	else
	{
		acutPolar(asDblArray(tmpPt), PI/2, (zdm.getGuanDi() - CDMXUtils::getMinElavation())*dYScale, asDblArray(m_blkInsert));
	}
	acutPolar(asDblArray(m_blkInsert), PI/2, dRadius, asDblArray(m_blkInsert));
	insertGdBlk(insertPt);
	insertUp(m_blkInsert, dRotate);

	return true;
}

void CDrawFM::insertUp(AcGePoint3d insertPt, double dRotate)
{
	CBlkBaseInFo blkInfo;
	CString strPath = MyBaseUtils::GetAppPath() + _T("zdm\\fm\\RFM1-UP.dwg");
	blkInfo.SetBlkName(_T("RFM1-UP"));
	blkInfo.SetFileName(strPath);
	blkInfo.SetInsertPt(insertPt);
	blkInfo.SetRotate(dRotate);
	//blkInfo.SetInsertPt();
	CBlkInsert blkInsert;
	blkInsert.SetBlkInfo(blkInfo);
	blkInsert.SetModified(true);
	if (blkInsert.Insert())
	{
		AcDbObjectId blkId = blkInsert.GetObjectId();
		blkId = MyEditEntity::openEntChangeLayer(blkId, m_layerId);
		m_idArrs.append(blkId);

		//生成两条直线
		AcDbExtents exts = MyEditEntity::OpenObjAndGetExts(blkId);
		AcGePoint3d leftPt,rightPt, NodePt, midPt,startPt, endPt;
		leftPt.x = exts.minPoint().x;
		leftPt.y = exts.maxPoint().y;
		leftPt.z = 0;

		rightPt.x = exts.maxPoint().x;
		rightPt.y = exts.maxPoint().y;
		rightPt.z = 0;
		acutPolar(asDblArray(insertPt), PI/2, 10, asDblArray(NodePt));
		acutPolar(asDblArray(insertPt), PI/2, 8, asDblArray(midPt));
		acutPolar(asDblArray(midPt), 0, 0.7, asDblArray(endPt));
		acutPolar(asDblArray(midPt), PI, 0.7, asDblArray(startPt));
		AcDbObjectId line1Id,line2Id,line3Id;
		line1Id = MyDrawEntity::DrawLine(leftPt, NodePt);
		line2Id = MyDrawEntity::DrawLine(rightPt, NodePt);
		line3Id = MyDrawEntity::DrawLine(startPt, endPt);
		line1Id = MyEditEntity::openEntChangeColor(line1Id, 3);
		line2Id = MyEditEntity::openEntChangeColor(line2Id, 3);
		line3Id = MyEditEntity::openEntChangeColor(line3Id, 3);
		m_idArrs.append(line1Id);
		m_idArrs.append(line2Id);
		m_idArrs.append(line3Id);
	}
	else
	{
		return;
	}
	return;
}

void CDrawFM::drawLineAndText()
{
	AcGePoint3d midPt,endPt;
	int nRet = acedGetPoint(asDblArray(m_blkInsert), _T("\n请指定终点"), asDblArray(midPt));
	if (nRet != RTNORM)
	{
		return;
	}
	CString strText = _T("阀") + m_strNo;
	AcDbObjectId textStyleId = CGWDesingUtils::getGlobalTextStyle();

	AcGePoint3d textPt1, textPt2;
	acutPolar(asDblArray(midPt), PI/2, 1, asDblArray(textPt1));
	acutPolar(asDblArray(midPt), 3*PI/2, 1, asDblArray(textPt2));
	AcDbObjectId textId1 = MyDrawEntity::DrawText(textPt1, strText, 3.0, textStyleId, AcDb::kTextLeft);
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
}

bool CDrawFM::GetZuoFa()
{
	TCHAR tempBuf[133];

	int nRet = acedGetString(1, _T("\n请输入通用图做法详尽<做法详尽>："),tempBuf);
	if (nRet == RTNORM)
	{
		CString strTmp = tempBuf;
		if (strTmp.IsEmpty())
		{
			m_strZF = _T("做法详尽");
		}
		else
		{
			m_strZF.Format(_T("做法详尽\"%s\""), strTmp);
		}
	}
	else if (nRet == RTNONE)
	{
		m_strZF = _T("做法详尽");
	}
	else
	{
		return false;
	}

	return true;
}
