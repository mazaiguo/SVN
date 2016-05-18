//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DistinguishData.h"
#include "DlgSettings.h"
#include "SerialNoJig.h"
#include "CGasPipe.h"
#include "GWDesingUtils.h"
//#include "DlgBasicSettings.h"
//#include "DlgTextSettings.h"
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
	// Qualifier: 基础设置
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_SZ(void)
	{
		// Add your code for command WRQ_GwDesign._SZ here
		CAcModuleResourceOverride rs;
		/*CDlgBasicSettings dlg;
		dlg.DoModal();*/
		/*CDlgTextSettings dlg;
		dlg.DoModal();*/
		CDlgSettings dlg;
		dlg.DoModal();
	}

	// - WRQ_GwDesign._CRXH command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_CRXH
	// FullName:  CGwDesignApp::WRQ_GwDesign_CRXH
	// Access:    public static 
	// Returns:   void
	// Qualifier:插入序号
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_XH(void)
	{
		// Add your code for command WRQ_GwDesign._CRXH here
		CSerialNoJig jig;
		jig.startJig();
	}

	// - WRQ_GwDesign._ZJGD command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_ZJGD
	// FullName:  CGwDesignApp::WRQ_GwDesign_ZJGD
	// Access:    public static 
	// Returns:   void
	// Qualifier: 增加管道
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_GD(void)
	{
		// Add your code for command WRQ_GwDesign._ZJGD here
		AcGePoint3d startPt,endPt, tmpPt;
		int nRet = acedGetPoint(NULL, _T("\n请选择起点："), asDblArray(startPt));
		if (nRet != RTNORM)
		{
			return;
		}

		nRet = acedGetPoint(asDblArray(startPt), _T("\n请选择终点："), asDblArray(endPt));
		if (nRet != RTNORM)
		{
			return;
		}
		AcDbObjectId layId = MySymble::CreateNewLayer(_T("abcd"), 1);
		
		ads_name ename;
		AcDbObjectId startId,endId;

		nRet = acedEntSel(_T("\n请选择序号:"), ename, asDblArray(tmpPt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(startId, ename);

		nRet = acedEntSel(_T("\n请选择序号:"), ename, asDblArray(tmpPt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(endId, ename);

		CGasPipe* pGsPipe = new CGasPipe;
		pGsPipe->setLayer(layId);
		pGsPipe->setStartPt(startPt);
		pGsPipe->setEndPt(endPt);
		pGsPipe->setStartId(startId);
		pGsPipe->setEndId(endId);
		MyBaseUtils::addToCurrentSpaceAndClose(pGsPipe);

	}

	// - WRQ_GwDesign._CRXH command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_CRXH
	// FullName:  CGwDesignApp::WRQ_GwDesign_CRXH
	// Access:    public static 
	// Returns:   void
	// Qualifier:	插入序号
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_CRXH(void)
	{
		// Add your code for command WRQ_GwDesign._CRXH here
		ads_name ename;
		AcGePoint3d pt;
		AcDbObjectId objId = AcDbObjectId::kNull;
		int nRet = acedEntSel(_T("\n请选择燃气管道："), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(objId, ename);
		CGasPipe* pGsPipe = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pGsPipe, objId, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		double dLength = pGsPipe->length();
		if (dLength < 3*CGWDesingUtils::getGlobalRadius())
		{
			AfxMessageBox(_T("太短了，不能在此处插入序号"));
			pGsPipe->close();
			return;
		}
		AcDbObjectId startId,endId;
		AcGePoint3d startPt,midPt,endPt;
		startId = pGsPipe->startId();
		endId = pGsPipe->endId();
		startPt = pGsPipe->startPt();
		endPt = pGsPipe->endPt();
		pGsPipe->close();
		
		bool bContinued = true;
		bool bErase = true;
		while (bContinued)
		{
			bContinued = false;
			nRet = acedGetPoint(asDblArray(startPt), _T("\n请选择插入点"), asDblArray(midPt));
			if (nRet != RTNORM)
			{
				bErase = false;
				break;
			}				
			
			if ((acutDistance(asDblArray(startPt), asDblArray(midPt)) < 2*CGWDesingUtils::getGlobalRadius())
				||(acutDistance(asDblArray(endPt), asDblArray(midPt)) < 2*CGWDesingUtils::getGlobalRadius()))
			{
				AfxMessageBox(_T("不能在此处插入序号，离端点太近，序号会重叠"));
				bContinued = true;
				bErase = true;
				continue;
			}
		}	
	
		
		if (bErase)
		{
			MyEditEntity::EraseObj(pGsPipe->objectId());
			AcDbObjectId layId = MySymble::CreateNewLayer(_T("abcd"), 1);
			CSerialNo* pNo = new CSerialNo;
			MyBaseUtils::addToCurrentSpaceAndClose(pNo);
			AcDbObjectId midId = pNo->objectId();
			CGasPipe* pStartPipe = new CGasPipe;
			pStartPipe->setStartPt(startPt);
			pStartPipe->setStartId(startId);
			pStartPipe->setEndId(midId);
			pStartPipe->setEndPt(midPt);
			pStartPipe->setLayer(layId);
			MyBaseUtils::addToCurrentSpaceAndClose(pStartPipe);

			CGasPipe* pEndPipe = new CGasPipe;
			pEndPipe->setStartPt(midPt);
			pEndPipe->setStartId(midId);
			pEndPipe->setEndId(endId);
			pEndPipe->setEndPt(endPt);
			pEndPipe->setLayer(layId);
			MyBaseUtils::addToCurrentSpaceAndClose(pEndPipe);
		}
		
	}

	// - WRQ_GwDesign._Test command (do not rename)
	static void WRQ_GwDesign_Test(void)
	{
		// Add your code for command WRQ_GwDesign._Test here
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGwDesignApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, GwDesign, _SBGW, SBGW, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _SZ, SZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _XH, XH, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _GD, GD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _CRXH, CRXH, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _Test, Test, ACRX_CMD_TRANSPARENT, NULL)
