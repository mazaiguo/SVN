#pragma once
#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

class SERVERDLL_API MyDrawEntity
{
public:
	MyDrawEntity(void);
	~MyDrawEntity(void);
	static AcDbObjectId DrawLine(AcGePoint3d ptStart, AcGePoint3d ptEnd, AcDbObjectId LayerId = AcDbObjectId::kNull);
	static AcDbObjectId DrawText(AcGePoint3d basePt, 
									CString strText, 
									double dHeight = 3.0,
									AcDbObjectId textStyleId = AcDbObjectId::kNull,
									AcDb::TextHorzMode HorMode = AcDb::kTextLeft, 
									AcDb::TextVertMode VerMode = AcDb::kTextBase);
	//���ݸ����Ļ��㣬�������ݺ�������ʽ�����ָ���ģʽ���ƶ�������
	static AcDbObjectId DrawMText(AcGePoint3d basePt, CString strText, double dHeight = 3.0, 
									AcDbObjectId textStyleId = AcDbObjectId::kNull, 
									AcDbMText::AttachmentPoint attachMode = AcDbMText::kTopLeft);
	//���Ƹ��ǵĶ�������
	static AcDbObjectId DrawMTextCover(AcGePoint3d basePt, CString strText, double dHeight = 3.0, 
										AcDbObjectId textStyleId = AcDbObjectId::kNull, 
										AcDbMText::AttachmentPoint attachMode = AcDbMText::kTopLeft);
	//static void DrawPolyline(AcGePoint3dArray ptArr);
	/*
		�������������ѡ��objIds��������Щʵ����ӵ�һ��������
		�������objectId
	*/
	static AcDbObjectId	MakeGroup(AcDbObjectIdArray& objIds, bool bIsSelectable = false, CString strGroupName = _T(""));
	//����Բ
	static AcDbObjectId DrawCircle(AcGePoint3d cenPt, double dRadius, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//����Բ��,�����������Բ��
	static AcDbObjectId DrawSemiCircle(AcGePoint3d cenPt, double dRadius, double dRotate, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//ͨ��Բ�ģ��뾶���������Բ��
	static AcDbObjectId DrawArc(AcGePoint3d cenPt, double dRadius, AcGePoint3d startPt, AcGePoint3d endPt, 
									AcDbObjectId LayerId = AcDbObjectId::kNull);
	//ͨ��Բ�ģ��뾶����ʼ���Ⱥ���ֹ���Ȼ���Բ��
	static AcDbObjectId DrawArc(AcGePoint3d cenPt, double dRadius, double startAng, double endAng, 
									int nColor = 5, AcDbObjectId LayerId = AcDbObjectId::kNull);

	static AcDbObjectId DrawEllipse(AcGePoint3d cenPt, AcGeVector3d unitVec, AcGeVector3d majorAxis, double dRadio);
	//���ݸ������ļ�·���Ͳ��������
	static AcDbObjectId InsertBlkRef(CString strFilePathName, AcGePoint3d ptInsert, double inputscale =1 ,
										double inputrotation =0.0 , AcDbObjectId LayerId = AcDbObjectId::kNull);
	//���ݸ������ļ�·���Ͳ����õ������id
	static AcDbObjectId GetBlkRef(CString strFilePathName);
	//����������ƶ����
	static AcDbObjectId DrawPlineByTwoPoint(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId LayerId = AcDbObjectId::kNull);
	//���ݵ�����ƶ����
	static AcDbObjectId DrawPlineByPoints(AcGePoint3dArray points, AcDbObjectId LayerId = AcDbObjectId::kNull);

};
