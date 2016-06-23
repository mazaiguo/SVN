#pragma once
#include "ZdmDataInfo.h"
//纵断面实用函数
class CZdmUtility
{
public:
	CZdmUtility(void);
	~CZdmUtility(void);

	//获取当前桩号所在点
	AcGePoint3d getCurdataPt(CZdmDataInfo data); 
	AcGePoint3d getRealDmxPt(CZdmDataInfo data);
	AcGePoint3d getDesignPt(CZdmDataInfo data);
	AcGePoint3d getGuandiPt(CZdmDataInfo data);
	AcGePoint3d getGuanTopPt(CZdmDataInfo data);

	double getLength(CZdmDataInfo data, AcGePoint3d pt);
};
