#pragma once
#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CBlkBaseInFo//块基本信息
{
public:
	CBlkBaseInFo(void);
	~CBlkBaseInFo();

	AcGePoint3d GetInsertPt() const;
	AcGeScale3d GetScale() const;
	double GetRotate() const;
	CString GetBlkName() const;
	CString GetFileName() const;


	void SetInsertPt(AcGePoint3d insertPt);
	void SetScale(AcGeScale3d sc);
	void SetRotate(double dRotate);
	void SetBlkName(LPCTSTR strBlkName);
	void SetFileName(LPCTSTR strFileName);

	CBlkBaseInFo&   operator = (const CBlkBaseInFo& scl);

private:
	AcGePoint3d m_insertPt;//插入点
	AcGeScale3d m_scale;//比例
	double m_dRotate;//旋转
	CString m_strBlkName;
	CString m_strFileName;//路径名
};



class ZDM_DLL_API CBlkInsert//插入块的函数
{
public:
	CBlkInsert(void);
	CBlkInsert(CBlkBaseInFo blkInfo);
	~CBlkInsert(void);

	void SetBlkInfo(CBlkBaseInFo blkInfo);
	//void SetDataInfo(CDataBaseInfo dataInfo);
	bool Insert();
	void SetDrawWP(bool bDrawWp);
	void SetModified(bool bModified);
	void assocWipeOut(AcDbObjectId blkId, AcDbObjectId wipeoutId);

	AcDbExtents GetObjExts(); 
	AcDbObjectId GetObjectId();
	AcGePoint3d GetPosition();
private:
	bool InsertBlk();
	bool DoEntData();
	Acad::ErrorStatus CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds);
	void setImageFrameOff();
private:
	CBlkBaseInFo m_Info;
	AcDbObjectId m_blockId;
	AcDbExtents m_Exts;
	bool m_bDrawWP;//是否绘制wipeout
	bool m_bIsModified;//是否修改
	AcGePoint3d  m_insertPt;
	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId m_blkId;//仪表
	AcGePoint3d m_tmpPoint;
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bZongxiang ;//纵向
	//////////////////////////////////////////////////////////////////////////
	//20150115添加的信息
	int m_nPosition;//文字位置
	bool m_bIsDrawText;//是否绘制位号
	CString m_strDefine;//自定义文字信息
	CString m_strStandard;//规格信息
};

