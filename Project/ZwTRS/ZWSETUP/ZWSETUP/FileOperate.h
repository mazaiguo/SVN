#pragma once
//操作文件
#include "OperateReg.h"
class CFileOperate
{
public:
	CFileOperate(void);
	~CFileOperate(void);
	//获取当前应用位置
	CString GetAppPath();
	//获取cad的安装路径
	bool GetCADPath();
	//填写搜索路径
	bool SetOptions();
	//自动加载
	//@param strFile为文件路径名
	bool SetAutoLoadZrx(LPCTSTR strFile);
	//启动CAD
	bool lauchCAD();
	//拷贝目录里的文件到另一个目录
	bool CopyDirTo(LPCTSTR strFrom, LPCTSTR strTo);
	//拷贝zwcad.lsp文件从一个文件位置到另一个位置
	bool CopyLspFile();
	//
	CString GetLoadCmdStr(CString strInput, CString strLoad);

private:
	CString m_strAppPath;
	CString m_strCADPath;
	OperateReg	m_Reg;
};

