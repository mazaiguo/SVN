//-----------------------------------------------------------------------------
//----- EditZhuangHao.h : Declaration of the CEditZhuangHao
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "Resource.h"
#include "ZdmDataInfo.h"
//-----------------------------------------------------------------------------
#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif
class ZDM_DLL_API CDlgEditZhuangHao : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgEditZhuangHao)

public:
	CDlgEditZhuangHao (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_EDITZHUANGHAO} ;
	void setZDMData(CZdmDataInfo* );

	CZdmDataInfo getData();
protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	CZdmDataInfo m_pZDM;
} ;
