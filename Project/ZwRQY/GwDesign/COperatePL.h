#pragma once
#include <vector>
#include <map>
#include <set>
using namespace std;
class  PlineInfo
{
public:
	PlineInfo()
	{
		//nIndex = 0;
		insertPt.set(0, 0, 0);
		dBulge = 0;
		dStartWidth = 0;
		dEndWidth = 0;
		bIsClosed = false;
	};
	~PlineInfo()
	{

	};
	PlineInfo& operator =(const PlineInfo& info)
	{
		//nIndex = info.nIndex;
		insertPt = info.insertPt;
		dBulge = info.dBulge;
		dStartWidth = info.dStartWidth;
		dEndWidth = info.dEndWidth;
		bIsClosed = info.bIsClosed;
		return *this;
	};

public:
	//int nIndex;
	AcGePoint3d insertPt;
	double dBulge;
	double dStartWidth;
	double dEndWidth;
	bool bIsClosed;
};

class COperatePL
{
public:
	COperatePL(void);
	~COperatePL(void);

	//************************************
	// Method:    JoinTwoPline
	// FullName:  CDrawUtility::JoinTwoPline
	// Access:    public 
	// Returns:   vector<PlineInfo>
	// Qualifier: 将两条多段线合并
	// Parameter: AcDbObjectId preId
	// Parameter: AcDbObjectId plineId
	//************************************
	vector<PlineInfo> JoinTwoPline(AcDbObjectId preId, AcDbObjectId plineId);
	//************************************
	// Method:    getdatabyObjId
	// FullName:  CDrawUtility::getdatabyObjId
	// Access:    public 
	// Returns:   vector<PlineInfo>
	// Qualifier: 得到多段线的数据
	// Parameter: AcDbObjectId plineId
	//************************************
	vector<PlineInfo> getdatabyObjId(AcDbObjectId plineId);

	set<CString> getMapInfoById(AcDbObjectId plineId);
	//************************************
	// Method:    drawPlineBydata
	// FullName:  CDrawUtility::drawPlineBydata
	// Access:    public 
	// Returns:   AcDbObjectId
	// Qualifier:通过数据将多段线绘制出来
	// Parameter: vector<PlineInfo> data
	//************************************
	AcDbObjectId drawPlineBydata(vector<PlineInfo> data);

	//************************************
	// Method:    getLength
	// FullName:  COperatePL::getLength
	// Access:    public 
	// Returns:   double
	// Qualifier:得到该多段线的长度
	// Parameter: AcDbObjectId plineId
	//************************************
	double getLength(AcDbObjectId plineId);
	//trim polyline
	AcDbObjectId trimbycircle(AcDbObjectId plineId);
	AcDbObjectId trimendBycircle(AcDbObjectId plineId);
	AcDbObjectId trimstartBycircle(AcDbObjectId plineId);

	//////////////////////////////////////////////////////////////////////////
	//还原trim的多段线
	AcDbObjectId restorePolyline(AcDbObjectId plineId, AcGePoint3d pt);

	//////////////////////////////////////////////////////////////////////////
	//************************************
	// Method:    CompareTwoPline
	// FullName:  COperatePL::CompareTwoPline
	// Access:    public 
	// Returns:   int
	// Qualifier: 返回0表示不重合，返回1表示前面的包含后面的，返回-1表示后面的包含前面的
	// Parameter: AcDbObjectId preId
	// Parameter: AcDbObjectId curId
	//************************************
	bool CompareTwoPline(AcDbObjectId preId, AcDbObjectId curId);
	//判定两条多段线重合
	bool JudgeTheSame(AcDbObjectId preId, AcDbObjectId curId);

	bool HalfPlByPt(AcDbObjectId curId, AcGePoint3d pt);

	AcGePoint3d startPt(AcDbObjectId curId);
	AcGePoint3d endPt(AcDbObjectId curId);
};
