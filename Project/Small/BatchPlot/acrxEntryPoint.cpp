// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DlgPlotConfig.h"
#include "OperateReg.h"
#include "Utility.h"
#include "DlgMPlotConfig.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
class AsdkDocToModReactor;

CDlgMPlotConfig* modelessDlg = NULL;
AsdkDocToModReactor* gpDocToModReactor = NULL;  // MDI Safe

const TCHAR* modeStr(AcAp::DocLockMode mode)
{
	switch (mode) {
	case AcAp::kNotLocked:
		return _T(/*NOXLATE*/"AcAp::kNotLocked");
	case AcAp::kRead:
		return _T(/*NOXLATE*/"AcAp::kRead");
	case AcAp::kWrite:
		return _T(/*NOXLATE*/"AcAp::kWrite");
	case AcAp::kAutoWrite:
		return _T(/*NOXLATE*/"AcAp::kAutoWrite");
	case AcAp::kProtectedAutoWrite:
		return _T(/*NOXLATE*/"AcAp::kProtectedAutoWrite");
	case AcAp::kXWrite:
		return _T(/*NOXLATE*/"AcAp::kXWrite");
	}
	return _T("ERROR");
}


class AsdkDocToModReactor : public AcApDocManagerReactor
{
public:
	AsdkDocToModReactor();
	~AsdkDocToModReactor();

	virtual void  documentCreated(AcApDocument* );
	virtual void  documentToBeDestroyed(AcApDocument* );
	virtual void  documentLockModeChanged(AcApDocument*,
		AcAp::DocLockMode,
		AcAp::DocLockMode,
		AcAp::DocLockMode,
		const TCHAR* pCommandName);
	virtual void  documentLockModeWillChange(AcApDocument*,
		AcAp::DocLockMode,
		AcAp::DocLockMode,
		AcAp::DocLockMode,
		const TCHAR* pGlobalCmdName);
	virtual void  documentBecameCurrent(AcApDocument* );

	virtual void  documentToBeActivated(AcApDocument* pActivatingDoc );
	virtual void  documentToBeDeactivated(AcApDocument* pDeactivatingDoc );
	virtual void  documentActivationModified(bool bActivation);
};

AsdkDocToModReactor::AsdkDocToModReactor()
{
	acDocManager->addReactor( this );
}

AsdkDocToModReactor::~AsdkDocToModReactor()
{
	acDocManager->removeReactor( this );
}

void AsdkDocToModReactor::documentCreated(AcApDocument* pDoc)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentCreated   called (%s)\n"), pDoc ? pDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_CREATEDOC, 0, 0L);
}

void AsdkDocToModReactor::documentToBeDestroyed(AcApDocument* pDoc)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentToBeDestroyed   called (%s)\n"), pDoc ? pDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_DESTROYDOC, 0, 0L);
}

void AsdkDocToModReactor::documentToBeActivated(AcApDocument* pActivatingDoc)
{
	if(!pActivatingDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentToBeActivated   called (%s)\n"), pActivatingDoc ? pActivatingDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_ACTIVATEDOC, 0, 0L);
}

void AsdkDocToModReactor::documentToBeDeactivated(AcApDocument* pDeactivatingDoc)
{
	if(!pDeactivatingDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentToBeDeactivated   called (%s)\n"), pDeactivatingDoc ? pDeactivatingDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_DEACTIVATEDOC, 0, 0L);
}


void AsdkDocToModReactor::documentLockModeChanged(AcApDocument* pDoc,
												  AcAp::DocLockMode myPrevMode,
												  AcAp::DocLockMode myCurMode,
												  AcAp::DocLockMode curMode,
												  const TCHAR* pCommandName)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentLockModeChanged   called (%s - from %s to %s)\n"),
		pDoc ? pDoc->fileName() : _T("NULL"), modeStr(myPrevMode), modeStr(myCurMode) );
	TCHAR *p = new TCHAR[21]; // this will be deleted on the dialog's side
	p[20] = 0;

	_tcscpy_s( p,20, modeStr(myCurMode) );

	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_LOCKMODCHANGED, 0, (LONG_PTR)p );
}


void AsdkDocToModReactor::documentLockModeWillChange(AcApDocument* pDoc,
													 AcAp::DocLockMode myCurMode,
													 AcAp::DocLockMode myNewMode,
													 AcAp::DocLockMode currentMode,
													 const TCHAR* pGlobalCmdName)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentLockModeWillChange called (%s - from %s to %s)\n"),
		pDoc ? pDoc->fileName() : _T("NULL"), modeStr(myCurMode), modeStr(myNewMode) );
	TCHAR *p = new TCHAR[21]; // this will be deleted on the dialog's side
	p[20] = 0;
	_tcscpy_s( p, 20, modeStr(myNewMode) ); 

	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_LOCKMODWILLCHANGE, 0, (LONG_PTR)p );
}


void AsdkDocToModReactor::documentBecameCurrent(AcApDocument* pDoc)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentBecameCurrent   called (%s)\n"), pDoc ? pDoc->fileName() : _T("NULL") );
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_CURRENTDOC, 0, 0L);
}

void AsdkDocToModReactor::documentActivationModified(bool bActivation)
{
	if( !modelessDlg )
		return;

	if (bActivation)
		modelessDlg->SendMessage(WM_ACAD_ACTMODIFIED, 0, 1L); // send a "TRUE"
	else
		modelessDlg->SendMessage(WM_ACAD_ACTMODIFIED, 0, 0L); // send a "FALSE"
}
Adesk::Boolean startDlg()
{
	CAcModuleResourceOverride resOverride;
	HWND hwndAcad = adsw_acadMainWnd();

	if ( !hwndAcad ) 
	{
		AfxMessageBox( _T("Unable to locate AutoCAD parent window.") );
		return Adesk::kFalse;
	}

	CWnd *pWnd = CWnd::FromHandle ( hwndAcad );
	if( modelessDlg == NULL ) 
	{
		if ( (modelessDlg = new CDlgMPlotConfig ( pWnd )) == NULL ) 
		{
			AfxMessageBox ( _T("Unable to allocate a CMDITestDialog.") );
			return Adesk::kFalse;
		}

		BOOL succeeded = modelessDlg->Create ( pWnd );
		if ( !succeeded ) 
		{
			AfxMessageBox ( _T("Unable to create the dialog.") );
			return Adesk::kFalse;
		}
		//gpDocToModReactor =new AsdkDocToModReactor();
	}
	modelessDlg->ShowWindow(SW_SHOWNORMAL);

	return Adesk::kTrue;
}

Adesk::Boolean endDlg()
{
	if ( !modelessDlg )
		return Adesk::kTrue;
	
	//modelessDlg->clear();
	BOOL ok = modelessDlg->DestroyWindow();
	delete modelessDlg;
	modelessDlg = NULL;

	/*delete gpDocToModReactor;
	gpDocToModReactor = NULL;*/
	return ok;
}

extern void OpenDoc( void *pData);

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CBatchPlotApp : public AcRxArxApp {

public:
	CBatchPlotApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		//endDlg();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - BatchPlot._BPlot command (do not rename)
	static void BatchPlotZWPlot(void)//批量打印
	{
		// Add your code for command BatchPlot._BPlot here
		Utility baseFuc;
		int nCmdecho=1;
		baseFuc.GetVar(_T("cmdecho"), &nCmdecho);
		baseFuc.SetVar(_T("cmdecho"), 0);
		int nBackGroundPlot=1;
		baseFuc.GetVar(_T("backgroundplot"), &nBackGroundPlot);
		baseFuc.SetVar(_T("backgroundplot"), 0);
		int nOsmode =1;
		baseFuc.GetVar(_T("osmode"), &nOsmode);
		baseFuc.SetVar(_T("osmode"), 0);
		int nDimzin=1;
		baseFuc.GetVar(_T("dimzin"), &nDimzin);
		baseFuc.SetVar(_T("dimzin"), 8);
		int nhighlight=1;
		baseFuc.GetVar(_T("highlight"), &nhighlight);
		baseFuc.SetVar(_T("highlight"), 1);
		int nucsicon=1;
		baseFuc.GetVar(_T("ucsicon"), &nucsicon);
		baseFuc.SetVar(_T("ucsicon"), 0);

		int nRet = 0;
		CString strResult;
		CString strRootKey = _T("Software\\ZWPLDY");
		DWORD dRes = 0;
		COperateReg Reg;
		nRet = Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, _T("First"), dRes);
		if (nRet == 0)
		{
			//AfxMessageBox(_T("第一次使用，请设置默认设置"));
			int nRet = MessageBox(NULL, _T("第一次使用，请设置默认数据"), _T("警告"), MB_OKCANCEL+MB_ICONQUESTION);
			if (nRet == IDOK)
			{
				CAcModuleResourceOverride rs;
				CDlgPlotConfig dlg(acedGetAcadDwgView(), NULL);
				dlg.DoModal();
			}
			else
			{
				return;
			}
			
		}
		else
		{
			CAcModuleResourceOverride rs;
			CDlgPlotConfig dlg(acedGetAcadDwgView(), NULL);
			dlg.OnBnClickedButtonSelect();
			dlg.OnBnClickedOk();
		}

		

		baseFuc.SetVar(_T("cmdecho"), nCmdecho);
		baseFuc.SetVar(_T("backgroundplot"), nBackGroundPlot);
		baseFuc.SetVar(_T("osmode"), nOsmode);
		baseFuc.SetVar(_T("dimzin"), nDimzin);
		baseFuc.SetVar(_T("highlight"), nhighlight);
		baseFuc.SetVar(_T("ucsicon"), nucsicon);
	}

	// - BatchPlot._ZWMPlot command (do not rename)
	static void BatchPlotZWMPlot(void)//中望多文档打印
	{
		// Add your code for command BatchPlot._ZWMPlot here
		Utility baseFuc;
		int nCmdecho=1;
		baseFuc.GetVar(_T("cmdecho"), &nCmdecho);
		baseFuc.SetVar(_T("cmdecho"), 0);
		int nBackGroundPlot=1;
		baseFuc.GetVar(_T("backgroundplot"), &nBackGroundPlot);
		baseFuc.SetVar(_T("backgroundplot"), 0);
		int nOsmode =1;
		baseFuc.GetVar(_T("osmode"), &nOsmode);
		baseFuc.SetVar(_T("osmode"), 0);
		int nDimzin=1;
		baseFuc.GetVar(_T("dimzin"), &nDimzin);
		baseFuc.SetVar(_T("dimzin"), 8);
		int nhighlight=1;
		baseFuc.GetVar(_T("highlight"), &nhighlight);
		baseFuc.SetVar(_T("highlight"), 1);
		int nucsicon=1;
		baseFuc.GetVar(_T("ucsicon"), &nucsicon);
		baseFuc.SetVar(_T("ucsicon"), 0);

		/*CAcModuleResourceOverride rs;
		CDlgPlotConfig dlg(acedGetAcadDwgView(), NULL, 1);
		dlg.DoModal();*/

		if( modelessDlg ) 
		{
			modelessDlg->ShowWindow( SW_SHOWNORMAL );
			return;
		}

		if( !modelessDlg && !startDlg() ) 
		{
			ads_printf( _T("Failed to create modeless dialog box.\n") );
		}

		baseFuc.SetVar(_T("cmdecho"), nCmdecho);
		baseFuc.SetVar(_T("backgroundplot"), nBackGroundPlot);
		baseFuc.SetVar(_T("osmode"), nOsmode);
		baseFuc.SetVar(_T("dimzin"), nDimzin);
		baseFuc.SetVar(_T("highlight"), nhighlight);
		baseFuc.SetVar(_T("ucsicon"), nucsicon);
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CBatchPlotApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CBatchPlotApp, BatchPlot, ZWPlot, PLDY, ACRX_CMD_TRANSPARENT, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CBatchPlotApp, BatchPlot, ZWMPlot, ZWMPlot, ACRX_CMD_TRANSPARENT, NULL)
