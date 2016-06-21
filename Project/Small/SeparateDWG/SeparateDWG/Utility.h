#pragma once

class Utility
{
public:
	Utility(void);
	virtual ~Utility(void);
public:
	Acad::ErrorStatus AddToModelSpace(AcDbEntity* pEnt, AcDbObjectId& retId);
	AcDbObjectId CreateNewLayer(CString strLayer, int nColor);
	AcDbObjectId CreateText(CString strText, double dScale, AcGePoint3d ptInsert);
	AcDbObjectId CreateText(CString strText, double dScale, AcGePoint2d ptInsert);
	AcDbObjectId CreateLine(AcGePoint3d ptStart, AcGePoint3d ptEnd);
	//AcDbObjectId CreateLine(AcGePoint2d ptStart, AcGePoint2d ptEnd);
	Acad::ErrorStatus ExplodeEntity(AcDbEntity* pEnt, AcDbVoidPtrArray& entitySet);
	bool PointInRect(AcGePoint2d point, AcGePoint2d minPoint, AcGePoint2d maxPoint);
	//mzg 
	double CalculateWidth(AcGePoint3d minPt, AcGePoint3d maxPt);//·µ»Øwidth
	double CalculateLength(AcGePoint3d minPt, AcGePoint3d maxPt);//·µ»Ølength

	BOOL GetVar(LPCTSTR var, int *nRes);
	BOOL GetVar(LPCTSTR var, ads_real *rRes);
	BOOL GetVar(LPCTSTR var, ads_point *pRes);
	BOOL GetVar(LPCTSTR var, LPTSTR sRes);
	BOOL SetVar(LPCTSTR szVarName, int       nArg);
	BOOL SetVar(LPCTSTR szVarName, ads_real  rArg);
	BOOL SetVar(LPCTSTR szVarName, ads_point pArg);
	BOOL SetVar(LPCTSTR szVarName, LPCTSTR   sArg);
	void GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr,
								LPCTSTR strTypeName, bool bIsFullPath = true);
	bool getDocFromFilename(CString csFileName, AcApDocument* &pNewDocument);


};

