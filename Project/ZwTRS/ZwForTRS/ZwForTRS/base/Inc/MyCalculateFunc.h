#pragma once

class MyCalculateFunc
{
public:
	MyCalculateFunc(void);
	~MyCalculateFunc(void);
	//判定一个点在圆内
	static BOOL JudgePointInCircle(AcGePoint3d basePt, AcGePoint3d centerPt, double dRadius);

};
