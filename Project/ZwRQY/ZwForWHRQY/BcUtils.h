#pragma once
#include "CBiaochiForRQY.h"
#include "ZdmDataInfo.h"
#include <utility>
#include <map>
using namespace std;

//typedef map<CString, CBiaochiForRQY> map<CString, CBiaochiForRQY>;
//////////////////////////////////////////////////////////////////////////
//��CBcUtils��������ص�����
//1.ɾ����������
//2.���뵥������
//3.�޸ĵ�������
//4.
//��дʱ�䣺2016��3��31��16:50:15

class CBcUtils
{
public:
	CBcUtils(void);
	~CBcUtils(void);
	//����dic�������
	//@param strLabel������
	//@param CZdmDataInfo�Ǿ�������
	bool add(LPCTSTR strLabel, CZdmDataInfo pData);

	////����dic�������
	////@param strLabel������
	////@param CZdmDataInfo�Ǿ�������
	//bool add(LPCTSTR strLabel, CBiaochiForRQY* pData);
	//����dic�������
	//@param strLabel������
	//@param CZdmDataInfo�Ǿ�������
	bool insert(LPCTSTR strLabel, CZdmDataInfo pData);
	//�޸�dic�������
	//@param strLabel������
	//@param CZdmDataInfo�Ǿ�������
	bool modify(LPCTSTR strLabel, CZdmDataInfo pData);
	//ɾ��dic��strLabel��Ӧ������
	bool del(LPCTSTR strLabel);
	//ͨ��strLabel�����Ƶõ��洢�ڸùؼ����µĶ�������
	//@param strLabelΪ�ؼ���
	CZdmDataInfo get(LPCTSTR strLabel);
	//��ȡ����
	//@param
	//@return ����dictionary�е�����
	int getCount();

	//bool OperateDic(CZdmDataInfo pData, )
	//��dwg�ļ�������CBiaochiForRQY::m_dictName = _T("ZW_BIAOCHI_DATA_IN_DWG")������ȡ����
	map<CString, CZdmDataInfo> getAllData();
private:
	
	//���������õ�dwg�ļ���
	void addAll();
	//ɾ��ZW_BIAOCHI_DATA_IN_DWG�е���������
	void delAll();

private:
	map<CString, CZdmDataInfo> m_Data;
};


