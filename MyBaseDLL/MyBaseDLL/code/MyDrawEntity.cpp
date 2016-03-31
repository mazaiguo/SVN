#include "StdAfx.h"
#include "MyDrawEntity.h"
#include "MyBaseUtils.h"
#include "MySymble.h"
//#include "Global.h"
MyDrawEntity::MyDrawEntity(void)
{
}

MyDrawEntity::~MyDrawEntity(void)
{
}

AcDbObjectId MyDrawEntity::DrawLine(AcGePoint3d ptStart, AcGePoint3d ptEnd, AcDbObjectId LayerId)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbLine* pLine = new AcDbLine;
	pLine->setDatabaseDefaults();
	pLine->setStartPoint(ptStart);
	pLine->setEndPoint(ptEnd);
	if (!LayerId.isNull())
	{
		pLine->setLayer(LayerId);
		pLine->setLinetypeScale(1.0);
	}

	MyBaseUtils::addToCurrentSpaceAndClose(pLine);
	//pLine->close();
	objId = pLine->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawText(AcGePoint3d basePt, CString strText, double dHeight ,AcDbObjectId textStyleId, AcDb::TextHorzMode HorMode, AcDb::TextVertMode VerMode)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbText* pText= new AcDbText;
	pText->setDatabaseDefaults();
	pText->setTextString(strText);
	if (!textStyleId.isNull())
	{
		pText->setTextStyle(textStyleId);
		double dScale, dTextSize;
		Acad::ErrorStatus es;
		es = MySymble::GetTextStyleInfo(textStyleId, dScale, dTextSize);
		if (es!=Acad::eOk)
		{
			delete pText;
			return objId;
		}
		if (abs(dHeight-dTextSize)>0.000001)
		{
			pText->setHeight(dHeight);
		}
		else
		{
			pText->setHeight(dTextSize);
		}
		pText->setWidthFactor(dScale);
	}
	else
	{
		pText->setHeight(dHeight);
	}	
	pText->setHorizontalMode(HorMode);
	pText->setVerticalMode(VerMode);
	Acad::ErrorStatus es;
	if (((HorMode==AcDb::kTextLeft)||(HorMode==AcDb::kTextFit)||(HorMode == AcDb::kTextAlign))&&(VerMode == AcDb::kTextBase))
	{
		es = pText->setPosition(basePt);
	}
	else
	{
		es = pText->setAlignmentPoint(basePt);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pText);
	objId = pText->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawMText(AcGePoint3d basePt, CString strText, double dHeight, AcDbObjectId textStyleId, AcDbMText::AttachmentPoint attachMode)
{
	Acad::ErrorStatus es;
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbMText* pMtext = new AcDbMText;
	pMtext->setDatabaseDefaults();
	pMtext->setAttachment(attachMode);
	pMtext->setLocation(basePt);
	pMtext->setContents(strText);
	if (!textStyleId.isNull())
	{
		double dScale, dTextSize;
		Acad::ErrorStatus es;
		es = MySymble::GetTextStyleInfo(textStyleId, dScale, dTextSize);
		if (es!=Acad::eOk)
		{
			delete pMtext;
			return objId;
		}
		if (abs(dHeight-dTextSize)>0.000001)
		{
			es = pMtext->setHeight(dHeight);
		}
		else
		{
			es = pMtext->setHeight(dTextSize);
		}	
		es = pMtext->setTextStyle(textStyleId);

	}
	else
	{
		es = pMtext->setTextHeight(dHeight);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pMtext);
	objId = pMtext->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawMTextCover(AcGePoint3d basePt, CString strText, double dHeight, AcDbObjectId textStyleId, AcDbMText::AttachmentPoint attachMode)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbMText* pMtext = new AcDbMText;
	pMtext->setDatabaseDefaults();
	pMtext->setAttachment(attachMode);
	pMtext->setLocation(basePt);
	pMtext->setContents(strText);
	if (!textStyleId.isNull())
	{
		pMtext->setTextStyle(textStyleId);
	}
	pMtext->setTextHeight(dHeight);
	pMtext->setBackgroundFill(true);
	AcCmColor cmColor;
	pMtext->getBackgroundFillColor(cmColor);
	int nColor;
	nColor = cmColor.colorIndex();
	pMtext->setBackgroundFillColor(cmColor);
	pMtext->setBackgroundScaleFactor(1.0);
	MyBaseUtils::addToCurrentSpaceAndClose(pMtext);
	objId = pMtext->objectId();
	return objId;
}


AcDbObjectId MyDrawEntity::MakeGroup(AcDbObjectIdArray& objIds, bool bIsSelectable, CString strGroupName)
{
	Acad::ErrorStatus es;
	AcDbGroup *pGroup = new AcDbGroup;
	
	AcDbDictionary *pGroupDict;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);

	AcDbObjectId groupId = AcDbObjectId::kNull;
	if (strGroupName.CompareNoCase(_T("")) == 0)
	{
		es = pGroupDict->setAt(_T("*"), pGroup, groupId);
	}
	else	
	{
		es = pGroupDict->setAt(strGroupName, pGroup, groupId);
	}
	if (es!=Acad::eOk)
	{
		pGroupDict->close();
		return AcDbObjectId::kNull;
	}
	pGroupDict->close();
	pGroup->setSelectable(bIsSelectable);//�������ɿɷֱ�ѡ���

	long i, length;
	ads_name ename;
	AcDbObjectId entId;
	length = objIds.length();
	for (i = 0; i < length; i++) 
	{
		entId = objIds.at(i);
		pGroup->append(entId);
	}
	pGroup->close();
	groupId = pGroup->objectId();
	return groupId;
}

AcDbObjectId MyDrawEntity::DrawCircle(AcGePoint3d cenPt, double dRadius, AcDbObjectId LayerId)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbCircle* pCircle = new AcDbCircle;
	pCircle->setDatabaseDefaults();
	pCircle->setCenter(cenPt);
	pCircle->setRadius(dRadius);
	if (!LayerId.isNull())
	{
		pCircle->setLayer(LayerId);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pCircle);
	objId = pCircle->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawSemiCircle(AcGePoint3d cenPt, double dRadius, double dRotate, AcDbObjectId LayerId /* = AcDbObjectId::kNull */)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbArc* pArc = new AcDbArc();
	pArc->setDatabaseDefaults();
	pArc->setRadius(dRadius);
	pArc->setCenter(cenPt);
	pArc->setStartAngle(dRotate);
	pArc->setEndAngle(dRotate+PI);
	MyBaseUtils::addToCurrentSpaceAndClose(pArc);
	objId = pArc->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawArc(AcGePoint3d cenPt, double dRadius, AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId LayerId)
{
	double dStartAng,dEndAng;
	dStartAng = acutAngle(asDblArray(cenPt), asDblArray(startPt));
	dEndAng = acutAngle(asDblArray(cenPt), asDblArray(endPt));
	AcDbArc* pArc = new AcDbArc;
	pArc->setDatabaseDefaults();
	pArc->setRadius(dRadius);
	pArc->setCenter(cenPt);
	pArc->setStartAngle(dStartAng);
	pArc->setEndAngle(dEndAng);
	if (!LayerId.isNull())
	{
		pArc->setLayer(LayerId);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pArc);
	AcDbObjectId objId = pArc->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawArc(AcGePoint3d cenPt, double dRadius, 
								   double dStartAng, double dEndAng, int nColor, AcDbObjectId LayerId)
{
	AcDbArc* pArc = new AcDbArc;
	pArc->setDatabaseDefaults();
	pArc->setRadius(dRadius);
	pArc->setCenter(cenPt);
	pArc->setStartAngle(dStartAng);
	pArc->setEndAngle(dEndAng);
	pArc->setColorIndex(nColor);
	if (!LayerId.isNull())
	{
		pArc->setLayer(LayerId);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pArc);
	AcDbObjectId objId = pArc->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::InsertBlkRef(CString strFilePathName, AcGePoint3d ptInsert, double inputscale,double inputrotation, AcDbObjectId LayerId)
{	
	AcDbObjectId objId = AcDbObjectId::kNull;
	CString strBlockname;

	CString strtemp1;
	strtemp1 = strFilePathName.Right(strFilePathName.GetLength() - strFilePathName.ReverseFind('\\')-1);	
	strBlockname=strtemp1.Left(strtemp1.GetLength() - 4);
	//Ѱ���Ƿ��Ѿ�����
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase(); 
	AcDbBlockTable *pBlkTable;
	AcDbObjectId blockId ; 
	pCurDb->getBlockTable(pBlkTable, AcDb::kForRead); 

	if(!pBlkTable->has(strBlockname)) 
	{ 
		pBlkTable->close();
		AcDbDatabase *pDwg =new AcDbDatabase (Adesk::kFalse) ; 
		Acad::ErrorStatus es = pDwg->readDwgFile (strFilePathName) ;

		AcDbDatabase  *pTempDb;	
		es  =  pDwg->wblock(pTempDb);	
		if(es  !=  Acad::eOk)	
		{		
			acutPrintf(  _T("wblock  failed!")  );
			return objId;	
		}	

		es = pCurDb->insert (blockId, strBlockname, pDwg);//, Adesk::kFalse) ; //
		delete pDwg ; 
		if ( es != Acad::eOk ) 
		{
			acutPrintf (_T("\n��������.")) ;
			return objId;
		} 
		delete pTempDb;
	} 

	else
		pBlkTable->getAt(strBlockname, blockId); 

	pBlkTable->close();

	//---- ���ò���㣬��ת�Ƕȣ������ȵ� 
	AcDbBlockReference *pBlkRef =new AcDbBlockReference(ptInsert,blockId) ; 
	pBlkRef->setScaleFactors(AcGeScale3d(inputscale,inputscale,inputscale));
	pBlkRef->setRotation (inputrotation) ; 
	if (!LayerId.isNull())
	{
		pBlkRef->setLayer(LayerId); //����ͼ��
	}

	MyBaseUtils::addToCurrentSpace(pBlkRef);

	pBlkRef->close();
	objId = pBlkRef->objectId();

	return objId;
}

AcDbObjectId MyDrawEntity::GetBlkRef(CString strFilePathName)
{
	CString strBlockname;

	CString strtemp1;
	strtemp1 = strFilePathName.Right(strFilePathName.GetLength() - strFilePathName.ReverseFind('\\')-1);	
	strBlockname=strtemp1.Left(strtemp1.GetLength() - 4);
	//Ѱ���Ƿ��Ѿ�����
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase(); 
	AcDbBlockTable *pBlkTable;
	AcDbObjectId blockId= AcDbObjectId::kNull ; 
	pCurDb->getBlockTable(pBlkTable, AcDb::kForRead); 

	if(!pBlkTable->has(strBlockname)) 
	{ 
		pBlkTable->close();
		AcDbDatabase *pDwg =new AcDbDatabase (Adesk::kFalse) ; 
		Acad::ErrorStatus es = pDwg->readDwgFile (strFilePathName) ;

		AcDbDatabase  *pTempDb;	
		es  =  pDwg->wblock(pTempDb);	
		if(es  !=  Acad::eOk)	
		{		
			acutPrintf(  _T("wblock  failed!")  );
			return blockId;	
		}	

		es = pCurDb->insert (blockId, strBlockname, pDwg);//, Adesk::kFalse) ; //
		delete pDwg ; 
		if ( es != Acad::eOk ) 
		{
			acutPrintf (_T("\n��������.")) ;
			return blockId;
		} 
		delete pTempDb;
	} 
	else
		pBlkTable->getAt(strBlockname, blockId); 
	
	pBlkTable->close();

	return blockId;
}

AcDbObjectId MyDrawEntity::DrawPlineByTwoPoint(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId LayerId)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbPolyline* pLine = new AcDbPolyline;
	pLine->setDatabaseDefaults();
	pLine->addVertexAt(0, startPt.convert2d(AcGePlane::kXYPlane));
	pLine->addVertexAt(1, endPt.convert2d(AcGePlane::kXYPlane));
	if (!LayerId.isNull())
	{
		pLine->setLayer(LayerId);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pLine);
	objId = pLine->objectId();
	return objId;
}

AcDbObjectId MyDrawEntity::DrawPlineByPoints(AcGePoint3dArray points, AcDbObjectId LayerId /* = AcDbObjectId::kNull */)
{
	AcDbObjectId objId = AcDbObjectId::kNull;
	if (points.length()<2L)
	{
		return AcDbObjectId::kNull;
	}
	AcDbPolyline* pLine = new AcDbPolyline;
	pLine->setDatabaseDefaults();
	for (int i=0; i<points.length(); i++)
	{
		pLine->addVertexAt(i, points.at(i).convert2d(AcGePlane::kXYPlane));
	}
	if (!LayerId.isNull())
	{
		pLine->setLayer(LayerId);
	}
	MyBaseUtils::addToCurrentSpaceAndClose(pLine);
	objId = pLine->objectId();
	return objId;
}



