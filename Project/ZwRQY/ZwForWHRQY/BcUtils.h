#pragma once
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
#ifdef WRQ_ZDM_MODULE //�ڴ�������DLL�Ĺ������Ȱ� 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CBcUtils
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
	bool get(LPCTSTR strLabel, CZdmDataInfo&);
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


