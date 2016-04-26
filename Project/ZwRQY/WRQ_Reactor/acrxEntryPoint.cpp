//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CBaseDbReactor.h"
#include "CBaseDocReactor.h"
#include "CBaseEdReactor.h"
#include "CBaseEntDoubleClick.h"
#include "CBaseEventReactor.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
CBaseDbReactor* g_baseDb = NULL;
CBaseEventReactor* g_baseEv = NULL;
CBaseEdReactor* g_baseEd = NULL;
CBaseEntDoubleClick* g_baseEnt = NULL;
CBaseDocReactor* g_baseDoc = NULL;
//CObjectAppendToDb* m_dbReactor = NULL;
//attach all
void attachDbReactorToAll()
{
	if (g_baseDb == NULL) 
	{
		g_baseDb = new CBaseDbReactor;
		AcDbDatabase* tmpDb;
		AcDbVoidPtrArray dbPtrs;
		MyBaseUtils::getAllDatabases(dbPtrs);

		//acutPrintf(_T("\nAttaching Database Reactor to all active databases..."));
		int len = dbPtrs.length();
		for (int i=0; i<len; i++) {
			tmpDb = static_cast<AcDbDatabase*>(dbPtrs[i]);
			tmpDb->addReactor(g_baseDb);
		}
	}
}
//Remove DbReactor
void removeDbReactorFromAll()
{
	if (g_baseDb != NULL) 
	{
		AcDbDatabase* tmpDb;
		AcDbVoidPtrArray dbPtrs;
		MyBaseUtils::getAllDatabases(dbPtrs);

		acutPrintf(_T("\nRemoving Database Reactor from all active databases..."));

		int len = dbPtrs.length();
		for (int i=0; i<len; i++) {
			tmpDb = static_cast<AcDbDatabase*>(dbPtrs[i]);
			tmpDb->removeReactor(g_baseDb);
		}
		if (g_baseDb != NULL)
		{
			delete g_baseDb;
			g_baseDb = NULL;
		}
	}
}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CWRQ_ReactorApp : public AcRxArxApp {

public:
	CWRQ_ReactorApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		g_baseEnt = new CBaseEntDoubleClick;
		g_baseEnt->Attach();
		
		g_baseEd = new CBaseEdReactor;
		g_baseEd->Attach();
		//acedRegisterFilterWinMsg(MsgHook);
		g_baseDoc = new CBaseDocReactor;
		g_baseDoc->Attach();

		g_baseEv = new CBaseEventReactor;
		g_baseEv->Attach();

		/*g_baseDb = new CBaseDbReactor;
		g_baseDb->Attach(acdbCurDwg());*/

		attachDbReactorToAll();
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		//±à¼­·´Ó¦Æ÷
		g_baseDoc->Detach();
		if (g_baseDoc != NULL)
		{
			delete g_baseDoc;
			g_baseDoc = NULL;
		}

		g_baseEnt->Detach();		
		if (g_baseEnt != NULL)
		{
			delete g_baseEnt;
			g_baseEnt = NULL;
		}	
		g_baseEd->Detach();
		if (g_baseEd != NULL)
		{
			delete g_baseEd;
			g_baseEd = NULL;
		}

		g_baseEv->Detach();
		if (g_baseEv != NULL)
		{
			delete g_baseEv;
			g_baseEv = NULL;
		}

		/*g_baseDb->Detach();
		if (g_baseDb != NULL)
		{
			g_baseDb = NULL;
		}*/
		removeDbReactorFromAll();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CWRQ_ReactorApp)

