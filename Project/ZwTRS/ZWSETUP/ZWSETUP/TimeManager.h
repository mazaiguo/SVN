#pragma once
#include "time.h"
#include <afxsock.h>      // MFC socket extensions 
#include <afxdtctl.h> 
class CTimeManager
{
public:
	CTimeManager(void);
	~CTimeManager(void);

	BOOL GetInternetTime(CTime* pTime, CString strServer); 
	BOOL SyncSystemClock(CTime tmServer);   

};
