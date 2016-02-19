
#include "stdafx.h
#include "BaseFunc.h"

CBaseFunc::CBaseFunc(void)
{
}

CBaseFunc::~CBaseFunc(void)
{
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::SetVar(LPCTSTR szVName, ads_real rArg)
{
	struct resbuf res;
	res.restype = RTREAL;
	res.resval.rreal   = rArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::SetVar(LPCTSTR szVName, ads_point pArg)
{
	struct resbuf res;
	res.restype = RT3DPOINT;
	memcpy(&res.resval.rpoint, &pArg, sizeof(res.resval.rpoint));
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::SetVar(LPCTSTR szVName, int nArg)
{
	struct resbuf res;
	res.restype = RTSHORT;
	res.resval.rint  = nArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::SetVar(LPCTSTR szVName, LPCTSTR szArg)
{
	ASSERT(AfxIsValidString(szArg));
	struct resbuf res;
	res.restype = RTSTR;
	res.resval.rstring =(LPTSTR) szArg;
	return ads_setvar(szVName, &res)== RTNORM;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::GetVar(LPCTSTR var, ads_real *rRes)
{
	ASSERT(rRes!=NULL);
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM)
		return FALSE;
	if (v.restype != RTREAL) 
		return FALSE;
	*rRes = v.resval.rreal;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::GetVar(LPCTSTR var, ads_point *pRes)
{
	ASSERT(pRes != NULL);
	struct resbuf v;
	if(ads_getvar(var, &v) !=RTNORM)
		return FALSE;
	if (v.restype != RT3DPOINT && v.restype != RTPOINT)
		return FALSE;
	ads_point_set(v.resval.rpoint, *pRes);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::GetVar(LPCTSTR var, int *nRes)
{
	ASSERT(nRes!=NULL);
	struct resbuf v;
	if(ads_getvar(var, &v)!=RTNORM)
		return FALSE;
	if (v.restype!=RTSHORT ) 
		return FALSE;
	*nRes = v.resval.rint;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CBaseFunc::GetVar(LPCTSTR var, LPTSTR sRes)
{
	struct resbuf v;

	if(ads_getvar(var, &v)!=RTNORM || v.restype!=RTSTR ) 
		return FALSE;
	_tcscpy(sRes, v.resval.rstring);
	ads_free (v.resval.rstring);	
	return TRUE;
}