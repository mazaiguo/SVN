#pragma once

#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

//��������ű������ص�
class SERVERDLL_API MySymble
{
public:
	MySymble(void);
	~MySymble(void);

	//@param strlayerΪͼ����
	//@return ͨ��ͼ������ͼ��Id
	static AcDbObjectId GetLayer(CString strlayer);
	//@param strLayerΪͼ����
	//@param nColorΪ����ɫ
	//@param bIsModColorΪ��ʾ�Ƿ��޸Ĵ��ڵ�strLayer����ɫ��Ϣ
	//@param strLineTypeNameͼ���������ϢĬ��ΪContinouse
	//@return ����ͼ��Id
	static AcDbObjectId CreateNewLayer(CString strLayer,
								int nColor = 7, 
								BOOL bIsModColor = FALSE, 
								CString strLineTypeName = _T("Continuous"));

	//@param stylenameΪ��������
	//@param strFontΪС������
	//@param strBigFontΪ����������
	//@param dTextSΪ�������
	//@param dTextSizeΪ����߶�
	//@param bIsModifiedΪ�Ƿ��޸�
	//@return ��������Id
	static AcDbObjectId CreateTextStyle(CString stylename,
									CString strFont,
									CString strBigfont,
									double dTextS = 1.0, 
									double dTextSize = 3.0, 
									BOOL bIsModified = FALSE);

	//@param ObjIdΪ����Id
	//@param dTextScaleΪ���صĸ�����ı���
	//@param dTextSizeΪ���صĸ�����ĸ߶�
	//@return ���غ���״̬
	static Acad::ErrorStatus GetTextStyleInfo(AcDbObjectId ObjId, double& dTextScale, double& dTextSize);

	//@param strLtNameΪ������
	//@param bIsModifiedΪ�Ƿ��޸�
	//@returnΪ��������Id
	static AcDbObjectId CreateLineType(CString strLtName, BOOL bIsModified = FALSE);

	//@return ��������������ʽ��
	static void GetAllTextStyleName(CStringArray& StrArr);


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
	//
	static AcDbObjectId CreateDimstyle(CString strDimName, double dheight = 2.5, double dDimlfac = 1.0,
		bool bDimTih = false, bool bDimToh = false);

};
