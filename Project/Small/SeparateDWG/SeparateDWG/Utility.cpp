#include "StdAfx.h"
#include "Utility.h"


Utility::Utility(void)
{
}


Utility::~Utility(void)
{
}


Acad::ErrorStatus Utility::AddToModelSpace(AcDbEntity* pEnt, AcDbObjectId& retId)
{
	AcDbDatabase* pDb = acdbCurDwg();
	AcDbBlockTable* pBlockTable = NULL;
	AcDbBlockTableRecord* pBlockTableRecord = NULL;

	Acad::ErrorStatus es = pDb->getBlockTable(pBlockTable, AcDb::kForRead);
	if(es != Acad::eOk)
	{
		return es;
	}

	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();
	if(es != Acad::eOk)
	{
		return es;
	}

	es = pBlockTableRecord->appendAcDbEntity(retId, pEnt);
	pBlockTableRecord->close();
	return es;
}


AcDbObjectId Utility::CreateNewLayer(CString strLayer, int nColor)
{
	Acad::ErrorStatus es;
	AcDbLayerTable* pTable = NULL;
	AcDbLayerTableRecord* pRecord = NULL;

	es = acdbCurDwg()->getLayerTable(pTable, AcDb::kForWrite);
	if (es != Acad::eOk)
	{
		return AcDbObjectId::kNull;
	}

	if(pTable->has(strLayer)) //这个层已经有了	
	{
		es = pTable->getAt(strLayer, pRecord, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			pTable->close();
			return AcDbObjectId::kNull;
		}

		AcCmColor layerColor;
		layerColor.setColorIndex(nColor);
		pRecord->setColor(layerColor);

		pRecord->close();
	}
	else					//这个层没有
	{
		pRecord = new AcDbLayerTableRecord;		
		pRecord->setName(strLayer);

		AcCmColor layerColor;
		layerColor.setColorIndex(nColor);
		pRecord->setColor(layerColor);

		pRecord->setIsFrozen(Adesk::kFalse);	//解冻
		pRecord->setIsLocked(Adesk::kFalse);	//解锁
		pRecord->setIsOff(Adesk::kFalse);		//on
		
		pTable->add(pRecord);
		pRecord->close();
	}
	pTable->close();

	acdbCurDwg()->getLayerTable(pTable, AcDb::kForRead);
	AcDbObjectId layerId;
	pTable->getAt(strLayer, layerId);
	pTable->close();
	return layerId;	
}

AcDbObjectId Utility::CreateText(CString strText, double dScale, AcGePoint3d ptInsert)
{
	AcDbObjectId LayerId = CreateNewLayer(_T("MY_TEXT"), 1);
	AcDbText* pText = new AcDbText;
	pText->setTextString(strText);
	pText->setHeight(dScale);
	pText->setColorIndex(1);
	pText->setPosition(ptInsert);
	pText->setThickness(dScale);
	pText->setLayer(LayerId);
	AcDbObjectId objId;
	Acad::ErrorStatus es = AddToModelSpace(pText, objId);
	if (es == Acad::eOk)
	{
		pText->close();
		return objId;
	}
	else
	{
		delete pText;
		pText = NULL;
		return AcDbObjectId::kNull;
	}
}

AcDbObjectId Utility::CreateText(CString strText, double dScale, AcGePoint2d ptInsert)
{
	AcDbObjectId LayerId = CreateNewLayer(_T("MY_TEXT"), 1);
	AcDbText* pText = new AcDbText;
	pText->setTextString(strText);
	pText->setHeight(dScale);
	pText->setColorIndex(1);
	pText->setPosition(AcGePoint3d(ptInsert.x, ptInsert.y, 0.));
	pText->setThickness(dScale);
	pText->setLayer(LayerId);
	AcDbObjectId objId;
	Acad::ErrorStatus es = AddToModelSpace(pText, objId);
	if (es == Acad::eOk)
	{
		pText->close();
		return objId;
	}
	else
	{
		delete pText;
		pText = NULL;
		return AcDbObjectId::kNull;
	}
}

AcDbObjectId Utility::CreateLine(AcGePoint3d ptStart, AcGePoint3d ptEnd)
{
	AcDbObjectId LayerId = CreateNewLayer(_T("MY_LINE"), 1);
	AcDbLine* pLine = new AcDbLine;
	pLine->setStartPoint(ptStart);
	pLine->setEndPoint(ptEnd);
	pLine->setLayer(LayerId);
	AcDbObjectId objId;
	Acad::ErrorStatus es = AddToModelSpace(pLine, objId);
	if (es == Acad::eOk)
	{
		pLine->close();
		return objId;
	}
	else
	{
		delete pLine;
		pLine = NULL;
		return AcDbObjectId::kNull;
	}
}
//
//AcDbObjectId Utility::CreateLine(AcGePoint2d ptStart, AcGePoint2d ptEnd)
//{
//	AcDbObjectId LayerId = CreateNewLayer(_T("MY_LINE"), 1);
//	AcDbLine* pLine = new AcDbLine;
//	pLine->setStartPoint(AcGePoint3d(ptStart.x, ptStart.y, 0.));
//	pLine->setEndPoint(AcGePoint3d(ptEnd.x, ptEnd.y, 0.));
//	pLine->setLayer(LayerId);
//	AcDbObjectId objId;
//	Acad::ErrorStatus es = AddToModelSpace(pLine, objId);
//	if (es == Acad::eOk)
//	{
//		pLine->close();
//		return objId;
//	}
//	else
//	{
//		delete pLine;
//		pLine = NULL;
//		return AcDbObjectId::kNull;
//	}
//}

Acad::ErrorStatus Utility::ExplodeEntity(AcDbEntity* pEnt, AcDbVoidPtrArray& entitySet)
{
	Acad::ErrorStatus es = Acad::eOk;
	
	if (!pEnt)
	{
		return Acad::eNullObjectPointer;
	}
	else if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbVoidPtrArray entSet;
		es = pEnt->explode(entSet);
		if (entSet.length() > 0)
		{
			entitySet.append(entSet);
		}
	}
	else if (pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbVoidPtrArray entSet;
		es = pEnt->explode(entSet);
		if (entSet.length() > 0)
		{
			entitySet.append(entSet);
		}
	}
	else
	{
		return Acad::eNotApplicable;
	}

	return es;
}

bool Utility::PointInRect(AcGePoint2d point, AcGePoint2d minPoint, AcGePoint2d maxPoint)
{
	return point.x >= minPoint.x && point.y >= minPoint.y
		&& point.x <= maxPoint.x && point.y <= maxPoint.y;
}
//mzg
double Utility::CalculateWidth(AcGePoint3d minPt, AcGePoint3d maxPt)//返回width
{
	return abs(maxPt.y - minPt.y);
}

double Utility::CalculateLength(AcGePoint3d minPt, AcGePoint3d maxPt)//返回length
{
	return abs(maxPt.x - minPt.x);
}

/////////////////////////////////////////////////////////////////////////////
BOOL Utility::SetVar(LPCTSTR szVName, ads_real rArg)
{
	struct resbuf res;
	res.restype = RTREAL;
	res.resval.rreal   = rArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::SetVar(LPCTSTR szVName, ads_point pArg)
{
	struct resbuf res;
	res.restype = RT3DPOINT;
	memcpy(&res.resval.rpoint, &pArg, sizeof(res.resval.rpoint));
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::SetVar(LPCTSTR szVName, int nArg)
{
	struct resbuf res;
	res.restype = RTSHORT;
	res.resval.rint  = nArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::SetVar(LPCTSTR szVName, LPCTSTR szArg)
{
	ASSERT(AfxIsValidString(szArg));
	struct resbuf res;
	res.restype = RTSTR;
	res.resval.rstring =(LPTSTR) szArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::GetVar(LPCTSTR var, ads_real *rRes)
{
	ASSERT(rRes!=NULL);
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM)
		return FALSE;
	if (v.restype != RTREAL) 
		return FALSE;
	*rRes = v.resval.rreal;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::GetVar(LPCTSTR var, ads_point *pRes)
{
	ASSERT(pRes != NULL);
	struct resbuf v;
	if(ads_getvar(var, &v) !=RTNORM)
		return FALSE;
	if (v.restype != RT3DPOINT && v.restype != RTPOINT)
		return FALSE;
	ads_point_set(v.resval.rpoint, *pRes);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::GetVar(LPCTSTR var, int *nRes)
{
	ASSERT(nRes!=NULL);
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM)
		return FALSE;
	if (v.restype!=RTSHORT ) 
		return FALSE;
	*nRes = v.resval.rint;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Utility::GetVar(LPCTSTR var, LPTSTR sRes)
{
	struct resbuf v;

	if(ads_getvar(var, &v)!=RTNORM || v.restype!=RTSTR ) 
		return FALSE;
	_tcscpy(sRes, v.resval.rstring);
	ads_free (v.resval.rstring);	
	return TRUE;
}

void Utility::GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, LPCTSTR strTypeName, bool bIsFullPath)
{
	CFileFind ff;
	BOOL b = FALSE;
	b = ff.FindFile(strPath);

	BOOL bFlgLast = FALSE; //标志着遍历的最后一个文件
	if(b)
	{
		bFlgLast = FALSE;
	}
	else
	{
		bFlgLast = TRUE;
	}

	while(b || !bFlgLast)
	{
		if (!b)
		{
			bFlgLast = TRUE;
		}	
		if(!ff.IsDirectory()&&!ff.IsDots())
		{
			CString strFilename;
			strFilename = ff.GetFilePath();
			strFilename.MakeUpper();
			if(strFilename.Find(strTypeName) != -1)
			{
				if (bIsFullPath)
				{
					strFileNameArr.Add(strFilename);
				}
				else
				{
					CString strtemp1;
					strtemp1 = strFilename.Right(strFilename.GetLength() - strFilename.ReverseFind('\\')-1);	
					CString strTemp2;
					strTemp2=strtemp1.Left(strtemp1.GetLength() - 4);
					strFileNameArr.Add(strTemp2);// strtemp1);
				}
			}			
		}		
		if(!bFlgLast)
		{
			b = ff.FindNextFile();
		}
	}
	ff.Close();
}
bool Utility::getDocFromFilename(CString csFileName, AcApDocument* &pNewDocument)
{
	AcApDocumentIterator* iter = acDocManager->newAcApDocumentIterator();
	AcApDocument* pThisDocument = NULL;
	CString csThisFilename;
	CString csThisFilenameShort;

	csFileName.MakeUpper(); // uppercase comparisons

	while(!iter->done()) {   // Tiptoe through the tulips
		pThisDocument = iter->document();
		csThisFilename = pThisDocument->fileName();		
		csThisFilename.MakeUpper();
		/*csThisFilenameShort = csThisFilename.Right(csThisFilename.GetLength() -
			csThisFilename.ReverseFind(_T('\\')) - 1);*/

		if(csFileName == csThisFilename 
			//||       // Full path match
			//csFileName == csThisFilenameShort || // Matches filename only
			//csFileName == csThisFilenameShort.Left( // Filename less extension
			//csThisFilenameShort.GetLength() - 4)
			) 
		{
			pNewDocument = pThisDocument;
			if( iter )
				delete iter;
			return true;
		}
		iter->step();
	}

	pNewDocument = NULL;
	if( iter )
		delete iter;
	return false;
}