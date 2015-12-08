// CZwcadmCom.h : CCZwcadmCom 的声明

#pragma once
#include "resource.h"       // 主符号



#include "zwcadm_com_i.h"
#import "ZwmPEBridgePlugins.dll" no_namespace raw_interfaces_only, raw_native_types, no_namespace, named_guids


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CCZwcadmCom

class ATL_NO_VTABLE CCZwcadmCom :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCZwcadmCom, &CLSID_CZwcadmCom>,
	public IDispatchImpl<ICZwcadmCom, &IID_ICZwcadmCom, &LIBID_zwcadm_comLib, /*wMajor =*/ 1, /*wMinor =*/ 0>, 
	public IProjectFrameworkAddin
{
public:
	CCZwcadmCom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CZWCADMCOM)


BEGIN_COM_MAP(CCZwcadmCom)
	COM_INTERFACE_ENTRY(ICZwcadmCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IProjectFrameworkAddin)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IProjectFrameworkAddin
	STDMETHOD(CallTitleMenuFunction)(BSTR bstrCommandName, IPETitleCollection** ppPETitleCollection, BSTR bstrFileName, BSTR* bstrErrorMsg, long *lSucceeded);
	STDMETHOD(CallBomMenuFunction)(BSTR bstrCommandName, BSTR FileName, IPETitleCollection** ppPETitleCollection, IPEBomCollections** ppPEBomCollections, BSTR* ErrorMsg, long *lSucceeded);


};

OBJECT_ENTRY_AUTO(__uuidof(CZwcadmCom), CCZwcadmCom)
