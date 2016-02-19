// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- DlgPlotConfig.h : Declaration of the CDlgPlotConfig
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "vector"
#include "FrameInfo.h"
#include "Utility.h"

#ifndef WM_ACAD_MFC_BASE
#define WM_ACAD_MFC_BASE        (1000)
#endif

// These are used by the document reactor to post messages to the dialog
#ifndef WM_ACAD_UPDATEDIALOG
#define WM_ACAD_UPDATEDIALOG       (WM_ACAD_MFC_BASE + 2)
#endif

#ifndef WM_ACAD_CREATEDOC
#define WM_ACAD_CREATEDOC	       (WM_ACAD_MFC_BASE + 3)
#endif

#ifndef WM_ACAD_DESTROYDOC
#define WM_ACAD_DESTROYDOC	       (WM_ACAD_MFC_BASE + 4)
#endif

#ifndef WM_ACAD_LOCKMODCHANGED
#define WM_ACAD_LOCKMODCHANGED     (WM_ACAD_MFC_BASE + 5)
#endif

#ifndef WM_ACAD_LOCKMODWILLCHANGE
#define WM_ACAD_LOCKMODWILLCHANGE  (WM_ACAD_MFC_BASE + 6)
#endif

#ifndef WM_ACAD_CURRENTDOC
#define WM_ACAD_CURRENTDOC	       (WM_ACAD_MFC_BASE + 7)
#endif

#ifndef WM_ACAD_ACTIVATEDOC
#define WM_ACAD_ACTIVATEDOC	       (WM_ACAD_MFC_BASE + 8)
#endif

#ifndef WM_ACAD_DEACTIVATEDOC
#define WM_ACAD_DEACTIVATEDOC	   (WM_ACAD_MFC_BASE + 9)
#endif

#ifndef WM_ACAD_ACTMODIFIED
#define WM_ACAD_ACTMODIFIED	       (WM_ACAD_MFC_BASE + 10)
#endif

//-----------------------------------------------------------------------------
class CDlgMPlotConfig : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgMPlotConfig)

public:
	CDlgMPlotConfig (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgMPlotConfig();

	BOOL Create( CWnd* pParent );


	enum { IDD = IDD_DLGMPLOTCONFIG} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	afx_msg LRESULT onAcadUpdateDialog ( WPARAM, LPARAM );
	afx_msg LRESULT onAcadCreateDoc ( WPARAM, LPARAM );
	afx_msg LRESULT onAcadDestroyDoc ( WPARAM, LPARAM );
	afx_msg LRESULT onAcadLockModeChanged ( WPARAM, LPARAM );
	afx_msg LRESULT onAcadLockModeWillChange ( WPARAM, LPARAM );
	afx_msg LRESULT onAcadCurrDoc ( WPARAM, LPARAM );
	afx_msg LRESULT onAcadActivateDoc ( WPARAM, LPARAM );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioLayer();
	afx_msg void OnBnClickedRadioBlock();
	afx_msg void OnBnClickedRadioSanxian();
	afx_msg void OnCbnSelchangeComboDevice();
	afx_msg void OnCbnSelchangeComboMedia();
	afx_msg void OnCbnSelchangeComboPlotsytle();
	afx_msg void OnBnClickedCheckFit();
	afx_msg void OnBnClickedCheckCenter();
	afx_msg void OnBnClickedRadioPlot();
	afx_msg void OnBnClickedRadioLayerout();
	afx_msg void OnBnClickedRadioAutorotate();
	afx_msg void OnBnClickedRadioHengxiang();
	afx_msg void OnBnClickedRadioZongxiang();
	afx_msg void OnBnClickedRadioOrder();
	afx_msg void OnBnClickedRadioLefttoright();
	afx_msg void OnBnClickedRadioUptodown();
	afx_msg void OnBnClickedCheckReverseorder();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonBright();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedButtonPreview();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPickbtn();
	afx_msg void OnBnClickedButtonBlockname();
	afx_msg void OnBnClickedButtonLayername();
	virtual BOOL OnInitDialog();
	void RefreshMediaNames();
	afx_msg void OnDestroy();
	void RefreshDevices();
	void RefreshCTBNames();
	afx_msg void OnClose();
	void clear();
	//afx_msg void OnBnClickedRadioPlt();

private:

	//std::vector<FrameInfo*> m_frames;
	FrameInfo* GetFrame(int nIndex, CString strFileName);//获取某一个图框

	std::map<CString, std::vector<FrameInfo*> > m_MultiFrames;//多文档的信息

	int GetFramecount();//获取图框的总个数
	void ClearAll();

	void OrderFrame(int nSelect);
	bool isPaperWidthLessHeight() const;
	bool isWHSwap() const;

	bool GetInfoFromReg();//从注册表中取信息
	void SaveInfoToReg();//将信息保存到注册表中

	CString GetStringFromReg(LPCTSTR strKey);
	int GetDwordFromReg(LPCTSTR strKey) const;

	CString GetDeFaultPaperSize() const;//获取cad中当前默认的图纸纸张
	
	CString GetPaperSize() const;//获取批量打印设置的图纸纸张
	CString GetMediaName() const;//获取批量打印设置的打印机名
	CString GetCtbName() const;//获取批量打印设置的打印样式名
	int GetSelectMode() const;//获取批量打印设置的选择图框模式
	BOOL GetFitMode() const;
	BOOL GetCenterMode() const;
	int GetPlotMode() const;
	int GetOrderMode() const;
	BOOL GetReserveMode() const;
	CString GetDeFaultPath() const;
	CString GetDefaultFileName() const;
	CString GetBlockName() const;
	CString GetLayerName()const;
	double GetDeFaultScale() const;
	int GetPlotCount() const;
	CString GetXOffsetText() const;
	CString GetYOffsetText() const;
	int GetPrecisionMode() const;

	CString m_LocalMedia;
	CComboBox m_CombDevice;
	CString m_strCurDevice;
	CComboBox m_CombMedia;
	CString m_strCurMedia;
	CComboBox m_CombPlotStyle;
	CString m_strCurPlotStyle;
	CSpinButtonCtrl m_spin;
	// 图框形式 0为图层 1为图块 2为散线 3为整个文件
	int m_nFrame;
	BOOL m_bIsFit;
	BOOL m_bIsCenter;
	// 打印输出方式
	int m_nPlot;
	int m_nDirection;
	// 选择顺序方式
	int m_nOrder;
	BOOL m_bIsReserve;
	CString m_strDeFaultPath;
	CString m_strDeFaultFileName;

	Adesk::Boolean m_bISModelType;//whether a named plot setup is a model space type or a paperspace (layout) type
	int m_bPlotToFile;//whether plot to a file or not or Must plot to a file;
	AcDbPlotSettingsValidator* pPSV;
	AcDbPlotSettings *pPlotSettings;
	AcDbObjectId layoutId;

	CStringArray m_strLayerOutArr;//获取的layerOut名字集
	int m_nMultiDoc;//是否多文档，0为单文档，1为多文档
	CStringArray m_strMultiDocNameArr;//多文档文档集
public:
	CString m_strBlockName;
	CString m_strLayerName;

	double m_dScale;
	int m_nCount;
	CString m_strXOffset;
	CString m_strYOffset;
	int m_nPrecision;

	/*afx_msg void OnBnClickedRadioHigh();
	afx_msg void OnBnClickedRadioMiddle();
	afx_msg void OnBnClickedRadioLow();*/
	BOOL m_bIsPlotToFile;
	afx_msg void OnBnClickedCheckPlottofile();
	//virtual BOOL DestroyWindow();
	//afx_msg void OnBnClickedCancel();
	//afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
} ;
