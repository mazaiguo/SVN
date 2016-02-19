#pragma once
#include "DlgMPlotConfig.h"
#include <string.h>
#include "adesk.h"
#include "aced.h"
#include "acdocman.h"
#include "adslib.h"
#include "actrans.h"
#include "rxmfcapi.h"
#include "AcExtensionModule.h"
/////////////////////////////////////////////////////////////////////////
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
