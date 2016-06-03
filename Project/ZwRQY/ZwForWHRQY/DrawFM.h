#pragma once

//////////////////////////////////////////////////////////////////////////
//���Ʒ���
//��������ͼ��FM-RQ
class CDrawFM
{
public:
	CDrawFM(void);
	~CDrawFM(void);

	bool doIt();
	bool del();
	
private:
	//��ȡ׮��
	bool GetZhuanghao();

	//************************************
	// Method:    GetDescription
	// FullName:  CDrawFM::GetDescription
	// Access:    private 
	// Returns:   bool
	// Qualifier����ȡ˵������
	//************************************
	bool GetDescription();

	Acad::ErrorStatus CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds);

	
private:
	AcDbObjectIdArray m_idArrs;

};
