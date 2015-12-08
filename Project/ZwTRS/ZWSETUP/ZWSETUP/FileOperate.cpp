#include "StdAfx.h"
#include "FileOperate.h"


CFileOperate::CFileOperate(void)
{

}


CFileOperate::~CFileOperate(void)
{

}


CString CFileOperate::GetAppPath()
{
	TCHAR buf[1000];
	GetModuleFileName(GetModuleHandle(_T("ZwmUpdate.exe")),buf,1000);
	CString strFileName = buf;
	m_strAppPath = strFileName.Left(strFileName.ReverseFind('\\'));
	return m_strAppPath;
}

bool CFileOperate::GetCADPath()
{
	CString tmpStr = _T("SOFTWARE\\ZWSOFT\\ZWCAD\\2014\\zwcadm:zh-CN");
	char ResStr[MAX_PATH];
	int nRet = m_Reg.GetRegister(HKEY_LOCAL_MACHINE, tmpStr, _T("Location"), _T(""), ResStr);
	m_strCADPath = ResStr;
	if (m_strCADPath.IsEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}
//填写搜索路径
//@param strFile为搜索路径
bool CFileOperate::SetOptions()
{
	CString tmpStr = "Software\\ZWSOFT\\ZWCAD\\2014";
	CString strPath;
	char ResStr[2048];
	//获取curver
	int nRet = m_Reg.GetRegister(HKEY_CURRENT_USER, tmpStr, _T("CurVer"), _T(""), ResStr);
	strPath = ResStr;
	if (strPath.IsEmpty())
	{
		return false;
	}
	tmpStr = tmpStr + _T("\\") + strPath + _T("\\Profiles");
	//获取当前默认的
	nRet = m_Reg.GetRegister(HKEY_CURRENT_USER, tmpStr, _T(""), _T(""), ResStr);
	strPath = ResStr;
	if (strPath.IsEmpty())
	{
		return false;
	}
	//填写搜索路径
	tmpStr = tmpStr + _T("\\") + strPath + _T("\\Config");
	nRet = m_Reg.GetRegister(HKEY_CURRENT_USER, tmpStr, _T("zwcad"), _T(""), ResStr);
	strPath = ResStr;
	CString strFilePath = m_strAppPath + _T(";");

	int nFind = strPath.Find(strFilePath);
	if (nFind < 0)
	{
		strPath = strPath + _T(";") + strFilePath;
		nRet = m_Reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("zwcad"), strPath);
	}
	return true;
}
//自动加载
//@param strFile为文件路径名
bool CFileOperate::SetAutoLoadZrx(LPCTSTR strFile)
{
	//写注册表
	////HKEY_CURRENT_USER\Software\ZWSOFT\ZWCAD\2014\zwcadm:zh-CN\Applications
	//CString tmpStr= _T("Software\\ZWSOFT\\ZWCAD\\2014\\zwcadm:zh-CN\\Applications\\ZRXZwForDL");
	//BOOL bIsOk = m_Reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("LOADER"), _T("ZwForDL.zrx"));
	//m_Reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("LOADCTRLS"), 2);
	//tmpStr += _T("\\Commands");
	//m_Reg.SetRegister(HKEY_CURRENT_USER, tmpStr, _T("ZwForDL"), _T("ZwForDL"));
	return true;
}
//启动CAD
bool CFileOperate::lauchCAD()
{
	//启动ZWCAD
	CString OutputPath;
	STARTUPINFO si; //一些必备参数设置
	memset(&si, 0, sizeof(STARTUPINFO)); 
	si.cb = sizeof(STARTUPINFO); 
	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_SHOWNORMAL; 
	PROCESS_INFORMATION pi; //必备参数设置结束
	OutputPath = m_strCADPath + _T("Zwcadm\\StartZwcadm_Z_CHS.exe");
	BOOL bRet = CreateProcess(NULL,OutputPath.GetBuffer(OutputPath.GetLength()),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	return bRet;
}

//拷贝目录里的文件到另一个目录
bool CFileOperate::CopyDirTo(LPCTSTR strFrom, LPCTSTR strTo)
{
	return true;
}
//拷贝文件从一个文件位置到另一个位置
bool CFileOperate::CopyLspFile()
{
	CString strIn,strOut;
	strIn = m_strAppPath + _T("\\zwcad2014.lsp");
	strOut = m_strCADPath + _T("Support\\zwcad2014.lsp");
	BOOL bRet = CopyFile(strIn,strOut, FALSE);
	return bRet;
}

CString CFileOperate::GetLoadCmdStr(CString strInput, CString strLoad)
{
	CString strReturn(strInput);
	strReturn.Replace(_T("\\"), _T("\\\\"));
	strReturn = _T("(") + strLoad + _T('\x20') + _T('\"') + strReturn + _T('\"') + _T(")");
	return strReturn;
}
