// SearchLines.cpp: implementation of the CSearchLines class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SearchLines.h"
//#include "TestFuc.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


int GetRegister(HKEY hKey, 
				LPCTSTR lpszSec, 
				LPCTSTR lpszKey, 
				LPCTSTR lpszDef, 
				LPTSTR lpszValue)
{
	HKEY hInfor;
	DWORD dwType;
	DWORD dwLength;

	if(RegOpenKeyEx(hKey,
		lpszSec,
		0, 
		KEY_READ|KEY_EXECUTE,
		&hInfor) != ERROR_SUCCESS)
	{
#ifdef UNICODE
		_tcscpy(lpszValue,lpszDef);
#else
		strcpy(lpszValue,lpszDef);
#endif
		
		return -1;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		NULL,
		NULL,
		&dwLength) != ERROR_SUCCESS)
	{
#ifdef UNICODE
		_tcscpy(lpszValue,lpszDef);
#else
		strcpy(lpszValue,lpszDef);
#endif
		RegCloseKey(hInfor);
		return -2;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		&dwType,
		(LPBYTE)lpszValue,
		&dwLength) != ERROR_SUCCESS)
	{
#ifdef UNICODE
		_tcscpy(lpszValue,lpszDef);
#else
		strcpy(lpszValue,lpszDef);
#endif
		RegCloseKey(hInfor);
		return -2;
	}
	RegCloseKey(hInfor);
#ifdef UNICODE
	return (_tcslen(lpszValue)+1);
#else
	return (strlen(lpszValue)+1);

#endif
}

CString SubString(CString string, CString Split, int index)
{
	int Num=string.Find(Split);
	if(Num==-1) return "";
	if(index==0)
		return string.Left(Num);
	int i=1;
	CString retStr="";
	while(i<index){
		Num=string.Find(Split,Num+1);
		if(Num==-1) return "";
		i++;
	}
	int Num2=string.Find(Split,Num+1);
	if(Num2==-1){
		retStr=string.Mid(Num+1);
	}
	else
	{
		retStr=string.Mid(Num+1,Num2-Num-1);

	}
	return retStr;
}

BOOL createNewLayer(AcDbObjectId& newLayerId, CString strLayName, UInt16 nLayColor, bool locked, bool frozen)
{		 
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if (!pDb) 
		return FALSE;

	AcDbLayerTable* pLayerTbl;
	if ( pDb->getLayerTable(pLayerTbl,AcDb::kForWrite)!=Acad::eOk )
		return FALSE;

	if ( !pLayerTbl->has(strLayName) )	//层不存在
	{
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(strLayName);
		pLayerTblRcd->setIsFrozen(frozen);	// layer to THAWED //0
		pLayerTblRcd->setIsOff(0);			// layer to ON
		pLayerTblRcd->setVPDFLT(0);			// viewport default
		pLayerTblRcd->setIsLocked(locked);	// un-locked //0

		AcCmColor color;
		color.setColorIndex(nLayColor);		// set color to red
		pLayerTblRcd->setColor(color);

		pLayerTbl->add(newLayerId,pLayerTblRcd);
		pLayerTblRcd->close();
	}
	else	//存在
	{
		AcDbLayerTableRecord* pLayerTblRcd;
		if ( (pLayerTbl->getAt(strLayName,pLayerTblRcd,AcDb::kForWrite))==Acad::eOk )
		{
			pLayerTbl->getAt(strLayName,newLayerId);

			pLayerTblRcd->setIsFrozen(frozen);	// layer to THAWED //0
			pLayerTblRcd->setIsOff(0);			// layer to ON
			pLayerTblRcd->setVPDFLT(0);			// viewport default
			pLayerTblRcd->setIsLocked(locked);	// un-locked //0

			AcCmColor color;
			color.setColorIndex(nLayColor);		// set color to red
			pLayerTblRcd->setColor(color);

			pLayerTblRcd->close();
		}
	}
	pLayerTbl->close();

	return TRUE;
}

Acad::ErrorStatus Poly(AcDbObjectId& retId,AcGePoint2dArray ptArr,
					  int nColor=256,double linWidth=0.0,BOOL isClosed=TRUE)
{
	int num=ptArr.length();
	if(num<2)
		return Acad::eKeyNotFound;

	AcDbBlockTable* pBlockTable;
	Acad::ErrorStatus es=acdbCurDwg()->getBlockTable(pBlockTable,AcDb::kForRead);
	if(es!=Acad::eOk)
		return es;

	AcDbBlockTableRecord* pBlockTableRecord;
	es=pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		pBlockTable->close();
		return es;
	}
	pBlockTable->close();

	AcDbPolyline* pPolyEnt = new AcDbPolyline(num);
	for(int i=0;i<num;i++)
		pPolyEnt->addVertexAt(i,ptArr[i]);

	if(linWidth!=0)
		pPolyEnt->setConstantWidth(linWidth);

	if(isClosed)
		pPolyEnt->setClosed(Adesk::kTrue);

	pPolyEnt->setColorIndex(nColor);

	es=pBlockTableRecord->appendAcDbEntity(retId,pPolyEnt);
	if(es!=Acad::eOk)
	{
		pBlockTableRecord->close(); 
		if(pPolyEnt)
			delete pPolyEnt;
		return es;
	}

	pBlockTableRecord->close(); 
	double dArea;
	es = pPolyEnt->getArea(dArea);
	if (dArea-0<1)
	{
		pPolyEnt->erase();
		pPolyEnt->close();
		return Acad::eNotImplementedYet;
	}
	pPolyEnt->close();
	return Acad::eOk;
}

void Double_Sort(AcGeIntArray& source,BOOL bUp=TRUE) 
{
	AcGeIntArray  result;
	result.setLogicalLength(0);
	int oldSize= source.length();
	int i =0;
	for (i =0;i<oldSize ;i++)
	{
		int a;
		a = source[i];
		for (int j = i + 1;j<oldSize;j++)
		{
			int aa;
			aa = source[j];

			if(bUp)
			{
				if (source[i] > source[j])
				{
					int c;
					c = source[i];
					source[i] = source[j];
					source[j] = c;
				}
			}
			else
			{
				if (source[i] < source[j])
				{
					int c;
					c = source[i];
					source[i] = source[j];
					source[j] = c;
				}				
			}
		}
	}

	for(i=0;i<oldSize;i++)
	{
		if(result.length()==0)
			result.append(source[i]);
		else
		{
			int oldLen=result.length();
			bool hasSame=FALSE;
			for(int z=0;z<oldLen;z++)
			{
				if(abs(source[i]-result[z])<EPSINON)
				{
					hasSame = TRUE;
					break;
				}
			}
			if(!hasSame)
				result.append(source[i]);
		}
	}

	source.setLogicalLength(0);
	for(i=0;i<result.length();i++)
		source.append(result.at(i));	
}

CSearchLines::CSearchLines()
{
	ptSelArr.setLogicalLength(0);
	m_selected.setLogicalLength(0);
	//linesArr.setLogicalLength(0);
	sptArr.setLogicalLength(0);
	eptArr.setLogicalLength(0);
	
}

CSearchLines::~CSearchLines()
{
}

BOOL CSearchLines::ConstructLines(ads_name &ssname)
{
	long nums=0;
	int rc=ads_sslength(ssname,&nums);
	if(rc!=RTNORM)
		return FALSE;
		
	if(nums<=0)
		return FALSE;

	for(int i=0; i<nums; i++)
	{
		ads_name oneent;
		if( ads_ssname(ssname,i,oneent)!=RTNORM )
			continue;	
		
		AcDbObjectId tId;
		if( acdbGetObjectId(tId,oneent)!=Acad::eOk )
			continue;
		
		AcDbEntity* pEnt=NULL;
		if( acdbOpenObject(pEnt,tId,AcDb::kForWrite)!=Acad::eOk )
			continue;
		
		if( pEnt->isKindOf(AcDbLine::desc()) )
		{
			AcDbLine* pLine=AcDbLine::cast(pEnt);
			AcGePoint3d spt = pLine->startPoint();
			AcGePoint3d ept = pLine->endPoint();
			double dDist = spt.distanceTo(ept);
			double dAng = acutAngle(asDblArray(spt), asDblArray(ept));
			if ((dDist - 0.1*139>1)||
				(dDist - 50000*4752<1)
				&&((abs(dAng-0.0)< 0.01)||(abs(dAng-PI/2)<0.01)||(abs(dAng-PI)<0.01)||(abs(dAng-3*PI/2)<0.01)||(abs(dAng-2*PI)<0.01)))
			{
				sptArr.append(spt);
				eptArr.append(ept);
				//linesArr.append(tId);
			}
		}
		pEnt->close();
	}

	return TRUE;
}
BOOL CSearchLines::ConstructLines(AcGePoint2dArray startPtArr, AcGePoint2dArray endPtArr)
{
	AcGePoint2d startPt,endPt;
	for (int i=0; i<startPtArr.length(); i++)
	{
		startPt = startPtArr.at(i);
		endPt = endPtArr.at(i);
		sptArr.append(My2d23d(startPt));
		eptArr.append(My2d23d(endPt));
	}
	return TRUE;
}


int CSearchLines::FindIndexAtPoint(AcGePoint3d endpt,int nOld)
{
	for(int i=0;i<sptArr.length();i++)
	{
		if(i==nOld)
			continue;
		
		int nAt=-1;
		if(m_selected.find(i,nAt))	//已有的跳过
			continue;
		
		double d1 = endpt.distanceTo(sptArr[i]);
		double d2 = endpt.distanceTo(eptArr[i]);
		int nBeishu =1;
		double dDist = 0.0;
		dDist = max(d1, d2);
		nBeishu = ceil(dDist/100);//100误差允许为0.1
		if(nBeishu ==0)
		{
			nBeishu = 1;
		}
		if( (d1<nBeishu*DISWUCHA) || (d2<nBeishu*DISWUCHA) )					//i是与endpt紧紧连着 
			return i;
	}

	return -1;
}

BOOL CSearchLines::SearchbyIndexPoint(int nCurr,AcGePoint3d point)
{	
	if(nCurr<0||nCurr>sptArr.length()-1)
		return FALSE;
	
	int nStart=FindIndexAtPoint(point,nCurr);
	if(nStart<0||nStart>sptArr.length()-1)
		return FALSE;
	
	int nAt=-1;
	if(!m_selected.find(nStart,nAt))
		m_selected.append(nStart);
	else
		return FALSE;
	
	AcGePoint3d npoint;
	double dDist1=point.distanceTo(sptArr[nStart]);
	double dDist2=point.distanceTo(eptArr[nStart]);
	if( dDist1<dDist2 )
	{
		npoint=eptArr[nStart];
		ptSelArr.append(sptArr[nStart]);
		ptSelArr.append(eptArr[nStart]);
	}
	else
	{
		npoint=sptArr[nStart];
		ptSelArr.append(eptArr[nStart]);
		ptSelArr.append(sptArr[nStart]);
	}

	BOOL boo=SearchbyIndexPoint(nStart,npoint);
	if(!boo)
		return FALSE;
	
	return TRUE;
}
BOOL CSearchLines::SearchAll(AcGePoint2dArray& startPtArr, AcGePoint2dArray& endPtArr)
{
	startPtArr.removeAll();
	endPtArr.removeAll();
	
	if ( sptArr.length()<1 )
		return FALSE;


	for (int i=0; i<sptArr.length(); i++)
	{
		m_selected.setLogicalLength(0);
		m_selected.append(i);
		ptSelArr.setLogicalLength(0);
		ptSelArr.append(sptArr[i]);
		ptSelArr.append(eptArr[i]);
		SearchbyIndexPoint(i,eptArr[i]);

		if (m_selected.length() != 4)
		{
			sptArr.removeAt(i);
			eptArr.removeAt(i);
			i--;
			continue;
		}

		AcGePoint2dArray pathPoints;
		pathPoints.setLogicalLength(0);

		double dis = sptArr[i].distanceTo(ptSelArr[ptSelArr.length()-1]);
		int nBeishu = ceil(dis/100);//mzg Test 将误差设为100允许误差0.1
		if (nBeishu ==0)
		{
			nBeishu = 1;
		}
		double dWucha =0.0;
		dWucha = nBeishu*DISWUCHA;
		ads_name ename;
		ads_name_clear(ename);
		Acad::ErrorStatus es;
		if( dis<dWucha)//能够围成一个封闭区域					
		{
			pathPoints.append( My3d22d(ptSelArr[0]) );
			for (int k=0; k<m_selected.length()-1; k++)
			{
				AcGeLine3d tmpLine1(sptArr[m_selected[k]],eptArr[m_selected[k]]);
				AcGeLine3d tmpLine2(sptArr[m_selected[k+1]],eptArr[m_selected[k+1]]);
				AcGeTol tol;
				tol.setEqualPoint(0.01);
				AcGePoint3d interpt;
				Adesk::Boolean boo= tmpLine1.intersectWith(tmpLine2,interpt,tol);
				if(boo)
					pathPoints.append( My3d22d(interpt) );
				else
					pathPoints.append( My3d22d(ptSelArr[2*k+1]) );
			}
			AcDbObjectId retId;
			es = Poly(retId,pathPoints);
			if (es==Acad::eOk)
			{
				AcDbEntity* pEnt = NULL;
				AcDbExtents exts;
				if (acdbOpenAcDbEntity(pEnt, retId, AcDb::kForWrite)!=Acad::eOk)
				{
					pEnt->close();
					continue;
				}
				pEnt->getGeomExtents(exts);
				pEnt->erase();
				pEnt->close();

				startPtArr.append(My3d22d(exts.minPoint()));
				endPtArr.append(My3d22d(exts.maxPoint()));
			}
		}

		//将已经找到的从原来的数组中删掉
		Double_Sort(m_selected,TRUE);

		for(int k=0; k<m_selected.length(); k++)
		{
			sptArr.removeAt(m_selected[k]-k);
			eptArr.removeAt(m_selected[k]-k);
		}

		i--;
	}
	return TRUE;
}

BOOL CSearchLines::SearchAll(ads_name& ssname)
{
	acedSSAdd(NULL, NULL, ssname);
	if ( sptArr.length()<1 )
		return FALSE;

	
	for (int i=0; i<sptArr.length(); i++)
	{
		m_selected.setLogicalLength(0);
		m_selected.append(i);
		ptSelArr.setLogicalLength(0);
		ptSelArr.append(sptArr[i]);
		ptSelArr.append(eptArr[i]);
		SearchbyIndexPoint(i,eptArr[i]);

		if (m_selected.length() != 4)
		{
			sptArr.removeAt(i);
			eptArr.removeAt(i);
			//linesArr.removeAt(i);
			i--;
			continue;
		}

		AcGePoint2dArray pathPoints;
		pathPoints.setLogicalLength(0);
		
		//double dis = sptArr[i].distanceTo(ptSelArr[ptSelArr.length()-1]);
		//int nBeishu = ceil(dis/100);//mzg Test 将误差设为100允许误差0.1
		//if (nBeishu ==0)
		//{
		//	nBeishu = 1;
		//}
		//double dWucha =0.0;
		//dWucha = nBeishu*DISWUCHA;
		ads_name ename;
		ads_name_clear(ename);
		Acad::ErrorStatus es;
		//if( dis<dWucha)//能够围成一个封闭区域					
		{
			pathPoints.append( My3d22d(ptSelArr[0]) );
			for (int k=0; k<m_selected.length()-1; k++)
			{
				AcGeLine3d tmpLine1(sptArr[m_selected[k]],eptArr[m_selected[k]]);
				AcGeLine3d tmpLine2(sptArr[m_selected[k+1]],eptArr[m_selected[k+1]]);
				AcGeTol tol;
				tol.setEqualPoint(0.01);
				AcGePoint3d interpt;
				Adesk::Boolean boo= tmpLine1.intersectWith(tmpLine2,interpt,tol);
				if(boo)
					pathPoints.append( My3d22d(interpt) );
				else
					pathPoints.append( My3d22d(ptSelArr[2*k+1]) );
			}
			AcDbObjectId retId;
			es = Poly(retId,pathPoints);
			if (es==Acad::eOk)
			{
				if (acdbGetAdsName(ename, retId)==Acad::eOk)
				{
					int nRet = acedSSAdd(ename, ssname, ssname);
				}
			}
		}

		//将已经找到的从原来的数组中删掉
		Double_Sort(m_selected,TRUE);

		for(int k=0; k<m_selected.length(); k++)
		{
			sptArr.removeAt(m_selected[k]-k);
			eptArr.removeAt(m_selected[k]-k);
		}

		i--;
	}
	long len;
	acedSSLength(ssname, &len);
	return TRUE;
}

