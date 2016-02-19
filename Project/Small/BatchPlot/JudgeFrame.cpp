#include "JudgeFrame.h"
#include "Utility.h"
#include "line.h"
#include "SearchLines.h"

CJudgeFrame::CJudgeFrame(void)
{
	for (std::vector<FrameInfo*>::iterator itr = m_frames.begin(); itr != m_frames.end(); itr++)
	{
		FrameInfo* pFrame = *itr;
		delete pFrame;
	}
	m_frames.clear();
	m_nMode = 0;
	m_nMultiDoc = 0;
}

CJudgeFrame::~CJudgeFrame(void)
{
	
}


std::vector<FrameInfo*> CJudgeFrame::GetDataFromDwg()
{
	if (m_nMode == 0)
	{
		GetDataByLayerName();
	}
	else if (m_nMode == 1)
	{
		GetDataByBlockName();
	}
	else if (m_nMode == 2)
	{
		GetDataBySanxian();
	}
	
	return m_frames;
}

void CJudgeFrame::SetLayerName(LPCTSTR strLayerName)
{
	m_strLayerName = strLayerName;
}
void CJudgeFrame::SetBlockName(LPCTSTR strBlockName)
{
	m_strBlockName = strBlockName;
}

void CJudgeFrame::SetMode(const int nMode)
{
	m_nMode = nMode;
}

void CJudgeFrame::SetMultiDoc(const int nMultiDoc)
{
	m_nMultiDoc = nMultiDoc;
}

BOOL CJudgeFrame::GetDataByLayerName()
{
	struct resbuf*	filter=acutBuildList(-4, _T("<and"), RTDXF0, _T("LWPOLYLINE"), 8, m_strLayerName, -4, _T("and>"), RTNONE);
	BOOL bRet = SelectMode(filter);
	acutRelRb(filter);
	return bRet;
}

BOOL CJudgeFrame::GetDataByBlockName()
{
	struct resbuf*	filter=acutBuildList(-4, _T("<and"), RTDXF0, _T("INSERT"), 2, m_strBlockName, -4, _T("and>"), RTNONE);
	BOOL bRet = SelectMode(filter);
	acutRelRb(filter);
	return TRUE;
}

void CJudgeFrame::GetDataBySanxian()
{
	struct resbuf*	filter=acutBuildList(-4,_T("<or"), -4,_T("<and"),RTDXF0,_T("LWPOLYLINE"), -4,_T("and>"), -4,_T("<and"),RTDXF0,_T("LINE"), -4,_T("and>"), -4,_T("<and"),RTDXF0,_T("INSERT"), -4,_T("and>"), -4,_T("<and"),RTDXF0,_T("TEXT"), -4,_T("and>"), -4,_T("or>"),RTNONE);
	SelectSpecail(filter);
	acutRelRb(filter);
	return;
}

BOOL CJudgeFrame::SelectMode(const resbuf* filter)
{
	Acad::ErrorStatus es;
	ads_name ss;
	if (m_nMultiDoc == 0)
	{
		int nRet = acedSSGet(NULL, NULL, NULL, filter, ss);
		if (RTNORM != nRet)
		{
			return FALSE;
		}		
	}
	else
	{
		int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ss);
		if (RTNORM != nRet)
		{
			return FALSE;
		}		
	}
	long nlen;
	acedSSLength(ss, &nlen);
	if (nlen<=0)
	{
		acedSSFree(ss);
		return FALSE;
	}
	AcDbEntity* pEnt = NULL;
	ads_name ename;
	AcDbObjectId objId;
	bool bIsContined = true;

	for (long i = 0; i < nlen; i++)
	{
		acedSSName(ss, i, ename);
		es = acdbGetObjectId(objId, ename);
		es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pLine = AcDbPolyline::cast(pEnt);
			CString dwgFrameName,dwgFrameSize, dwgFrame;
			/*double dWidth;
			pLine->getConstantWidth(dWidth);
			if (dWidth>0)
			{
				pLine->close();
				continue;
			}*/
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

				AcGePoint2d pt1, pt2, pt3, pt4;
				pt1 = My3d22d(StartPT);

				pt2.x = StartPT.x;
				pt2.y = EndPt.y;

				pt3= My3d22d(EndPt);

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
			pLine->close();
		}
		else if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			if (!pBlkRef->treatAsAcDbBlockRefForExplode())
			{				
				//CString dwgFrame;
				//dwgFrame = m_strCurWBCZ;
				AcDbExtents exts;
				AcGePoint3d StartPT,EndPt;
				pBlkRef->getGeomExtents(exts);
				StartPT = exts.minPoint();
				EndPt = exts.maxPoint();

				AcGePoint2d pt1, pt2, pt3, pt4;
				pt1 = My3d22d(StartPT);

				pt2.x = StartPT.x;
				pt2.y = EndPt.y;

				pt3= My3d22d(EndPt);

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
			pBlkRef->close();
		}
		else
		{
			pEnt->close();
		}
	}
	acedSSFree(ss);
	if (!bIsContined)
	{
		return TRUE;
	}
	return TRUE;
}

void CJudgeFrame::SelectSpecail(const resbuf* filter)
{
	AcGePoint2dArray startPtArr,endPtArr;//记录点集
	Acad::ErrorStatus es = Acad::eOk;
	Utility util;
	AcGeTol tol;
	tol.setEqualPoint(0.01);

	long ssLen = 0;
	ads_name nmEnt;
	AcDbObjectId idEnt;
	AcDbEntity* pEnt = NULL;
	AcDbVoidPtrArray entitySet;
	std::list<CLine*> lines;
	double dTextHeight = 0;
	ads_name sname;
	if (m_nMultiDoc == 0)
	{
		int nRet = acedSSGet(NULL, NULL, NULL, filter, sname);
		if (RTNORM != nRet)
			return;
	}
	else
	{
		int nRet = acedSSGet(_T("X"), NULL, NULL, filter, sname);
		if (RTNORM != nRet)
			return;
	}

	acedSSLength(sname, &ssLen);
	for (long i = 0; i < ssLen; i++)
	{
		acedSSName(sname, i, nmEnt);
		es = acdbGetObjectId(idEnt, nmEnt);
		es = acdbOpenAcDbEntity(pEnt, idEnt, AcDb::kForRead);
		if (pEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine* pDbLine = AcDbLine::cast(pEnt);
			if (pDbLine)
			{
				double dAng = acutAngle(asDblArray(pDbLine->startPoint()), asDblArray(pDbLine->endPoint()));//mzg
				if (((abs(dAng-0.0)< 0.01)||(abs(dAng-PI/2)<0.01)||(abs(dAng-PI)<0.01)||(abs(dAng-3*PI/2)<0.01)||(abs(dAng-2*PI)<0.01)))
				{
					lines.push_back(new CLine(pDbLine->startPoint(), pDbLine->endPoint()));
				}
			}
		}
		else if (pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText* pDbText = AcDbText::cast(pEnt);
			if (pDbText)
			{
				if (dTextHeight == 0)
					dTextHeight = pDbText->height();
				else if (dTextHeight > pDbText->height())
					dTextHeight =  pDbText->height();
			}
		}
		else if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			//如果是通过区域选取的方式，那就不需要获取polyline,line及blockreference的数据
			////////////////////////////////////////////////
			AcDbPolyline* pLine = AcDbPolyline::cast(pEnt);
            //[Modified by mzg 20130110 
            //Reason: fix for 一些外框有宽度 
            double dWidth; 
            pLine->getConstantWidth(dWidth); 
            if (dWidth>0) 
            { 
				pLine->close(); 
				continue; 
            } 
            //]Modified by mzg 20130110
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
				AcGePoint2d pt1, pt2, pt3, pt4;
				pt1 = My3d22d(StartPT);

				pt2.x = StartPT.x;
				pt2.y = EndPt.y;

				pt3= My3d22d(EndPt);

				pt4.x = EndPt.x;
				pt4.y = StartPT.y;

				double dAng1 = acutAngle(asDblArray(pt1), asDblArray(pt2));//mzg
				double dAng2 = acutAngle(asDblArray(pt2), asDblArray(pt3));//mzg
				double dAng3 = acutAngle(asDblArray(pt3), asDblArray(pt4));//mzg
				double dAng4 = acutAngle(asDblArray(pt4), asDblArray(pt1));//mzg
				if (((abs(dAng1-0.0)< 0.01)||(abs(dAng1-PI/2)<0.01)||(abs(dAng1-PI)<0.01)||(abs(dAng1-3*PI/2)<0.01)||(abs(dAng1-2*PI)<0.01))
					&&((abs(dAng2-0.0)< 0.01)||(abs(dAng2-PI/2)<0.01)||(abs(dAng2-PI)<0.01)||(abs(dAng2-3*PI/2)<0.01)||(abs(dAng2-2*PI)<0.01))
					&&((abs(dAng3-0.0)< 0.01)||(abs(dAng3-PI/2)<0.01)||(abs(dAng3-PI)<0.01)||(abs(dAng3-3*PI/2)<0.01)||(abs(dAng3-2*PI)<0.01))
					&&((abs(dAng4-0.0)< 0.01)||(abs(dAng4-PI/2)<0.01)||(abs(dAng4-PI)<0.01)||(abs(dAng4-3*PI/2)<0.01)||(abs(dAng4-2*PI)<0.01)))
				{
					FrameInfo* pFrameInfo = new FrameInfo;

					pFrameInfo->m_minPoint = pt1;
					pFrameInfo->m_maxPoint = pt3;
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
			else
			{
				util.ExplodeEntity(pEnt, entitySet);
			}
		}
		//添加对属性块的处理
		else 
		{
			util.ExplodeEntity(pEnt, entitySet);
		}
		pEnt->close();
		pEnt = NULL;

		int expLength = entitySet.length();
		for (int j = 0; j < expLength; j++)
		{
			util.ExplodeEntity((AcDbEntity*)entitySet.at(j), entitySet);
		}

		for (int k=0; k<entitySet.length(); k++)
		{
			util.ExplodeEntity((AcDbEntity*)entitySet.at(k), entitySet);
		}

		expLength = entitySet.length();
		for (int j = 0; j < expLength; j++)
		{
			AcDbEntity* pTempEnt = (AcDbEntity*)entitySet.at(j);
			if (pTempEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pDbLine = AcDbLine::cast(pTempEnt);
				if (pDbLine)
				{
					double dAng = acutAngle(asDblArray(pDbLine->startPoint()), asDblArray(pDbLine->endPoint()));//mzg
					if (((abs(dAng-0.0)< 0.01)||(abs(dAng - PI/2)<0.01)||(abs(dAng-PI)<0.01)||(abs(dAng-3*PI/2)<0.01)||(abs(dAng-2*PI)<0.01)))
					{
						lines.push_back(new CLine(pDbLine->startPoint(), pDbLine->endPoint()));
					}
				}
			}
			delete pTempEnt;
			pTempEnt = NULL;
		}
		entitySet.removeAll();
	}
	// 消重
	for(std::list<CLine*>::iterator itr = lines.begin();
		itr != lines.end();
		itr++)
	{
		CLine* pLine1 = *itr;
		std::list<CLine*>::iterator itr2 = itr;
		std::list<CLine*>::iterator itrPre;
		for(itrPre = itr, itr2++; itr2 != lines.end(); itrPre = itr,itr2++)
		{
			CLine* pLine2 = *itr2;
			//mzg 
			//int  nRet1,nRet2;
			/*nRet1 = pLine1->IsEndpoint(pLine2->startPoint(), tol);
			nRet2 = pLine1->IsEndpoint(pLine2->endPoint(), tol);
			if (!((nRet1!=0) || (nRet2!=0)|| ((nRet1==nRet2) && (nRet1 ==0))))
			{
				lines.erase(itr2);
				itr2 = itrPre;
			}*/
			if (pLine1->IsEndpoint(pLine2->startPoint(), tol) && pLine1->IsEndpoint(pLine2->endPoint(), tol))
			{
				lines.erase(itr2);
				itr2 = itrPre;
			}
		}
	}
#ifdef TEST
	acutPrintf(_T("\nlines.size = %d"), lines.size()); // TEST
#endif
	if ((lines.size() < 4) &&(m_frames.size() == 0))
		return;

	//mzg test
	//将数据全加到点集中
	
	for(std::list<CLine*>::iterator itr = lines.begin();
		itr != lines.end();
		itr++)
	{
		CLine* pLine1 = *itr;
		startPtArr.append(pLine1->startPoint());
		endPtArr.append(pLine1->endPoint());
	}
	CSearchLines tSeatch;
	AcDbObjectIdArray objIds;
	tSeatch.ConstructLines(startPtArr, endPtArr);
	tSeatch.SearchAll(startPtArr, endPtArr);

	AcGePoint2d startPt, endPt;
	double dWidth,dLength;
	for (int i=0; i<startPtArr.length(); i++)
	{
		FrameInfo* pFrameInfo = new FrameInfo;
		startPt = startPtArr.at(i);
		endPt = endPtArr.at(i);
		pFrameInfo->m_minPoint = startPt;
		pFrameInfo->m_maxPoint = endPt;
		dWidth = util.CalculateWidth(My2d23d(startPt), My2d23d(endPt));
		dLength = util.CalculateLength(My2d23d(startPt), My2d23d(endPt));
		if (dWidth<dLength)
		{
			pFrameInfo->SetFrameDirection(true);
		}
		else
		{
			pFrameInfo->SetFrameDirection(false);
		}

		//////////////////////////////////////////////////////////////////////////////
		bool bFrameExists = false;

		BOOL bIsContinued = TRUE;
		while(bIsContinued)
		{
			bIsContinued = FALSE;
			for (std::vector<FrameInfo*>::iterator iter = m_frames.begin(); iter!=m_frames.end(); iter++)
			{
				FrameInfo* pTempFI = *iter;
				int	nRet = JudgeAreaInOtherArea(startPt, endPt, pTempFI->m_minPoint, pTempFI->m_maxPoint);
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
	///////////////////////////////////////////////////////////////////
	for(std::list<CLine*>::iterator itr = lines.begin();
		itr != lines.end();
		itr++)
	{
		CLine* pTempLine = *itr;
		delete pTempLine;
		pTempLine = NULL;
	}
	return;
}


//calculate函数
//判断点是否在区域内
//传入点pt,区域exts
BOOL CJudgeFrame::JudgePointInArea(AcGePoint2d insertPt, AcGePoint2d minPt, AcGePoint2d maxPt)
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
//判断图框是否在区域内
//返回1表示curExts在preExts内
//返回2表示preExts在curExts内
//返回3表示curExts与preExts没有关系
int CJudgeFrame::JudgeAreaInOtherArea(AcDbExtents curExts, AcDbExtents preExts)
{
	AcGePoint3d curMinPt,curMaxPt,preMinPt,preMaxPt;
	curMaxPt = curExts.maxPoint();
	curMinPt = curExts.minPoint();

	preMinPt = preExts.minPoint();
	preMaxPt = preExts.maxPoint();
	BOOL bIsInArea1 = FALSE;
	BOOL bIsInArea2 = FALSE;
	bIsInArea1 = JudgePointInArea(My3d22d(curMinPt), My3d22d(preMinPt), My3d22d(preMaxPt));
	bIsInArea2 = JudgePointInArea(My3d22d(curMaxPt), My3d22d(preMinPt), My3d22d(preMaxPt));
	if (bIsInArea1&&bIsInArea2)
	{
		return 1;
	}

	bIsInArea1 = JudgePointInArea(My3d22d(preMinPt), My3d22d(curMinPt), My3d22d(curMaxPt));
	bIsInArea2 = JudgePointInArea(My3d22d(preMaxPt), My3d22d(curMinPt), My3d22d(curMaxPt));
	if (bIsInArea1&&bIsInArea2)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}


int CJudgeFrame::JudgeAreaInOtherArea(AcGePoint2d curMinPt,AcGePoint2d curMaxPt,AcGePoint2d preMinPt,AcGePoint2d preMaxPt)
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

