#pragma once
#ifdef WRQ_ENTITY_MODULE //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
class WRQ_ENTITY_DLL CBaseInfoInDwgFileUtils
{
public:
	CBaseInfoInDwgFileUtils(void);
	~CBaseInfoInDwgFileUtils(void);

	//����base�������
	static void setNumCount(CString strCount);
	static CString getNumCount();

	static void SetCurNum(CString strCount);
	static CString getCurNum();

};

