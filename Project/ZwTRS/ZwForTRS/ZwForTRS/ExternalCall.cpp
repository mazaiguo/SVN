#include "StdAfx.h"
#include "ExternalCall.h"
#include "DlgSQLDB.h"
//
//CExternalCall::CExternalCall(void)
//{
//}
//
//
//CExternalCall::~CExternalCall(void)
//{
//}
//
//
//CDataBaseInfo CExternalCall::GetData()
//{
//	CDataBaseInfo InFo;
//	CAcModuleResourceOverride rs;
//	CDlgSQLDB dlg;
//	if (dlg.DoModal() == IDOK)
//	{
//		InFo = dlg.GetData();
//	}
//	return InFo;
//}

bool GetData()
{
	bool bRet = false;
	CDataBaseInfo InFo;
	CAcModuleResourceOverride rs;
	CDlgSQLDB dlg;
	if (dlg.DoModal() == IDOK)
	{
		
		return true;
	}
	else
	{
		return false;
	}
}