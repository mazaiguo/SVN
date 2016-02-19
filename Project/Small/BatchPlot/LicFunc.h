#pragma once

namespace ZwApp
{
	ZWLIC_API bool CheckLic(LPCTSTR szAppName, LPCTSTR szAppCode, DWORD dwAppNo);
	ZWLIC_API bool IsLicValid(LPCTSTR szAppName, DWORD dwAppNo);
}


//////////////////////////////////////////////////////////////////////////
//
/*
授权验证，有两种情况，均在acrxEntryPoint文件的On_kInitAppMsg中使用。
1. 验证授权是否存在，如果不存在或失效，则输入授权
	使用宏 CHECKLIC()，
	AppFormatName	- 应用名称
	AppCode			- Guid，由部门统一分配，请找杨珊
	AppNo			- 编号，由部门统一分配，请找杨珊

2. 验证授权是否存在，如果不存在，直接失败退出
	使用宏 LIC()
	AppCode			- Guid，由部门统一分配，请找杨珊
	AppNo			- 编号，由部门统一分配，请找杨珊

两个宏均可自动返回错误。
Lisp验证函数 (registerapp)，同样需要3个参数

CHECKLIC和registerapp均会调用输入注册码的操作，所以不要重复调用。

用法示例：
1. CHECKLIC
virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
// 授权验证
CHECKLIC(_T("通信测试"), _T("A0B079C3-6DED-4822-979B-D1D11A6942B5"), 0x40);
return (retCode) ;
}

2. LIC
virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
// 授权验证
LIC(_T("A0B079C3-6DED-4822-979B-D1D11A6942B5"), 0x40);
return (retCode) ;
}

3. registerapp
在lisp文件中：
(setq lic (registerapp "通信测试" "A0B079C3-6DED-4822-979B-D1D11A6942B5" 64))
 (if (= nil lic)
	退出
	)
	

*/
//
//////////////////////////////////////////////////////////////////////////

#define CHECKFAILD(retCode) \
	retCode = AcRx::kRetError; \
	return retCode;

#define CHECKLIC(AppFormatName, AppCode, AppNo) \
	CAcModuleResourceOverride rs; \
	if (!ZwApp::CheckLic(AppFormatName, AppCode, AppNo)) \
		{CHECKFAILD(retCode)}


#define LIC(AppCode, AppNo) \
	if (!ZwApp::IsLicValid(AppCode, AppNo)) \
		{CHECKFAILD(retCode)}