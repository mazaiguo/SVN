#pragma once

class CBlkInfo//�����Ļ�����Ϣ
{
public:
	CBlkInfo(void);
	CBlkInfo(const CBlkInfo& scl);
	~CBlkInfo(void);
	//��������
	CString GetBlkType() const;
	AcGePoint3d GetInsertPt() const;
	AcGeScale3d GetScale() const;
	double GetRotate() const;
	LPCTSTR GetBlkName() const;
	LPCTSTR GetFilePath() const;
	//��������
	void SetBlkType(LPCTSTR str);
	void SetInsertPt(AcGePoint3d pt);
	void SetScale(AcGeScale3d sc);
	void SetRotate(double dRotate);
	void SetBlkName(LPCTSTR strBlkName);
	void SetFilePath(LPCTSTR strFileName);

	CBlkInfo&   operator = (const CBlkInfo& scl);
private:
	//��Ļ�����Ϣ
	CString m_strBlkType;//ͼ������
	AcGePoint3d m_insertPt;//�����
	AcGeScale3d m_scale;//����
	double m_dRotate;//��ת
	CString m_strBlkName;
	CString m_strFileName;//·����
};


class CBlkInsert//�����ĺ���
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

	//������������
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
	bool m_bDrawWP;//�Ƿ����wipeout
	bool m_bIsModified;//�Ƿ��޸�
	AcGePoint3d  m_insertPt;
	CString m_strName;
	CString m_strNo;
	CString m_strText;
	AcDbObjectId m_textId1;
	AcDbObjectId m_textId2;
	AcDbObjectId m_textId3;
	AcDbObjectId m_textId4;//��¼���
	AcDbObjectId m_textId5;//�Զ�������
	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId m_blkId;//�Ǳ�
	AcDbObjectId m_lineId;//�Ǳ�
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

class CBlkBaseInFo//�������Ϣ
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

