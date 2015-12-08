#pragma once
#include <vector>
using namespace std;
class MyTransFunc
{
public:
	MyTransFunc(void);
	~MyTransFunc(void);

	static AcGePoint3d My2d23d(AcGePoint2d);
	static AcGePoint2d My3d22d(AcGePoint3d);
	static AcGePoint3dArray My2d23d(AcGePoint2dArray);
	static AcGePoint2dArray My3d22d(AcGePoint3dArray);
	static AcGePoint3d MyMidPoint(AcGePoint3d, AcGePoint3d);
	static double MyTransFunc::GetBulgeOf3Pt(AcGePoint3d &startPt //起点
											,AcGePoint3d &interPt //中间一点
											,AcGePoint3d &endPt   //终点
											);
	//将弧度转为角度
	static double CurveToRotation(double dCurve);
	//将角度转为弧度
	static double RotationToCurve(double dRotation);
	//通过传入的两点数据得到四点的点组
	static AcGePoint3dArray OperateTwoPointsAndGetPoints(AcGePoint3d minPt, AcGePoint3d maxPt);

	static AcDbSortentsTable* get_sortents_table_of(AcDbEntity* pEnt);

	static void DuiXiangPaiXu(AcDbObjectId ent_id, bool TopOrDow = true);//对象排序
	//kmp字符串匹配
	//第一个参数为原始数据，第二个数据为部分匹配的字符串
	//返回-1表示没有找到，其它值为找到匹配了
	//static int KMP(CString s,CString t);
	static void get_next(CString t, int next[ ]);
	static int KMP(CString text, CString find); 


};
