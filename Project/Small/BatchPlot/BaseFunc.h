#pragma once


class CBaseFunc
{
public:
	CBaseFunc(void);
	~CBaseFunc(void);

	BOOL GetVar(LPCTSTR var, int *nRes);
	BOOL GetVar(LPCTSTR var, ads_real *rRes);
	BOOL GetVar(LPCTSTR var, ads_point *pRes);
	BOOL GetVar(LPCTSTR var, LPTSTR sRes);
	BOOL SetVar(LPCTSTR szVarName, int       nArg);
	BOOL SetVar(LPCTSTR szVarName, ads_real  rArg);
	BOOL SetVar(LPCTSTR szVarName, ads_point pArg);
	BOOL SetVar(LPCTSTR szVarName, LPCTSTR   sArg);
};
