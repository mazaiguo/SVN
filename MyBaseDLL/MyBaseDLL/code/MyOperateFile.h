#pragma once

#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API MyOperateFile
{
public:
	MyOperateFile(void);
	~MyOperateFile(void);
	//��ȡ��Ŀ¼
	static CString GetSubPath(LPCTSTR strPath);
	//������Ŀ¼
	static bool FindSubDir(LPCTSTR strPath);
	//��ȡ�ļ���
	static CString GetFileNameByPath(LPCTSTR strPath);
	//
	static void GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, 
		CString strTypeName, bool bIsFullPath = false);

};
