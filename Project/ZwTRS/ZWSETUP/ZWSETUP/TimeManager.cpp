#include "StdAfx.h"
#include "TimeManager.h"

CTimeManager::CTimeManager(void)
{
}

CTimeManager::~CTimeManager(void)
{
}

  
//int _tmain(int argc, _TCHAR* argv[]) 
//{     
//	CTime tm(1990, 1, 1, 0, 0, 0);      
//	if(GetInternetTime(&tm, "time-nw.nist.gov"))     
//	{        
//		SyncSystemClock(tm);    
//	}      
//	else    
//	{         
//		printf("Not sucessful!\n");    
//	}       
//	return 0;   
//	//让程序完成任务直接退出吧...
//}  
BOOL CTimeManager::GetInternetTime(CTime* pTime, CString strServer)   
{    
	AfxSocketInit();  
	CSocket sockClient;     
	if(!sockClient.Create())            //创建socket      
		printf("Create socket failure.\n");      
	if(!sockClient.Connect((LPCTSTR)strServer, 37)) // strServer：时间服务器网址； 37：端口号     
		printf("Connect failure.\n");     
	DWORD dwTime = 0;               //用来存放服务器传来的标准时间数据     
	unsigned char nTime[8];         //临时接收数据    
	memset(nTime, 0, sizeof(nTime));       
	sockClient.Receive(nTime, sizeof(nTime));   //接收服务器发送来得4个字节的数据      
	sockClient.Close();             //关闭socket          
	dwTime += nTime[0] << 24;     //整合数据   
	dwTime += nTime[1] << 16;      
	dwTime += nTime[2] << 8; 
	dwTime += nTime[3];         
	if(0 == dwTime) 
		return FALSE;        
	//服务器传来的数据是自从1900年以来的秒数      //取得 1900~1970 的时间差(以秒数计算) ，放在dwSpan里面    
	COleDateTime t00( 1900, 1, 1, 0, 0, 0 ); // 1900.1.1 00:00:00      
	COleDateTime t70( 1970, 1, 1, 0, 0, 0 ); // 1970.1.1 00:00:00    
	COleDateTimeSpan ts70to00 = t70 - t00;       
	DWORD dwSpan = (DWORD)ts70to00.GetTotalSeconds();    
	ASSERT( dwSpan == 2208988800L );               //把时间变为基于1970年的，便于用CTime处理     
	dwTime -= dwSpan;              //构造当前时间的CTime对象    
	CTime tm = (CTime)dwTime;       
	*pTime = tm;             
	return TRUE; 
}   
BOOL CTimeManager::SyncSystemClock(CTime tmServer)
{       //如果是在XP下，则先提升进程的权限   
	OSVERSIONINFO osv;     
	osv.dwOSVersionInfoSize=sizeof OSVERSIONINFO;      
	GetVersionEx(&osv);    
	if(osv.dwPlatformId==VER_PLATFORM_WIN32_NT)      
	{         
		HANDLE hToken;    
		TOKEN_PRIVILEGES tkp;           // Get a token for this process.       
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))              
			return( FALSE );           // Get the LUID for the shutdown privilege.      
		LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &tkp.Privileges[0].Luid);         
		tkp.PrivilegeCount = 1;  // one privilege to set      
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;           // Get the shutdown privilege for this process.      
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);    
		if (GetLastError() != ERROR_SUCCESS)         
			return FALSE;   
	}      //设置系统时间    
	SYSTEMTIME systm;      
	systm.wYear = tmServer.GetYear();   
	systm.wMonth = tmServer.GetMonth();    
	systm.wDay = tmServer.GetDay();     
	systm.wHour = tmServer.GetHour();    
	systm.wMinute = tmServer.GetMinute();     
	systm.wSecond = tmServer.GetSecond();       
	systm.wMilliseconds = 0;     
	return SetLocalTime(&systm);  
}  