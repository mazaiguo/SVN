#pragma once
#include "SerialNo.h"
//-----------------------------------------------------------------------------
#ifdef MYBASEDLL_EXPORTS //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

class SERVERDLL_API CSerialNoJig : public AcEdJig
{
private:
	//- Member variables
	//- current input level, increment for each input
	int mCurrentInputLevel ;
	//- Dynamic dimension info
	AcDbDimDataPtrArray mDimData ;
	AcGePoint3d m_basePt;

public:
	//- Array of input points, each level corresponds to the mCurrentInputLevel
	AcGePoint3dArray mInputPoints ;
	//- Entity being jigged
	//AcEdJig *mpEntity ;
	CSerialNo *mpEntity;
	//设置serial的属性
	//void setBasePt(AcGePoint3d basePt);
	void setRadius(double dRadius);
	void setTextHeight(double dTextHeight);
	void setTextId(AcDbObjectId textId);
	void setLayerId(AcDbObjectId layerId);
	void setstrText(CString strText);
public:
	CSerialNoJig () ;
	~CSerialNoJig () ;

	//- Command invoke the jig, call passing a new'd instance of the object to jig
	AcEdJig::DragStatus startJig () ;

protected:
	//- AcEdJig overrides
	//- input sampler
	virtual DragStatus sampler () ;
	//- jigged entity update
	virtual Adesk::Boolean update () ;
	//- jigged entity pointer return
	virtual AcDbEntity *entity () const ;
	//- dynamic dimension data setup
	virtual AcDbDimDataPtrArray *dimData (const double dimScale) ;
	//- dynamic dimension data update
	virtual Acad::ErrorStatus setDimValue (const AcDbDimData *pDimData, const double dimValue) ;

	//- Standard helper functions
	//- dynamic dimdata update function
	virtual Adesk::Boolean updateDimData () ;

	//- Std input to get a point with no rubber band
	AcEdJig::DragStatus GetStartPoint () ;
	//- Std input to get a point with rubber band from point
	AcEdJig::DragStatus GetNextPoint () ;
} ;
