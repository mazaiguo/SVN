//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DistinguishData.h"
#include "DlgBasicSettings.h"
#include "DlgTextSettings.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CGwDesignApp : public AcRxArxApp {

public:
	CGwDesignApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - GwDesign._SBGW command (do not rename)
	static void GwDesign_SBGW(void)
	{
		// Add your code for command GwDesign._SBGW here
		/*resbuf* filter = acutBuildList(-4,_T("<and"),RTDXF0,_T("*POLYLINE,LINE"),-4,_T("and>"),RTNONE);
		ads_name ssname;
		int nRet = acedSSGet(NULL, NULL, NULL, filter, ssname);
		acutRelRb(filter);
		if (nRet != RTNORM)
		{
			return;
		}
		long sslen;
		acedSSLength(ssname, &sslen);
		if (sslen < 1)
		{
			return;
		}

		AcDbEntity* pEnt = NULL;
		ads_name ename;
		AcDbObjectId objId = AcDbObjectId::kNull;
		for (int i=0; i<sslen; i++)
		{
			acedSSName(ssname, i, ename);
			acdbGetObjectId(objId, ename);
			if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
			{
				continue;
			}
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pLine = AcDbLine::cast(pEnt);

			}
			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline* pLine = AcDbPolyline::cast(pEnt);
				int nSize = pLine->numVerts();
				bool bIsClosed = pLine->isClosed();
				
			}
		}*/
		CDistinguishData dData;
		dData.doIt();
	}

	// - WRQ_GwDesign._SZ command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_SZ
	// FullName:  CGwDesignApp::WRQ_GwDesign_SZ
	// Access:    public static 
	// Returns:   void
	// Qualifier: ª˘¥°…Ë÷√
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_SZ(void)
	{
		// Add your code for command WRQ_GwDesign._SZ here
		CAcModuleResourceOverride rs;
		/*CDlgBasicSettings dlg;
		dlg.DoModal();*/
		CDlgTextSettings dlg;
		dlg.DoModal();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGwDesignApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, GwDesign, _SBGW, SBGW, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _SZ, SZ, ACRX_CMD_TRANSPARENT, NULL)
