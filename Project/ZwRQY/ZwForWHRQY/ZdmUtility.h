#pragma once
#include "ZdmDataInfo.h"
//�ݶ���ʵ�ú���
class CZdmUtility
{
public:
	CZdmUtility(void);
	~CZdmUtility(void);

	//��ȡ��ǰ׮�����ڵ�
	AcGePoint3d getCurdataPt(CZdmDataInfo data); 
	AcGePoint3d getRealDmxPt(CZdmDataInfo data);
	AcGePoint3d getDesignPt(CZdmDataInfo data);
	AcGePoint3d getGuandiPt(CZdmDataInfo data);
	AcGePoint3d getGuanTopPt(CZdmDataInfo data);

	double getLength(CZdmDataInfo data, AcGePoint3d pt);
};
