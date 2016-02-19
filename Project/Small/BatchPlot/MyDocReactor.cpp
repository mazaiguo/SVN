#include "MyDocReactor.h"

extern CDlgMPlotConfig* modelessDlg;

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
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 0L);
}

void AsdkDocToModReactor::documentToBeDestroyed(AcApDocument* pDoc)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentToBeDestroyed   called (%s)\n"), pDoc ? pDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 0L);
}

void AsdkDocToModReactor::documentToBeActivated(AcApDocument* pActivatingDoc)
{
	if(!pActivatingDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentToBeActivated   called (%s)\n"), pActivatingDoc ? pActivatingDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 0L);
}

void AsdkDocToModReactor::documentToBeDeactivated(AcApDocument* pDeactivatingDoc)
{
	if(!pDeactivatingDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentToBeDeactivated   called (%s)\n"), pDeactivatingDoc ? pDeactivatingDoc->fileName() : _T("NULL"));
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 0L);
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
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, (LONG_PTR)p );
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
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, (LONG_PTR)p );
}


void AsdkDocToModReactor::documentBecameCurrent(AcApDocument* pDoc)
{
	if(!pDoc)
		return;
	TRACE(_T("#### Docman   AsdkDocToModReactor::documentBecameCurrent   called (%s)\n"), pDoc ? pDoc->fileName() : _T("NULL") );
	if( modelessDlg )
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 0L);
}

void AsdkDocToModReactor::documentActivationModified(bool bActivation)
{
	if( !modelessDlg )
		return;

	if (bActivation)
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 1L); // send a "TRUE"
	else
		modelessDlg->SendMessage(WM_ACAD_UPDATEDIALOG, 0, 0L); // send a "FALSE"
}

