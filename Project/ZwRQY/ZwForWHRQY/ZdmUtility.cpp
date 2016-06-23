#include "StdAfx.h"
#include "ZdmUtility.h"
#include "ZDMUtils.h"

CZdmUtility::CZdmUtility(void)
{
}

CZdmUtility::~CZdmUtility(void)
{
}

AcGePoint3d CZdmUtility::getCurdataPt(CZdmDataInfo data)
{
	AcGePoint3d retPt;
	AcGePoint3d basePt = CDMXUtils::getbasePt();
	double dvalue = data.getcurData();
	acutPolar(asDblArray(basePt), 0, 20 + (dvalue - CDMXUtils::startzh())*CDMXUtils::globalXScale(), asDblArray(retPt));
	return retPt;
}

AcGePoint3d CZdmUtility::getRealDmxPt(CZdmDataInfo data)
{
	AcGePoint3d tmpPt;
	double dvalue = data.getRealDmx();
	AcGePoint3d basePt = getCurdataPt(data);
	acutPolar(asDblArray(basePt), PI/2, (dvalue - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale(), asDblArray(tmpPt));
	return tmpPt;
}

AcGePoint3d CZdmUtility::getDesignPt(CZdmDataInfo data)
{
	AcGePoint3d tmpPt;
	double dvalue = data.getDesignDmx();
	AcGePoint3d basePt = getCurdataPt(data);
	acutPolar(asDblArray(basePt), PI/2, (dvalue - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale(), asDblArray(tmpPt));
	return tmpPt;
}

AcGePoint3d CZdmUtility::getGuandiPt(CZdmDataInfo data)
{
	AcGePoint3d tmpPt;
	double dvalue = data.getGuanDi();
	AcGePoint3d basePt = getCurdataPt(data);
	acutPolar(asDblArray(basePt), PI/2, (dvalue - CDMXUtils::getMinElavation())*CDMXUtils::globalYScale(), asDblArray(tmpPt));
	return tmpPt;
}

AcGePoint3d CZdmUtility::getGuanTopPt(CZdmDataInfo data)
{
	AcGePoint3d tmpPt;
	double dvalue = data.getPipeDiameter()/1000;
	AcGePoint3d basePt = getGuandiPt(data);
	acutPolar(asDblArray(basePt), PI/2, dvalue*CDMXUtils::globalYScale(), asDblArray(tmpPt));
	return tmpPt;
}

double CZdmUtility::getLength(CZdmDataInfo data, AcGePoint3d pt)
{
	AcGePoint3d basePt = getCurdataPt(data);
	double dLen = (pt.y - basePt.y)/CDMXUtils::globalYScale();
	double dRet = dLen + CDMXUtils::getMinElavation();
	return dRet;
}

