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
//----- DlgPlotConfig.cpp : Implementation of CDlgMPlotConfig
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgMPlotConfig.h"
#include "axlock.h"
#include "DlgBlockName.h"
#include "DlgListLayerName.h"
#include "JudgeFrame.h"
#include "OperateReg.h"
#include "DlgListLayerout.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgMPlotConfig, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgMPlotConfig, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_RADIO_LAYER, &CDlgMPlotConfig::OnBnClickedRadioLayer)
	ON_BN_CLICKED(IDC_RADIO_BLOCK, &CDlgMPlotConfig::OnBnClickedRadioBlock)
	ON_BN_CLICKED(IDC_RADIO_SANXIAN, &CDlgMPlotConfig::OnBnClickedRadioSanxian)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, &CDlgMPlotConfig::OnCbnSelchangeComboDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_MEDIA, &CDlgMPlotConfig::OnCbnSelchangeComboMedia)
	ON_CBN_SELCHANGE(IDC_COMBO_PLOTSYTLE, &CDlgMPlotConfig::OnCbnSelchangeComboPlotsytle)
	ON_BN_CLICKED(IDC_CHECK_FIT, &CDlgMPlotConfig::OnBnClickedCheckFit)
	ON_BN_CLICKED(IDC_CHECK_CENTER, &CDlgMPlotConfig::OnBnClickedCheckCenter)
	ON_BN_CLICKED(IDC_RADIO_PLOT, &CDlgMPlotConfig::OnBnClickedRadioPlot)
	ON_BN_CLICKED(IDC_RADIO_LAYEROUT, &CDlgMPlotConfig::OnBnClickedRadioLayerout)
	ON_BN_CLICKED(IDC_RADIO_AUTOROTATE, &CDlgMPlotConfig::OnBnClickedRadioAutorotate)
	ON_BN_CLICKED(IDC_RADIO_HENGXIANG, &CDlgMPlotConfig::OnBnClickedRadioHengxiang)
	ON_BN_CLICKED(IDC_RADIO_ZONGXIANG, &CDlgMPlotConfig::OnBnClickedRadioZongxiang)
	ON_BN_CLICKED(IDC_RADIO_ORDER, &CDlgMPlotConfig::OnBnClickedRadioOrder)
	ON_BN_CLICKED(IDC_RADIO_LEFTTORIGHT, &CDlgMPlotConfig::OnBnClickedRadioLefttoright)
	ON_BN_CLICKED(IDC_RADIO_UPTODOWN, &CDlgMPlotConfig::OnBnClickedRadioUptodown)
	ON_BN_CLICKED(IDC_CHECK_REVERSEORDER, &CDlgMPlotConfig::OnBnClickedCheckReverseorder)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CDlgMPlotConfig::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_BRIGHT, &CDlgMPlotConfig::OnBnClickedButtonBright)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CDlgMPlotConfig::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CDlgMPlotConfig::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDOK, &CDlgMPlotConfig::OnBnClickedOk)
	ON_BN_CLICKED(PICKBTN, &CDlgMPlotConfig::OnBnClickedPickbtn)
	ON_BN_CLICKED(IDC_BUTTON_BLOCKNAME, &CDlgMPlotConfig::OnBnClickedButtonBlockname)
	ON_BN_CLICKED(IDC_BUTTON_LAYERNAME, &CDlgMPlotConfig::OnBnClickedButtonLayername)
	ON_BN_CLICKED(IDC_CHECK_PLOTTOFILE, &CDlgMPlotConfig::OnBnClickedCheckPlottofile)
	//ON_BN_CLICKED(IDCANCEL, &CDlgMPlotConfig::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgMPlotConfig::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgMPlotConfig::OnBnClickedButtonCancel)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDlgMPlotConfig::CDlgMPlotConfig (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgMPlotConfig::IDD, pParent, hInstance) 
, m_bIsPlotToFile(FALSE)
{
	m_nMultiDoc = 0;
	GetInfoFromReg();
	ClearAll();
	m_strLayerOutArr.RemoveAll();
	m_strMultiDocNameArr.RemoveAll();
}

CDlgMPlotConfig::~CDlgMPlotConfig()
{
	if (pPlotSettings!=NULL)
	{
		delete pPlotSettings;
	}
	ClearAll();
	m_strLayerOutArr.RemoveAll();
	m_strMultiDocNameArr.RemoveAll();
}
void CDlgMPlotConfig::clear()
{
	m_strLayerOutArr.RemoveAll();
	m_strMultiDocNameArr.RemoveAll();
}

BOOL CDlgMPlotConfig::Create( CWnd* pParent )
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(CDlgMPlotConfig::IDD, pParent);
}
//-----------------------------------------------------------------------------
void CDlgMPlotConfig::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_CombDevice);
	DDX_CBString(pDX, IDC_COMBO_DEVICE, m_strCurDevice);
	DDX_Control(pDX, IDC_COMBO_MEDIA, m_CombMedia);
	DDX_CBString(pDX, IDC_COMBO_MEDIA, m_strCurMedia);
	DDX_Control(pDX, IDC_COMBO_PLOTSYTLE, m_CombPlotStyle);
	DDX_CBString(pDX, IDC_COMBO_PLOTSYTLE, m_strCurPlotStyle);
	DDX_Control(pDX, IDC_SPIN_COUNT, m_spin);
	DDX_Radio(pDX, IDC_RADIO_LAYER, m_nFrame);
	DDX_Check(pDX, IDC_CHECK_FIT, m_bIsFit);
	DDX_Check(pDX, IDC_CHECK_CENTER, m_bIsCenter);
	DDX_Radio(pDX, IDC_RADIO_PLOT, m_nPlot);
	DDX_Radio(pDX, IDC_RADIO_AUTOROTATE, m_nDirection);
	DDX_Radio(pDX, IDC_RADIO_ORDER, m_nOrder);
	DDX_Check(pDX, IDC_CHECK_REVERSEORDER, m_bIsReserve);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strDeFaultPath);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strDeFaultFileName);
	DDX_Text(pDX, IDC_EDIT_BLOCKNAME, m_strBlockName);
	DDX_Text(pDX, IDC_EDIT_LAYERNAME, m_strLayerName);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_dScale);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_XOFFSET, m_strXOffset);
	DDX_Text(pDX, IDC_EDIT_YOFFSET, m_strYOffset);
	DDX_Check(pDX, IDC_CHECK_PLOTTOFILE, m_bIsPlotToFile);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDlgMPlotConfig::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgMPlotConfig::OnBnClickedRadioLayer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFrame = 0;
	GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(PICKBTN)->EnableWindow(TRUE);
}

void CDlgMPlotConfig::OnBnClickedRadioBlock()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFrame = 1;
	GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(TRUE);
	GetDlgItem(PICKBTN)->EnableWindow(TRUE);
}

void CDlgMPlotConfig::OnBnClickedRadioSanxian()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFrame = 2;
	GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(PICKBTN)->EnableWindow(FALSE);
}

void CDlgMPlotConfig::RefreshMediaNames()
{
	AcArray<const TCHAR*> MediaList;

	m_CombMedia.ResetContent();

	pPSV->canonicalMediaNameList(pPlotSettings,MediaList);


	for(int c=0;c<MediaList.length();c++)
		m_CombMedia.AddString(MediaList[c]);
	
	int nCur = m_CombMedia.FindString(0,(CString)m_strCurMedia);
	if (nCur == -1)
	{
		//nCur = 0;
		const TCHAR *pCurName;
		pPlotSettings->getCanonicalMediaName(pCurName);
		m_CombMedia.SetCurSel(m_CombMedia.SelectString(0,(CString)pCurName));
	}
	else
	{
		m_CombMedia.SetCurSel(nCur);
	}
	//m_CombMedia.UpdateData();
}
void CDlgMPlotConfig::OnCbnSelchangeComboDevice()
{
	// TODO: 在此添加控件通知处理程序代码
	AcAxDocLock lock;

	m_CombDevice.GetLBText(m_CombDevice.GetCurSel(),m_strCurDevice);

	Acad::ErrorStatus es=pPSV->setPlotCfgName(pPlotSettings,m_strCurDevice);
	pPSV->refreshLists(pPlotSettings);
	RefreshMediaNames();
	AcPlPlotConfig *pPlotConfig =  NULL;
	if(Acad::eOk == acplPlotConfigManagerPtr()->setCurrentConfig(pPlotConfig,m_strCurDevice))
	{
		if(pPlotConfig->plotToFileCapability()==AcPlPlotConfig::kNoPlotToFile)
		{
			m_bPlotToFile = 0;
			GetDlgItem(IDC_CHECK_PLOTTOFILE)->EnableWindow(FALSE);
		}
		else if (pPlotConfig->plotToFileCapability() == AcPlPlotConfig::kPlotToFileAllowed)
		{
			m_bPlotToFile = 1;
			GetDlgItem(IDC_CHECK_PLOTTOFILE)->EnableWindow(TRUE);
			OnBnClickedCheckPlottofile();
		}
		else if (pPlotConfig->plotToFileCapability() == AcPlPlotConfig::kMustPlotToFile)
		{
			m_bPlotToFile = 2;
			CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_PLOTTOFILE);
			pBtn->SetCheck(BST_CHECKED);
			GetDlgItem(IDC_CHECK_PLOTTOFILE)->EnableWindow(FALSE);
			OnBnClickedCheckPlottofile();
		}	
	}
}

void CDlgMPlotConfig::OnCbnSelchangeComboMedia()
{
	// TODO: 在此添加控件通知处理程序代码
	AcAxDocLock lock;

	m_CombMedia.UpdateData();
	m_CombMedia.GetLBText(m_CombMedia.GetCurSel(),m_strCurMedia);

	Acad::ErrorStatus es=pPSV->setCanonicalMediaName(pPlotSettings,m_strCurMedia);
}

void CDlgMPlotConfig::OnCbnSelchangeComboPlotsytle()
{
	// TODO: 在此添加控件通知处理程序代码
	AcAxDocLock lock;

	//m_CombPlotStyle.UpdateData();
	if (m_strCurPlotStyle.CompareNoCase(_T("")) == 0)
	{
		m_CombPlotStyle.GetLBText(0, m_strCurPlotStyle);
		m_CombPlotStyle.SetCurSel(0);
	}
	else
	{
		int nSel = m_CombPlotStyle.GetCurSel();
		if (nSel == -1)
		{
			nSel = 0;
		}
		m_CombPlotStyle.GetLBText(nSel,m_strCurPlotStyle);
	}
	Acad::ErrorStatus es = pPlotSettings->setPlotSettingsName(m_strCurPlotStyle);
}
void CDlgMPlotConfig::OnBnClickedCheckFit()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_FIT);
	m_bIsFit = (BOOL)pBtn->GetCheck();
	if (m_bIsFit)
	{
		GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SCALE)->EnableWindow(FALSE);
		Acad::ErrorStatus es = pPSV->setUseStandardScale(pPlotSettings,Adesk::kTrue);
		es = pPSV->setStdScaleType(pPlotSettings,AcDbPlotSettings::kScaleToFit);
		es = pPSV->setPlotType(pPlotSettings,AcDbPlotSettings::kExtents);
		es = pPSV->setPlotCentered(pPlotSettings,true);
		double numerator,denominator;
		es = pPlotSettings->getCustomPrintScale(numerator,denominator);
		m_dScale = numerator/denominator;
		CString strTmp;
		strTmp.Format(_T("%f"), m_dScale);
		GetDlgItem(IDC_EDIT_SCALE)->SetWindowText(strTmp);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SCALE)->EnableWindow(TRUE);
	}
}

void CDlgMPlotConfig::OnBnClickedCheckCenter()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_CENTER);
	m_bIsCenter = (BOOL)pBtn->GetCheck();
	GetDlgItem(IDC_EDIT_XPOSITION)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_EDIT_YPOSITION)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_EDIT_XOFFSET)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_EDIT_YOFFSET)->EnableWindow(!m_bIsCenter);
	double dXOffset,dYOffset;
	if (m_bIsCenter)
	{
		if (isWHSwap())
		{		
			pPlotSettings->getPlotOrigin(dYOffset, dXOffset);
		}
		else
		{	
			pPlotSettings->getPlotOrigin(dXOffset, dYOffset);
		}
		m_strXOffset.Format(_T("%f"), dXOffset);
		m_strYOffset.Format(_T("%f"), dYOffset);
		GetDlgItem(IDC_EDIT_XOFFSET)->SetWindowText(m_strXOffset);
		GetDlgItem(IDC_EDIT_YOFFSET)->SetWindowText(m_strYOffset);
	}

}

void CDlgMPlotConfig::OnBnClickedRadioPlot()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPlot = 0;
	GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);

	if (m_nFrame == 0)
	{
		OnBnClickedRadioLayer();
	}
	else if (m_nFrame == 1)
	{
		OnBnClickedRadioBlock();
	}
	else if (m_nFrame == 2)
	{
		OnBnClickedRadioSanxian();
	}

	GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(!m_bIsFit);
	GetDlgItem(IDC_STATIC_SCALE)->EnableWindow(!m_bIsFit);

	GetDlgItem(IDC_EDIT_XPOSITION)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_EDIT_YPOSITION)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_EDIT_XOFFSET)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_EDIT_YOFFSET)->EnableWindow(!m_bIsCenter);
	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_AUTOROTATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_HENGXIANG)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_ZONGXIANG)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_ORDER)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LEFTTORIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_UPTODOWN)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_FIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_REVERSEORDER)->EnableWindow(TRUE);
}

void CDlgMPlotConfig::OnBnClickedRadioLayerout()
{
	m_nPlot = 1;
	CAcModuleResourceOverride rs;
	CDlgListLayerout dlg;
	if (dlg.DoModal() == IDOK)
	{
		dlg.getDwgLayerOutName(m_strLayerOutArr);
		int nCount = m_strLayerOutArr.GetSize();
		if (nCount >0)
		{
			CString strTmp;
			strTmp.Format(_T("%d"), nCount);
			GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strTmp);
			GetDlgItem(IDC_BUTTON_PREVIEW)->EnableWindow(TRUE);
		}
		if (m_bPlotToFile == 0)
		{	
			GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_EDIT_XPOSITION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_YPOSITION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_XOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_YOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SCALE)->EnableWindow(FALSE);	
		GetDlgItem(IDC_BUTTON_BRIGHT)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(FALSE);			
		GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(FALSE);
		GetDlgItem(PICKBTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOROTATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HENGXIANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ZONGXIANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ORDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LEFTTORIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_UPTODOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_REVERSEORDER)->EnableWindow(FALSE);
	}
}
void CDlgMPlotConfig::OnBnClickedRadioAutorotate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDirection = 0;
}

void CDlgMPlotConfig::OnBnClickedRadioHengxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDirection = 1;
	Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
	OnBnClickedCheckFit();
	OnBnClickedCheckCenter();
}

void CDlgMPlotConfig::OnBnClickedRadioZongxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDirection = 2;
	Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
	OnBnClickedCheckFit();
	OnBnClickedCheckCenter();
}

void CDlgMPlotConfig::OnBnClickedRadioOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOrder = 0;
}

void CDlgMPlotConfig::OnBnClickedRadioLefttoright()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOrder = 1;
}

void CDlgMPlotConfig::OnBnClickedRadioUptodown()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOrder = 2;
}

void CDlgMPlotConfig::OnBnClickedCheckReverseorder()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_REVERSEORDER);
	m_bIsReserve = (BOOL)pBtn->GetCheck();
}

void CDlgMPlotConfig::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	CString strRes;
	if (m_bPlotToFile == 0)
	{
		strRes.LoadString(IDS_STRING_OUT3);
		AfxMessageBox(strRes);
		return;
	}
	if (m_strCurMedia.CompareNoCase(_T(""))==0)
	{
		strRes.LoadString(IDS_STRING_MEDIASIZE);
		AfxMessageBox(strRes);
		return;
	}
	GetDlgItem(IDC_EDIT_LAYERNAME)->GetWindowText(m_strLayerName);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->GetWindowText(m_strBlockName);
	int nCout = 0;
	//BeginEditorCommand();

	Utility ulity;
	CString strPath =  _T("F:\\ARX\\test\\pldy\\dwg\\*.dwg");
	ulity.GetFileNameFromThePath(strPath, m_strMultiDocNameArr, _T(".DWG"));
	if (m_strMultiDocNameArr.GetSize() < 1L)
	{
		return;
	}
	AcDbDatabase* pDb = acdbCurDwg();
	acDocManager->lockDocument(curDoc(),AcAp::kWrite,NULL,NULL,true); 
	Acad::ErrorStatus es; 
	for (int i=0; i< m_strMultiDocNameArr.GetSize(); i++)
	{
		CString strFileName = m_strMultiDocNameArr.GetAt(i);
		AcDbDatabase* pCurDb = new AcDbDatabase(Adesk::kFalse);
		es = pCurDb->readDwgFile(strFileName, _SH_DENYNO);
		if (es != Acad::eOk)
		{
			continue;
		}
		acdbHostApplicationServices()->setWorkingDatabase(pCurDb);	
		CJudgeFrame JudgeFrame;
		JudgeFrame.SetMode(m_nFrame);
		JudgeFrame.SetBlockName(m_strBlockName);
		JudgeFrame.SetLayerName(m_strLayerName);
		JudgeFrame.SetMultiDoc(1);
		std::vector<FrameInfo*> m_frames = JudgeFrame.GetDataFromDwg();
		if (m_frames.size() > 0)
		{
			m_MultiFrames.insert(std::make_pair(strFileName, m_frames));
		}

		//es = pCurDb->closeInput(true);
		if (pCurDb != NULL)
		{
			delete pCurDb;
			pCurDb = NULL;
		}
		acdbHostApplicationServices()->setWorkingDatabase(pDb);	
	}
	acDocManager->unlockDocument(curDoc()); 
	nCout = m_MultiFrames.size();

	if (nCout>0)
	{
		CString strTmp;
		strTmp.Format(_T("%d"), nCout);
		GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strTmp);
		GetDlgItem(IDC_BUTTON_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BRIGHT)->EnableWindow(FALSE);
		OnBnClickedCheckCenter();
		OnBnClickedCheckFit();
	}
	//CompleteEditorCommand();
}

void CDlgMPlotConfig::OnBnClickedButtonBright()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginEditorCommand();
	CompleteEditorCommand();
}

void CDlgMPlotConfig::OnBnClickedButtonBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR Mycom[_MAX_PATH];  
	BROWSEINFO Myfold;  
	Myfold.hwndOwner= this->m_hWnd;// sds_getmainhwnd();
	Myfold.pidlRoot=NULL;  
	Myfold.pszDisplayName=Mycom;  
	CString strRes;
	strRes.LoadString(IDS_STRING_OUT7);
	Myfold.lpszTitle = strRes;  
	Myfold.ulFlags=0;  
	Myfold.lpfn=NULL;  
	Myfold.lParam=NULL;  
	Myfold.iImage=NULL;  

	Mycom[0]=_T('\0');  
	SHGetPathFromIDList(SHBrowseForFolder(&Myfold),Mycom);  

	m_strDeFaultPath.Format(_T("%s"),Mycom);
	m_strDeFaultPath.TrimRight(_T("\\"));
	if(m_strDeFaultPath.GetLength()<=0)
		return;

	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(m_strDeFaultPath);
}


void openSyncDocHelper( void *pData)
{
	if (acDocManager->isApplicationContext()) 
	{
		const TCHAR * fileName = (const TCHAR *)pData;
		Acad::ErrorStatus es = acDocManager->appContextOpenDocument(fileName);
		assert(es == Acad::eOk);
	} 
	else
		assert(0 && "ERROR: in Document context.");
}

void OpenDoc( void *pData)
{
	AcApDocument* pDoc = acDocManager-> curDocument();
	Acad::ErrorStatus es;
	if (acDocManager->isApplicationContext())
	{
		es = acDocManager->appContextOpenDocument ((const TCHAR *)pData);
	}
	else
	{
		acutPrintf(_T("\nERROR To Open Doc!\n"));
	}
}
void CDlgMPlotConfig::OnBnClickedButtonPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Acad::ErrorStatus es;
	CString strRes;
	this->ShowWindow(SW_HIDE);
	Utility ulity;
	AcDbDatabase* pDb = acdbCurDwg();
	AcDbDatabase* pCurDb = NULL;
	CString strScale;
	if (!m_bIsFit)
	{
		GetDlgItem(IDC_EDIT_SCALE)->GetWindowText(strScale);
#ifdef UNICODE
		m_dScale = _tcstod(strScale, 0);
#else
		m_dScale = atof(strScale);
#endif
		if (m_dScale == 0)
		{
			CString strRes;
			strRes.LoadString(IDS_STRING_SCALE);
			AfxMessageBox(strRes);
			return;
		}
	}
	es = acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true); 
	int j = 0;
	for (std::map<CString, std::vector<FrameInfo*> >::iterator iter = m_MultiFrames.begin(); 
		iter != m_MultiFrames.end(); iter++)
	{
		CString strFileName = iter->first;	
		AcDbDatabase* pCurDb = new AcDbDatabase(Adesk::kFalse);
		es = pCurDb->readDwgFile(strFileName, _SH_DENYNO);
		if (es != Acad::eOk)
		{
			continue;
		}
		acdbHostApplicationServices()->setWorkingDatabase(pCurDb);	
	
		if (pPlotSettings != NULL)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
		
		pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
		AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); 
		AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
		AcDbBlockTableRecord *pBTR;
		Acad::ErrorStatus es=acdbOpenObject(pBTR,btrId,AcDb::kForRead);
		layoutId=pBTR->getLayoutId();
		AcDbLayout *pLayout=NULL;
		acdbOpenObject(pLayout,layoutId,AcDb::kForRead);

		m_bISModelType = pLayout->modelType();
		pPlotSettings=new AcDbPlotSettings(m_bISModelType);
		pPlotSettings->copyFrom(pLayout);
		pLayout->close();
		es = pBTR->close();

		pPSV->refreshLists(pPlotSettings);
		es = pPSV->setPlotCfgName(pPlotSettings,m_strCurDevice);
		es = pPSV->setCanonicalMediaName(pPlotSettings, m_strCurMedia);//设置纸张

		std::vector<FrameInfo*> frames = iter->second;
		for (std::vector<FrameInfo*>::iterator itr = frames.begin(); itr != frames.end(); itr++)
		{
			FrameInfo* pFrame = *itr;
			AcGePoint2d minPt, maxPt;
			minPt = pFrame->GetminPoint();
			maxPt = pFrame->GetmaxPoint();
			acdbWcs2Ucs(asDblArray(maxPt), asDblArray(maxPt), false);
			acdbWcs2Ucs(asDblArray(minPt), asDblArray(minPt), false);
			ads_point targetPt;
			resbuf rb;
			acedGetVar(_T("target"), &rb);
			targetPt[0] = rb.resval.rpoint[0];
			targetPt[1] = rb.resval.rpoint[1];
			targetPt[2] = rb.resval.rpoint[2];

			minPt.x -= targetPt[0];
			minPt.y -= targetPt[1];

			maxPt.x -= targetPt[0];
			maxPt.y -= targetPt[1];	
			if (m_nDirection == 0)//临时处理
			{
				if (isPaperWidthLessHeight())
				{
					if (pFrame->GetFrameDirection())
					{
						Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
					}
					else
					{
						Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
					}
				}
				else
				{
					if (pFrame->GetFrameDirection())
					{
						Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
					}
					else
					{
						Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
					}
				}

			}
			else if (m_nDirection == 1)
			{
				Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);

			}
			else if (m_nDirection == 2)
			{
				Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
			}
			

			es = pPSV->setPlotWindowArea(pPlotSettings, minPt.x, minPt.y, maxPt.x, maxPt.y);
			if (!m_bIsCenter)
			{
				double xPosition,yPosition;
#ifdef UNICODE
				xPosition = _tcstod(m_strXOffset, 0);
				yPosition = _tcstod(m_strYOffset, 0);
#else
				xPosition = atof(m_strXOffset);
				yPosition = atof(m_strYOffset);
#endif
				if (isWHSwap())
				{		
					es = pPSV->setPlotOrigin(pPlotSettings, yPosition, xPosition);
				}
				else
				{	
					es = pPSV->setPlotOrigin(pPlotSettings, xPosition, yPosition);
				}
			}
			else
			{
				es=pPSV->setPlotCentered(pPlotSettings,true);
			}
			es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);     //设置打印区域为窗口

			AcDbPlotSettings::PlotPaperUnits preScale = pPlotSettings->plotPaperUnits();
			es = pPSV->setPlotPaperUnits(pPlotSettings, AcDbPlotSettings::kMillimeters);

			if (m_bIsFit)
			{
				es = pPSV->setUseStandardScale(pPlotSettings,Adesk::kTrue);
				es = pPSV->setStdScaleType(pPlotSettings,AcDbPlotSettings::kScaleToFit);
			}
			else
			{
#ifdef UNICODE
				m_dScale = _tcstod(strScale, 0);
#else
				m_dScale = atof(strScale);
#endif
				es=pPSV->setUseStandardScale(pPlotSettings,Adesk::kFalse);
				es=pPSV->setCustomPrintScale(pPlotSettings,m_dScale,1);
			}


			AcPlPlotEngine* pEngine = NULL;
			if(Acad::eOk==AcPlPlotFactory::createPreviewEngine(pEngine))
			{
				pEngine->beginPlot(NULL);

				AcPlPlotPageInfo pageInfo;
				AcPlPlotInfo plotInfo;

				plotInfo.setLayout(layoutId);

				plotInfo.setOverrideSettings(pPlotSettings);

				AcPlPlotInfoValidator validator;
				validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
				es = validator.validate(plotInfo);
				if (es != Acad::eOk)
				{
					CString strRes;
					strRes.LoadString(IDS_STRING_OUT3);
					AfxMessageBox(strRes);
					CompleteEditorCommand();
					pEngine->destroy();
					pEngine = NULL;
					break;
				}
				CString szDocName;
				szDocName.Format(_T("%s_%d"), strFileName, j);
				es = pEngine->beginDocument(plotInfo, szDocName);
				es = pEngine->beginPage(pageInfo, plotInfo, true);    	
				es = pEngine->beginGenerateGraphics();
				es = pEngine->endGenerateGraphics();
				es = pEngine->endPage();
				es = pEngine->endDocument();
				es = pEngine->endPlot();

				pEngine->destroy();
				pEngine = NULL;
			}
			else
				AfxMessageBox(_T("Plot Engine is Busy..."));
			j++;
		}
		

		//设置数据库
		//es = pCurDoc->popDbmod();
		//es = pCurDb->closeInput(true);	
		if (pCurDb != NULL)
		{
			delete pCurDb;
			pCurDb = NULL;
		}
		//关闭文件
		/*es = acDocManager->unlockDocument(pDoc); 
		es = acDocManager->activateDocument(pDoc);
		es = acDocManager->setCurDocument(pDoc);
		es = acDocManager->closeDocument(pCurDoc);*/
	}	
	acdbHostApplicationServices()->setWorkingDatabase(pDb);
	es = acDocManager->unlockDocument(curDoc()); 

	this->ShowWindow(SW_SHOWNORMAL);
}

void CDlgMPlotConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Acad::ErrorStatus es;
	CString strRes;
	if (m_bPlotToFile == 0)
	{
		strRes.LoadString(IDS_STRING_OUT3);
		AfxMessageBox(strRes);
		return;
	}
	if (m_nPlot == 1)
	{
		if (m_strLayerOutArr.GetSize() < 1L)
		{
			strRes.LoadString(IDS_STRING_LAYEROUT);
			MessageBox(strRes);
			return;
		}
	}
	else
	{
		long nLen = GetFramecount();
		if (nLen<=0)
		{
			strRes.LoadString(IDS_STRING_OUT5);
			MessageBox(strRes);
			return;
		}
		//OrderFrame(m_nOrder);
	}
	
	AcAxDocLock lock;
	CString strScale;

	CString strNum;
	GetDlgItem(IDC_EDIT_COUNT)->GetWindowText(strNum);
#ifdef UNICODE
	m_nCount = _tstoi(strNum);
#else
	m_nCount = atoi(strNum);
#endif
	if (m_nCount<1)
	{
		strRes.LoadString(IDS_STRING_COUNT);
		AfxMessageBox(strRes);
		return;
	}

	if (m_bIsPlotToFile)
	{	
		CString strPathName;
		GetDlgItem(IDC_EDIT_PATH)->GetWindowText(strPathName);
		if (strPathName.CompareNoCase(_T(""))==0)
		{
			strRes.LoadString(IDS_STRING_PATH);
			AfxMessageBox(strRes);
			return;
		}
	}
	//打印比例不能为0
	if (!m_bIsFit)
	{
		GetDlgItem(IDC_EDIT_SCALE)->GetWindowText(strScale);
#ifdef UNICODE
		m_dScale = _tcstod(strScale, 0);
#else
		m_dScale = atof(strScale);
#endif
		if (abs(m_dScale - 0)<0.000001)
		{
			strRes.LoadString(IDS_STRING_SCALE);
			AfxMessageBox(strRes);
			return;
		}
	}


	AcDbDatabase* pDb = acdbCurDwg();
	//BeginEditorCommand();
	es = acDocManager->lockDocument(curDoc(),AcAp::kWrite,NULL,NULL,true); 

	this->ShowWindow(SW_HIDE);
	AcPlPlotEngine* pEngine = NULL;
	if(Acad::eOk==AcPlPlotFactory::createPublishEngine(pEngine))
	{
		if (m_nPlot == 1)//处理打印布局
		{
			CString strLayerOut;
			int i=1;

			for (int j=0; j<m_strLayerOutArr.GetSize(); j++)
			{
				strLayerOut = m_strLayerOutArr.GetAt(j);
				//删掉Plotsettings
				if (pPlotSettings != NULL)
				{
					delete pPlotSettings;
					pPlotSettings = NULL;
				}

				pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
				AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); 
				AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(strLayerOut);
				es = pLayoutManager->setCurrentLayout(strLayerOut);
				m_bISModelType = pLayout->modelType();
				pPlotSettings=new AcDbPlotSettings(m_bISModelType);
				pPlotSettings->copyFrom(pLayout);
				pLayout->close();
				layoutId = pLayout->objectId();

				pPSV->refreshLists(pPlotSettings);
				const TCHAR* plotCfgchar;
				es = pPlotSettings->getPlotCfgName(plotCfgchar);
				AcPlPlotConfig *pPlotConfig =  NULL;
				if(Acad::eOk == acplPlotConfigManagerPtr()->setCurrentConfig(pPlotConfig, plotCfgchar))
				{
					if(pPlotConfig->plotToFileCapability()==AcPlPlotConfig::kNoPlotToFile)
					{
						m_bPlotToFile = 0;
						BOOL bIsFalse = strRes.LoadString(IDS_STRING_LAYOUTFILE);
						CString strTmp = strLayerOut + strRes;
						AfxMessageBox(strTmp);
						continue;
					}
					else if (pPlotConfig->plotToFileCapability() == AcPlPlotConfig::kPlotToFileAllowed)
					{
						m_bPlotToFile = 1;
					}
					else if (pPlotConfig->plotToFileCapability() == AcPlPlotConfig::kMustPlotToFile)
					{
						m_bPlotToFile = 2;
					}	
				}
				if (isPaperWidthLessHeight())
				{
					es = pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
				}
				else
				{
					es = pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
				}
				es = pPSV->setPlotCentered(pPlotSettings,true);
				es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kLayout);     //设置打印区域为布局
				es = pPSV->setUseStandardScale(pPlotSettings,Adesk::kTrue);
				es = pPSV->setStdScaleType(pPlotSettings,AcDbPlotSettings::kScaleToFit);
				AcPlPlotProgressDialog *pPlotProgDlg = acplCreatePlotProgressDialog(acedGetAcadFrame()->m_hWnd,false,1);

				pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kDialogTitle,_T("Plot API Progress"));
				pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelJobBtnMsg,_T("Cancel Job"));
				pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelSheetBtnMsg,_T("Cancel Sheet"));
				pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetSetProgressCaption,_T("Job Progress"));
				pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetProgressCaption,_T("Sheet Progress"));

				pPlotProgDlg->setPlotProgressRange(0,100);
				pPlotProgDlg->onBeginPlot();
				pPlotProgDlg->setIsVisible(true);

				es = pEngine->beginPlot(NULL);

				AcPlPlotPageInfo pageInfo;
				AcPlPlotInfo plotInfo;

				plotInfo.setLayout(layoutId);

				plotInfo.setOverrideSettings(pPlotSettings);

				AcPlPlotInfoValidator validator;
				validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
				es = validator.validate(plotInfo);
				if (es != Acad::eOk)
				{
					strRes.LoadString(IDS_STRING_LAYOUTFILE);
					CString strTmp = strLayerOut + strRes;
					AfxMessageBox(strTmp);
					pPlotProgDlg->destroy();
					//CompleteEditorCommand();
					continue;
				}
				const TCHAR *szDocName=acDocManager->curDocument()->fileName();
				if (m_bPlotToFile == 2)
				{
					CString strFileName;
					CString strTmp;
					GetDlgItem(IDC_EDIT_FILENAME)->GetWindowText(m_strDeFaultFileName);
					strFileName = m_strDeFaultPath+_T("\\") + m_strDeFaultFileName + _T("_");
					strFileName+= strLayerOut;
					es = pEngine->beginDocument(plotInfo, szDocName, NULL, m_nCount, true, strFileName);
					if (es!=Acad::eOk)
					{
						strRes.LoadString(IDS_STRING_FILE);
						strTmp = strFileName + strRes;
						pPlotProgDlg->destroy();
						AfxMessageBox(strTmp);
						continue;
					}
				}
				else
					es = pEngine->beginDocument(plotInfo, szDocName, NULL, m_nCount);

				pPlotProgDlg->onBeginSheet();
				pPlotProgDlg->setSheetProgressRange(0, 100);
				pPlotProgDlg->setSheetProgressPos(0);

				es = pEngine->beginPage(pageInfo, plotInfo, true);    	

				es = pEngine->beginGenerateGraphics();
				es = pEngine->endGenerateGraphics();
				es = pEngine->endPage();

				pPlotProgDlg->setSheetProgressPos(100);
				pPlotProgDlg->onEndSheet();
				pPlotProgDlg->setPlotProgressPos(100);

				es = pEngine->endDocument();
				es = pEngine->endPlot();
				pPlotProgDlg->destroy();
			}
			pEngine->destroy();
			pEngine = NULL;
		}
		else
		{
			int i=1;
			for (std::map<CString, std::vector<FrameInfo*> >::iterator iter = m_MultiFrames.begin(); 
				iter != m_MultiFrames.end(); iter++)
			{
				CString strFileName = iter->first;	
				AcDbDatabase* pCurDb = new AcDbDatabase(Adesk::kFalse);
				es = pCurDb->readDwgFile(strFileName, _SH_DENYNO);
				if (es != Acad::eOk)
				{
					continue;
				}
				acdbHostApplicationServices()->setWorkingDatabase(pCurDb);	

				if (pPlotSettings != NULL)
				{
					delete pPlotSettings;
					pPlotSettings = NULL;
				}

				pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
				AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); 
				AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
				AcDbBlockTableRecord *pBTR;
				Acad::ErrorStatus es=acdbOpenObject(pBTR,btrId,AcDb::kForRead);
				layoutId=pBTR->getLayoutId();
				AcDbLayout *pLayout=NULL;
				acdbOpenObject(pLayout,layoutId,AcDb::kForRead);

				m_bISModelType = pLayout->modelType();
				pPlotSettings = new AcDbPlotSettings(m_bISModelType);
				pPlotSettings->copyFrom(pLayout);
				pLayout->close();
				es = pBTR->close();

				pPSV->refreshLists(pPlotSettings);
				es = pPSV->setPlotCfgName(pPlotSettings,m_strCurDevice);
				es = pPSV->setCanonicalMediaName(pPlotSettings, m_strCurMedia);//设置纸张

				std::vector<FrameInfo*> frames = iter->second;
				for (std::vector<FrameInfo*>::iterator itr = frames.begin(); itr != frames.end(); itr++)
				{
					pPSV->refreshLists(pPlotSettings);
					FrameInfo* pFrame = *itr;
					AcGePoint2d minPt, maxPt;
					minPt = pFrame->GetminPoint();
					maxPt = pFrame->GetmaxPoint();
					acdbWcs2Ucs(asDblArray(maxPt), asDblArray(maxPt), false);
					acdbWcs2Ucs(asDblArray(minPt), asDblArray(minPt), false);
					ads_point targetPt;
					resbuf rb;
					acedGetVar(_T("target"), &rb);
					targetPt[0] = rb.resval.rpoint[0];
					targetPt[1] = rb.resval.rpoint[1];
					targetPt[2] = rb.resval.rpoint[2];

					es = pPSV->setCanonicalMediaName(pPlotSettings, m_strCurMedia);//设置纸张
					if (m_nDirection == 0)//临时处理
					{
						if (isPaperWidthLessHeight())
						{
							if (pFrame->GetFrameDirection())
							{
								Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
							}
							else
							{
								Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
							}
						}
						else
						{
							if (pFrame->GetFrameDirection())
							{
								Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
							}
							else
							{
								Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
							}
						}
					}
					else if (m_nDirection == 1)
					{
						Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);

					}
					else if (m_nDirection == 2)
					{
						Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
					}

					minPt.x -= targetPt[0];
					minPt.y -= targetPt[1];

					maxPt.x -= targetPt[0];
					maxPt.y -= targetPt[1];	

					es = pPSV->setPlotWindowArea(pPlotSettings, minPt.x, minPt.y, maxPt.x, maxPt.y);
					if (!m_bIsCenter)
					{
						double xPosition,yPosition;
#ifdef UNICODE
						xPosition = _tcstod(m_strXOffset, 0);
						yPosition = _tcstod(m_strYOffset, 0);
#else
						xPosition = atof(m_strXOffset);
						yPosition = atof(m_strYOffset);
#endif
						if (isWHSwap())
						{		
							es = pPSV->setPlotOrigin(pPlotSettings, yPosition, xPosition);
						}
						else
						{	
							es = pPSV->setPlotOrigin(pPlotSettings, xPosition, yPosition);
						}
					}
					else
					{
						es = pPSV->setPlotCentered(pPlotSettings,true);
					}
					es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);     //设置打印区域为窗口

					AcDbPlotSettings::PlotPaperUnits preScale = pPlotSettings->plotPaperUnits();
					es = pPSV->setPlotPaperUnits(pPlotSettings, AcDbPlotSettings::kMillimeters);

					if (m_bIsFit)
					{
						es = pPSV->setUseStandardScale(pPlotSettings,Adesk::kTrue);
						es = pPSV->setStdScaleType(pPlotSettings,AcDbPlotSettings::kScaleToFit);
					}
					else
					{
#ifdef UNICODE
						m_dScale = _tcstod(strScale, 0);
#else
						m_dScale = atof(strScale);
#endif
						es=pPSV->setUseStandardScale(pPlotSettings,Adesk::kFalse);
						es=pPSV->setCustomPrintScale(pPlotSettings,m_dScale,1);
					}
					AcPlPlotProgressDialog *pPlotProgDlg = acplCreatePlotProgressDialog(acedGetAcadFrame()->m_hWnd,false,1);

					pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kDialogTitle,_T("Plot API Progress"));
					pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelJobBtnMsg,_T("Cancel Job"));
					pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelSheetBtnMsg,_T("Cancel Sheet"));
					pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetSetProgressCaption,_T("Job Progress"));
					pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetProgressCaption,_T("Sheet Progress"));

					pPlotProgDlg->setPlotProgressRange(0,100);
					pPlotProgDlg->onBeginPlot();
					pPlotProgDlg->setIsVisible(true);

					es = pEngine->beginPlot(NULL);

					AcPlPlotPageInfo pageInfo;
					AcPlPlotInfo plotInfo;

					plotInfo.setLayout(layoutId);

					plotInfo.setOverrideSettings(pPlotSettings);

					AcPlPlotInfoValidator validator;
					validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
					es = validator.validate(plotInfo);
					if (es != Acad::eOk)
					{
						strRes.LoadString(IDS_STRING_OUT3);
						AfxMessageBox(strRes);
						pPlotProgDlg->destroy();
						CompleteEditorCommand();
						break;
					}
					const TCHAR *szDocName = acDocManager->curDocument()->fileName();
					if (m_bIsPlotToFile)
					{
						CString strFileName;
						CString strTmp;
						GetDlgItem(IDC_EDIT_FILENAME)->GetWindowText(m_strDeFaultFileName);
						strFileName = m_strDeFaultPath+_T("\\") + m_strDeFaultFileName;
						strTmp.Format(_T("%d"), i);
						strFileName+= strTmp;
						es = pEngine->beginDocument(plotInfo, szDocName, NULL, m_nCount, true, strFileName);
						if (es!=Acad::eOk)
						{
							strRes.LoadString(IDS_STRING_FILE);
							strTmp = strFileName + strRes;
							AfxMessageBox(strTmp);
							pPlotProgDlg->destroy();
							continue;
						}
					}
					else
						es = pEngine->beginDocument(plotInfo, szDocName, NULL, m_nCount);

					pPlotProgDlg->onBeginSheet();
					pPlotProgDlg->setSheetProgressRange(0, 100);
					pPlotProgDlg->setSheetProgressPos(0);

					es = pEngine->beginPage(pageInfo, plotInfo, true);    	

					es = pEngine->beginGenerateGraphics();
					es = pEngine->endGenerateGraphics();
					es = pEngine->endPage();

					pPlotProgDlg->setSheetProgressPos(100);
					pPlotProgDlg->onEndSheet();
					pPlotProgDlg->setPlotProgressPos(100);

					es = pEngine->endDocument();
					es = pEngine->endPlot();

					pPlotProgDlg->destroy();
					i++;
				}
				//es = pCurDb->closeInput(true);
				if (pCurDb != NULL)
				{
					delete pCurDb;
					pCurDb = NULL;
				}
			}
			pEngine->destroy();
			pEngine = NULL;	
			acdbHostApplicationServices()->setWorkingDatabase(pDb);
		}
	}
	else
	{
		strRes.LoadString(IDS_STRING_PLOT);
		AfxMessageBox(strRes);
	}
	es = acDocManager->unlockDocument(curDoc()); 
	SaveInfoToReg();
	//CompleteEditorCommand();
	CAcUiDialog::OnOK();
	this->ShowWindow(SW_SHOWNORMAL);
}

void CDlgMPlotConfig::OnBnClickedPickbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginEditorCommand();
	CString strRes;
	bool bIsContinued = true;
	if (m_nFrame == 1)
	{
		ads_name bname;
		ads_name_clear(bname);
		AcGePoint3d basePt;
		while(bIsContinued)
		{
			strRes.LoadString(IDS_STRING_FRAME);
			int nRet = acedEntSel(strRes, bname, asDblArray(basePt));
			if (nRet == RTCAN)
			{
				bIsContinued = false;
				break;
			}
			else if (nRet!=RTNORM)
			{
				strRes.LoadString(IDS_STRING_OUT5);
				acutPrintf(strRes);
				continue;
			}
			 
			AcDbEntity* pEnt = NULL;
			AcDbObjectId objId;//记录实体的ID
			CString name;//记录实体的块名
			Acad::ErrorStatus es;
			es = acdbGetObjectId(objId, bname);
			if (es != Acad::eOk)
			{
				continue;
			}
			es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
			if (es != Acad::eOk)
			{
				continue;
			}
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pObjRef = AcDbBlockReference::cast(pEnt);
				AcDbObjectId refId;
				refId = pObjRef->blockTableRecord();
				AcDbBlockTableRecord* pBlkRec = NULL;

				es = acdbOpenObject(pBlkRec, refId, AcDb::kForRead);
				if (es != Acad::eOk)
				{
					pObjRef->close();
					continue;
				}
				else
				{
					const TCHAR* locName;
					pBlkRec->getName(locName);
					name = locName;
					es = pBlkRec->close();
				}
				pObjRef->close();
				GetDlgItem(IDC_EDIT_BLOCKNAME)->SetWindowText(name);
				bIsContinued = false;
			}
			else
			{
				strRes.LoadString(IDS_STRING_FRAME);
				acutPrintf(strRes);
				pEnt->close();
			}
		}
	}
	else if (m_nFrame == 0)
	{
		ads_name bname;
		ads_name_clear(bname);
		AcGePoint3d basePt;
		while(bIsContinued)
		{
			strRes.LoadString(IDS_STRING_FRAME);
			int nRet = acedEntSel(strRes, bname, asDblArray(basePt));
			if (nRet == RTCAN)
			{
				bIsContinued = false;
				break;
			}
			else if (nRet!=RTNORM)
			{
				strRes.LoadString(IDS_STRING_OUT5);
				acutPrintf(strRes);
				continue;
			}
			AcDbEntity* pEnt = NULL;
			AcDbObjectId objId;//记录实体的ID
			CString name;//记录实体的块名
			Acad::ErrorStatus es;
			es = acdbGetObjectId(objId, bname);
			if (es != Acad::eOk)
			{
				continue;
			}
			es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
			if (es != Acad::eOk)
			{
				continue;
			}
			const TCHAR* locName;
			locName = pEnt->layer();
			name = locName;
			pEnt->close();
			GetDlgItem(IDC_EDIT_LAYERNAME)->SetWindowText(name);
			bIsContinued = false;
		}
	}
	CompleteEditorCommand();
}



void CDlgMPlotConfig::OnBnClickedButtonBlockname()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcModuleResourceOverride rs;
	CDlgBlockName dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_BLOCKNAME)->SetWindowText(dlg.GetBlockName());
	}
}

void CDlgMPlotConfig::OnBnClickedButtonLayername()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcModuleResourceOverride rs;
	CDlgListLayerName dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_LAYERNAME)->SetWindowText(dlg.GetLayerName());
	}
}

BOOL CDlgMPlotConfig::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CenterWindow(acedGetAcadDwgView());
	AcAxDocLock lock;
	m_spin.SetRange32(1,1000);

	pPSV=(AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); 
	AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	Acad::ErrorStatus es=acdbOpenObject(pBTR,btrId,AcDb::kForRead);
	layoutId=pBTR->getLayoutId();
	AcDbLayout *pLayout=NULL;
	acdbOpenObject(pLayout,layoutId,AcDb::kForRead);

	m_bISModelType = pLayout->modelType();
	pPlotSettings=new AcDbPlotSettings(m_bISModelType);
	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	es = pBTR->close();


	RefreshDevices();
	OnCbnSelchangeComboDevice();
	RefreshMediaNames();
	OnCbnSelchangeComboMedia();
	RefreshCTBNames();
	OnCbnSelchangeComboPlotsytle();

	OnBnClickedCheckCenter();
	OnBnClickedCheckFit();
	if (m_nDirection == 0)
	{
		OnBnClickedRadioAutorotate();
	}
	else if (m_nDirection == 1)
	{
		OnBnClickedRadioHengxiang();
	}
	else if (m_nDirection == 2)
	{
		OnBnClickedRadioZongxiang();
	}

	GetDlgItem(IDC_BUTTON_BRIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PREVIEW)->EnableWindow(FALSE);
	if (m_nPlot == 0)
	{
		OnBnClickedRadioPlot();
	}
	else if (m_nPlot == 1)
	{
		OnBnClickedRadioLayerout();
	}
	/*else if (m_nPlot == 2)
	{
		OnBnClickedRadioPlt();
	}*/

	CString tmpScaleStr;
	tmpScaleStr.Format(_T("%f"), m_dScale);
	GetDlgItem(IDC_EDIT_SCALE)->SetWindowText(tmpScaleStr);

	tmpScaleStr.Format(_T("%d"), m_nCount);
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(tmpScaleStr);

	if (m_nFrame == 0)
	{
		OnBnClickedRadioLayer();
	}
	else if (m_nFrame == 1)
	{
		OnBnClickedRadioBlock();
	}
	else if (m_nFrame == 2)
	{
		OnBnClickedRadioSanxian();
	}

	OnBnClickedCheckPlottofile();
	//GetDlgItem(IDC_EDIT_LAYERNAME)->SetWindowText(_T("*图框,*TITLE"));
	//GetDlgItem(IDC_EDIT_BLOCKNAME)->SetWindowText(_T("*图框,*TITLE"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMPlotConfig::RefreshCTBNames()
{
	AcArray<const TCHAR*> StyleSheetList;
	m_CombPlotStyle.ResetContent();
	pPSV->plotStyleSheetList(StyleSheetList);

	for(int i=0; i<StyleSheetList.length(); i++)
	{
		m_CombPlotStyle.AddString(StyleSheetList[i]);
	}
	if (m_strCurPlotStyle.CompareNoCase(_T("")) != 0)
	{
		int nCur = m_CombPlotStyle.FindString(0, m_strCurPlotStyle);
		if (nCur == -1)
		{
			nCur = 0;
		}
		m_CombPlotStyle.SetCurSel(nCur);
	}
	else
	{
		const TCHAR *pCurStyleName;
		pPlotSettings->getCurrentStyleSheet(pCurStyleName);
		m_CombPlotStyle.SetCurSel(m_CombPlotStyle.FindString(0, (CString)pCurStyleName));
	}	
}

void CDlgMPlotConfig::OnDestroy()
{
	CDialog::OnDestroy();
	delete pPlotSettings;
	delete this;
}
void CDlgMPlotConfig::RefreshDevices(void)
{
	// Set the list of available devices on the system
	AcArray<const TCHAR*> deviceList;// Temporary device array

	m_CombDevice.ResetContent();// Clear the Combo Box

	pPSV->plotDeviceList(deviceList);// Fill the temporary array,
	// and copy the Combo Box and mirror with the results.
	for(int c=0;c<deviceList.length();c++)
		m_CombDevice.AddString(deviceList[c]);

	// Set the current selection to reflect the current output device.
	if (m_strCurDevice.CompareNoCase(_T("")) != 0)
	{
		int nCur = m_CombDevice.SelectString(0, m_strCurDevice);
		if (nCur == -1)
		{
			nCur = 0;
		}
		m_CombDevice.SetCurSel(nCur);
	}
	else
	{
		const TCHAR* plotCfgchar;
		pPlotSettings->getPlotCfgName(plotCfgchar);
		int nSelect = m_CombDevice.SelectString(0, plotCfgchar);
		if (nSelect !=-1)
		{
			m_CombDevice.SetCurSel(nSelect);
		}
		else
		{
			m_CombDevice.SetCurSel(0);
		}
	}
}
void CDlgMPlotConfig::OrderFrame(int nSelect)
{
	//ads_name ssname;
	//ads_name_clear(ssname);
	//acedSSAdd(NULL,NULL,ssname);

	//long nLen = GetFramecount();
	//int* IndexArr = new int[nLen];
	//int nFlag;//记录
	//AcGePoint3dArray minPtArr,midPtArr,maxPtArr;
	//AcGePoint3d tmpPt;
	//bool bIsChange = false;
	//minPtArr.removeAll();
	//maxPtArr.removeAll();
	//midPtArr.removeAll();
	////保存原有的顺序

	//CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_REVERSEORDER);
	//int m_bIsReserve = pBtn->GetCheck();
	//if (m_bIsReserve)
	//{
	//	for (int i=0; i<nLen; i++)
	//	{
	//		IndexArr[i]=nLen-i-1;
	//	}
	//}
	//else
	//{
	//	for (int i=0; i<nLen; i++)
	//	{
	//		IndexArr[i]=i;
	//	}
	//	if (m_nOrder!=0)
	//	{
	//		for (std::vector<FrameInfo*>::iterator iter = m_frames.begin(); iter!=m_frames.end(); iter++)
	//		{
	//			CString strScale,strDirection,strUpDown,strOffset;
	//			FrameInfo* pFrame = *iter;
	//			AcGePoint3d minPt, maxPt,midPt;
	//			minPt = My2d23d(pFrame->GetminPoint());
	//			maxPt = My2d23d(pFrame->GetmaxPoint());
	//			midPt.x = (minPt.x + maxPt.x)/2;
	//			midPt.y = (minPt.y + maxPt.y)/2;
	//			midPt.z = (minPt.z + minPt.z)/2;
	//			minPtArr.append(minPt);
	//			maxPtArr.append(maxPt);
	//			midPtArr.append(midPt);
	//		}

	//		for (int i=1; i<nLen; i++)
	//		{
	//			for (int j=0; j<nLen-i; j++)
	//			{
	//				if (nSelect == 1)
	//				{
	//					bIsChange = (midPtArr[j].x > midPtArr[j+1].x);
	//					if (abs(midPtArr[j].x-midPtArr[j+1].x)<0.0001)
	//					{
	//						bIsChange = midPtArr[j].y<midPtArr[j+1].y;
	//					}
	//				}
	//				else if (nSelect ==2)
	//				{
	//					bIsChange = (midPtArr[j].y < midPtArr[j+1].y);
	//					if (abs(midPtArr[j].y-midPtArr[j+1].y)<0.0001)
	//					{
	//						bIsChange = midPtArr[j].x>midPtArr[j+1].x;
	//					}
	//				}

	//				if (bIsChange)
	//				{
	//					tmpPt = midPtArr[j];
	//					midPtArr[j]=midPtArr[j+1];
	//					midPtArr[j+1]=tmpPt;

	//					nFlag = IndexArr[j];
	//					IndexArr[j] = IndexArr[j+1];
	//					IndexArr[j+1] = nFlag;
	//				}
	//			}
	//		}
	//	}
	//}

	//std::vector<FrameInfo*> tmpFrames;//临时变量
	//for (int i=0; i<nLen; i++)
	//{
	//	FrameInfo* pFrame = NULL;
	//	pFrame = GetFrame(IndexArr[i]);
	//	tmpFrames.push_back(pFrame);
	//}
	//m_frames.clear();
	//for (std::vector<FrameInfo*>::iterator iter = tmpFrames.begin(); iter!=tmpFrames.end(); iter++)
	//{
	//	m_frames.push_back(*iter);
	//}
	//tmpFrames.clear();
	//delete []IndexArr;
}


//获取其中的某一个图框
FrameInfo* CDlgMPlotConfig::GetFrame(int nIndex, CString strFileName)
{
	FrameInfo* dwgInfo = NULL;
	int   i   =   0; 
	if (nIndex > m_MultiFrames.size())
	{
		return dwgInfo;
	}
	std::map<CString, std::vector<FrameInfo*> >::iterator itr = m_MultiFrames.find(strFileName);
	if (itr == m_MultiFrames.end())
	{
		return dwgInfo;
	}
	std::vector<FrameInfo*> m_frames = itr->second;
	std::vector<FrameInfo*>::iterator iter = m_frames.begin();
	while(i <nIndex) 
	{ 
		++iter; 
		i++; 
	} 
	dwgInfo = *iter;
	return   dwgInfo;
}

int CDlgMPlotConfig::GetFramecount()
{
	int nCount = m_MultiFrames.size();
	return nCount;
}

void CDlgMPlotConfig::ClearAll()
{
	for (std::map<CString, std::vector<FrameInfo*> >::iterator iter = m_MultiFrames.begin();
		iter != m_MultiFrames.end(); iter++)
	{
		std::vector<FrameInfo*> frames = iter->second;
		for (std::vector<FrameInfo*>::iterator itr = frames.begin();
			itr != frames.end();
			itr++)
		{
			FrameInfo* pFrame = *itr;
			delete pFrame;
		}
	}
}


bool CDlgMPlotConfig::isPaperWidthLessHeight() const
{
	double paperWidth, paperHeight;
	pPlotSettings->getPlotPaperSize(paperWidth, paperHeight);
	return paperWidth < paperHeight;
}

bool CDlgMPlotConfig::isWHSwap() const
{
	AcDbPlotSettings::PlotRotation pr = pPlotSettings->plotRotation();
	return pr == AcDbPlotSettings::k90degrees || pr == AcDbPlotSettings::k270degrees;
}

CString CDlgMPlotConfig::GetStringFromReg(LPCTSTR strKey)
{
	CString strResult;
	CString strRootKey = _T("Software\\ZWMPLDY");
	TCHAR ResStr[MAX_PATH];
	COperateReg Reg;
	if (Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, _T(""), ResStr) > 0)
	{
		strResult = ResStr;
	}
	return strResult;
}

int CDlgMPlotConfig::GetDwordFromReg(LPCTSTR strKey) const
{
	int nRet = 0;
	CString strResult;
	CString strRootKey = _T("Software\\ZWPLDY");
	DWORD dRes = 0;
	COperateReg Reg;
	nRet = Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, dRes);
	return nRet;
}

bool CDlgMPlotConfig::GetInfoFromReg()
{
	m_strBlockName = GetStringFromReg(_T("BlockName"));
	m_strLayerName = GetStringFromReg(_T("LayerName"));
	if (m_strBlockName.IsEmpty())
	{
		m_strBlockName = _T("*图框,*TITLE");
	}
	if (m_strLayerName.IsEmpty())
	{
		m_strLayerName = _T("*图框,*TITLE");
	}
	m_strCurPlotStyle = GetStringFromReg(_T("CtbName"));
	m_strCurDevice = GetStringFromReg(_T("DeviceName"));
	m_strCurMedia = GetStringFromReg(_T("MediaName"));
	m_strDeFaultPath = GetStringFromReg(_T("DeFaultPath"));
	if (m_strDeFaultPath.IsEmpty())
	{
		TCHAR   szPath[MAX_PATH];
		SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE);  
		m_strDeFaultPath = szPath;
	}
	m_strDeFaultFileName = GetStringFromReg(_T("DeFaultFileName"));
	CString strScale = GetStringFromReg(_T("DeFaultScale"));
#ifdef UNICODE
	m_dScale = _tcstod(strScale, 0);
#else
	m_dScale = atof(strScale);
#endif
	m_strXOffset = GetStringFromReg(_T("XOffsetText"));
	m_strYOffset = GetStringFromReg(_T("YOffsetText"));

	m_bIsCenter = (BOOL)GetDwordFromReg(_T("CenterMode"));
	m_bIsFit = (BOOL)GetDwordFromReg(_T("FitMode"));
	m_bIsReserve = (BOOL)GetDwordFromReg(_T("ReserveMode"));
	m_nOrder = GetDwordFromReg(_T("OrderMode"));
	m_nCount = GetDwordFromReg(_T("PlotCount"));
	if (m_nCount <= 0)
	{
		m_nCount = 1;
	}
	m_nFrame = GetDwordFromReg(_T("SelectMode"));
	m_nPlot = GetDwordFromReg(_T("PlotMode"));
	if (m_nPlot == 1)
	{
		m_nPlot = 0;
	}
	m_bIsPlotToFile = (BOOL)GetDwordFromReg(_T("PLOTTOFILE"));
	m_nPrecision = GetDwordFromReg(_T("PrecisionMode"));
	m_nDirection = GetDwordFromReg(_T("DirectionMode"));
	return true;
}

void CDlgMPlotConfig::SaveInfoToReg()
{
	CString strRootKey = _T("Software\\ZWMPLDY");
	COperateReg Reg;
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("BlockName"), m_strBlockName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("LayerName"), m_strLayerName);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("CtbName"), m_strCurPlotStyle);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DeviceName"), m_strCurDevice);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("MediaName"), m_strCurMedia);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DeFaultPath"), m_strDeFaultPath);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DeFaultFileName"), m_strDeFaultFileName);
	CString strScale;
	strScale.Format(_T("%f"), m_dScale);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DeFaultScale"), strScale);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("XOffsetText"), m_strXOffset);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("YOffsetText"), m_strYOffset);

	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("CenterMode"), (DWORD)m_bIsCenter);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("FitMode"), (DWORD)m_bIsFit);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("ReserveMode"), (DWORD)m_bIsReserve);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("OrderMode"), (DWORD)m_nOrder);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("PlotCount"), (DWORD)m_nCount);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("SelectMode"), (DWORD)m_nFrame);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("PlotMode"), (DWORD)m_nPlot);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("PLOTTOFILE"), (DWORD)m_bIsPlotToFile);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("PrecisionMode"), (DWORD)m_nPrecision);
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("DirectionMode"), (DWORD)m_nDirection);

}

CString CDlgMPlotConfig::GetDeFaultPaperSize() const
{
	CString strPaperSize(_T(""));
	CString strRootKey, strProfiles, strLanguage;
	strRootKey = acdbHostApplicationServices()->getRegistryProductRootKey();
	COperateReg Reg;
	TCHAR ResStr[MAX_PATH];
	strRootKey += _T("\\Profiles");
	if (Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, _T(""), _T(""), ResStr) < 0)
	{
		return strPaperSize;
	}
	strProfiles = ResStr;
	strProfiles = strRootKey + _T("\\") + strProfiles + _T("\\Previous plot settings\\Model");

	Reg.GetRegister(HKEY_CURRENT_USER, strProfiles, _T("Papersize desc"), _T(""), ResStr);
	strPaperSize = ResStr;

	return strPaperSize;
}

void CDlgMPlotConfig::OnBnClickedCheckPlottofile()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_PLOTTOFILE);
	m_bIsPlotToFile = (BOOL)pBtn->GetCheck();
	if (m_bIsPlotToFile)
	{
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
	}
}

//BOOL CDlgMPlotConfig::DestroyWindow()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CAcUiDialog::DestroyWindow();
//}

extern Adesk::Boolean endDlg();

void CDlgMPlotConfig::OnClose()
{
	CDialog::OnClose();

	endDlg();
}

// void CDlgMPlotConfig::OnBnClickedCancel()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CDialog::OnCancel();
// 	endDlg();
// }

//void CDlgMPlotConfig::OnBnClickedButtonOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	this->ShowWindow(SW_HIDE);
//}

void CDlgMPlotConfig::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
	endDlg();
}
