#pragma once
//�������
class CBiaochiInfo
{
public:
	CBiaochiInfo(void);
	~CBiaochiInfo(void);
	bool start();
	//�����µĵ�����
	bool newLine();
	//��ԭ�е������ϼ���
	bool oldLine();
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
private:

	//************************************
	// Method:    initData
	// FullName:  CBiaochiInfo::initData
	// Access:    private 
	// Returns:   void
	// Qualifier:��ʼ������
	//************************************
	void initData();

private:
	bool m_bIsNode;//�Ƿ��нڵ�
	AcGePoint3d m_basePt;//����
};
