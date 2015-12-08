#pragma once
typedef CArray<double, double> CDoubleArray;
typedef CArray<int, int> CIntArray;

class CCutEntity
{
public:
	CCutEntity(void);
	~CCutEntity(void);
	
	void doIt();
	bool AddRegion();
	void CloneAndMoveEntity(AcDbObjectIdArray idArrs, AcGeMatrix3d m);
	void EraseEntity(AcDbObjectIdArray idArrs);
	void move();
	void OperateLine(AcDbPolyline* pPolyline, AcDbLine* pLine);
	void OperateArc(AcDbPolyline* pPolyline, AcDbArc* pArc);

	void OperateCircle(AcDbPolyline* pPolyline, AcDbCircle* pCircle);

	void OperateEllipse(AcDbPolyline* pPolyline, AcDbEllipse* pEllipse);
	
	void OperateSpline(AcDbPolyline* pPolyline, AcDbSpline* pSpline);

	void OperatePolyline(AcDbPolyline* pPolyline, AcDbPolyline* pPoly);

	void Operate2dPolyline(AcDbPolyline* pPolyline, AcDb2dPolyline* pPoly);

	void Operate3dPolyline(AcDbPolyline* pPolyline, AcDb3dPolyline* pPoly);

	Acad::ErrorStatus ExplodeEntity(AcDbEntity* pEnt, AcDbVoidPtrArray& entitySet);

	void OperateExplodeEntity(AcDbPolyline* pPolyline,AcDbVoidPtrArray entitySet);
	//设置属性到新建的实体
	AcDbObjectId SetPropertiesFromEnt(AcDbEntity* pEnt, AcDbObjectId entId);
	//对点组以基点为基准按弧度从小到大排序
	void sortPtArray(AcGePoint3d basePt, AcGePoint3dArray& ptArr);

	void sortPtArrayByCurve(AcDbCurve* pCurve, AcGePoint3dArray& ptArr);
	AcGePoint3d m_minPt;
	AcGePoint3d m_maxPt;
	AcDbObjectIdArray m_IdArrs;
	AcDbObjectIdArray m_tmpIdArrs;//临时实体id集
};
