//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CBaseDataForZdDwg.h"
#include "CBiaochiForRQY.h"
#include "CBaseDataForGwDesign.h"
#include "SerialNo.h"
#include "Global.h"
//#include "CGasPipe.h"
//#include "SpecialText.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CWRQ_EntityApp : public AcRxArxApp {

public:
	CWRQ_EntityApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		gGlobal.SetIni(gGlobal.GetIniPath());

		CBaseDataForZdDwg::rxInit();
		CBiaochiForRQY::rxInit();
		CBaseDataForGwDesign::rxInit();
		CSerialNo::rxInit();
		//CGasPipe::rxInit();
		//CSpecialText::rxInit();
		acrxBuildClassHierarchy();	
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		deleteAcRxClass(CBaseDataForZdDwg::desc());
		deleteAcRxClass(CBiaochiForRQY::desc());
		deleteAcRxClass(CBaseDataForGwDesign::desc());
		deleteAcRxClass(CSerialNo::desc());
		//deleteAcRxClass(CGasPipe::desc());
		//deleteAcRxClass(CSpecialText::desc());


		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CWRQ_EntityApp)

