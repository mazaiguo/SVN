

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jun 23 10:41:03 2016
 */
/* Compiler settings for zwcadm_com.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __zwcadm_com_i_h__
#define __zwcadm_com_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICZwcadmCom_FWD_DEFINED__
#define __ICZwcadmCom_FWD_DEFINED__
typedef interface ICZwcadmCom ICZwcadmCom;
#endif 	/* __ICZwcadmCom_FWD_DEFINED__ */


#ifndef __CZwcadmCom_FWD_DEFINED__
#define __CZwcadmCom_FWD_DEFINED__

#ifdef __cplusplus
typedef class CZwcadmCom CZwcadmCom;
#else
typedef struct CZwcadmCom CZwcadmCom;
#endif /* __cplusplus */

#endif 	/* __CZwcadmCom_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICZwcadmCom_INTERFACE_DEFINED__
#define __ICZwcadmCom_INTERFACE_DEFINED__

/* interface ICZwcadmCom */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICZwcadmCom;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DCCEF66A-ACC1-4ED1-851F-FDDC5E70BA86")
    ICZwcadmCom : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICZwcadmComVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICZwcadmCom * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICZwcadmCom * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICZwcadmCom * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICZwcadmCom * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICZwcadmCom * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICZwcadmCom * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICZwcadmCom * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICZwcadmComVtbl;

    interface ICZwcadmCom
    {
        CONST_VTBL struct ICZwcadmComVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICZwcadmCom_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICZwcadmCom_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICZwcadmCom_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICZwcadmCom_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICZwcadmCom_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICZwcadmCom_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICZwcadmCom_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICZwcadmCom_INTERFACE_DEFINED__ */



#ifndef __zwcadm_comLib_LIBRARY_DEFINED__
#define __zwcadm_comLib_LIBRARY_DEFINED__

/* library zwcadm_comLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_zwcadm_comLib;

EXTERN_C const CLSID CLSID_CZwcadmCom;

#ifdef __cplusplus

class DECLSPEC_UUID("CE33CDA5-6D9E-442D-9372-D84D49EF6467")
CZwcadmCom;
#endif
#endif /* __zwcadm_comLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


