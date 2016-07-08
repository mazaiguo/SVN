#pragma once
#ifdef WRQ_ZDM_MODULE //�ڴ�������DLL�Ĺ������Ȱ� 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CBlkBaseInFo//�������Ϣ
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
	AcGePoint3d m_insertPt;//�����
	AcGeScale3d m_scale;//����
	double m_dRotate;//��ת
	CString m_strBlkName;
	CString m_strFileName;//·����
};



class ZDM_DLL_API CBlkInsert//�����ĺ���
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
	bool m_bDrawWP;//�Ƿ����wipeout
	bool m_bIsModified;//�Ƿ��޸�
	AcGePoint3d  m_insertPt;
	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId m_blkId;//�Ǳ�
	AcGePoint3d m_tmpPoint;
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bZongxiang ;//����
	//////////////////////////////////////////////////////////////////////////
	//20150115��ӵ���Ϣ
	int m_nPosition;//����λ��
	bool m_bIsDrawText;//�Ƿ����λ��
	CString m_strDefine;//�Զ���������Ϣ
	CString m_strStandard;//�����Ϣ
};

