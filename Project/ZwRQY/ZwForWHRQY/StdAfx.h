//-----------------------------------------------------------------------------
//- StdAfx.h : include file for standard system include files,
//-      or project specific include files that are used frequently,
//-      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

#pragma pack (push, 8)
#pragma warning(disable: 4786 4996)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------



//- ObjectARX and OMF headers needs this
#include <map>

//-----------------------------------------------------------------------------
#include <afxwin.h>				//- MFC core and standard components
#include <afxext.h>				//- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//- MFC OLE classes
#include <afxodlgs.h>			//- MFC OLE dialog classes
#include <afxdisp.h>			//- MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//- MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//- MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//- MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//-----------------------------------------------------------------------------
//- Include ObjectDBX/ObjectARX headers
//- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_			//- Support for the BRep API
//#define _HLR_SUPPORT_				//- Support for the Hidden Line Removal API
//#define _AMODELER_SUPPORT_		//- Support for the AModeler API
//#define _ASE_SUPPORT_				//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_			//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_		//- Include legacy headers in this project
#include "arxHeaders.h"

//-----------------------------------------------------------------------------
#include "DocData.h" //- Your document specific data class holder

#include "MyBase.h"
#include "Global.h"
#define BC_DICT _T("ZW_FOR_WHRQY_")
#define BC_DICT_GD _T("ZW_FOR_WHRQY_GD_")
#define JC_DICT		_T("ZW_FOR_WHRQY_JC_")//障碍物
#define JC_DICT_ZA		_T("ZW_FOR_WHRQY_JC_ZA")//修改了管道的障碍物
#define FM_DICT _T("ZW_FOR_WHRQY_FM_")//阀门
#define WP_DICT _T("OBJECT_TO_NOTIFY_")
#define NSG_DICT _T("ZW_FOR_WHRQY_LSG_")//凌水缸
#define JSD_DICT	_T("ZW_FOR_WHRQY_JSD")//警示带
#define GG_DICT		_T("ZW_FOR_WHRQY_GG")//管沟
#define TG_DICT		_T("ZW_FOR_WHRQY_TG")//套管

//////////////////////////////////////////////////////////////////////////
//xdata
#define ZDM_DESIGNDMX						_T("设计地面线")
#define ZDM_REALDMX							_T("实际地面线")
#define ZDM_JIEDIAN							_T("节点")
#define ZDM_PIPETYPE						_T("管道类型")
#define ZDM_CURDATA							_T("当前桩号")
#define ZDM_GUANDI							_T("管底")
#define ZDM_WASHEN							_T("挖深")
#define ZDM_PODU							_T("坡度")
#define	ZDM_JULI							_T("距离")
#define ZDM_DESINGDMXS						_T("第二个设计地面线")
#define ZDM_REALS							_T("第二个实际地面线")
#define ZDM_PIPEDIAMETER					_T("管径")
#define ZDM_JC_ADD							_T("障碍物与管道相交")

//double m_dDesignDmx;//设计地面线
//double m_dRealDmx;//实际地面线
//CString m_strJiedian;//节点
//CString m_strPipeType;//管道类型
//double m_dcurData;//当前桩号数据
//double m_dGuanDi;//管底
//double m_dWaShen;//挖深
//double m_dPoDu;//坡度
//double m_dJuli;//距离
////////////////////////////////////////////////////////////////////////////
////在同一处凸起
//bool   m_bHasBulge;//是否有凸起
//double m_dDesignDmxS;//第二个设计地面线
//double m_dRealDmxS;//第二个实际地面线
//double m_dJiedianS;
#define XDT_XG_CSTRING			20000
#define XDT_XG_INT				20001
#define XDT_XG_DOUBLE			20002
#define XDT_XG_HANDLE			20003
#define XDT_XG_POINT			20004

//- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;

#pragma pack (pop)

