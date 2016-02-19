// SearchLines.h: interface for the CSearchLines class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHLINES_H__BCF45F8D_2590_44F7_AAC9_0B95A1035C67__INCLUDED_)
#define AFX_SEARCHLINES_H__BCF45F8D_2590_44F7_AAC9_0B95A1035C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define DISWUCHA 0.1			//�������
#define EPSINON 0.001
#define My3d22d(pt) AcGePoint2d(pt.x,pt.y)
#define My2d23d(pt) AcGePoint3d(pt.x,pt.y,0.0)
class CSearchLines  
{
public:
	CSearchLines();
	virtual ~CSearchLines();

	BOOL ConstructLines(ads_name &ssname);
	int FindIndexAtPoint(AcGePoint3d endpt,int nOld);		//�Ƿ���
	BOOL SearchbyIndexPoint(int nCurr,AcGePoint3d point);
	BOOL SearchAll(ads_name& ssname);

	BOOL SearchAll(AcGePoint2dArray&, AcGePoint2dArray&);
	BOOL ConstructLines(AcGePoint2dArray, AcGePoint2dArray);

	//AcDbObjectIdArray linesArr;
	AcGePoint3dArray sptArr;
	AcGePoint3dArray eptArr;

	AcGeIntArray m_selected;		//ѡ�е���������
	AcGePoint3dArray ptSelArr;		//ѡ�е������ߵ�˳���

};

#endif // !defined(AFX_SEARCHLINES_H__BCF45F8D_2590_44F7_AAC9_0B95A1035C67__INCLUDED_)
