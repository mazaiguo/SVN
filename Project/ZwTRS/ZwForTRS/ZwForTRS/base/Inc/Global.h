// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__10D86CB2_8BCE_49FE_B56F_E3657D5A006E__INCLUDED_)
#define AFX_GLOBAL_H__10D86CB2_8BCE_49FE_B56F_E3657D5A006E__INCLUDED_

#include <afxtempl.h>
#pragma once

#include "StdAfx.h"

#include "map"
class CGlobal  
{
	CGlobal();
	
public:
	//	BOOL WriteDictionary();
	BOOL SetIniValue(LPCTSTR key,LPCTSTR Name,LPCTSTR value);
	BOOL SetIniValue(LPCTSTR key, LPCTSTR Name, double value);
	int GetIniValueNum(CString key);
	CString GetIniPath();
	CString SubString(CString string,CString Split,int index);
	void WriteIniFile();
	void SetIni(CString path);//设置INI路径并读取内容
	virtual ~CGlobal();
	static CGlobal& Get();
	BOOL GetVar(LPCTSTR var, int *nRes);
	BOOL GetVar(LPCTSTR var, ads_real *rRes);
	BOOL GetVar(LPCTSTR var, ads_point *pRes);
	BOOL GetVar(LPCTSTR var, LPTSTR sRes);
	BOOL SetVar(LPCTSTR szVarName, int       nArg);
	BOOL SetVar(LPCTSTR szVarName, ads_real  rArg);
	BOOL SetVar(LPCTSTR szVarName, ads_point pArg);
	BOOL SetVar(LPCTSTR szVarName, LPCTSTR   sArg);

	
	 CString GetAppPath();

	 CString GetCsvFile();
	/////////////////////////////////////////////////////////////////////////////
	//功 能：读取注册表读字符串的值
	//传 入：hKey-注册表的HKEY_CLASSES_ROOT、HKEY_CURRENT_CONFIG、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS
	//lpszSec-路径 lpszDef-默认值 lpszKey-主键
	//传 出：lpszValue
	//返 回：-1打开路径失败 -2打开主键失败 成功返回字符串的长度
	int GetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszDef, LPTSTR lpszValue);
	/////////////////////////////////////////////////////////////////////////////
	//读注册表,读DWORD的值
	DWORD GetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, DWORD DefValue);
	/////////////////////////////////////////////////////////////////////////////
	//功 能：写入注册表,写字符串
	//传 入：hKey-注册表的HKEY_CLASSES_ROOT、HKEY_CURRENT_CONFIG、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS
	//lpszSec-路径 lpszKey-主键 lpszValue-值
	//返 回：FALSE-失败 TRUE-成功
	BOOL SetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey, LPCTSTR lpszValue);
	/////////////////////////////////////////////////////////////////////////////
	//功 能：写入注册表,写DWORD
	BOOL SetRegister(HKEY hKey, LPCTSTR lpszSec, LPCTSTR lpszKey,DWORD Value);
	//得到Cyz数据
	LPCTSTR GetIniValue(LPCTSTR Key,LPCTSTR name);
	double GetIniValue(LPCTSTR Key,LPCTSTR name,double);
	/////////////////////////////////////////////////////////////////////////////
	//设置Cyz数据

	int Myatoi(CString inputStr);
	BOOL Myatoi(CString inputStr,int& iresult);	

	BOOL IsFileExist(CString strFileName);	

	void setBlkRefCount(int nCount);

	int getBlkRefCount();

};

#endif
#define gGlobal CGlobal::Get()



