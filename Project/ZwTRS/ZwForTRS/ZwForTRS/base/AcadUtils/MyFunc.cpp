#include "StdAfx.h"
#include "ArxDbgUtils.h"


AcDbObjectId ArxDbgUtils::GetLayer(CString strlayer)
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead)	!= Acad::eOk) 
		return AcDbObjectId::kNull;

	AcDbObjectId layerId;
	pTable->getAt(strlayer, layerId);
	pTable->close();
	return layerId;
}
AcDbObjectId ArxDbgUtils::CreateNewLayer(CString strLayer,int nColor, BOOL bIsModColor,CString strLineTypeName)
{
	AcDbLayerTable* pTable;
	if (acdbCurDwg()->getLayerTable(pTable,AcDb::kForWrite)	!= Acad::eOk) 
		return AcDbObjectId::kNull;

	AcDbLayerTableRecord* pRecord;
	if(pTable->has(strLayer))//这个层已经有了	
	{
		if ( pTable->getAt(strLayer, pRecord, AcDb::kForWrite) != Acad::eOk )
		{
			pTable->close();
			return AcDbObjectId::kNull;
		}

		///////////
		if (strLineTypeName.CompareNoCase(_T("Continuous")) != 0)
		{
			AcDbLinetypeTable   *pLinetypeTbl; 
			AcDbObjectId   ltId; 
			acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
			if(pLinetypeTbl-> getAt(strLineTypeName,ltId)!=Acad::eOk) 
			{ 
				pLinetypeTbl-> close(); 
				Acad::ErrorStatus es;
#ifdef ARX
				es = acdbCurDwg()-> loadLineTypeFile(strLineTypeName, _T("acad.lin")); 
#else
				es = acdbCurDwg()->loadLineTypeFile(strLineTypeName, _T("zwcad.lin"));
#endif
				acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
				pLinetypeTbl-> getAt(strLineTypeName,ltId); 
			} 
			pLinetypeTbl-> close(); 
			pRecord->setLinetypeObjectId(ltId);
		}
		/////////////////

		if (bIsModColor)
		{
			AcCmColor layerColor;
			layerColor.setColorIndex(nColor);
			pRecord->setColor(layerColor);
		}
		pRecord->close();
	}
	else					//这个层没有
	{
		pRecord = new AcDbLayerTableRecord;		
		pRecord->setName(strLayer);	
		AcCmColor layerColor;
		Acad::ErrorStatus es;
		es = layerColor.setColorIndex(nColor);
		pRecord->setColor(layerColor);
		pRecord->setIsFrozen(Adesk::kFalse);	//解冻
		pRecord->setIsLocked(Adesk::kFalse);	//解锁
		pRecord->setIsOff(Adesk::kFalse);		//on

		///////
		if (strLineTypeName.CompareNoCase(_T("Continuous")) != 0)
		{
			AcDbLinetypeTable   *pLinetypeTbl; 
			AcDbObjectId   ltId; 
			acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
			if(pLinetypeTbl-> getAt(strLineTypeName,ltId)!=Acad::eOk) 
			{ 
				pLinetypeTbl-> close(); 
#ifdef ARX
				es = acdbCurDwg()-> loadLineTypeFile(strLineTypeName, _T("acad.lin")); 
#else
				es = acdbCurDwg()->loadLineTypeFile(strLineTypeName, _T("zwcad.lin"));
#endif
				acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
				pLinetypeTbl-> getAt(strLineTypeName,ltId); 
			} 
			pLinetypeTbl-> close(); 
			pRecord->setLinetypeObjectId(ltId);
		}
		//////////
		pTable->add(pRecord);
		pRecord->close();
	}
	pTable->close();

	acdbCurDwg()->getLayerTable(pTable,AcDb::kForRead);
	AcDbObjectId layerId;
	pTable->getAt(strLayer, layerId);
	pTable->close();
	return layerId;	
}

AcDbObjectId ArxDbgUtils::CreateTextStyle(CString stylename,CString strFont,CString strBigfont,double dTextScale, double dTextSize, BOOL bIsModified)
{
	Acad::ErrorStatus es;
	AcDbTextStyleTable *pTextStyleTbl = new AcDbTextStyleTable;
	if (acdbCurDwg()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite) == Acad::eOk)
	{	
		AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;
		if(pTextStyleTbl->has(stylename) == Adesk::kFalse)
		{
			TCHAR sPath[MAX_PATH];
			if( acedFindFile( strFont, sPath ) != RTNORM )
			{
				if (pTextStyleTblRcd->setFont(strFont, FALSE, FALSE, 1, 1)!= Acad::eOk)
				{
					acutPrintf(_T("\n setFont出错!"));
				}
				es = pTextStyleTblRcd->setName(stylename);
				es = pTextStyleTblRcd->setXScale(dTextScale);
				es = pTextStyleTblRcd->setTextSize(dTextSize);
				es = pTextStyleTbl->add(pTextStyleTblRcd);
				es = pTextStyleTblRcd->close();
			}
			else
			{
				es = pTextStyleTblRcd->setFileName(strFont); 
				es = pTextStyleTblRcd->setBigFontFileName(strBigfont);				
				es = pTextStyleTblRcd->setName(stylename);
				es = pTextStyleTblRcd->setXScale(dTextScale);
				es = pTextStyleTblRcd->setTextSize(dTextSize);
				es = pTextStyleTbl->add(pTextStyleTblRcd);
				es = pTextStyleTblRcd->close();
			}
			es = pTextStyleTbl->close();
		}
		else
		{
			if (bIsModified)
			{
				if ( pTextStyleTbl->getAt(stylename, pTextStyleTblRcd, AcDb::kForWrite) != Acad::eOk )
				{
					pTextStyleTbl->close();
					return AcDbObjectId::kNull;
				}
				if (_tcslen(strBigfont)!=0)
				{
					pTextStyleTblRcd->setFileName(strFont);
					pTextStyleTblRcd->setBigFontFileName(strBigfont);
				}
				else
				{
					if(pTextStyleTblRcd->setFont(strFont, FALSE, FALSE, 1, 1)!= Acad::eOk)
					{
						pTextStyleTblRcd->close();
						pTextStyleTbl->close();
						return AcDbObjectId::kNull;
					}
				}
				es = pTextStyleTblRcd->setXScale(dTextScale);
				es = pTextStyleTblRcd->setTextSize(dTextSize);
				pTextStyleTblRcd->close();
			}
			pTextStyleTbl->close();
		}
	}

	acdbCurDwg()->getTextStyleTable(pTextStyleTbl,AcDb::kForRead);
	AcDbObjectId TextStyleId;
	pTextStyleTbl->getAt(stylename, TextStyleId);
	es = pTextStyleTbl->close();
	return TextStyleId;
}

Acad::ErrorStatus ArxDbgUtils::GetTextStyleInfo(AcDbObjectId ObjId, double& dTextScale, double& dTextSize)
{
	Acad::ErrorStatus es;
	if (ObjId.isNull())
	{
		return Acad::eNotApplicable;
	}

	AcDbObject* pObj = NULL;
	if (acdbOpenAcDbObject(pObj, ObjId, AcDb::kForRead)!=Acad::eOk)
	{
		return Acad::eNotApplicable;
	}
	if (pObj->isKindOf(AcDbTextStyleTableRecord::desc()))
	{
		AcDbTextStyleTableRecord* pTextStyleTblRcd = AcDbTextStyleTableRecord::cast(pObj);
		dTextScale = pTextStyleTblRcd->xScale();
		dTextSize = pTextStyleTblRcd->textSize();
		pTextStyleTblRcd->close();
		return Acad::eOk;
	}
	else
	{
		pObj->close();
		return Acad::eNotApplicable;
	}
}

AcDbObjectId ArxDbgUtils::CreateLineType(CString strLtName, BOOL bIsModified)
{
	AcDbLinetypeTable   *pLinetypeTbl; 
	AcDbObjectId   ltId; 
	Acad::ErrorStatus es;
	acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForWrite); 
	if(pLinetypeTbl-> getAt(strLtName,ltId)!=Acad::eOk) 
	{ 
		pLinetypeTbl-> close(); 
#ifdef ARX
		es = acdbCurDwg()-> loadLineTypeFile(strLtName, _T("acad.lin")); 
#else
		es = acdbCurDwg()->loadLineTypeFile(strLtName, _T("zwcad.lin"));
#endif
		acdbCurDwg()-> getLinetypeTable(pLinetypeTbl,AcDb::kForRead); 
		pLinetypeTbl-> getAt(strLtName,ltId); 
	} 
	pLinetypeTbl-> close(); 
	return ltId;
}

void ArxDbgUtils::GetAllTextStyleName(CStringArray& StrArr)
{
	CString strName;
	AcDbTextStyleTable* pTextTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()  ->getSymbolTable(pTextTbl, AcDb::kForRead); 

	AcDbTextStyleTableIterator* pTblIter = NULL;
	AcDbTextStyleTableRecord* pTextTblRec = NULL;
	pTextTbl->newIterator(pTblIter);
	for (; !pTblIter->done(); pTblIter->step())
	{
		const TCHAR* locName;
		pTblIter->getRecord(pTextTblRec, AcDb::kForRead);
		pTextTblRec->getName(locName);
		strName = locName;
		StrArr.Add(strName);
		pTextTblRec->close();
	}
	pTextTbl->close();
	delete pTblIter;
}

int ArxDbgUtils::GetPileLength(CString strPile, CString& strPrefix)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	for (int i=nCount-1; i>=0; i--)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
			break;
		++nNum;
	}
	CString strLast;
	strLast = strPile.Right(nNum);
#ifdef UNICODE
	nlen = _tstoi(strLast);
#else
	nlen = atoi(strLast);
#endif
	strPrefix = strPile.Left(nCount-nNum);
	return nlen;
}


CString ArxDbgUtils::GetNameAndStandard(CString strPile, CString& strPrefix)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	for (int i=0; i<nCount; i++)
	{
		if (strPile[i] < 255)
		{
			break;
		}
		++nNum;
	}
	CString strLast;
	strPrefix = strPile.Left(nNum);

	strLast = strPile.Right(nCount-nNum);
	strLast.Trim();
	return strLast;
}

int ArxDbgUtils::GetPileLengthEx(CString strPile, CString& strPrefix, CString& strNext)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	int nNumJ = 0;
	for (int i=nCount-1; i>=0; i--)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
		{
			if (nNum>0)
			{
				break;
			}
			++nNumJ;
			//break;
		}
		else
		{
			if (nNumJ>0)
			{
				break;
			}
			++nNum;
		}
	}
	strNext = strPile.Right(nNumJ);

	if (nNumJ>0)
	{
		strPile = strPile.Left(nCount - nNumJ);
		int ntmpCount = strPile.GetLength();
		for (int i=ntmpCount-1; i>=0; i--)
		{
			if(!isdigit(strPile[i]))//主序号是数字的情况
				break;
			++nNum;
		}
	}

	CString strLast;
	strLast = strPile.Right(nNum);
#ifdef UNICODE
	nlen = _tstoi(strLast);
#else
	nlen = atoi(strLast);
#endif
	strPrefix = strPile.Left(nCount-nNum-nNumJ);
	return nlen;
}

int ArxDbgUtils::GetPrePileLengthEx(CString strPile, CString& strPrefix, CString& strNext)
{
	int nlen = 0;
	int nCount = strPile.GetLength();
	int nNum = 0;
	int nNumJ = 0;
	for (int i=0; i<nCount; i++)
	{
		if(!isdigit(strPile[i]))//主序号是数字的情况
		{
			if (nNum>0)
			{
				break;
			}
			++nNumJ;
			//break;
		}
		else
		{
			if (nNumJ>0)
			{
				break;
			}
			++nNum;
		}
	}
	strPrefix = strPile.Left(nNumJ);

	if (nNumJ>0)
	{
		strPile = strPile.Right(nCount - nNumJ);
		int ntmpCount = strPile.GetLength();
		for (int i=0; i<ntmpCount; i++)
		{
			if(!isdigit(strPile[i]))//主序号是数字的情况
				break;
			++nNum;
		}
	}

	CString strLast;
	strLast = strPile.Left(nNum);
#ifdef UNICODE
	nlen = _tstoi(strLast);
#else
	nlen = atoi(strLast);
#endif
	strNext = strPile.Right(nCount-nNum-nNumJ);
	return nlen;
}

AcDb::LineWeight ArxDbgUtils::StrToLineType(CString& str)
{
	AcDb::LineWeight type;
	double dLweight = 0.0;
#ifdef UNICODE
	dLweight = _tcstod(str, 0);
#else
	dLweight = atof(str);
#endif

	if ((dLweight>=0)&&(dLweight<0.05))
	{
		type = AcDb::kLnWt000;
	}
	else if ((dLweight>=0.05)&&(dLweight<0.09))
	{
		type = AcDb::kLnWt005;
	}
	else if ((dLweight>=0.09)&&(dLweight<0.13))
	{
		type = AcDb::kLnWt009;
	}
	else if ((dLweight>=0.13)&&(dLweight<0.15))
	{
		type = AcDb::kLnWt013;
	}
	else if ((dLweight>=0.15)&&(dLweight<0.18))
	{
		type = AcDb::kLnWt015;
	}
	else if ((dLweight>=0.18)&&(dLweight<0.20))
	{
		type = AcDb::kLnWt018;
	}
	else if ((dLweight>=0.20)&&(dLweight<0.25))
	{
		type = AcDb::kLnWt020;
	}
	else if ((dLweight>=0.25)&&(dLweight<0.30))
	{
		type = AcDb::kLnWt025;
	}
	else if ((dLweight>=0.30)&&(dLweight<0.35))
	{
		type = AcDb::kLnWt030;
	}
	else if ((dLweight>=0.35)&&(dLweight<0.40))
	{
		type = AcDb::kLnWt035;
	}
	else if ((dLweight>=0.40)&&(dLweight<0.50))
	{
		type = AcDb::kLnWt040;
	}
	else if ((dLweight>=0.50)&&(dLweight<0.53))
	{
		type = AcDb::kLnWt050;
	}
	else if ((dLweight>=0.53)&&(dLweight<0.60))
	{
		type = AcDb::kLnWt053;
	}
	else if ((dLweight>=0.60)&&(dLweight<0.70))
	{
		type = AcDb::kLnWt060;
	}
	else if ((dLweight>=0.70)&&(dLweight<0.80))
	{
		type = AcDb::kLnWt070;
	}
	else if ((dLweight>=0.80)&&(dLweight<0.90))
	{
		type = AcDb::kLnWt080;
	}
	else if ((dLweight>=0.90)&&(dLweight<1.00))
	{
		type = AcDb::kLnWt090;
	}
	else if ((dLweight>=1.00)&&(dLweight<1.06))
	{
		type = AcDb::kLnWt100;
	}
	else if ((dLweight>=1.06)&&(dLweight<1.20))
	{
		type = AcDb::kLnWt106;
	}
	else if ((dLweight>=1.20)&&(dLweight<1.40))
	{
		type = AcDb::kLnWt120;
	}
	else if ((dLweight>=1.40)&&(dLweight<1.58))
	{
		type = AcDb::kLnWt140;
	}
	else if ((dLweight>=1.58)&&(dLweight<2.00))
	{
		type = AcDb::kLnWt158;
	}
	else if ((dLweight>=2.00)&&(dLweight<2.11))
	{
		type = AcDb::kLnWt200;
	}
	else if (dLweight>=2.11)
	{
		type = AcDb::kLnWt211;
	}
	else
	{
		type = AcDb::kLnWtByLayer;
	}
	return type;
}

void ArxDbgUtils::GetCommaFromString(CString strInput, CStringArray& strResultArr)
{
	strResultArr.RemoveAll();
	int nCount = strInput.Find(_T("、"));
	int nLength;
	if (nCount==-1)
	{
		strResultArr.Add(strInput);
		return;
	}
	CString strPrefix, strNext;
	strNext = strInput;
	CString strTmp;
	while(nCount!=-1)
	{
		strPrefix = strNext.Left(nCount);
		strResultArr.Add(strPrefix);
		nLength = strNext.GetLength();
#ifdef UNICODE
		strTmp = strNext.Right(nLength - nCount - 1);
#else
		strTmp = strNext.Right(nLength - nCount - 2);
#endif
		strNext = strTmp;
		nCount = strNext.Find(_T("、"));
	}
	strResultArr.Add(strNext);
	return;
}

void ArxDbgUtils::GetPlusFromString(CString strInput, CStringArray& strResultArr)
{
	strResultArr.RemoveAll();
	int nCount = strInput.Find(_T("+"));
	int nLength;
	if (nCount==-1)
	{
		strResultArr.Add(strInput);
		return;
	}
	CString strPrefix, strNext;
	strNext = strInput;
	CString strTmp;
	while(nCount!=-1)
	{
		strPrefix = strNext.Left(nCount);
		strResultArr.Add(strPrefix);
		nLength = strNext.GetLength();
		strTmp = strNext.Right(nLength - nCount - 1);
		strNext = strTmp;
		nCount = strNext.Find(_T("+"));
	}
	strResultArr.Add(strNext);
	return;
}

void ArxDbgUtils::GetParenthesesFromString(CString strInput, CStringArray& strResultArr)
{
	strResultArr.RemoveAll();
	CString strOutput(_T(""));
	int nCountLeft = strInput.Find(_T("("));
	int nCountRight = strInput.Find(_T(")"));
	while(nCountRight>nCountLeft)
	{
		strOutput = strInput.Mid(nCountLeft+1, nCountRight - nCountLeft - 1);
		strResultArr.Add(strOutput);
		strInput = strInput.Mid(nCountRight+1);
		nCountLeft = strInput.Find(_T("("));
		nCountRight = strInput.Find(_T(")"));
	}	
	return;
}
CString ArxDbgUtils::GetParenthesesFromString(CString strInput)
{
	CString strOutput(_T(""));
	int nCountLeft = strInput.Find(_T("("));
	int nCountRight = strInput.Find(_T(")"));
	if (nCountRight<nCountLeft)
	{
		return strOutput;
	}
	strOutput = strInput.Mid(nCountLeft+1, nCountRight - nCountLeft - 1);
	return strOutput;
}
AcDbObjectIdArray ArxDbgUtils::OpenGroupAndGetIds(AcDbObjectId GroupId)
{
	AcDbObjectIdArray Ids;
	Ids.removeAll();

	AcDbGroup* pGroup = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbObject((AcDbObject*&)pGroup, GroupId, AcDb::kForWrite);
	if (es!= Acad::eOk)
	{
		pGroup->close();
	}
	else
	{
		AcDbEntity* pEnt = NULL;
		AcDbObjectId objId;
		int nLength = 0;
		nLength = pGroup->allEntityIds(Ids);
		pGroup->close();
	}
	return Ids;
}

int ArxDbgUtils::StringToInt(CString str)
{
	int nRet = 0;
#ifdef UNICODE
	nRet = _ttoi(str);
#else
	nRet = atoi(str);
#endif
	return nRet;
}

double ArxDbgUtils::StringToDouble(CString str)
{
	double dRet =0.0;
#ifdef UNICODE
	dRet = _tcstod(str, 0);
#else
	dRet = atof(str);
#endif
	return dRet;
}

void ArxDbgUtils::GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, const CString strTypeName, bool bIsFullPath)
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

CString ArxDbgUtils::OutPutStringByNumericalDigit(CString strPile, int nWeishu, int nFlag)
{
	CString strPrefix, strNext, strNum;
	int nlen = ArxDbgUtils::GetPileLengthEx(strPile, strPrefix, strNext);
	if (nFlag == 0)
	{	
		nlen++;	
	}
	else if (nFlag == 1)
	{
		nlen--;
	}
	
	if (nWeishu == 1)
	{
		strNum.Format(_T("%d"), nlen);
	}
	else if (nWeishu == 2)
	{
		if ((nlen<100)&&(nlen>=10) )
		{
			strNum.Format(_T("%d"), nlen);
		}
		else
		{
			strNum.Format(_T("0%d"), nlen);
		}
	}
	else
	{
		if ((nlen<1000)&&(nlen>=100))
		{
			strNum.Format(_T("%d"), nlen);
		}
		else if ((nlen<100)&&(nlen>=10))
		{
			strNum.Format(_T("0%d"), nlen);
		}
		else
		{
			strNum.Format(_T("00%d"), nlen);
		}
	}
	strPile = strPrefix + strNum + strNext;
	return strPile;
}

bool ArxDbgUtils::JudgeStr(CString str)
{
	bool bIsShuzi = false;
	int nCount = str.GetLength();
	for (int i=nCount-1; i>=0; i--)
	{
		if(isdigit(str[i]))//主序号是数字的情况
		{
			bIsShuzi = true;
		}
		else
		{
			CString strTmp = str[i];
			if (strTmp.CompareNoCase(_T("."))==0)
			{
				bIsShuzi = true;
			}
			else
			{
				bIsShuzi = false;
				break;
			}
		}
	}
	return bIsShuzi;
}
//获得当前的视图设置
void ArxDbgUtils::GetCurrentView(AcDbViewTableRecord& view)
{
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// 获得当前视口的“查看”模式
	acedGetVar(_T("VIEWMODE"), &rb);
	view.setPerspectiveEnabled(rb.resval.rint & 1);
	view.setFrontClipEnabled(rb.resval.rint & 2);
	view.setBackClipEnabled(rb.resval.rint & 4);
	view.setFrontClipAtEye(!(rb.resval.rint & 16)); 

	// 当前视口中视图的中心点（UCS坐标）
	acedGetVar(_T("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], rb.resval.rpoint[Y])); 

	// 当前视口透视图中的镜头焦距长度（单位为毫米）
	acedGetVar(_T("LENSLENGTH"), &rb);
	view.setLensLength(rb.resval.rreal);

	// 当前视口中目标点的位置（以 UCS 坐标表示）
	acedGetVar(_T("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget(AcGePoint3d(rb.resval.rpoint[X],	rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的观察方向（UCS）
	acedGetVar(_T("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的视图高度（图形单位）
	acedGetVar(_T("VIEWSIZE"), &rb);
	view.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	// 以像素为单位的当前视口的大小（X 和 Y 值）
	acedGetVar(_T("SCREENSIZE"), &rb);
	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);

	// 当前视口的视图扭转角
	acedGetVar(_T("VIEWTWIST"), &rb);
	view.setViewTwist(rb.resval.rreal);

	// 将模型选项卡或最后一个布局选项卡置为当前
	acedGetVar(_T("TILEMODE"), &rb);
	int tileMode = rb.resval.rint;
	// 设置当前视口的标识码
	acedGetVar(_T("CVPORT"), &rb);
	int cvport = rb.resval.rint;


	// 是否是模型空间的视图
	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	view.setIsPaperspaceView(paperspace);

	if (!paperspace)
	{
		// 当前视口中前向剪裁平面到目标平面的偏移量
		acedGetVar(_T("FRONTZ"), &rb);
		view.setFrontClipDistance(rb.resval.rreal);

		// 获得当前视口后向剪裁平面到目标平面的偏移值
		acedGetVar(_T("BACKZ"), &rb);
		view.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}
}
bool ArxDbgUtils::getCurViewExt(AcDbExtents& dcsExt)
{
	resbuf rbSCREENSIZE;
	int rt = acedGetVar(_T("SCREENSIZE"), &rbSCREENSIZE);

	double dblHwSca = rbSCREENSIZE.resval.rpoint[1] / rbSCREENSIZE.resval.rpoint[0];

	resbuf rbVIEWSIZE;
	rt = acedGetVar(_T("VIEWSIZE"), &rbVIEWSIZE);

	double dx = rbVIEWSIZE.resval.rreal / dblHwSca / 2.0;
	double dy = rbVIEWSIZE.resval.rreal / 2.0;

	resbuf rbVIEWCTR;
	rt = acedGetVar(_T("VIEWCTR"), &rbVIEWCTR);

	ads_point pntScopCen;
	ads_point_set(rbVIEWCTR.resval.rpoint, pntScopCen);

	acdbUcs2Wcs(pntScopCen, pntScopCen, 0);

	ads_point pntScopMin;
	pntScopMin[0] = pntScopCen[0] - dx;
	pntScopMin[1] = pntScopCen[1] - dy;
	pntScopMin[2] = 0.0;

	ads_point pntScopMax;
	pntScopMax[0] = pntScopCen[0] + dx;
	pntScopMax[1] = pntScopCen[1] + dy;
	pntScopMax[2] = 0.0;

	resbuf wcs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;

	resbuf dcs;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;
	acedTrans(pntScopMin, &wcs, &dcs, 0, pntScopMin);
	acedTrans(pntScopMax, &wcs, &dcs, 0, pntScopMax);

	dcsExt.addPoint(asPnt3d(pntScopMin));
	dcsExt.addPoint(asPnt3d(pntScopMax));

	return true;

}

void ArxDbgUtils::ZOOMEXT(double dScale)
{
	//缩放前纪录视图数据
	AcDbExtents dcsLastViewExt;
	getCurViewExt(dcsLastViewExt);

	resbuf rbLIMMIN;
	acedGetVar(_T("LIMMIN"), &rbLIMMIN);

	resbuf rbLIMMAX;
	acedGetVar(_T("LIMMAX"), &rbLIMMAX);

	acdbCurDwg()->updateExt();

	resbuf rbEXTMIN;
	acedGetVar(_T("EXTMIN"), &rbEXTMIN);

	resbuf rbEXTMAX;
	acedGetVar(_T("EXTMAX"), &rbEXTMAX);

	resbuf rbVIEWDIR;
	acedGetVar(_T("VIEWDIR"), &rbVIEWDIR);

	AcGeVector3d vecViewDir = asVec3d(rbVIEWDIR.resval.rpoint);

	bool m_bHasEntInCurrView = false;
	ads_name lastename;	
	if (RTNORM  == acdbEntLast(lastename))
	{
		m_bHasEntInCurrView = true;
	}

	AcDbExtents maxExt;

	if (m_bHasEntInCurrView)
	{
		AcGeVector3d wcsViewDir;
		acdbUcs2Wcs(asDblArray(vecViewDir), asDblArray(wcsViewDir), true);
		wcsViewDir = wcsViewDir.normal();

		if (wcsViewDir.isEqualTo(AcGeVector3d::kZAxis))
		{
			maxExt.addPoint(asPnt3d(rbLIMMIN.resval.rpoint));
			maxExt.addPoint(asPnt3d(rbLIMMAX.resval.rpoint));
		}

		if (rbEXTMAX.resval.rpoint[0] >= rbEXTMIN.resval.rpoint[0] &&
			rbEXTMAX.resval.rpoint[1] >= rbEXTMIN.resval.rpoint[1])
		{
			maxExt.addPoint(asPnt3d(rbEXTMIN.resval.rpoint));
			maxExt.addPoint(asPnt3d(rbEXTMAX.resval.rpoint));
		}
	}
	else
	{
		maxExt.addPoint(asPnt3d(rbLIMMIN.resval.rpoint));
		maxExt.addPoint(asPnt3d(rbLIMMAX.resval.rpoint));
	}

	// 计算长方形最小包围盒的顶点
	ads_point pt[/*7*/8];
	pt[0][X] = pt[3][X] = pt[4][X] = pt[7][X] = maxExt.minPoint().x;
	pt[1][X] = pt[2][X] = pt[5][X] = pt[6][X] = maxExt.maxPoint().x;
	pt[0][Y] = pt[1][Y] = pt[4][Y] = pt[5][Y] = maxExt.minPoint().y;
	pt[2][Y] = pt[3][Y] = pt[6][Y] = pt[7][Y] = maxExt.maxPoint().y;
	pt[0][Z] = pt[1][Z] = pt[2][Z] = pt[3][Z] = maxExt.minPoint().z;
	pt[4][Z] = pt[5][Z] = pt[6][Z] = pt[7][Z] = maxExt.maxPoint().z; 

	// 将长方体的所有角点转移到DCS中
	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	acedTrans(pt[0], &wcs, &ucs, 0, pt[0]);
	acedTrans(pt[1], &wcs, &ucs, 0, pt[1]);
	acedTrans(pt[2], &wcs, &ucs, 0, pt[2]);
	acedTrans(pt[3], &wcs, &ucs, 0, pt[3]);
	acedTrans(pt[4], &wcs, &ucs, 0, pt[4]);
	acedTrans(pt[5], &wcs, &ucs, 0, pt[5]);
	acedTrans(pt[6], &wcs, &ucs, 0, pt[6]);
	acedTrans(pt[7], &wcs, &ucs, 0, pt[7]);

	acedTrans(pt[0], &ucs, &dcs, 0, pt[0]);
	acedTrans(pt[1], &ucs, &dcs, 0, pt[1]);
	acedTrans(pt[2], &ucs, &dcs, 0, pt[2]);
	acedTrans(pt[3], &ucs, &dcs, 0, pt[3]);
	acedTrans(pt[4], &ucs, &dcs, 0, pt[4]);
	acedTrans(pt[5], &ucs, &dcs, 0, pt[5]);
	acedTrans(pt[6], &ucs, &dcs, 0, pt[6]);
	acedTrans(pt[7], &ucs, &dcs, 0, pt[7]);

	// 获得所有角点在DCS中最小的包围矩形
	double xMax = pt[0][X], xMin = pt[0][X];
	double yMax = pt[0][Y], yMin = pt[0][Y];
	for (int i = 1; i <= 7; i++)
	{
		if (pt[i][X] > xMax)
			xMax = pt[i][X];
		if (pt[i][X] < xMin)
			xMin = pt[i][X];
		if (pt[i][Y] > yMax)
			yMax = pt[i][Y];
		if (pt[i][Y] < yMin)
			yMin = pt[i][Y];
	}

	//创建VIEW作为缩放参数
	AcDbViewTableRecord view;
	GetCurrentView(view);

	// 设置视图的中心点
	view.setCenterPoint(AcGePoint2d((xMin + xMax) / 2, (yMin + yMax) / 2));
	// 设置视图的高度和宽度
	view.setHeight(fabs(yMax - yMin)*dScale);
	view.setWidth(fabs(xMax - xMin)*dScale);
	// 将视图对象设置为当前视图
	Acad::ErrorStatus es = acedSetCurrentView(&view, NULL);

}
void ArxDbgUtils::ZOOMWINDOW(AcGePoint3d minPt, AcGePoint3d maxPt)
{
	// get the extents of the drawing
	AcDbViewTableRecord view;

	AcGePoint2d max_2d (maxPt[X], maxPt[Y]);
	AcGePoint2d min_2d (minPt[X], minPt[Y]);
	// now set the view centre point
	view.setCenterPoint (min_2d + (max_2d - min_2d) / 2.0);
	// now height and width of view
	view.setHeight(max_2d[Y] - min_2d[Y]);
	view.setWidth (max_2d[X] - min_2d[X]);
	// set the view
	acedSetCurrentView (&view, NULL);
	// updates the extents
	acdbHostApplicationServices()->workingDatabase()->updateExt(TRUE);
}

void  ArxDbgUtils::ZOOMWindowByDeFault(AcGePoint3d cenPt)
{
	AcDbExtents dcsLastViewExt;
	getCurViewExt(dcsLastViewExt);

	AcGePoint3d minPt,maxPt;
	minPt = dcsLastViewExt.minPoint();
	maxPt = dcsLastViewExt.maxPoint();
	
	double dHeight = maxPt.y - minPt.y;
	double dLength = maxPt.x - minPt.x;
	AcDbViewTableRecord view;

	
	// now set the view centre point
	view.setCenterPoint (cenPt.convert2d(AcGePlane::kXYPlane));
	// now height and width of view
	view.setHeight(dHeight);
	view.setWidth (dLength);
	// set the view
	acedSetCurrentView (&view, NULL);
	// updates the extents
	acdbHostApplicationServices()->workingDatabase()->updateExt(TRUE);
}

CString ArxDbgUtils::GetFileNameByPath(CString strPath)
{
	CString strtemp1;
	strtemp1 = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\')-1);	
	strtemp1=strtemp1.Left(strtemp1.GetLength() - 4);
	return strtemp1;
}

CString ArxDbgUtils::AddZeroForNumber(CString strValue, CString strNumber)
{
	int nValue = StringToInt(strValue);
	int nTmp = nValue;
	int nNum = StringToInt(strNumber);
	int nCount = 0;
	while(nTmp>0)
	{
		nTmp = nTmp/10;
		nCount++;
	}
	if (nCount >= nNum)
	{
		strValue.Format(_T("%d"), nValue);
		return strValue;
	}
	else
	{
		int j = nNum - nCount;
		strValue.Format(_T("%d"), nValue);
		for (int k=0; k<j; k++)
		{
			strValue.Insert(0, _T("0"));
		}
	}
	return strValue;
}
