#pragma once

namespace ZwApp
{
	ZWLIC_API bool CheckLic(LPCTSTR szAppName, LPCTSTR szAppCode, DWORD dwAppNo);
	ZWLIC_API bool IsLicValid(LPCTSTR szAppName, DWORD dwAppNo);
}


//////////////////////////////////////////////////////////////////////////
//
/*
��Ȩ��֤�����������������acrxEntryPoint�ļ���On_kInitAppMsg��ʹ�á�
1. ��֤��Ȩ�Ƿ���ڣ���������ڻ�ʧЧ����������Ȩ
	ʹ�ú� CHECKLIC()��
	AppFormatName	- Ӧ������
	AppCode			- Guid���ɲ���ͳһ���䣬������ɺ
	AppNo			- ��ţ��ɲ���ͳһ���䣬������ɺ

2. ��֤��Ȩ�Ƿ���ڣ���������ڣ�ֱ��ʧ���˳�
	ʹ�ú� LIC()
	AppCode			- Guid���ɲ���ͳһ���䣬������ɺ
	AppNo			- ��ţ��ɲ���ͳһ���䣬������ɺ

����������Զ����ش���
Lisp��֤���� (registerapp)��ͬ����Ҫ3������

CHECKLIC��registerapp�����������ע����Ĳ��������Բ�Ҫ�ظ����á�

�÷�ʾ����
1. CHECKLIC
virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
// ��Ȩ��֤
CHECKLIC(_T("ͨ�Ų���"), _T("A0B079C3-6DED-4822-979B-D1D11A6942B5"), 0x40);
return (retCode) ;
}

2. LIC
virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
// ��Ȩ��֤
LIC(_T("A0B079C3-6DED-4822-979B-D1D11A6942B5"), 0x40);
return (retCode) ;
}

3. registerapp
��lisp�ļ��У�
(setq lic (registerapp "ͨ�Ų���" "A0B079C3-6DED-4822-979B-D1D11A6942B5" 64))
 (if (= nil lic)
	�˳�
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