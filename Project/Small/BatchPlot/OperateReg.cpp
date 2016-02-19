#include "StdAfx.h"
#include "OperateReg.h"


COperateReg::COperateReg(void)
{
}


COperateReg::~COperateReg(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//�� �ܣ���ȡע�����ַ�����ֵ
//�� �룺hKey-ע����HKEY_CLASSES_ROOT��HKEY_CURRENT_CONFIG��HKEY_CURRENT_USER��HKEY_LOCAL_MACHINE��HKEY_USERS
//lpszSec-·�� lpszDef-Ĭ��ֵ lpszKey-����
//�� ����lpszValue
//�� �أ�-1��·��ʧ�� -2������ʧ�� �ɹ������ַ����ĳ���
int COperateReg::GetRegister(HKEY hKey, 
	LPCTSTR lpszSec, 
	LPCTSTR lpszKey, 
	LPCTSTR lpszDef, 
	LPTSTR lpszValue)
{
	HKEY hInfor;
	DWORD dwType;
	DWORD dwLength;

	if(RegOpenKeyEx(hKey,
		lpszSec,
		0, 
		KEY_READ|KEY_EXECUTE,
		&hInfor) != ERROR_SUCCESS)
	{
		_tcscpy(lpszValue,lpszDef);
		return -1;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		NULL,
		NULL,
		&dwLength) != ERROR_SUCCESS)
	{
		_tcscpy(lpszValue,lpszDef);
		RegCloseKey(hInfor);
		return -2;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		&dwType,
		(LPBYTE)lpszValue,
		&dwLength) != ERROR_SUCCESS)
	{
		_tcscpy(lpszValue,lpszDef);
		RegCloseKey(hInfor);
		return -2;
	}
	RegCloseKey(hInfor);
	return (_tcslen(lpszValue)+1);
}
/////////////////////////////////////////////////////////////////////////////
//��ע���,��DWORD��ֵ
DWORD COperateReg::GetRegister(HKEY hKey,LPCTSTR lpszSec,LPCTSTR lpszKey,DWORD DefValue)
{

	HKEY hInfor;
	DWORD dwType;
	DWORD dwLength;
	DWORD Value = DefValue;

	if(RegOpenKeyEx(hKey,
		lpszSec,
		0, 
		KEY_READ|KEY_EXECUTE,
		&hInfor) != ERROR_SUCCESS)
		return Value;

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		NULL,
		NULL,
		&dwLength) != ERROR_SUCCESS)
	{
		RegCloseKey(hInfor);
		return Value;
	}

	if(RegQueryValueEx(hInfor,
		lpszKey,
		NULL,
		&dwType,
		(LPBYTE)&Value,
		&dwLength) != ERROR_SUCCESS)
	{
		RegCloseKey(hInfor);
		return Value;
	}
	RegCloseKey(hInfor);
	return Value;
}

/////////////////////////////////////////////////////////////////////////////
//�� �ܣ�д��ע���,д�ַ���
//�� �룺hKey-ע����HKEY_CLASSES_ROOT��HKEY_CURRENT_CONFIG��HKEY_CURRENT_USER��HKEY_LOCAL_MACHINE��HKEY_USERS
//lpszSec-·�� lpszKey-���� lpszValue-ֵ
//�� �أ�FALSE-ʧ�� TRUE-�ɹ�
BOOL COperateReg::SetRegister(HKEY hKey,LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	HKEY hInfor;
	DWORD dwStatus;	
	int nLen = 0;

#ifdef UNICODE
	nLen = 2*(_tcslen(lpszValue) + 1);
#else
	nLen = _tcslen(lpszValue) + 1;
#endif
	if(RegCreateKeyEx(hKey,
		lpszSec, 0, NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE, 
		NULL,
		&hInfor,&dwStatus) == ERROR_SUCCESS) 
	{		
		dwStatus = RegSetValueEx(hInfor,
			lpszKey,
			0,
			REG_SZ,
			(LPBYTE)lpszValue,
			nLen);
		RegCloseKey(hInfor);
		return dwStatus==ERROR_SUCCESS;
	}
	else
		return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//�� �ܣ�д��ע���,дDWORD
BOOL COperateReg::SetRegister(HKEY hKey,LPCTSTR lpszSec,LPCTSTR lpszKey,DWORD Value)
{
	HKEY hInfor;
	DWORD dwStatus;			
	if(RegCreateKeyEx(hKey,
		lpszSec, 0, 
		NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE, NULL,
		&hInfor,
		&dwStatus)==ERROR_SUCCESS) 
	{
		dwStatus = RegSetValueEx(hInfor,
			lpszKey,0,
			REG_DWORD,
			(LPBYTE)&Value,
			sizeof(DWORD));
		RegCloseKey(hInfor);
		return dwStatus==ERROR_SUCCESS;
	}
	else
		return FALSE;
}
