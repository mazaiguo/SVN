#include "StdAfx.h"
#include "MyTransFunc.h"

MyTransFunc::MyTransFunc(void)
{
}

MyTransFunc::~MyTransFunc(void)
{
}


AcGePoint3d MyTransFunc::My2d23d(AcGePoint2d pt2d)
{
	AcGePoint3d pt;
	pt.x = pt2d.x;
	pt.y = pt2d.y;
	pt.z = 0;
	return pt;
}

AcGePoint2d MyTransFunc::My3d22d(AcGePoint3d pt3d)
{
	AcGePoint2d pt;
	pt.x = pt3d.x;
	pt.y = pt3d.y;
	return pt;
}

AcGePoint3dArray MyTransFunc::My2d23d(AcGePoint2dArray pt2dArr)
{
	AcGePoint3dArray ptArr;
	AcGePoint3d tmpPt;
	for (int i=0; i< pt2dArr.length(); i++)
	{
		tmpPt.x = pt2dArr.at(i).x;
		tmpPt.y = pt2dArr.at(i).y;
		tmpPt.z = 0;
		ptArr.append(tmpPt);
	}
	return ptArr;
}

AcGePoint2dArray MyTransFunc::My3d22d(AcGePoint3dArray pt3dArr)
{
	AcGePoint2dArray ptArr;
	AcGePoint2d tmpPt;
	for (int i=0; i< pt3dArr.length(); i++)
	{
		tmpPt.x = pt3dArr.at(i).x;
		tmpPt.y = pt3dArr.at(i).y;
		ptArr.append(tmpPt);
	}
	return ptArr;
}


AcGePoint3d MyTransFunc::MyMidPoint(AcGePoint3d ptStart, AcGePoint3d ptEnd)
{
	AcGePoint3d midPt;
	midPt.x = (ptStart.x + ptEnd.x)/2;
	midPt.y = (ptStart.y + ptEnd.y)/2;
	midPt.z = (ptStart.z + ptEnd.z)/2;
	return midPt;
}

//凸度计算函数
double MyTransFunc::GetBulgeOf3Pt(AcGePoint3d &startPt                        //起点
								 ,AcGePoint3d &interPt                //中间一点
								 ,AcGePoint3d &endPt                        //终点
							   )
{
	double dTmp;
	AcGeCircArc3d aa(startPt, interPt, endPt);
	dTmp = (startPt.x - endPt.x) * (interPt.y - endPt.y) - (interPt.x - endPt.x) * (startPt.y - endPt.y);
	acutPrintf(_T("\n起始弧度为%f, 终止弧度为%f"), aa.startAng(), aa.endAng());
	//圆弧逆时针方向
	if (dTmp > 0)        {
		return tan((aa.endAng() - aa.startAng()) * 0.25);
	} 
	//圆弧顺时针方向
	else if (dTmp < 0)        {
		return -tan((aa.endAng() - aa.startAng()) * 0.25);
	}
	return 0;
}

double MyTransFunc::CurveToRotation(double dCurve)
{
	double dRotation;
	dRotation = 180*dCurve/PI;
	return dRotation;
}

double MyTransFunc::RotationToCurve(double dRotation)
{
	double dCurve;
	dCurve = PI*dRotation/180;
	return dCurve;
}

AcGePoint3dArray MyTransFunc::OperateTwoPointsAndGetPoints(AcGePoint3d minPt, AcGePoint3d maxPt)
{
	AcGePoint3dArray points;
	points.removeAll();
	AcGePoint3d tmpPt1, tmpPt2;
	tmpPt1.x = minPt.x;
	tmpPt1.y = maxPt.y;
	tmpPt1.z = minPt.z;

	tmpPt2.x = maxPt.x;
	tmpPt2.y = minPt.y;
	tmpPt2.z = maxPt.z;

	points.append(minPt);
	points.append(tmpPt1);
	points.append(maxPt);
	points.append(tmpPt2);
	points.append(minPt);
	return points;
}

AcDbSortentsTable* MyTransFunc::get_sortents_table_of(AcDbEntity* pEnt)
{
	AcDbObjectId owner_id = pEnt->ownerId();
	if (AcDbObjectId::kNull == owner_id)
		return NULL;
	AcDbBlockTableRecord *pRec = NULL;
	if (Acad::eOk != acdbOpenObject( pRec, owner_id, AcDb::kForWrite ))
		return NULL;
	AcDbObjectId ext_id = pRec->extensionDictionary();
	if (AcDbObjectId::kNull == ext_id)
	{
		pRec->createExtensionDictionary();
		ext_id = pRec->extensionDictionary();
		if (AcDbObjectId::kNull == ext_id)
		{
			pRec->close();
			return NULL;
		}
	}
	pRec->close();
	AcDbDictionary *pExt = NULL;
	Acad::ErrorStatus es = acdbOpenObject( pExt, ext_id, AcDb::kForWrite );
	if (Acad::eOk != es)
		return NULL;
	AcDbObject *pObj;
	if (Acad::eOk != pExt->getAt( _T("ACAD_SORTENTS"), pObj, AcDb::kForWrite ))
	{
		AcDbSortentsTable *pSt = new AcDbSortentsTable;
		if (NULL == pSt)
		{
			pExt->close();
			return NULL;
		}

		AcDbObjectId new_id;
		if (Acad::eOk != pExt->setAt( _T("ACAD_SORTENTS"), pSt, new_id ))
		{
			delete pSt;
			pExt->close();
			return NULL;
		}
		pSt->setBlockId( owner_id );
		pObj = pSt;
	}
	pExt->close();

	if (!pObj->isKindOf( AcDbSortentsTable::desc() ))
	{
		pObj->close();
		return NULL;
	}
	return (AcDbSortentsTable*)pObj;
}

void MyTransFunc::DuiXiangPaiXu(AcDbObjectId ent_id, bool TopOrDow/*=TRUE*/)//对象排序
{	
	AcDbEntity* pEnt = NULL;
	if (acdbOpenAcDbEntity(pEnt, ent_id, AcDb::kForWrite) != Acad::eOk)
	{
		return;
	}
	AcDbSortentsTable *pSt = get_sortents_table_of( pEnt );
	pEnt->close();
	if (NULL == pSt)
		return;	
	AcDbObjectIdArray entity_array;
	entity_array.append(ent_id);
	Acad::ErrorStatus es;
	if (TopOrDow==TRUE)
		es = pSt->moveToTop(entity_array);//移到最前面
	if (TopOrDow==FALSE)
		es = pSt->moveToBottom(entity_array);//移到底部
	pSt->close();
}

void MyTransFunc::get_next(CString t, int next[ ])
{
 int t_len=t.GetLength();
 int i=0;         //求解每个next[i]
 next[0]=-1; //递推基本条件,然后求解next[i+1]
 int j=-1;     //向后递推位置下标
 /*
 next[i]=k =>T0...Tk-1=Ti-k...Ti-1
    求解next[i+1]
 1> 如果T0..Tk-1Tk=Ti-k...Ti-1Ti=>next[i+1]=k+1=next[i]+1;
 2>Tk<>Ti,next[k]=k', 如果Ti=Tk'=>next[i+1]=k'+1=next[k]+1=next[next[i]]+1;
 3>依次递推 最后情况next[i+1]=next[0]+1=0,即
 */
 while(i<t_len)
 {
   if(j==-1 ||t[i]==t[j])  //j==-1证明已经与t[0]不匹配了，此时next[i+1]=0
   {
    i++;
    j++;
    next[i]=j;
   }
   else
   {
       j=next[j]; 
   }
 }
}

//int MyTransFunc::KMP(CString s,CString t)
//{
//	int s_len=s.GetLength();
//	int t_len=t.GetLength();
//	int i=0;
//	int j=0;
//	int* next=new int[t_len];
//	get_next(t,next);
//	if(t_len>s_len) 
//	{
//		delete []next;
//		return -1;
//	}
//	while(i<s_len&&j<t_len)
//	{
//		if(j==-1||s[i]==t[j])
//		{
//			i++;
//			j++;
//		}
//		else
//		{
//			j=next[j];
//		}
//	}//end while
//	delete []next;
//	if(j>=t_len)
//	{	
//		//delete []next;
//		return i-j;
//	}
//	else
//	{
//		//delete []next;
//		return -1;
//	}
//}
void NEXT(CString&T,vector<int>&next)
{
	//按模式串生成vector,next(T.size())
	next[0]=-1;
	for(int i=1;i<T.GetLength();i++){
		int j=next[i-1];
		while(T[i]!=T[j+1]&&j>=0)
			j=next[j];//递推计算
		if(T[i]==T[j+1])
			next[i]=j+1;
		else 
			next[i]=0;//
	}
}
int MyTransFunc::KMP(CString S, CString T)  
{  
	//if (text.IsEmpty() || find.IsEmpty())
	//{
	//	return -1;
	//}
	//int find_len = find.GetLength();  
	//int text_len = text.GetLength();  
	//if (text_len < find_len)  
	//	return -1;  
	//int map[find_len];  
	//memset(map, 0, find_len*sizeof(int));  
	////initial the kmp base array: map  
	//map[0] = 0;  
	//map[1] = 0;  
	//int i = 2;  
	//int j = 0;  
	//for (i=2; i<find_len; i++)  
	//{  
	//	while (1)  
	//	{  
	//		if (find[i-1] == find[j])  
	//		{  
	//			j++;  
	//			if (find[i] == find[j])  
	//			{  
	//				map[i] = map[j];  
	//			}  
	//			else  
	//			{  
	//				map[i] = j;  
	//			}  
	//			break;  
	//		}  
	//		else  
	//		{  
	//			if (j == 0)  
	//			{  
	//				map[i] = 0;  
	//				break;  
	//			}  
	//			j = map[j];  
	//		}  
	//	}  
	//}  
	//i = 0;  
	//j = 0;  
	//for (i=0; i<text_len;)  
	//{  
	//	if (text[i] == find[j])  
	//	{  
	//		i++;  
	//		j++;  
	//	}  
	//	else  
	//	{  
	//		j = map[j];  
	//		if (j == 0)  
	//			i++;  
	//	}  
	//	if (j == (find_len))  
	//		return i-j;  
	//}  
	//return -1; 
	//利用模式串T的next函数求T在主串S中的个数count的KMP算法
	//其中T非空，
	S = S.MakeUpper();
	T = T.MakeUpper();
	vector<int> next(T.GetLength());
	if (next.size() < 1)
	{
		return 0;
	}
	NEXT(T,next);
	int index =0;
	int count =-1;
	for(index=0;index<S.GetLength();++index)
	{
		int pos=0;
		int iter=index;
		while(pos<T.GetLength()&&iter<S.GetLength())
		{
			if(S[iter]==T[pos]){
				++iter;++pos;
			}
			else
			{
				if(pos==0)
					++iter;
				else 
					pos=next[pos-1]+1;
			}
		}//whileend
		if(pos==T.GetLength()&&(iter-index)==T.GetLength())
			++count;
	}//forend
	return count;
}  

// int COUNT_KMP(CString&S, CString&T)
// {
// 	//利用模式串T的next函数求T在主串S中的个数count的KMP算法
// 	//其中T非空，
// 	vector<int>next(T.GetLength());
// 	NEXT(T,next);
// 	int index,count=0;
// 	for(index=0;index<S.GetLength();++index)
// 	{
// 		int pos=0;
// 		int iter=index;
// 		while(pos<T.GetLength()&&iter<S.GetLength())
// 		{
// 			if(S[iter]==T[pos]){
// 				++iter;++pos;
// 			}
// 			else
// 			{
// 				if(pos==0)
// 					++iter;
// 				else 
// 					pos=next[pos-1]+1;
// 			}
// 		}//whileend
// 		if(pos==T.GetLength()&&(iter-index)==T.GetLength())++count;
// 	}//forend
// 	return count;
// }