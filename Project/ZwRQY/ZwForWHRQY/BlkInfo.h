#pragma once

class CBlkInfo//插入块的基本信息
{
public:
	CBlkInfo(void);
	CBlkInfo(const CBlkInfo& scl);
	~CBlkInfo(void);
	//基本数据
	CString GetBlkType() const;
	AcGePoint3d GetInsertPt() const;
	AcGeScale3d GetScale() const;
	double GetRotate() const;
	LPCTSTR GetBlkName() const;
	LPCTSTR GetFilePath() const;
	//基本数据
	void SetBlkType(LPCTSTR str);
	void SetInsertPt(AcGePoint3d pt);
	void SetScale(AcGeScale3d sc);
	void SetRotate(double dRotate);
	void SetBlkName(LPCTSTR strBlkName);
	void SetFilePath(LPCTSTR strFileName);

	CBlkInfo&   operator = (const CBlkInfo& scl);
private:
	//块的基本信息
	CString m_strBlkType;//图块类型
	AcGePoint3d m_insertPt;//插入点
	AcGeScale3d m_scale;//比例
	double m_dRotate;//旋转
	CString m_strBlkName;
	CString m_strFileName;//路径名
};


class CBlkInsert//插入块的函数
{
public:
	CBlkInsert(void);
	CBlkInsert(CBlkInfo blkInfo/*, CDataBaseInfo dataInfo*/);
	~CBlkInsert(void);

	void SetBlkInfo(CBlkInfo blkInfo);
	//void SetDataInfo(CDataBaseInfo dataInfo);
	bool Insert();
	void SetDrawWP(bool bDrawWp);
	void SetModified(bool bModified);
	void assocWipeOut(AcDbObjectId blkId, AcDbObjectId wipeoutId);

	//设置文字内容
	void SetName(LPCTSTR strName);
	void SetNo(LPCTSTR strNo);
	void SetText(LPCTSTR strText);
	void SetZongxiang(BOOL bZongxiang);
	//////////////////////////////////////////////////////////////////////////
	void SetDefine(LPCTSTR strDefine);
	void SetStandard(LPCTSTR strStandard);
	void SetPosition(int nPosition);
	void SetDrawText(bool bIsDrawText);
	//////////////////////////////////////////////////////////////////////////
	void SetTmpPoint(AcGePoint3d pt);

	AcDbExtents GetObjExts(); 
	AcDbObjectId GetObjectId();
	AcGePoint3d GetPosition();
private:
	bool InsertBlk();
private:
	CBlkInfo m_Info;
	//CDataBaseInfo m_DataInfo;
	AcDbObjectId m_blockId;
	AcDbExtents m_Exts;
	bool m_bDrawWP;//是否绘制wipeout
	bool m_bIsModified;//是否修改
	AcGePoint3d  m_insertPt;
	CString m_strName;
	CString m_strNo;
	CString m_strText;
	AcDbObjectId m_textId1;
	AcDbObjectId m_textId2;
	AcDbObjectId m_textId3;
	AcDbObjectId m_textId4;//记录规格
	AcDbObjectId m_textId5;//自定义文字
	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId m_blkId;//仪表
	AcDbObjectId m_lineId;//仪表
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

class CBlkBaseInFo//块基本信息
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

