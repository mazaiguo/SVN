#include "StdAfx.h"
#include "BlkInfo.h"
#include "ArxDbgUtils.h"
#include "dbwipe.h"
#include "MyTransFunc.h"
#include "MyDrawEntity.h"
#include "MyEditEntity.h"
#include "BlkJig.h"
#include "ObjectToNotify.h"
#include "Global.h"
#include "MyDrag.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CBlkInfo::CBlkInfo(void)
{	
	m_insertPt.set(0, 0, 0);
	m_scale.set(1.0, 1.0, 1.0);
	m_dRotate = 0.0;
}

CBlkInfo::CBlkInfo(const CBlkInfo& scl)
{
	*this = scl;
}


CBlkInfo::~CBlkInfo(void)
{
}

//基本数据
CString CBlkInfo::GetBlkType() const
{
	return m_strBlkType;
}

AcGePoint3d CBlkInfo::GetInsertPt() const
{
	return m_insertPt;
}

AcGeScale3d CBlkInfo::GetScale() const
{
	return m_scale;
}

double CBlkInfo::GetRotate() const
{
	return m_dRotate;
}

LPCTSTR CBlkInfo::GetBlkName() const
{
	return m_strBlkName;
}

LPCTSTR CBlkInfo::GetFilePath() const
{
	return m_strFileName;
}
//基本数据
void CBlkInfo::SetBlkType(LPCTSTR str)
{
	m_strBlkType = str;
}
void CBlkInfo::SetInsertPt(AcGePoint3d pt)
{
	m_insertPt = pt;
}

void CBlkInfo::SetScale(AcGeScale3d sc)
{
	m_scale = sc;
}

void CBlkInfo::SetRotate(double dRotate)
{
	m_dRotate = dRotate;
}

void CBlkInfo::SetBlkName(LPCTSTR strBlkName)
{
	m_strBlkName = strBlkName;
}

void CBlkInfo::SetFilePath(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

CBlkInfo&   CBlkInfo::operator = (const CBlkInfo& scl)
{
	m_strBlkType = scl.GetBlkType();
	//块的基本信息
	m_insertPt = scl.GetInsertPt();//插入点
	m_scale = scl.GetScale();//比例
	m_dRotate = scl.GetRotate();//旋转
	m_strBlkName = scl.GetBlkName();
	m_strFileName = scl.GetFilePath();
	return *this;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////插入块及在块上做区域覆盖/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
CBlkInsert::CBlkInsert(void)
{
	m_bDrawWP = true;
	m_blockId = AcDbObjectId::kNull;
	m_bIsModified = false;

	//////////////////////////////////////////////////////////////////////////
	m_strDefine = _T("");
	m_strStandard = _T("");
	m_nPosition = 2;
	m_textId2 = AcDbObjectId::kNull;
	m_textId3 = AcDbObjectId::kNull;
	m_textId4 = AcDbObjectId::kNull;
	m_textId5 = AcDbObjectId::kNull;
	m_blkId = AcDbObjectId::kNull;
	m_lineId = AcDbObjectId::kNull;
	
}

CBlkInsert::CBlkInsert(CBlkInfo blkInfo, CDataBaseInfo dataInfo)
{
	m_bDrawWP = true;
	m_Info = blkInfo;
	m_DataInfo = dataInfo;
	m_bIsModified = false;
	//////////////////////////////////////////////////////////////////////////
	m_strDefine = _T("");
	m_strStandard = _T("");
	m_nPosition = 2;
	m_textId2 = AcDbObjectId::kNull;
	m_textId3 = AcDbObjectId::kNull;
	m_textId4 = AcDbObjectId::kNull;
	m_textId5 = AcDbObjectId::kNull;
	m_blkId = AcDbObjectId::kNull;
	m_lineId = AcDbObjectId::kNull;
}

CBlkInsert::~CBlkInsert(void)
{
	m_bDrawWP = true;
}

void CBlkInsert::SetBlkInfo(CBlkInfo blkInfo)
{
	m_Info = blkInfo;
}

void CBlkInsert::SetDataInfo(CDataBaseInfo dataInfo)
{
	m_DataInfo = dataInfo;
}


bool CBlkInsert::Insert()
{
	bool bRet = InsertBlk();
	if (!bRet)
	{
		return false;
	}
	if (m_bDrawWP)
	{
		bRet = DoEntData();
		if (!bRet)
		{
			return false;
		}
	}
	AddDataToBlk();
	//setImageFrameOff();
	return true;
}

void CBlkInsert::SetDrawWP(bool bDrawWp)
{
	m_bDrawWP = bDrawWp;
}

void CBlkInsert::SetModified(bool bModified)
{
	m_bIsModified = bModified;
}

void CBlkInsert::AddDataToBlk()
{
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_TYPE, m_Info.GetBlkType());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_DAIHAO, m_DataInfo.GetFItemId());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_MINGCHENG, m_DataInfo.GetFName());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_SHULIANG, m_DataInfo.GetShuliang());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_CAILIAO, m_DataInfo.GetCailiao());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_DANZHONG, m_DataInfo.GetDanzhong());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_ZONGZHONG, m_DataInfo.GetZongzhong());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_BEIZHU, m_DataInfo.GetBeizhu());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_DANWEI, m_DataInfo.GetDanwei());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_LINGJIANLEIXING, m_DataInfo.GetFErpCls());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_RECHULI, m_DataInfo.GetReChuli());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_BIAOMIANCHULI, m_DataInfo.GetBiaoMianChuli());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_FNUMBER, m_DataInfo.GetFNumber());
	MyEditEntity::OpenObjAppendStrToXdata(m_blockId, ZRXDBG_LIUCHENGTU_NUM, /*m_DataInfo.GetChartNum()*/m_strText);
}

bool CBlkInsert::DoEntData()
{
	AcGePoint3dArray ptArr;
	ptArr.removeAll();
	ptArr = MyTransFunc::OperateTwoPointsAndGetPoints(m_Exts.minPoint(), m_Exts.maxPoint());
	//AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbObjectId wipeoutId = AcDbObjectId::kNull;
	if (CreateWipeout(wipeoutId, ptArr)!= Acad::eOk)
	{
		return false;
	}

	MyTransFunc::DuiXiangPaiXu(wipeoutId, true);	
	MyTransFunc::DuiXiangPaiXu(m_blockId, true);

	MyTransFunc::DuiXiangPaiXu(m_textId2, true);
	MyTransFunc::DuiXiangPaiXu(m_textId3, true);


	assocWipeOut(m_blockId, wipeoutId);

	/*AcDbObjectIdArray idArr;
	idArr.removeAll();
	idArr.append(m_blockId);
	idArr.append(objId);
	MyDrawEntity::MakeGroup(idArr, true, m_Info.GetBlkName());*/

	return true;
}

//主要操作数据的函数
bool CBlkInsert::InsertBlk()
{
	AcDbObjectId LayerId;
	//寻找是否已经定义
	//AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase(); 
	//AcDbBlockTable *pBlkTable;
	//AcDbObjectId blockId ; 
	//pCurDb->getBlockTable(pBlkTable, AcDb::kForRead); 

	//if(!pBlkTable->has(m_Info.GetBlkName())) 
	//{ 
	//	pBlkTable->close();
	//	AcDbDatabase *pDwg =new AcDbDatabase (Adesk::kFalse) ; 
	//	Acad::ErrorStatus es = pDwg->readDwgFile (m_Info.GetFilePath()) ;

	//	es = pCurDb->insert (blockId, m_Info.GetBlkName(), pDwg);//, Adesk::kFalse) ; //
	//	delete pDwg ; 
	//	if ( es != Acad::eOk ) 
	//	{
	//		acutPrintf (_T("\n插入块错误.")) ;
	//		pBlkTable->close();
	//		return false;
	//	} 
	//} 

	//else
	//	pBlkTable->getAt(m_Info.GetBlkName(), blockId); 

	//pBlkTable->close();

	////---- 设置插入点，旋转角度，比例等等 
	//AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_Info.GetInsertPt(), blockId) ; 
	//pBlkRef->setScaleFactors(m_Info.GetScale());
	//pBlkRef->getGeomExtents(m_Exts);
	//ArxDbgUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
	//m_blockId = pBlkRef->objectId();
	
	AcDbObjectId objId = MyDrawEntity::GetBlkRef(m_Info.GetFilePath());
	if (!m_bIsModified)
	{
		/*CBlkJig jig;
		jig.SetBlkRefId(objId);
		AcEdJig::DragStatus es = jig.startJig();
		if (es != AcEdJig::kNormal)
		{
			return false;
		}
		m_blockId = jig.GetBlkRefId();	
		m_Exts = MyEditEntity::OpenObjAndGetExts(m_blockId);*/
		//jig效果不好，换用dragen
		AcGePoint3d basePt;
		basePt.set(0, 0,0 );
		ads_name ssname,ename;
		acedSSAdd(NULL, NULL, ssname);
		AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_Info.GetInsertPt(), objId) ; 
		pBlkRef->setScaleFactors(m_Info.GetScale());
		pBlkRef->getGeomExtents(m_Exts);
		pBlkRef->setPosition(basePt);
		ArxDbgUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		m_blockId = pBlkRef->objectId();
		acdbGetAdsName(ename, m_blockId);
		acedSSAdd(ename, ssname, ssname);

		if (!DrawText(basePt, ssname))
		{
			acedSSFree(ssname);
			return false;
		}
		int nRet = MyDrag::Draggen(_T("\n指定插入点:"), ssname, asDblArray(basePt), asDblArray(basePt), _T(""), -1);
		if (nRet == -1)
		{
			acedSSFree(ssname);
			return false;
		}
		AcGeMatrix3d mat;
		mat.setToIdentity();
		AcGeVector3d vec;
		vec = basePt.asVector();
		mat.setToTranslation(vec);
		m_Exts.transformBy(mat);
		acedSSFree(ssname);
		m_insertPt = basePt;
	}
	else
	{
		AcDbBlockReference *pBlkRef =new AcDbBlockReference(m_Info.GetInsertPt(), objId) ; 
		pBlkRef->setScaleFactors(m_Info.GetScale());
		pBlkRef->getGeomExtents(m_Exts);
		m_insertPt = pBlkRef->position();
		ArxDbgUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
		m_blockId = pBlkRef->objectId();
		ads_name ssname;
		DrawText(m_insertPt, ssname);
	}

	return true;
}


bool CBlkInsert::DrawText(AcGePoint3d basePt, ads_name& ssname)
{
	bool bRet = true;
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//处理文字
	AcGePoint3d minPt,maxPt,cenPt;
	minPt = m_Exts.minPoint();
	maxPt = m_Exts.maxPoint();
	double dLength = abs(maxPt.x - minPt.x);
	double dHeight = abs(maxPt.y - minPt.y);
	cenPt.x = (maxPt.x + minPt.x)/2;
	cenPt.y = (maxPt.y + minPt.y)/2;
	cenPt.z = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//处理方向
	//绘制管道位号	
	AcDbObjectId layerId = ArxDbgUtils::CreateNewLayer(_T("TEXT"), 6);	
	AcGePoint3d textPt;
	if (m_nPosition == 0)//上面
	{
		textPt.x = cenPt.x;
		textPt.y = cenPt.y + dHeight;
		textPt.z = 0;
	}
	else if (m_nPosition == 1)//左面
	{
		textPt.x = cenPt.x - dLength;
		textPt.y = cenPt.y;
		textPt.z = 0;
	}
	else if (m_nPosition == 2)//下面
	{
		textPt.x = cenPt.x;
		textPt.y = cenPt.y - dHeight;
		textPt.z = 0;
	}
	else if (m_nPosition == 3)//右面
	{
		textPt.x = cenPt.x + dLength;
		textPt.y = cenPt.y;
		textPt.z = 0;
	}
	//绘制设备的规格
	if ((!m_strStandard.IsEmpty()))
	{
		if (!m_bZongxiang)
		{
			m_textId4 = MyDrawEntity::DrawText(textPt, m_strStandard, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
			m_textId4 = MyEditEntity::openEntChangeLayer(m_textId4, layerId);			
		}
		else
		{
			m_textId4 = MyDrawEntity::DrawText(textPt, m_strStandard, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
			m_textId4 = MyEditEntity::openEntChangeRotation(m_textId4, PI/2);
			m_textId4 = MyEditEntity::openEntChangeLayer(m_textId4, layerId);
		}
		if (!m_bIsModified)
		{
			ads_name ename;
			acdbGetAdsName(ename, m_textId4);
			acedSSAdd(ename, ssname, ssname);
		}
		if (!m_bZongxiang)
		{
			if ((m_nPosition == 0)||
				(m_nPosition == 1))//上面//左面
			{
				textPt.y = textPt.y + 2.5;
			}
			else if ((m_nPosition == 2)||
				(m_nPosition == 3))//下面 右面
			{
				textPt.y = textPt.y - 2.5;
			}
		}
		else
		{
			if ((m_nPosition == 0)||
				(m_nPosition == 1))//上面//左面
			{
				textPt.x = textPt.x - 2.5;
			}
			else if ((m_nPosition == 2)||
				(m_nPosition == 3))//下面 右面
			{
				textPt.x = textPt.x + 2.5;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if ((!m_strText.IsEmpty())&&(m_bIsDrawText))
	{
		if (!m_bZongxiang)
		{
			m_textId1 = MyDrawEntity::DrawText(textPt, m_strText, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
			m_textId1 = MyEditEntity::openEntChangeLayer(m_textId1, layerId);			
		}
		else
		{
			m_textId1 = MyDrawEntity::DrawText(textPt, m_strText, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
			m_textId1 = MyEditEntity::openEntChangeRotation(m_textId1, PI/2);
			m_textId1 = MyEditEntity::openEntChangeLayer(m_textId1, layerId);
		}
		if (!m_bIsModified)
		{
			ads_name ename;
			acdbGetAdsName(ename, m_textId1);
			acedSSAdd(ename, ssname, ssname);
		}
		if (!m_bZongxiang)
		{
			if ((m_nPosition == 0)||
				(m_nPosition == 1))//上面//左面
			{
				textPt.y = textPt.y + 2.5;
			}
			else if ((m_nPosition == 2)||
				(m_nPosition == 3))//下面 右面
			{
				textPt.y = textPt.y - 2.5;
			}
		}
		else
		{
			if ((m_nPosition == 0)||
				(m_nPosition == 1))//上面//左面
			{
				textPt.x = textPt.x - 2.5;
			}
			else if ((m_nPosition == 2)||
				(m_nPosition == 3))//下面 右面
			{
				textPt.x = textPt.x + 2.5;
			}
		}
		
	}
	//////////////////////////////////////////////////////////////////////////
	//绘制自定义文字
	if (!m_strDefine.IsEmpty())
	{
		if (!m_bZongxiang)
		{
			m_textId5 = MyDrawEntity::DrawText(textPt, m_strDefine, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
			m_textId5 = MyEditEntity::openEntChangeLayer(m_textId5, layerId);			
		}
		else
		{
			m_textId5 = MyDrawEntity::DrawText(textPt, m_strDefine, 2.23, AcDbObjectId::kNull, AcDb::kTextMid, AcDb::kTextBase);
			m_textId5 = MyEditEntity::openEntChangeRotation(m_textId5, PI/2);
			m_textId5 = MyEditEntity::openEntChangeLayer(m_textId5, layerId);
		}
		if (!m_bIsModified)
		{
			ads_name ename;
			acdbGetAdsName(ename, m_textId5);
			acedSSAdd(ename, ssname, ssname);
		}
	}
	//绘制名称
	if (!m_strName.IsEmpty())
	{
		m_textId2 = MyDrawEntity::DrawText(basePt, m_strName, 2.23, AcDbObjectId::kNull, AcDb::kTextCenter, AcDb::kTextBase);
		m_textId2 = MyEditEntity::openEntChangeLayer(m_textId2, layerId);
		if (!m_bIsModified)
		{
			ads_name ename;
			acdbGetAdsName(ename, m_textId2);
			acedSSAdd(ename, ssname, ssname);
		}
	}
	
	//绘制流程图序号
	if (!m_strNo.IsEmpty())
	{
		m_textId3 = MyDrawEntity::DrawText(basePt, m_strNo, 2.23, AcDbObjectId::kNull, AcDb::kTextCenter, AcDb::kTextTop);
		m_textId3 = MyEditEntity::openEntChangeLayer(m_textId3, layerId);
		if (!m_bIsModified)
		{	
			ads_name ename;
			acdbGetAdsName(ename, m_textId3);
			acedSSAdd(ename, ssname, ssname);
		}
	}
	
	//绘制直线
	if (m_Info.GetBlkType().CompareNoCase(_T("仪器")) == 0)
	{
		m_lineId = MyDrawEntity::DrawLine(m_tmpPoint, m_insertPt);
		if (!m_bIsModified)
		{	
			ads_name ename;
			acdbGetAdsName(ename, m_textId3);
			acedSSAdd(ename, ssname, ssname);
		}

		AcGeVector3d vec = m_insertPt - m_tmpPoint;
		AcGeVector2d vec2d = vec.convert2d(AcGePlane::kXYPlane);
		double dAng = vec2d.angle();
		int nCount = (int)gGlobal.GetIniValue(_T("插入仪器"), _T("阀门"), 0);
		if (nCount > 0)
		{
			CString strPath = gGlobal.GetAppPath();
			CString strName = strPath  + _T("fm.dwg");

			AcDbObjectId objId = MyDrawEntity::GetBlkRef(strName);
			for (int i=0; i<nCount; i++)
			{
				AcGePoint3d insertPt;
				acutPolar(asDblArray(m_insertPt), dAng + PI, 6 + 5*i, asDblArray(insertPt));
				AcDbBlockReference *pBlkRef =new AcDbBlockReference(insertPt, objId) ; 
				pBlkRef->setRotation(dAng);
				ArxDbgUtils::addToCurrentSpaceAndClose(pBlkRef, acdbHostApplicationServices()->workingDatabase());
				m_blkId = pBlkRef->objectId();
				if (!m_bIsModified)
				{	
					ads_name ename;
					acdbGetAdsName(ename, m_textId3);
					acedSSAdd(ename, ssname, ssname);
				}
			}
		}	
	}

	return bRet;
}

//创建WipeOut对象
Acad::ErrorStatus CBlkInsert::CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds)
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


void CBlkInsert::assocWipeOut(AcDbObjectId blkId, AcDbObjectId wipeoutId)
{
	//处理blkref
	AcDbBlockReference* pBlkRef = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, blkId, AcDb::kForWrite) != Acad::eOk)
	{
		return;
	}
	AcGePoint3d basePt = pBlkRef->position();
	double dScale = pBlkRef->scaleFactors().sx;
	double dRotate = pBlkRef->rotation();


	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//反应器操作
	AcDbDictionary *pNamedObj;
	AcDbDictionary *pNameList;    
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	pDb->getNamedObjectsDictionary(pNamedObj,
		AcDb::kForWrite);
	if (pNamedObj->getAt(_T("ZW_DICT_WIPEOUT"),
		(AcDbObject*&)pNameList, AcDb::kForWrite)
		== Acad::eKeyNotFound)
	{
		pNameList = new AcDbDictionary;
		AcDbObjectId DictId;
		pNamedObj->setAt(_T("ZW_DICT_WIPEOUT"), pNameList, DictId);
	}
	pNamedObj->close();

	// Create the AsdkObjectToNotify for lineA
	//
	CObjectToNotify *pObj = new CObjectToNotify();
	pObj->link(wipeoutId, m_textId1, m_textId2, m_textId3, m_textId4, m_textId5, m_blkId, m_lineId);

	AcDbObjectId objId;
	int nBlkRefCount = gGlobal.getBlkRefCount();
	CString strName;
	strName.Format(_T("object_to_notify_%d"), nBlkRefCount);
	if ((pNameList->getAt(strName, objId))
		== Acad::eKeyNotFound)
	{
		pNameList->setAt(strName, pObj, objId);
		pObj->close();
	} 
	else 
	{
		delete pObj;
	}
	pNameList->close();

	// Set up persistent reactor link between line a
	// and AsdkObjectToNotify
	//
	pBlkRef->addPersistentReactor(objId);
	pBlkRef->close();

	gGlobal.setBlkRefCount(nBlkRefCount+1);
}

void CBlkInsert::SetName(LPCTSTR strName)
{
	m_strName = strName;
}

void CBlkInsert::SetNo(LPCTSTR strNo)
{
	m_strNo = strNo;
}

void CBlkInsert::SetText(LPCTSTR strText)
{
	m_strText = strText;
}

void CBlkInsert::SetZongxiang(BOOL bZongxiang)
{
	m_bZongxiang = bZongxiang;
}

void CBlkInsert::SetDefine(LPCTSTR strDefine)
{
	m_strDefine = strDefine;
}

void CBlkInsert::SetStandard(LPCTSTR strStandard)
{
	m_strStandard = strStandard;
}

void CBlkInsert::SetPosition(int nPosition)
{
	m_nPosition = nPosition;
}


void CBlkInsert::SetDrawText(bool bIsDrawText)
{
	m_bIsDrawText = bIsDrawText;
}

AcDbExtents CBlkInsert::GetObjExts()
{
	return m_Exts;
}

AcDbObjectId CBlkInsert::GetObjectId()
{
	return m_blockId;
}

AcGePoint3d CBlkInsert::GetPosition()
{
	return m_insertPt;
}

void CBlkInsert::setImageFrameOff()
{
	AcDbObjectId StyleId;

	AcDbDictionary* dict = NULL;
	AcDbDictionary* rootDict;
	AcDbObject* obj;
	Acad::ErrorStatus es = acdbCurDwg()->getNamedObjectsDictionary(rootDict, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		es = rootDict->getAt(_T("ACAD_WIPEOUT_VARS"), obj, AcDb::kForWrite);
		if (es == Acad::eOk)
		{
			CString strName = obj->isA()->name();
			/*	if (obj->isKindOf(AcDbRasterVariables::desc()))
			{
			AcDbRasterVariables* pRasterVar = AcDbRasterVariables::cast(obj);
			es = pRasterVar->setImageFrame(AcDbRasterVariables::kImageFrameOff);
			}*/
			obj->close();
		}
		rootDict->close();
	}
	else
	{
		return;
	}	
}


void CBlkInsert::SetTmpPoint(AcGePoint3d pt)
{
	m_tmpPoint = pt;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CBlkBaseInFo::CBlkBaseInFo(void)
{
	m_insertPt.set(0, 0, 0);
	m_scale.set(1.0, 1.0, 1.0);
	m_dRotate = 0.0;
}

CBlkBaseInFo::~CBlkBaseInFo()
{

}

AcGePoint3d CBlkBaseInFo::GetInsertPt() const
{
	return m_insertPt;
}

AcGeScale3d CBlkBaseInFo::GetScale() const
{
	return m_scale;
}

double CBlkBaseInFo::GetRotate() const
{
	return m_dRotate;
}

CString CBlkBaseInFo::GetBlkName() const
{
	return m_strBlkName;
}

CString CBlkBaseInFo::GetFileName() const
{
	return m_strFileName;
}


void CBlkBaseInFo::SetInsertPt(AcGePoint3d insertPt)
{
	m_insertPt = insertPt;
}

void CBlkBaseInFo::SetScale(AcGeScale3d sc)
{
	m_scale = sc;
}

void CBlkBaseInFo::SetRotate(double dRotate)
{
	m_dRotate = dRotate;
}

void CBlkBaseInFo::SetBlkName(LPCTSTR strBlkName)
{
	m_strBlkName = strBlkName;
}

void CBlkBaseInFo::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

CBlkBaseInFo&   CBlkBaseInFo::operator = (const CBlkBaseInFo& scl)
{
	//块的基本信息
	m_insertPt = scl.GetInsertPt();//插入点
	m_scale = scl.GetScale();//比例
	m_dRotate = scl.GetRotate();//旋转
	m_strBlkName = scl.GetBlkName();
	m_strFileName = scl.GetFileName();
	return *this;
}

