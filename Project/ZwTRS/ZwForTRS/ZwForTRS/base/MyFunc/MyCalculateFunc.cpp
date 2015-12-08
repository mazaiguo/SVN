#include "StdAfx.h"
#include "MyCalculateFunc.h"

MyCalculateFunc::MyCalculateFunc(void)
{
}

MyCalculateFunc::~MyCalculateFunc(void)
{
}

BOOL MyCalculateFunc::JudgePointInCircle(AcGePoint3d basePt, AcGePoint3d centerPt, double dRadius)
{
	double dDist = acutDistance(asDblArray(basePt), asDblArray(centerPt));
	if (dDist - dRadius < 0.01)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}