#pragma once

#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

#define PI 3.1415926535897932384626433832795

class SERVERDLL_API MyBaseUtils
{
public:
	MyBaseUtils(void);
	~MyBaseUtils(void);

	enum Color {
		kByblock    = 0,
		kRed        = 1,
		kYellow     = 2,
		kGreen      = 3,
		kCyan       = 4,
		kBlue       = 5,
		kMagenta    = 6,
		kWhite      = 7,
		kBylayer    = 256
	};

	enum LinetypeFile {
		kStandardLinetypes,
		kComplexLinetypes,
		kAllLinetypes
	};

	enum LinearUnit {
		kCurrentLU      = -1,
		kScientific     = 1,
		kDecimal        = 2,
		kEngineering    = 3,
		kArchitectural  = 4,
		kFractional     = 5
	};

	enum AngularUnit {
		kCurrentAU      = -1,
		kDegrees        = 0,
		kDegMinSec      = 1,
		kGrads          = 2,
		kRadians        = 3,
		kSurveyor       = 4
	};

	enum Precision {
		kCurrentPrecLU   = -1,
		kCurrentPrecAU   = -1
	};


	static const TCHAR*	rxErrorStr(Acad::ErrorStatus msgId);

	static void rxErrorMsg(Acad::ErrorStatus msgId);

	/////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ACAD SYMBOL TABLE UTILITIES //////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	//@param symbolIdΪ���ű�ID
	//@param ����symbol name
	static Acad::ErrorStatus    symbolIdToName(const AcDbObjectId& symbolId, CString& name);
	//@param mlineStyleIdΪ���ߵ�id
	//@param nameΪ���ֵ��Ϊ������ʽ��
	static Acad::ErrorStatus    mlineStyleIdToName(const AcDbObjectId& mlineStyleId, CString& name);
	static Acad::ErrorStatus    nameToSymbolId(AcRxClass* symTblClass, LPCTSTR name,
		AcDbObjectId& symbolId, AcDbDatabase* db);

	static bool                 symbolExists(AcRxClass* symTblClass, LPCTSTR name, AcDbDatabase* db);
	static AcDbSymbolTable*     openSymbolTable(AcRxClass* classType, AcDb::OpenMode mode, AcDbDatabase* db);
	static Acad::ErrorStatus    collectSymbolIds(AcRxClass* symTblClass, AcDbObjectIdArray& objIds, AcDbDatabase* db);
	//static Acad::ErrorStatus	collectBlockIds(SdStrObjIdList& list, bool excludeMsPs,
	//	bool excludeXref,
	//	bool excludeAnonymous, AcDbDatabase* db);
	
	static Acad::ErrorStatus    addNewSymbolAndClose(AcDbSymbolTable* symTbl, AcDbSymbolTableRecord* newRecord);

	static Acad::ErrorStatus    addNewLayer(LPCTSTR layerName, AcDbDatabase* db);

	static Acad::ErrorStatus    defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec,
		AcDbObjectId& newBlkRecId, AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec,
		AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewBlock(LPCTSTR blkName, AcDbObjectId& newBlkRecId,
		AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewAnonymousBlock(AcDbBlockTableRecord*& newBlkRec,
		AcDbObjectId& newBlkRecId, AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewAnonymousBlock(AcDbBlockTableRecord*& newBlkRec,
		AcDbDatabase* db);
	static Acad::ErrorStatus    defineNewAnonymousBlock(AcDbObjectId& newBlkRecId,
		AcDbDatabase* db);
	//@Description ͨ���鶨�崴������¼
	static Acad::ErrorStatus        openBlockDef(LPCTSTR blkName, AcDbBlockTableRecord*& blkRec,
		AcDb::OpenMode mode, AcDbDatabase* db);
	static AcDbBlockTableRecord*    openCurrentSpaceBlock(AcDb::OpenMode mode, AcDbDatabase* db);

	static Acad::ErrorStatus    addToCurrentSpace(AcDbEntity* newEnt, AcDbDatabase* db);
	static Acad::ErrorStatus    addToCurrentSpace(AcDbEntity* newEnt);
	static Acad::ErrorStatus    addToCurrentSpaceAndClose(AcDbEntity* newEnt, AcDbDatabase* db);
	static Acad::ErrorStatus    addToCurrentSpaceAndClose(AcDbEntity* newEnt);
	static Acad::ErrorStatus    AddtoModelSpace(AcDbEntity*pEnt,AcDbObjectId&retId,AcDbDatabase* pDb=NULL);
	static Acad::ErrorStatus    AddtoModelSpaceAndClose(AcDbEntity*pEnt,AcDbObjectId&retId,AcDbDatabase* pDb=NULL);
	static bool					isOnLockedLayer(AcDbObjectId& id, bool printMsg);
	static bool					isOnLockedLayer(AcDbEntity* ent, bool printMsg);

	/////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////// ACAD DICTIONARY UTILITIES //////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	static AcDbObjectId		getRegularDictionaryId(LPCTSTR dictName, bool createIfNotFound, AcDbDatabase* db);
	static AcDbObjectId		getSubDictionaryId(LPCTSTR dictName, bool createIfNotFound,
		AcDbDictionary* parentDict);

	static AcDbDictionary*	openDictionaryForRead(LPCTSTR dictName, AcDbDatabase* db);
	static AcDbDictionary*  openDictionaryForRead(LPCTSTR dictName, const AcDbObjectId& parentDict);
	static AcDbDictionary*  openDictionaryForRead(LPCTSTR dictName, AcDbDictionary* parentDict);

	static AcDbDictionary*  openDictionaryForWrite(LPCTSTR dictName, bool createIfNotFound, AcDbDatabase* db);
	static AcDbDictionary*  openDictionaryForWrite(LPCTSTR dictName, bool createIfNotFound, const AcDbObjectId& parentDict);
	static AcDbDictionary*  openDictionaryForWrite(LPCTSTR dictName, bool createIfNotFound, AcDbDictionary* parentDict);

	//static Acad::ErrorStatus	collectDictionaryEntryNames(const AcDbObjectId& dictId, SdStrObjIdList& list, AcRxClass* classType);
	//static Acad::ErrorStatus	collectDictionaryEntryNames(const AcDbObjectId& dictId, AcDbObjectIdArray& list, AcRxClass* classType);

	static bool    lookUpDictEntryName(const AcDbObjectId& objId, CString& entryName);

	// extension dictionary routines
	static AcDbDictionary*    openExtDictForRead(const AcDbObject* obj);
	static AcDbDictionary*    openExtDictForWrite(AcDbObject* obj, bool createIfNotFound);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//����ϵͳ����
	static BOOL GetVar(LPCTSTR var, int *nRes);
	static BOOL GetVar(LPCTSTR var, ads_real *rRes);
	static BOOL GetVar(LPCTSTR var, ads_point *pRes);
	static BOOL GetVar(LPCTSTR var, LPTSTR sRes);
	static BOOL SetVar(LPCTSTR szVarName, int       nArg);
	static BOOL SetVar(LPCTSTR szVarName, ads_real  rArg);
	static BOOL SetVar(LPCTSTR szVarName, ads_point pArg);
	static BOOL SetVar(LPCTSTR szVarName, LPCTSTR   sArg);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	static void    alertBox(LPCTSTR msg);
	static void    stopAlertBox(LPCTSTR msg);
	static void    infoAlertBox(LPCTSTR msg);
	static void    stubAlert(LPCTSTR msg);
	static UINT    yesNoAlert(LPCTSTR msg, UINT defButton = IDYES);
	static UINT    yesNoCancelAlert(LPCTSTR msg, UINT defButton = IDCANCEL);
	//@return ���ص�ǰӦ�ó����·��
	static CString GetAppPath();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//����ZOOM��ز���
	//ZOOMEXT����
	static void GetCurrentView(AcDbViewTableRecord& view);
	static bool getCurViewExt(AcDbExtents& dcsExt);
	static void ZOOMEXT(double dScale = 1.0);
	//ZOOMW����
	static void ZOOMWINDOW(AcGePoint3d minPt, AcGePoint3d maxPt);
	//zoom����
	//@param cenPt�ӿڵ����ĵ�
	static void ZOOMWindowByDeFault(AcGePoint3d cenPt);

	static void ZoomToWindow(AcGePoint3d wmin, AcGePoint3d wmax);


	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// ACAD DB UTILITIES ///////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	static Acad::ErrorStatus    objIdToEname(const AcDbObjectId& objId, ads_name& ent);
	static Acad::ErrorStatus    enameToObjId(ads_name ent, AcDbObjectId& objId);
	static Acad::ErrorStatus    handleStrToObjId(AcDbDatabase* db, LPCTSTR handleStr, AcDbObjectId& objId, bool speak = true);

	static LPCTSTR				objIdToHandleStr(const AcDbObjectId& objId, CString& str);

	// clone wrappers
	static Acad::ErrorStatus    cloneObjects(AcDbDatabase* db, const AcDbObjectId& entToClone,
		const AcDbObjectId& ownerBlockId,
		bool debugSpeak = false);
	static Acad::ErrorStatus    cloneObjects(AcDbDatabase* db, const AcDbObjectIdArray& entsToClone,
		const AcDbObjectId& ownerBlockId,
		bool debugSpeak = false);
	static Acad::ErrorStatus    cloneAndXformObjects(AcDbDatabase* db, const AcDbObjectId& entToClone,
		const AcDbObjectId& ownerBlockId, const AcGeMatrix3d& xformMat,
		bool debugSpeak = false);
	static Acad::ErrorStatus    cloneAndXformObjects(AcDbDatabase* db, const AcDbObjectIdArray& entsToClone,
		const AcDbObjectId& ownerBlockId, const AcGeMatrix3d& xformMat,
		bool debugSpeak = false);
	// polyline utility functions
	static bool			plineHasWidth(const AcDb2dPolyline* pline);
	static bool			plineHasWidth(const AcDbPolyline* pline);
	static bool			plineHasZeroLengthSegments(const AcDbPolyline* pline);
	static int			numSegments(const AcDbPolyline* pline);
	static void			collectVertices(const AcDb2dPolyline* pline, AcGePoint3dArray& pts,
		AcGeDoubleArray& bulges, bool asWcsPts = true);
	static void			collectVertices(const AcDb2dPolyline* pline, AcGePoint3dArray& pts, AcDbIntArray& types,
		AcGeDoubleArray& bulges, AcGeDoubleArray& startWidths, 
		AcGeDoubleArray& endWidths, bool& hasWidth);
	static void			collectVertices(const AcDb3dPolyline* pline, AcGePoint3dArray& pts);
	static void			collectVertices(const AcDbPolyFaceMesh* pface, AcDbObjectIdArray& vfaces, AcGePoint3dArray& pts);
	static bool			collectVertices(const AcDbPolygonMesh* pmesh, AcGePoint3dArray& pts, int& mSize, int& nSize);

	static bool         isPaperSpace(AcDbDatabase* db);

	// UCS/Geometry stuff relative to a particular database
	static void             transformToWcs(AcDbEntity* ent, AcDbDatabase* db);
	static void             transformToUcs(AcDbEntity* ent, AcDbDatabase* db);

	static AcGeVector3d		getUcsXAxis(AcDbDatabase* pDb);
	static AcGeVector3d     getUcsYAxis(AcDbDatabase* pDb);
	static AcGeVector3d     getUcsZAxis(AcDbDatabase* pDb);

	static AcGePlane		getUcsPlane(AcDbDatabase* pDb);
	static void             getUcsToWcsMatrix(AcGeMatrix3d& m, AcDbDatabase* pDb);
	static void             getWcsToUcsMatrix(AcGeMatrix3d& m, AcDbDatabase* pDb);
	static void             getUcsToWcsOriginMatrix(AcGeMatrix3d& m, const AcGePoint3d& wcsBasePt, AcDbDatabase* pDb);
	static void             getEcsPlane(const AcGeVector3d& entNormal, AcGePlane& ecsPlane);
	static void             getEcsPlane(const AcGePoint3d& origin, const AcGeVector3d& entNormal, AcGePlane& ecsPlane, AcGeVector3d& ecsXAxis);
	static void             getEcsXAxis(const AcGeVector3d& ecsZAxis, AcGeVector3d& xAxis);
	static void             getEcsToWcsMatrix(const AcGePoint3d& origin, const AcGeVector3d& zAxis, AcGeMatrix3d& mat);

	// these functions all assume current document for the AcDbDatabase*
	static AcGePoint3d      wcsToUcs(const AcGePoint3d& pt);
	static AcGeVector3d     wcsToUcs(const AcGeVector3d& vec);
	static void             wcsToUcs(AcGePoint3dArray& ptArray);
	static AcGePoint3d      ucsToWcs(const AcGePoint3d& pt);
	static AcGeVector3d     ucsToWcs(const AcGeVector3d& vec);
	static void             ucsToWcs(AcGePoint3dArray& ptArray);
	static AcGePoint3d      ecsToWcs(const AcGePoint3d& pt, const AcGeVector3d& entNormal);
	static AcGeVector3d     ecsToWcs(const AcGeVector3d& vec, const AcGeVector3d& entNormal);
	static AcGePoint2d      ucsToDcs(const AcGePoint3d& pt);
	static AcGePoint3d      dcsToUcs(const AcGePoint2d& pt);

	static void				getAllDatabases(AcDbVoidPtrArray& dbPtrs);
	static void				getAllDocuments(AcDbVoidPtrArray& docPtrs);

	static Acad::ErrorStatus	verifyClassType(const AcDbObject* obj, AcRxClass* classType);
	static Acad::ErrorStatus	verifyEntityReference(const AcDbEntity* ent,
		const AcDbObjectId& idToValidate,
		AcRxClass* classType, bool allowNull,
		bool allowErased);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//ˢ��ʵ��

	//************************************
	// Method:    RegenEntId
	// FullName:  MyBaseUtils::RegenEntId
	// Access:    public static 
	// Returns:   void
	// Qualifier: ˢ�µ���ʵ��
	// Parameter: AcDbObjectId objId
	//************************************
	static Acad::ErrorStatus flushGraphics(AcDbObjectId objId);
	static Acad::ErrorStatus flushGraphics(AcDbObjectIdArray objIds);
	static Acad::ErrorStatus flushGraphics(AcDbDatabase* pDb);

};

/////////////////////////////////////////////////////////////////////

inline void
copyAdsPt(ads_point pt1, ads_point pt2)
{
	pt1[0] = pt2[0];
	pt1[1] = pt2[1];
	pt1[2] = pt2[2];
}

/////////////////////////////////////////////////////////////////////

inline void
copyEname(ads_name ename1, ads_name ename2)
{
	ename1[0] = ename2[0];
	ename1[1] = ename2[1];
}

/////////////////////////////////////////////////////////////////////
