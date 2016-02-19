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
//----- DlgPlotConfig.cpp : Implementation of CDlgPlotConfig
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DlgPlotConfig.h"
#include "axlock.h"
#include "DlgBlockName.h"
#include "DlgListLayerName.h"
#include "JudgeFrame.h"
#include "OperateReg.h"
#include "DlgListLayerout.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDlgPlotConfig, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDlgPlotConfig, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_RADIO_LAYER, &CDlgPlotConfig::OnBnClickedRadioLayer)
	ON_BN_CLICKED(IDC_RADIO_BLOCK, &CDlgPlotConfig::OnBnClickedRadioBlock)
	ON_BN_CLICKED(IDC_RADIO_SANXIAN, &CDlgPlotConfig::OnBnClickedRadioSanxian)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, &CDlgPlotConfig::OnCbnSelchangeComboDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_MEDIA, &CDlgPlotConfig::OnCbnSelchangeComboMedia)
	ON_CBN_SELCHANGE(IDC_COMBO_PLOTSYTLE, &CDlgPlotConfig::OnCbnSelchangeComboPlotsytle)
	ON_BN_CLICKED(IDC_CHECK_FIT, &CDlgPlotConfig::OnBnClickedCheckFit)
	ON_BN_CLICKED(IDC_CHECK_CENTER, &CDlgPlotConfig::OnBnClickedCheckCenter)
	ON_BN_CLICKED(IDC_RADIO_PLOT, &CDlgPlotConfig::OnBnClickedRadioPlot)
	ON_BN_CLICKED(IDC_RADIO_LAYEROUT, &CDlgPlotConfig::OnBnClickedRadioLayerout)
	ON_BN_CLICKED(IDC_RADIO_AUTOROTATE, &CDlgPlotConfig::OnBnClickedRadioAutorotate)
	ON_BN_CLICKED(IDC_RADIO_HENGXIANG, &CDlgPlotConfig::OnBnClickedRadioHengxiang)
	ON_BN_CLICKED(IDC_RADIO_ZONGXIANG, &CDlgPlotConfig::OnBnClickedRadioZongxiang)
	ON_BN_CLICKED(IDC_RADIO_ORDER, &CDlgPlotConfig::OnBnClickedRadioOrder)
	ON_BN_CLICKED(IDC_RADIO_LEFTTORIGHT, &CDlgPlotConfig::OnBnClickedRadioLefttoright)
	ON_BN_CLICKED(IDC_RADIO_UPTODOWN, &CDlgPlotConfig::OnBnClickedRadioUptodown)
	ON_BN_CLICKED(IDC_CHECK_REVERSEORDER, &CDlgPlotConfig::OnBnClickedCheckReverseorder)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CDlgPlotConfig::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_BRIGHT, &CDlgPlotConfig::OnBnClickedButtonBright)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CDlgPlotConfig::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CDlgPlotConfig::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDOK, &CDlgPlotConfig::OnBnClickedOk)
	ON_BN_CLICKED(PICKBTN, &CDlgPlotConfig::OnBnClickedPickbtn)
	ON_BN_CLICKED(IDC_BUTTON_BLOCKNAME, &CDlgPlotConfig::OnBnClickedButtonBlockname)
	ON_BN_CLICKED(IDC_BUTTON_LAYERNAME, &CDlgPlotConfig::OnBnClickedButtonLayername)
	//ON_BN_CLICKED(IDC_RADIO_PLT, &CDlgPlotConfig::OnBnClickedRadioPlt)
	ON_BN_CLICKED(IDC_CHECK_PLOTTOFILE, &CDlgPlotConfig::OnBnClickedCheckPlottofile)
	ON_BN_CLICKED(IDC_RADIO_DWG, &CDlgPlotConfig::OnBnClickedRadioDwg)
END_MESSAGE_MAP()

Acad::ErrorStatus addToCurrentSpace(AcDbEntity*pEnt,AcDbObjectId&retId)
{
	AcDbDatabase* pDb = acdbCurDwg();

	AcDbBlockTableRecord* blkRec;

	Acad::ErrorStatus es = acdbOpenObject(blkRec, pDb->currentSpaceId(), AcDb::kForWrite);
	if (es != Acad::eOk)
		return es;

	if (blkRec == NULL)
		return Acad::eInvalidInput;

	// append new entity to current space block
	es = blkRec->appendAcDbEntity(retId, pEnt);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\nERROR: could not add entity to current space."));
	}

	blkRec->close();
	return es;
}


//-----------------------------------------------------------------------------
CDlgPlotConfig::CDlgPlotConfig (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDlgPlotConfig::IDD, pParent, hInstance) 
, m_bIsPlotToFile(FALSE)
{
	m_nMultiDoc = 0;
	GetInfoFromReg();
	ClearAll(m_frames);
	m_strLayerOutArr.RemoveAll();
	m_strMultiDocNameArr.RemoveAll();
}

CDlgPlotConfig::~CDlgPlotConfig()
{
	if (pPlotSettings!=NULL)
	{
		delete pPlotSettings;
	}
	ClearAll(m_frames);
	m_strLayerOutArr.RemoveAll();
	m_strMultiDocNameArr.RemoveAll();
}
BOOL CDlgPlotConfig::Create( CWnd* pParent )
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::Create(CDlgPlotConfig::IDD, pParent);
}
//-----------------------------------------------------------------------------
void CDlgPlotConfig::DoDataExchange (CDataExchange *pDX) {
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
LRESULT CDlgPlotConfig::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CDlgPlotConfig::OnBnClickedRadioLayer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFrame = 0;
	GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(PICKBTN)->EnableWindow(TRUE);
}

void CDlgPlotConfig::OnBnClickedRadioBlock()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFrame = 1;
	GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(TRUE);
	GetDlgItem(PICKBTN)->EnableWindow(TRUE);
}

void CDlgPlotConfig::OnBnClickedRadioSanxian()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFrame = 2;
	GetDlgItem(IDC_EDIT_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LAYERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BLOCKNAME)->EnableWindow(FALSE);
	GetDlgItem(PICKBTN)->EnableWindow(FALSE);
}

void CDlgPlotConfig::RefreshMediaNames()
{
	AcArray<const TCHAR*> MediaList;

	m_CombMedia.ResetContent();

	pPSV->canonicalMediaNameList(pPlotSettings,MediaList);
	const TCHAR* locName;
	CString localName;
	Acad::ErrorStatus es;
	for(int c=0;c<MediaList.length();c++)
	{	
		es = pPSV->getLocaleMediaName(pPlotSettings, MediaList[c], locName);
		localName = locName;
		m_CombMedia.AddString(localName);
	}
	
	
	int nCur = m_CombMedia.FindString(0,(CString)m_strCurMedia);
	if (nCur == -1)
	{
		//nCur = 0;
		const TCHAR *pCurName;
		pPlotSettings->getCanonicalMediaName(pCurName);
		es = pPSV->getLocaleMediaName(pPlotSettings, pCurName, locName);
		m_CombMedia.SetCurSel(m_CombMedia.SelectString(0,(CString)locName));
	}
	else
	{
		m_CombMedia.SetCurSel(nCur);
	}
	//m_CombMedia.UpdateData();
}
void CDlgPlotConfig::OnCbnSelchangeComboDevice()
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

void CDlgPlotConfig::OnCbnSelchangeComboMedia()
{
	// TODO: 在此添加控件通知处理程序代码
	AcAxDocLock lock;

	m_CombMedia.UpdateData();
	m_CombMedia.GetLBText(m_CombMedia.GetCurSel(),m_strCurMedia);

	Acad::ErrorStatus es=pPSV->setCanonicalMediaName(pPlotSettings,m_strCurMedia);
}

void CDlgPlotConfig::OnCbnSelchangeComboPlotsytle()
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
	Acad::ErrorStatus es;
	pPlotSettings->setPlotPlotStyles(Adesk::kTrue);
	es	= pPlotSettings->setPlotSettingsName(m_strCurPlotStyle);
	const TCHAR *pCurStyleName;
	es = pPSV->setCurrentStyleSheet(pPlotSettings, m_strCurPlotStyle);
	//es = pPlotSettings->getCurrentStyleSheet(pCurStyleName);
}
void CDlgPlotConfig::OnBnClickedCheckFit()
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

void CDlgPlotConfig::OnBnClickedCheckCenter()
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

void CDlgPlotConfig::OnBnClickedRadioPlot()
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
	GetDlgItem(IDC_BUTTON_PREVIEW)->EnableWindow(TRUE);
}

void CDlgPlotConfig::OnBnClickedRadioLayerout()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("该功能还未完善，请等待后续更新"));
	/*CString strRes;
	strRes.LoadString(IDS_STRING_OUT2);
	MessageBox(strRes);
	return;*/

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


//void CDlgPlotConfig::OnBnClickedRadioPlt()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_nPlot = 2;
//	GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(TRUE);
//	GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
//
//	if (m_nFrame == 0)
//	{
//		OnBnClickedRadioLayer();
//	}
//	else if (m_nFrame == 1)
//	{
//		OnBnClickedRadioBlock();
//	}
//	else if (m_nFrame == 2)
//	{
//		OnBnClickedRadioSanxian();
//	}
//	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_AUTOROTATE)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_HENGXIANG)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_ZONGXIANG)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_ORDER)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_LEFTTORIGHT)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_UPTODOWN)->EnableWindow(TRUE);
//	GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(TRUE);
//	GetDlgItem(IDC_CHECK_FIT)->EnableWindow(TRUE);
//	GetDlgItem(IDC_CHECK_REVERSEORDER)->EnableWindow(TRUE);
//}


void CDlgPlotConfig::OnBnClickedRadioAutorotate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDirection = 0;
}

void CDlgPlotConfig::OnBnClickedRadioHengxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDirection = 1;
	Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k90degrees);
	OnBnClickedCheckFit();
	OnBnClickedCheckCenter();
}

void CDlgPlotConfig::OnBnClickedRadioZongxiang()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nDirection = 2;
	Acad::ErrorStatus es=pPSV->setPlotRotation(pPlotSettings,AcDbPlotSettings::k0degrees);
	OnBnClickedCheckFit();
	OnBnClickedCheckCenter();
}

void CDlgPlotConfig::OnBnClickedRadioOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOrder = 0;
}

void CDlgPlotConfig::OnBnClickedRadioLefttoright()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOrder = 1;
}

void CDlgPlotConfig::OnBnClickedRadioUptodown()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOrder = 2;
}

void CDlgPlotConfig::OnBnClickedCheckReverseorder()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_REVERSEORDER);
	m_bIsReserve = (BOOL)pBtn->GetCheck();
}

void CDlgPlotConfig::OnBnClickedButtonSelect()
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
	BeginEditorCommand();
	if (m_nMultiDoc == 0)
	{
		CJudgeFrame JudgeFrame;
		JudgeFrame.SetMode(m_nFrame);
		JudgeFrame.SetBlockName(m_strBlockName);
		JudgeFrame.SetLayerName(m_strLayerName);
		m_frames = JudgeFrame.GetDataFromDwg();

		nCout = GetFramecount();
	}
	else//多文档
	{
		Utility ulity;
		CString strPath =  _T("F:\\ARX\\test\\pldy\\dwg\\*.dwg");
		ulity.GetFileNameFromThePath(strPath, m_strMultiDocNameArr, _T(".DWG"));
		if (m_strMultiDocNameArr.GetSize() < 1L)
		{
			return;
		}
		nCout = m_strMultiDocNameArr.GetSize();
	}
	if (nCout>0)
	{
		CString strTmp;
		strTmp.Format(_T("%d"), nCout);
		GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strTmp);
		GetDlgItem(IDC_BUTTON_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_BRIGHT)->EnableWindow(TRUE);
		OnBnClickedCheckCenter();
		OnBnClickedCheckFit();
	}
	CompleteEditorCommand();
}

void CDlgPlotConfig::OnBnClickedButtonBright()
{
	// TODO: 在此添加控件通知处理程序代码
	BeginEditorCommand();
	ads_name ssName;
	ads_name_clear(ssName);
	acedSSAdd(NULL, NULL, ssName);
	long nLen = GetFramecount();
	if (nLen<=0)
	{
		CString strRes;
		strRes.LoadString(IDS_STRING_OUT5);
		MessageBox(strRes);
		CancelEditorCommand();
		return;
	}

	for (std::vector<FrameInfo*>::iterator iter = m_frames.begin(); iter!=m_frames.end(); iter++)
	{
		FrameInfo* pTempFI = *iter;

		AcGePoint3d minPt, maxPt;
		minPt = My2d23d(pTempFI->GetminPoint());
		maxPt = My2d23d(pTempFI->GetmaxPoint());

		AcDbObjectId lineId1;
		AcDbLine* pLine1 = new AcDbLine;
		pLine1->setStartPoint(minPt);
		pLine1->setEndPoint(maxPt);
		pLine1->setColorIndex(1);

		if ( addToCurrentSpace(pLine1,lineId1)!=Acad::eOk )
		{
			delete pLine1;
			CancelEditorCommand();
			return;
		}	
		pLine1->close();

		ads_name en1;
		if (acdbGetAdsName(en1, lineId1)!=Acad::eOk)
		{
			CancelEditorCommand();
			return;
		}
		if (acedSSAdd(en1, ssName, ssName)!=RTNORM)
		{
			CancelEditorCommand();
			return;
		}
		AcDbObjectId lineId2;
		AcDbLine* pLine2 = new AcDbLine;
		pLine2->setStartPoint(AcGePoint3d(minPt.x, maxPt.y, 0));
		pLine2->setEndPoint(AcGePoint3d(maxPt.x, minPt.y, 0));
		pLine2->setColorIndex(1);

		if ( addToCurrentSpace(pLine2,lineId2)!=Acad::eOk )
		{
			delete pLine2;
			CancelEditorCommand();
			return;
		}	
		pLine2->close();
		ads_name en2;
		if (acdbGetAdsName(en2, lineId2)!=Acad::eOk)
		{
			CancelEditorCommand();
			return;
		}
		if (acedSSAdd(en2, ssName, ssName)!=RTNORM)
		{
			CancelEditorCommand();
			return;
		}

		AcDbObjectId tId;
		AcDbPolyline* pPoly=new AcDbPolyline;
		AcGePoint2d minPt2d,maxPt2d;
		minPt2d.set(minPt.x, minPt.y);
		maxPt2d.set(maxPt.x, maxPt.y);
		pPoly->addVertexAt(0,minPt2d);
		pPoly->addVertexAt(1,AcGePoint2d(minPt.x, maxPt.y));
		pPoly->addVertexAt(2,maxPt2d);
		pPoly->addVertexAt(3,AcGePoint2d(maxPt.x, minPt.y));
		pPoly->addVertexAt(4, minPt2d);
		pPoly->setColorIndex(1);
		if ( addToCurrentSpace(pPoly,tId)!=Acad::eOk )
		{
			delete pPoly;
			CancelEditorCommand();
			return;
		}	
		pPoly->close();

		ads_name en3;
		if (acdbGetAdsName(en3, tId)!=Acad::eOk)
		{
			CancelEditorCommand();
			return;
		}
		if (acedSSAdd(en3, ssName, ssName)!=RTNORM)
		{
			CancelEditorCommand();
			return;
		}
	}

	TCHAR tmpName[132];
	CString strRes;
	strRes.LoadString(IDS_STRING_OUT6);
	acedGetString(0, strRes, tmpName);

	long len1;
	acedSSLength(ssName, &len1);
	if (len1<=0)
	{
		CancelEditorCommand();
		return;
	}
	else
	{
		AcDbEntity* pEnt1 = NULL;
		for (int i=0; i<len1; i++)
		{
			ads_name en;
			AcDbObjectId oId;
			acedSSName(ssName, i, en);
			acdbGetObjectId(oId, en);
			if (acdbOpenAcDbEntity(pEnt1, oId, AcDb::kForWrite)!=Acad::eOk)
			{
				CancelEditorCommand();
				return;
			}
			pEnt1->erase();
			pEnt1->close();
		}
	}
	acedSSFree(ssName);
	acTransactionManagerPtr()->queueForGraphicsFlush();
	acTransactionManagerPtr()->flushGraphics();//刷新
	acedUpdateDisplay();
	CompleteEditorCommand();
}

void CDlgPlotConfig::OnBnClickedButtonBrowse()
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


void CDlgPlotConfig::OnBnClickedButtonPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Acad::ErrorStatus es;
	CString strRes;

	if (m_nPlot == 1)
	{
		BeginEditorCommand();
		CString strLayerOut;
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
			//es = pPSV->setCurrentStyleSheet(pPlotSettings, m_strCurPlotStyle);
			AcPlPlotEngine* pEngine = NULL;
			if(Acad::eOk==AcPlPlotFactory::createPreviewEngine(pEngine))
			{
				es = pEngine->beginPlot(NULL);
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
					CompleteEditorCommand();
					continue;
				}

				const TCHAR *szDocName=acDocManager->curDocument()->fileName();

				es = pEngine->beginDocument(plotInfo, szDocName);
				es = pEngine->beginPage(pageInfo, plotInfo, true);    	

				es = pEngine->beginGenerateGraphics();
				es = pEngine->endGenerateGraphics();
				es = pEngine->endPage();
				es = pEngine->endDocument();
				es = pEngine->endPlot();
			}
			pEngine->destroy();
			pEngine = NULL;
		}
		CompleteEditorCommand();
	}
	else
	{
		AcAxDocLock lock;
		CString strScale;
		OnBnClickedCheckCenter();
		OnBnClickedCheckFit();

		OrderFrame(m_nOrder);//先屏蔽
		//打印比例不能为0
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

		BeginEditorCommand();
		int i=0;
		for (std::vector<FrameInfo*>::iterator itr = m_frames.begin(); itr != m_frames.end(); itr++)
		{
			if (i>0)
			{
				TCHAR szKword[256];
				int rc = acedInitGet(0, _T("No Yes"));
				CString strRes;
				strRes.LoadString(IDS_STRING_PREVIEW);
				rc = acedGetKword(strRes, szKword);
				if (rc == RTNORM)
				{
					if (_tcscmp(szKword, _T("No")) == 0)
					{
						break;
					}
				}
				else if (rc == RTCAN)
				{
					rc = acedInitGet(0, _T("No Yes"));
					rc = acedGetKword(strRes, szKword);
					if (rc == RTNORM)
					{
						if (_tcscmp(szKword, _T("No")) == 0)
						{
							break;
						}
					}
					else if (rc == RTCAN)
					{
						break;
					}
				}
			}
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
			//es = pPSV->setCurrentStyleSheet(pPlotSettings, m_strCurPlotStyle);

			AcPlPlotEngine* pEngine = NULL;
			if(Acad::eOk==AcPlPlotFactory::createPreviewEngine(pEngine))
			{
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
					CString strRes;
					strRes.LoadString(IDS_STRING_OUT3);
					AfxMessageBox(strRes);
					CompleteEditorCommand();
					pEngine->destroy();
					pEngine = NULL;
					break;
				}
				const TCHAR *szDocName=acDocManager->curDocument()->fileName();

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
			{
				CString strRes;
				strRes.LoadString(IDS_STRING_PLOT);
				AfxMessageBox(strRes);
			}
			i++;
		}
		CompleteEditorCommand();
	}
}

void CDlgPlotConfig::OnBnClickedOk()
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
		OrderFrame(m_nOrder);
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


	int i=1;

	BeginEditorCommand();
	
	if (m_nPlot == 2)
	{
		for (std::vector<FrameInfo*>::iterator itr = m_frames.begin(); itr != m_frames.end(); itr++)
		{
			FrameInfo* pFrame = *itr;
			AcGePoint2d minPt, maxPt;
			minPt = pFrame->GetminPoint();
			maxPt = pFrame->GetmaxPoint();
			acdbWcs2Ucs(asDblArray(maxPt), asDblArray(maxPt), false);
			acdbWcs2Ucs(asDblArray(minPt), asDblArray(minPt), false);
			AcGePoint3d minOut,maxOut;
			minOut.x = minPt.x;
			minOut.y = minPt.y;
			minOut.z = 0;

			maxOut.x = maxPt.x;
			maxOut.y = maxPt.y;
			maxOut.z = 0;
			//文件
			CString strFileName;
			CString strTmp;
			GetDlgItem(IDC_EDIT_FILENAME)->GetWindowText(m_strDeFaultFileName);
			strFileName = m_strDeFaultPath+_T("\\") + m_strDeFaultFileName;
			strTmp.Format(_T("%d"), i);
			strFileName+= strTmp;
			strFileName += _T(".dwg");

			OutPutDwg(minOut, maxOut, strFileName);

			i++;

		}
	}
	else
	{
		AcPlPlotEngine* pEngine = NULL;
		if(Acad::eOk==AcPlPlotFactory::createPublishEngine(pEngine))
		{
			if (m_nPlot == 1)//处理打印布局
			{
				CString strLayerOut;
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
					//es = pPSV->setCurrentStyleSheet(pPlotSettings, m_strCurPlotStyle);
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
						CompleteEditorCommand();
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
				for (std::vector<FrameInfo*>::iterator itr = m_frames.begin(); itr != m_frames.end(); itr++)
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
					//es = pPSV->setCurrentStyleSheet(pPlotSettings, m_strCurPlotStyle);
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
					const TCHAR *szDocName=acDocManager->curDocument()->fileName();
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
				pEngine->destroy();
				pEngine = NULL;
			}
		}
		else
		{
			strRes.LoadString(IDS_STRING_PLOT);
			AfxMessageBox(strRes);
		}
	}
	
	SaveInfoToReg();
	CompleteEditorCommand();
	CAcUiDialog::OnOK();
}

void CDlgPlotConfig::OnBnClickedPickbtn()
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



void CDlgPlotConfig::OnBnClickedButtonBlockname()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcModuleResourceOverride rs;
	CDlgBlockName dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_BLOCKNAME)->SetWindowText(dlg.GetBlockName());
	}
}

void CDlgPlotConfig::OnBnClickedButtonLayername()
{
	// TODO: 在此添加控件通知处理程序代码
	CAcModuleResourceOverride rs;
	CDlgListLayerName dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_LAYERNAME)->SetWindowText(dlg.GetLayerName());
	}
}

BOOL CDlgPlotConfig::OnInitDialog()
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
	acdbOpenObject(pLayout,layoutId,AcDb::kForWrite);

	m_bISModelType = pLayout->modelType();
	pPlotSettings=new AcDbPlotSettings(m_bISModelType);	
	//es = pPSV->setCurrentStyleSheet(pLayout, _T("monochrome.ctb"));
	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	es = pBTR->close();


	RefreshDevices();
	OnCbnSelchangeComboDevice();
	//RefreshMediaNames();
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

void CDlgPlotConfig::RefreshCTBNames()
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

void CDlgPlotConfig::OnDestroy()
{
	CDialog::OnDestroy();
	delete pPlotSettings;
	delete this;
}
void CDlgPlotConfig::RefreshDevices(void)
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
void CDlgPlotConfig::OrderFrame(int nSelect)
{
	ads_name ssname;
	ads_name_clear(ssname);
	acedSSAdd(NULL,NULL,ssname);

	long nLen = GetFramecount();
	int* IndexArr = new int[nLen];
	int nFlag;//记录
	AcGePoint3dArray minPtArr,midPtArr,maxPtArr;
	AcGePoint3d tmpPt;
	bool bIsChange = false;
	minPtArr.removeAll();
	maxPtArr.removeAll();
	midPtArr.removeAll();
	//保存原有的顺序

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_REVERSEORDER);
	int m_bIsReserve = pBtn->GetCheck();
	if (m_bIsReserve)
	{
		for (int i=0; i<nLen; i++)
		{
			IndexArr[i]=nLen-i-1;
		}
	}
	else
	{
		for (int i=0; i<nLen; i++)
		{
			IndexArr[i]=i;
		}
		if (m_nOrder!=0)
		{
			for (std::vector<FrameInfo*>::iterator iter = m_frames.begin(); iter!=m_frames.end(); iter++)
			{
				CString strScale,strDirection,strUpDown,strOffset;
				FrameInfo* pFrame = *iter;
				AcGePoint3d minPt, maxPt,midPt;
				minPt = My2d23d(pFrame->GetminPoint());
				maxPt = My2d23d(pFrame->GetmaxPoint());
				midPt.x = (minPt.x + maxPt.x)/2;
				midPt.y = (minPt.y + maxPt.y)/2;
				midPt.z = (minPt.z + minPt.z)/2;
				minPtArr.append(minPt);
				maxPtArr.append(maxPt);
				midPtArr.append(midPt);
			}

			for (int i=1; i<nLen; i++)
			{
				for (int j=0; j<nLen-i; j++)
				{
					if (nSelect == 1)
					{
						bIsChange = (midPtArr[j].x > midPtArr[j+1].x);
						if (abs(midPtArr[j].x-midPtArr[j+1].x)<0.0001)
						{
							bIsChange = midPtArr[j].y<midPtArr[j+1].y;
						}
					}
					else if (nSelect ==2)
					{
						bIsChange = (midPtArr[j].y < midPtArr[j+1].y);
						if (abs(midPtArr[j].y-midPtArr[j+1].y)<0.0001)
						{
							bIsChange = midPtArr[j].x>midPtArr[j+1].x;
						}
					}

					if (bIsChange)
					{
						tmpPt = midPtArr[j];
						midPtArr[j]=midPtArr[j+1];
						midPtArr[j+1]=tmpPt;

						nFlag = IndexArr[j];
						IndexArr[j] = IndexArr[j+1];
						IndexArr[j+1] = nFlag;
					}
				}
			}
		}
	}

	std::vector<FrameInfo*> tmpFrames;//临时变量
	for (int i=0; i<nLen; i++)
	{
		FrameInfo* pFrame = NULL;
		pFrame = GetFrame(IndexArr[i]);
		tmpFrames.push_back(pFrame);
	}
	m_frames.clear();
	for (std::vector<FrameInfo*>::iterator iter = tmpFrames.begin(); iter!=tmpFrames.end(); iter++)
	{
		m_frames.push_back(*iter);
	}
	tmpFrames.clear();
	delete []IndexArr;
}


//获取其中的某一个图框
FrameInfo* CDlgPlotConfig::GetFrame(int nIndex)
{
	FrameInfo* dwgInfo = NULL;
	int   i   =   0; 
	if(nIndex> m_frames.size()) 
	{
		return dwgInfo; 
	}
	std::vector<FrameInfo*>::iterator iter = m_frames.begin();
	while(i <nIndex) 
	{ 
		++iter; 
		i++; 
	} 
	dwgInfo = *iter;
	return   dwgInfo;
}

int CDlgPlotConfig::GetFramecount()
{
	int nCount = m_frames.size();
	return nCount;
}

void CDlgPlotConfig::ClearAll(std::vector<FrameInfo*> frames)
{
	for (std::vector<FrameInfo*>::iterator itr = frames.begin();
		itr != frames.end();
		itr++)
	{
		FrameInfo* pFrame = *itr;
		delete pFrame;
	}
}


bool CDlgPlotConfig::isPaperWidthLessHeight() const
{
	double paperWidth, paperHeight;
	pPlotSettings->getPlotPaperSize(paperWidth, paperHeight);
	return paperWidth < paperHeight;
}

bool CDlgPlotConfig::isWHSwap() const
{
	AcDbPlotSettings::PlotRotation pr = pPlotSettings->plotRotation();
	return pr == AcDbPlotSettings::k90degrees || pr == AcDbPlotSettings::k270degrees;
}

CString CDlgPlotConfig::GetStringFromReg(LPCTSTR strKey)
{
	CString strResult;
	CString strRootKey = _T("Software\\ZWPLDY");
	TCHAR ResStr[2048];
	COperateReg Reg;
	if (Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, _T(""), ResStr) > 0)
	{
		strResult = ResStr;
	}
	return strResult;
}

int CDlgPlotConfig::GetDwordFromReg(LPCTSTR strKey) const
{
	int nRet = 0;
	CString strResult;
	CString strRootKey = _T("Software\\ZWPLDY");
	DWORD dRes = 0;
	COperateReg Reg;
	nRet = Reg.GetRegister(HKEY_CURRENT_USER, strRootKey, strKey, dRes);
	return nRet;
}

bool CDlgPlotConfig::GetInfoFromReg()
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

void CDlgPlotConfig::SaveInfoToReg()
{
	CString strRootKey = _T("Software\\ZWPLDY");
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
	Reg.SetRegister(HKEY_CURRENT_USER, strRootKey, _T("First"), 1);
}

CString CDlgPlotConfig::GetDeFaultPaperSize() const
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

void CDlgPlotConfig::OnBnClickedCheckPlottofile()
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

void CDlgPlotConfig::OnBnClickedRadioDwg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPlot = 2;

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
	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_AUTOROTATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_HENGXIANG)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_ZONGXIANG)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_ORDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LEFTTORIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_UPTODOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_FIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_REVERSEORDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PREVIEW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
}

bool CDlgPlotConfig::OutPutDwg(AcGePoint3d minPt, AcGePoint3d maxPt, LPCTSTR strDwgFile)
{

	ads_name ssname;
	int nRet = acedSSGet(_T("C"), asDblArray(minPt), asDblArray(maxPt), NULL, ssname);
	if (nRet != RTNORM)
	{
		return false;
	}

	AcDbObjectIdArray objIds;
	objIds.removeAll();
	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	AcDbObjectId entId = AcDbObjectId::kNull;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(entId, ename);
		objIds.append(entId);
	}
	acedSSFree(ssname);
	AcDbDatabase *pDwg = new AcDbDatabase(); 
	Acad::ErrorStatus es;
	AcDbObjectId objId = AcDbObjectId::kNull;
	AcDbIdMapping idMap;
	AcDbObjectId ownerId;
	ownerId = pDwg->currentSpaceId();

	es = acdbHostApplicationServices()->workingDatabase()->wblockCloneObjects(objIds, ownerId, idMap, AcDb::kDrcIgnore);

	es = pDwg->saveAs(strDwgFile, false);
	if (pDwg != NULL)
	{
		delete pDwg;
	}
	pDwg = NULL;
	acTransactionManagerPtr()->queueForGraphicsFlush();
	acTransactionManagerPtr()->flushGraphics();//刷新
	acedUpdateDisplay();

	return true;
}