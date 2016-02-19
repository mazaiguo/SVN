#pragma once
class COperateReg
{
public:
	COperateReg(void);
	~COperateReg(void);

	/////////////////////////////////////////////////////////////////////////////
	//�� �ܣ���ȡע�����ַ�����ֵ
	//�� �룺hKey-ע����HKEY_CLASSES_ROOT��HKEY_CURRENT_CONFIG��HKEY_CURRENT_USER��HKEY_LOCAL_MACHINE��HKEY_USERS
	//lpszSec-·�� lpszDef-Ĭ��ֵ lpszKey-����
	//�� ����lpszValue
	//�� �أ�-1��·��ʧ�� -2������ʧ�� �ɹ������ַ����ĳ���
	int GetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszDef, LPTSTR lpszValue);
	/////////////////////////////////////////////////////////////////////////////
	//��ע���,��DWORD��ֵ
	DWORD GetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, DWORD DefValue);
	/////////////////////////////////////////////////////////////////////////////
	//�� �ܣ�д��ע���,д�ַ���
	//�� �룺hKey-ע����HKEY_CLASSES_ROOT��HKEY_CURRENT_CONFIG��HKEY_CURRENT_USER��HKEY_LOCAL_MACHINE��HKEY_USERS
	//lpszSec-·�� lpszKey-���� lpszValue-ֵ
	//�� �أ�FALSE-ʧ�� TRUE-�ɹ�
	BOOL SetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszValue);
	/////////////////////////////////////////////////////////////////////////////
	//�� �ܣ�д��ע���,дDWORD
	BOOL SetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey,DWORD Value);
};

