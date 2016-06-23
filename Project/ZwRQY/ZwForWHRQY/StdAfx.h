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
#define JC_DICT		_T("ZW_FOR_WHRQY_JC_")//�ϰ���
#define JC_DICT_ZA		_T("ZW_FOR_WHRQY_JC_ZA")//�޸��˹ܵ����ϰ���
#define FM_DICT _T("ZW_FOR_WHRQY_FM_")//����
#define WP_DICT _T("OBJECT_TO_NOTIFY_")
#define NSG_DICT _T("ZW_FOR_WHRQY_LSG_")//��ˮ��
#define JSD_DICT	_T("ZW_FOR_WHRQY_JSD")//��ʾ��
#define GG_DICT		_T("ZW_FOR_WHRQY_GG")//�ܹ�
#define TG_DICT		_T("ZW_FOR_WHRQY_TG")//�׹�

//////////////////////////////////////////////////////////////////////////
//xdata
#define ZDM_DESIGNDMX						_T("��Ƶ�����")
#define ZDM_REALDMX							_T("ʵ�ʵ�����")
#define ZDM_JIEDIAN							_T("�ڵ�")
#define ZDM_PIPETYPE						_T("�ܵ�����")
#define ZDM_CURDATA							_T("��ǰ׮��")
#define ZDM_GUANDI							_T("�ܵ�")
#define ZDM_WASHEN							_T("����")
#define ZDM_PODU							_T("�¶�")
#define	ZDM_JULI							_T("����")
#define ZDM_DESINGDMXS						_T("�ڶ�����Ƶ�����")
#define ZDM_REALS							_T("�ڶ���ʵ�ʵ�����")
#define ZDM_PIPEDIAMETER					_T("�ܾ�")
#define ZDM_JC_ADD							_T("�ϰ�����ܵ��ཻ")

//double m_dDesignDmx;//��Ƶ�����
//double m_dRealDmx;//ʵ�ʵ�����
//CString m_strJiedian;//�ڵ�
//CString m_strPipeType;//�ܵ�����
//double m_dcurData;//��ǰ׮������
//double m_dGuanDi;//�ܵ�
//double m_dWaShen;//����
//double m_dPoDu;//�¶�
//double m_dJuli;//����
////////////////////////////////////////////////////////////////////////////
////��ͬһ��͹��
//bool   m_bHasBulge;//�Ƿ���͹��
//double m_dDesignDmxS;//�ڶ�����Ƶ�����
//double m_dRealDmxS;//�ڶ���ʵ�ʵ�����
//double m_dJiedianS;
#define XDT_XG_CSTRING			20000
#define XDT_XG_INT				20001
#define XDT_XG_DOUBLE			20002
#define XDT_XG_HANDLE			20003
#define XDT_XG_POINT			20004

//- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;

#pragma pack (pop)

