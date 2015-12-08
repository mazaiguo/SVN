#pragma once

class MyOperateFile
{
public:
	MyOperateFile(void);
	~MyOperateFile(void);
	//获取子目录
	CString GetSubPath(LPCTSTR strPath);
	//发现子目录
	bool FindSubDir(LPCTSTR strPath);
	//获取文件名
	CString GetFileNameByPath(LPCTSTR strPath);
	//
	void GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, 
		CString strTypeName, bool bIsFullPath);

};
