#include "StdAfx.h"
#include "OutPutDwg.h"
#include "Utility.h"
#include <list>
#include "Line.h"

extern "C" HWND adsw_acadMainWnd();


COutPutDwg::COutPutDwg(void)
{
	for (std::vector<FrameInfo*>::iterator itr = m_frames.begin(); itr != m_frames.end(); itr++)
	{
		FrameInfo* pFrame = *itr;
		delete pFrame;
	}
	m_frames.clear();
}

COutPutDwg::~COutPutDwg(void)
{
	for (std::vector<FrameInfo*>::iterator itr = m_frames.begin(); itr != m_frames.end(); itr++)
	{
		FrameInfo* pFrame = *itr;
		delete pFrame;
	}
	m_frames.clear();
}

bool COutPutDwg::doIt()
{
	if (!sel())
	{
		return false;
	}
	bool bRet = true;
	for (vector<FrameInfo*>::iterator iter = m_frames.begin();
		iter != m_frames.end();
		++iter)
	{
		FrameInfo* pInfo = *iter;
	
		if (!getWidthAndHeight(pInfo))
		{
			bRet = false;
			break;
		}
		if (!getText(pInfo))
		{
			bRet = false;
			break;
		}
	}
	if (!bRet)
	{
		return false;
	}
	if (!GetOutPutPath())
	{
		return false;
	}
	for (vector<FrameInfo*>::iterator iter = m_frames.begin();
		iter != m_frames.end();
		++iter)
	{
		FrameInfo* pInfo = *iter;
		//acedCommand(RTSTR, _T(".zoom"), RTSTR, _T("W"), RTPOINT, asDblArray(pInfo->GetminPoint()), RTPOINT, asDblArray(pInfo->GetmaxPoint()), RTNONE);
		CString strTmp = pInfo->GetFileName() + _T(".DWG");
		CString strFileName = m_strPath + strTmp;
		strFileName.Remove(_T( '\r ')); 
		strFileName.Remove(_T( '\n '));
		AcGePoint3d minPt,maxPt;
		minPt = pInfo->GetminPoint();
		maxPt = pInfo->GetmaxPoint();
		acdbWcs2Ucs(asDblArray(maxPt), asDblArray(maxPt), false);
		acdbWcs2Ucs(asDblArray(minPt), asDblArray(minPt), false);
		OutPutDwg(minPt, maxPt, strFileName);
	}

	return true;
}

bool COutPutDwg::sel()
{
	struct resbuf*	filter=acutBuildList(-4, _T("<and"), RTDXF0, _T("LWPOLYLINE"), 8, _T("外框"), -4, _T("and>"), RTNONE);
	ads_name ss;
	int nRet = acedSSGet(NULL, NULL, NULL, filter, ss);
	if (RTNORM != nRet)
	{
		return false;
	}	
	acutRelRb(filter);
	long nlen;
	acedSSLength(ss, &nlen);
	if (nlen<=0)
	{
		acedSSFree(ss);
		return false;
	}
	AcDbEntity* pEnt = NULL;
	ads_name ename;
	AcDbObjectId objId;
	bool bIsContined = true;
	Acad::ErrorStatus es;
	for (long i = 0; i < nlen; i++)
	{
		acedSSName(ss, i, ename);
		es = acdbGetObjectId(objId, ename);
		es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine = AcDbPolyline::cast(pEnt);
			int plCount = 0;
			plCount = pLine->numVerts();

			Adesk::Boolean bIsClosed = FALSE;
			bIsClosed = pLine->isClosed();

			AcGePoint3d StartPT, EndPt;
			pLine->getStartPoint(StartPT);
			pLine->getEndPoint(EndPt);
			double dLen = StartPT.distanceTo(EndPt);
			bool bo1 = false;
			bool bo2 = false;
			bo1 = (dLen<0.01 && (plCount == 5));
			bo2 = ((plCount == 4)&&bIsClosed); 
			if ((bo1||bo2))
			{
				AcDbExtents exts;
				pLine->getGeomExtents(exts);
				StartPT = exts.minPoint();
				EndPt = exts.maxPoint();

				AcGePoint3d pt1, pt2, pt3, pt4;
				pt1 = StartPT;

				pt2.x = StartPT.x;
				pt2.y = EndPt.y;

				pt3= EndPt;

				pt4.x = EndPt.x;
				pt4.y = StartPT.y;

				FrameInfo* pFrameInfo = new FrameInfo;
				//pFrameInfo->SetDwgFrameSize(dwgFrameSize);
				//pFrameInfo->SetDwgFrame(dwgFrame);
				pFrameInfo->m_minPoint = pt1;
				pFrameInfo->m_maxPoint = pt3;
				Utility util;
				double dWidth = util.CalculateWidth(StartPT, EndPt);
				double dLength = util.CalculateLength(StartPT, EndPt);
				if (dWidth<dLength)
				{
					pFrameInfo->SetFrameDirection(true);
				}
				else
				{
					pFrameInfo->SetFrameDirection(false);
				}
				bool bFrameExists = false;

				BOOL bIsContinued = TRUE;
				while(bIsContinued)
				{
					bIsContinued = FALSE;
					for (std::vector<FrameInfo*>::iterator iter = m_frames.begin(); iter!=m_frames.end(); iter++)
					{
						FrameInfo* pTempFI = *iter;
						int	nRet = JudgeAreaInOtherArea(pt1, pt3, pTempFI->m_minPoint, pTempFI->m_maxPoint);
						if (nRet==1)
						{
							bFrameExists = true;
							bIsContinued = FALSE;
							break;
						}
						else if (nRet == 2)
						{
							m_frames.erase(iter);
							bIsContinued = TRUE;
							bFrameExists = false;
							break;
						}
						if (iter == m_frames.end())
						{
							bIsContinued = FALSE;
						}
					}
				}
				if (!bFrameExists)
				{
					m_frames.push_back(pFrameInfo);
				}
				else
				{
					delete pFrameInfo;
					pFrameInfo = NULL;
				}
			}
		}
		pEnt->close();
	}
	acedSSFree(ss);
	return true;
}


bool COutPutDwg::OutPutDwg(AcGePoint3d minPt, AcGePoint3d maxPt, CString strDwgFile)
{
	/*acutPrintf(_T("\n"));
	acutPrintf(strDwgFile);*/
	AcGeVector3d vec = maxPt - minPt;
	double dlen = acutDistance(asDblArray(minPt), asDblArray(maxPt));
	double dAng = vec.angleOnPlane(AcGePlane::kXYPlane);
	acutPolar(asDblArray(minPt), dAng, 0.01*dlen, asDblArray(minPt));
	acutPolar(asDblArray(maxPt), PI+dAng, 0.01*dlen, asDblArray(maxPt));
	ZOOMWINDOW(minPt, maxPt);
	ads_name ssname;
	int nRet = acedSSGet(_T("C"), asDblArray(minPt), asDblArray(maxPt), NULL, ssname);
	if (nRet != RTNORM)
	{
		return false;
	}

	AcDbObjectIdArray objIds;
	objIds.removeAll();
	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	AcDbObjectId entId = AcDbObjectId::kNull;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(entId, ename);	
		objIds.append(entId);
	}
	acedSSFree(ssname);
	AcDbDatabase *pDwg = new AcDbDatabase(); 
	Acad::ErrorStatus es;
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbIdMapping idMap;
	AcDbObjectId ownerId;
	ownerId = pDwg->currentSpaceId();

	es = acdbHostApplicationServices()->workingDatabase()->wblockCloneObjects(objIds, ownerId, idMap, AcDb::kDrcIgnore);

	es = pDwg->saveAs(strDwgFile, false);
	if (pDwg != NULL)
	{
		delete pDwg;
	}
	pDwg = NULL;

	
	acTransactionManagerPtr()->queueForGraphicsFlush();
	acTransactionManagerPtr()->flushGraphics();//刷新
	acedUpdateDisplay();

	return true;
}
//calculate函数
//判断点是否在区域内
//传入点pt,区域exts
BOOL COutPutDwg::JudgePointInArea(AcGePoint3d insertPt, AcGePoint3d minPt, AcGePoint3d maxPt)
{
	bool bo1,bo2,bo3,bo4;
	bo1 = ((insertPt.x - minPt.x)>=0);
	bo2 = ((insertPt.x - maxPt.x)<=0);
	bo3 = ((insertPt.y-minPt.y)>=0);
	bo4 = ((insertPt.y-maxPt.y)<=0);
	if (((insertPt.x - minPt.x)>=0)&&
		((insertPt.x - maxPt.x)<=0)&&
		((insertPt.y-minPt.y)>=0)&&
		((insertPt.y-maxPt.y)<=0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void COutPutDwg::ZOOMWINDOW(AcGePoint3d minPt, AcGePoint3d maxPt)
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
bool COutPutDwg::getText(FrameInfo* pInfo)
{
	AcGePoint3d minPt,maxPt;
	minPt = pInfo->GetminPoint();
	maxPt = pInfo->GetmaxPoint();
	acdbWcs2Ucs(asDblArray(maxPt), asDblArray(maxPt), false);
	acdbWcs2Ucs(asDblArray(minPt), asDblArray(minPt), false);
	ZOOMWINDOW(minPt, maxPt);
	struct resbuf*	filter=acutBuildList(-4,_T("<or"), -4, _T("<and"), RTDXF0, _T("TEXT"), -4, _T("and>"), -4,_T("<and"),RTDXF0,_T("MTEXT"), -4,_T("and>"), -4,_T("or>"), RTNONE);
	ads_name ss;
	int nRet = acedSSGet(_T("C"), asDblArray(minPt), asDblArray(maxPt), filter, ss);
	if (RTNORM != nRet)
	{
		return false;
	}	
	acutRelRb(filter);
	long nlen;
	acedSSLength(ss, &nlen);
	if (nlen<=0)
	{
		acedSSFree(ss);
		return false;
	}
	AcDbEntity* pEnt = NULL;
	ads_name ename;
	AcDbObjectId objId;
	CString strText;
	m_strFile = _T("");
	for (long i = 0; i < nlen; i++)
	{
		acedSSName(ss, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText = AcDbText::cast(pEnt);
			strText = pText->textStringConst();
			strText.Replace(_T("XX"), m_strHeight);
			strText.Replace(_T("YYY"), m_strWidth);
		
			m_strFile += strText;
		}
		else if (pEnt->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pText = AcDbMText::cast(pEnt);
			
			strText = pText->text();
			strText.Replace(_T("XX"), m_strHeight);
			strText.Replace(_T("YYY"), m_strWidth);
			m_strFile += strText;
		}
		pEnt->close();
	}
	/*acutPrintf(_T("\n"));
	acutPrintf(m_strFile);*/
	pInfo->SetFileName(m_strFile);
	acedSSFree(ss);
	return true;
}

bool COutPutDwg::getWidthAndHeight(FrameInfo* pInfo)
{
	AcGePoint3d minPt,maxPt;
	minPt = pInfo->GetminPoint();
	maxPt = pInfo->GetmaxPoint();
	acdbWcs2Ucs(asDblArray(maxPt), asDblArray(maxPt), false);
	acdbWcs2Ucs(asDblArray(minPt), asDblArray(minPt), false);
	ZOOMWINDOW(minPt, maxPt);
	struct resbuf*	filter=acutBuildList(-4, _T("<and"), RTDXF0, _T("DIMENSION"), -4, _T("and>"), RTNONE);
	ads_name ss;
	int nRet = acedSSGet(_T("W"), asDblArray(minPt), asDblArray(maxPt), filter, ss);
	if (nRet != RTNORM)
	{
		return false;
	}
	acutRelRb(filter);
	long nlen;
	acedSSLength(ss, &nlen);
	if (nlen<=0)
	{
		acedSSFree(ss);
		return false;
	}
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbEntity* pEnt = NULL;
	double dRotate, dHeight,dWidth;
	AcGePoint3d linePt1,linePt2;
	double dStaticHeight = 0;
	double dStaticWidth = 0;
	for (int i=0; i<nlen; i++)
	{
		acedSSName(ss, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		if (pEnt->isKindOf(AcDbRotatedDimension::desc()))
		{
			AcDbRotatedDimension* pDim = AcDbRotatedDimension::cast(pEnt);
			dRotate = pDim->rotation();
			linePt1 = pDim->xLine1Point();
			linePt2 = pDim->xLine2Point();
			if (dRotate < GeTol)
			{
				dWidth = abs(linePt2.x - linePt1.x);
				if (dWidth > dStaticWidth)
				{
					dStaticWidth = dWidth;
				}
			}
			else if (abs(dRotate - PI/2) <GeTol)
			{
				dHeight = abs(linePt2.y - linePt1.y);
				if (dHeight > dStaticHeight)
				{
					dStaticHeight = dHeight;
				}
			}
		}
		pEnt->close();
	}
	acedSSFree(ss);
	if ((dStaticWidth > 0) && (dStaticHeight > 0))
	{
		m_strHeight.Format(_T("%.f"), dStaticHeight);
		m_strWidth.Format(_T("%.f"), dStaticWidth);
	}
	else
	{
		return false;
	}
	return true;

}

bool COutPutDwg::GetOutPutPath()
{
	/*struct resbuf *rb = acutNewRb(RTSTR);
	TCHAR pathIn[MAX_PATH];
	if (RTNORM == acedGetFileNavDialog(_T("存储路径"),NULL,NULL, NULL, 1, &rb)) 
		_tcscpy(pathIn, rb->resval.rstring);
	else
	{   
		acutRelRb(rb);
		return false;	
	}
	m_strPath = pathIn;
	acutRelRb(rb);*/
	TCHAR Mycom[_MAX_PATH];  
	BROWSEINFO Myfold;  
	Myfold.hwndOwner= adsw_acadMainWnd();// sds_getmainhwnd();
	Myfold.pidlRoot=NULL;  
	Myfold.pszDisplayName=Mycom;  
	Myfold.lpszTitle=_T("请选择工程路径:");  
	Myfold.ulFlags=0;  
	Myfold.lpfn=NULL;  
	Myfold.lParam=NULL;  
	Myfold.iImage=NULL;  

	Mycom[0]=_T('\0');  
	SHGetPathFromIDList(SHBrowseForFolder(&Myfold),Mycom);  

	m_strPath.Format(_T("%s"),Mycom);
	m_strPath += _T("\\");
	return true;
}

//判断图框是否在区域内
//返回1表示curExts在preExts内
//返回2表示preExts在curExts内
//返回3表示curExts与preExts没有关系
int COutPutDwg::JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts)
{
	AcGePoint3d curMinPt,curMaxPt,preMinPt,preMaxPt;
	curMaxPt = curExts.maxPoint();
	curMinPt = curExts.minPoint();

	preMinPt = preExts.minPoint();
	preMaxPt = preExts.maxPoint();
	BOOL bIsInArea1 = FALSE;
	BOOL bIsInArea2 = FALSE;
	bIsInArea1 = JudgePointInArea(curMinPt, preMinPt, preMaxPt);
	bIsInArea2 = JudgePointInArea(curMaxPt, preMinPt, preMaxPt);
	if (bIsInArea1&&bIsInArea2)
	{
		return 1;
	}

	bIsInArea1 = JudgePointInArea(preMinPt, curMinPt, curMaxPt);
	bIsInArea2 = JudgePointInArea(preMaxPt, curMinPt, curMaxPt);
	if (bIsInArea1&&bIsInArea2)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}


int COutPutDwg::JudgeAreaInOtherArea(AcGePoint3d curMinPt,AcGePoint3d curMaxPt,AcGePoint3d preMinPt,AcGePoint3d preMaxPt)
{
	BOOL bIsInArea1 = FALSE;
	BOOL bIsInArea2 = FALSE;
	bIsInArea1 = JudgePointInArea(curMinPt, preMinPt, preMaxPt);
	bIsInArea2 = JudgePointInArea(curMaxPt, preMinPt, preMaxPt);
	if (bIsInArea1&&bIsInArea2)
	{
		return 1;
	}

	bIsInArea1 = JudgePointInArea(preMinPt, curMinPt, curMaxPt);
	bIsInArea2 = JudgePointInArea(preMaxPt, curMinPt, curMaxPt);
	if (bIsInArea1&&bIsInArea2)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

