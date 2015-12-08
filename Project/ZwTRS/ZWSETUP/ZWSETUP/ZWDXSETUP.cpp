
// ZWDXSETUP.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ZWDXSETUP.h"
#include "ZWDXSETUPDlg.h"
#include "TimeManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "OperateReg.h"
#include "FileOperate.h"

// CZWDXSETUPApp

BEGIN_MESSAGE_MAP(CZWDXSETUPApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CZWDXSETUPApp 构造

CZWDXSETUPApp::CZWDXSETUPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CZWDXSETUPApp 对象

CZWDXSETUPApp theApp;

CString GetLoadCmdStr(CString strInput, CString strLoad)
{
	CString strReturn(strInput);
	strReturn.Replace(_T("\\"), _T("\\\\"));
	strReturn = _T("(") + strLoad + _T('\x20') + _T('\"') + strReturn + _T('\"') + _T(")");
	return strReturn;
}
#include <Windows.h>
#include <TlHelp32.h>

void CloseProcess(char *sProcessName)
{
	bool bIsOpened = false;
	if(sProcessName==NULL)
		return;
	PROCESSENTRY32 pe32;
	//在使用这个结构前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	//给系统内所有的进程拍个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS,0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return;
	DWORD hMsDevId=0;
	//遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		if(stricmp(pe32.szExeFile,sProcessName)==0)
		{
			hMsDevId=pe32.th32ProcessID;
			bIsOpened = true;
			break;
		}
		bMore = ::Process32Next(hProcessSnap,&pe32);
	}
	//不要忘记清除掉snapshot对象
	::CloseHandle(hProcessSnap);
	if (bIsOpened)
	{
		//结束进程
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,hMsDevId);
		::TerminateProcess(hProcess,4);
	}
} 
// CZWDXSETUPApp 初始化

BOOL CZWDXSETUPApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	
	//找到当前exe文件所在位置
	CFileOperate foperate;
	bool bRet = foperate.GetCADPath();
	if (!bRet)
	{
		AfxMessageBox(_T("没有安装zwcad2014机械"));
		return FALSE;
	}

	CloseProcess("ZWCAD.exe");

	//读取当前cad的vernumber	
	CString strAppPath = foperate.GetAppPath();
	foperate.CopyLspFile();
	foperate.SetOptions();
	foperate.lauchCAD();

	return FALSE;
}
