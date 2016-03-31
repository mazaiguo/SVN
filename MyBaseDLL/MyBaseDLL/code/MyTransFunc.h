#pragma once
#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

#include <vector>
using namespace std;
class SERVERDLL_API MyTransFunc 
{
public:
	MyTransFunc(void);
	~MyTransFunc(void);

	static AcGePoint3d My2d23d(AcGePoint2d);
	static AcGePoint2d My3d22d(AcGePoint3d);
	static AcGePoint3dArray My2d23d(AcGePoint2dArray);
	static AcGePoint2dArray My3d22d(AcGePoint3dArray);
	static AcGePoint3d MyMidPoint(AcGePoint3d, AcGePoint3d);
	static double MyTransFunc::GetBulgeOf3Pt(AcGePoint3d &startPt //���
											,AcGePoint3d &interPt //�м�һ��
											,AcGePoint3d &endPt   //�յ�
											);
	//������תΪ�Ƕ�
	static double CurveToRotation(double dCurve);
	//���Ƕ�תΪ����
	static double RotationToCurve(double dRotation);
	//ͨ��������������ݵõ��ĵ�ĵ���
	static AcGePoint3dArray OperateTwoPointsAndGetPoints(AcGePoint3d minPt, AcGePoint3d maxPt);
	//
	//��strתΪint
	static int StringToInt(CString);
	//��strתΪdouble
	static double StringToDouble(CString);

	static LPCTSTR      ptToStr(const AcGePoint3d& pt, CString& str, int unit = -1, int prec = -1);
	static LPCTSTR      ptToStr(const AcGePoint2d& pt, CString& str, int unit = -1, int prec = -1);
	static LPCTSTR      vectorToStr(const AcGeVector3d& vec, CString& str, int unit = -1, int prec = -1);
	static LPCTSTR      vectorToStr(const AcGeVector2d& vec, CString& str, int unit = -1, int prec = -1);
	static LPCTSTR      angleToStr(const double angle, CString& str, int unit = -1, int prec = -1);
	static LPCTSTR      doubleToStr(const double realVal, CString& str, int unit = -1, int prec = -1);
	static LPCTSTR      intToStr(const int intVal, CString& str);
	static LPCTSTR      intDbIdToStr(const Adesk::IntDbId intVal, CString& str);
	static LPCTSTR      ptrToStr(void* ptr, CString& str);
	static LPCTSTR		dbToStr(AcDbDatabase* db, CString& str);
	static LPCTSTR      booleanToStr(bool b, CString& str);
	static LPCTSTR      booleanToStr(Adesk::Boolean b, CString& str);
	static LPCTSTR      colorToStr(int color, CString& str, bool convertStandard = true);
	static LPCTSTR      objToHandleStr(const AcDbObject* obj, CString& str);
	static LPCTSTR      objToEnameStr(const AcDbObject* obj, CString& str);
	static LPCTSTR      objToClassStr(const AcRxObject* obj);
	static LPCTSTR		objToClassAndHandleStr(const AcDbObjectId& objId, CString& str);
	static LPCTSTR		objToClassAndHandleStr(const AcDbObject* obj, CString& str);
	static LPCTSTR		textModeToStr(AcDb::TextHorzMode horizMode, CString& str);
	static LPCTSTR		textModeToStr(AcDb::TextVertMode vertMode, CString& str);
	static LPCTSTR		vertexTypeToStr(AcDb::Vertex3dType vertexType, CString& str);
	static LPCTSTR		hatchTypeToStr(AcDbHatch::HatchPatternType hatchType, CString& str);
	static LPCTSTR		hatchStyleToStr(AcDbHatch::HatchStyle style, CString& str);
	static LPCTSTR		hatchLoopTypeToStr(Adesk::Int32 loopType, CString& str);
	static LPCTSTR		mtextAttachmentTypeToStr(AcDbMText::AttachmentPoint type, CString& str);
	static LPCTSTR		mtextFlowDirTypeToStr(AcDbMText::FlowDirection type, CString& str);
	static LPCTSTR		lineSpacingTypeToStr(AcDb::LineSpacingStyle type, CString& str);
	static LPCTSTR		proxyMetafileTypeToStr(AcDbProxyEntity::GraphicsMetafileType type, CString& str);
	static LPCTSTR		lineWeightTypeToStr(AcDb::LineWeight type, CString& str);
	static LPCTSTR		mergeStyleToStr(AcDb::DuplicateRecordCloning type, CString& str);
	static LPCTSTR		standardScaleTypeToStr(AcDbViewport::StandardScaleType type, CString& str);
	static LPCTSTR		dwgVersionToStr(AcDb::AcDbDwgVersion type, CString& str);
	static LPCTSTR		orthoViewToStr(AcDb::OrthographicView type, CString& str);
	static LPCTSTR		renderModeToStr(AcDbAbstractViewTableRecord::RenderMode type, CString& str);
	static LPCTSTR		measurementTypeToStr(AcDb::MeasurementValue type, CString& str);
	static LPCTSTR		unitTypeToStr(AcDb::UnitsValue type, CString& str);
	static LPCTSTR		xrefStatusToStr(AcDb::XrefStatus stat, CString& str);
	static LPCTSTR		saveFormatToStr(AcApDocument::SaveFormat type, CString& str);
	static LPCTSTR		longTransactionTypeToStr(int type, CString& str);
	static LPCTSTR		docLockModeToStr(AcAp::DocLockMode activity, CString& str);
	static LPCTSTR		dateToStr(const AcDbDate& date, CString& str);
	static LPCTSTR		promptStatusToStr(Acad::PromptStatus stat, CString& str);
	static LPCTSTR		initFlagsToStr(int initFlags, CString& str);
	static LPCTSTR		deepCloneContextToStr(AcDb::DeepCloneType context, CString& str);
	static LPCTSTR		filerTypeToStr(AcDb::FilerType type, CString& str);
	static LPCTSTR		osnapToStr(int osnapMode, CString& str);
	static LPCTSTR		doubleArray3dToStr(double array[], CString& str);
	static LPCTSTR		doubleArray2dToStr(double array[], CString& str);


	static AcDbSortentsTable* get_sortents_table_of(AcDbEntity* pEnt);

	static void DuiXiangPaiXu(AcDbObjectId ent_id, bool TopOrDow = true);//��������
	//kmp�ַ���ƥ��
	//��һ������Ϊԭʼ���ݣ��ڶ�������Ϊ����ƥ����ַ���
	//����-1��ʾû���ҵ�������ֵΪ�ҵ�ƥ����
	//static int KMP(CString s,CString t);
	static void get_next(CString t, int next[ ]);
	static int KMP(CString text, CString find); 


};
