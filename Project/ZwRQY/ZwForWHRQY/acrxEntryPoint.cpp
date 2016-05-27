
//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "Global.h"
#include "DlgTKSZ.h"
#include "BiaochiInfo.h"
#include "BcUtils.h"
#include "DrawDMXProcess.h"
#include "StartDrawRQGD.h"
#include "DlgDrawJSD.h"
#include "DlgInsertOther.h"
#include "MakeBlkFile.h"
#include "DlgObstacle.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CZwForWHRQYApp : public AcRxArxApp 
{

public:
	CZwForWHRQYApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) 
	{
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		gGlobal.SetIni(gGlobal.GetIniPath());

		//acrxBuildClassHierarchy();	
		
		

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		return (retCode) ;
	}

	virtual void RegisterServerComponents () 
	{
	}

	// - ZwForWHRQY._TKSZ command (do not rename)
	static void WRQ_ZDM_TKSZ(void)//图框设置
	{
		// Add your code for command ZwForWHRQY._TKSZ here
		CAcModuleResourceOverride rs;
		CDlgTKSZ dlg;
		dlg.DoModal();
	}

	// - ZwForWHRQY._XJDMX command (do not rename)
	static void WRQ_ZDM_XJDMX(void)//新建标尺及地面线
	{
		// Add your code for command ZwForWHRQY._XJDMX here
		CBiaochiInfo info;
		info.start();
	}

	// - ZwForWHRQY._INSERTDATA command (do not rename)
	//************************************
	// Method:    ZwForWHRQY_INSERTDATA
	// FullName:  CZwForWHRQYApp::ZwForWHRQY_INSERTDATA
	// Access:    public static 
	// Returns:   void
	// Qualifier: 插入数据并修改现有的图形，主要是现状水面线和实际水面线及增加的坡度线
	// Parameter: void
	//************************************
	static void WRQ_ZDM_INSERTDATA(void)
	{
		// Add your code for command ZwForWHRQY._INSERTDATA here
		DrawDMXProcess dmxProcess;
		dmxProcess.Insert();
	}

	// - ZwForWHRQY._EditDM command (do not rename)
	//************************************
	// Method:    ZwForWHRQY_EditDM
	// FullName:  CZwForWHRQYApp::ZwForWHRQY_EditDM
	// Access:    public static 
	// Returns:   void
	// Qualifier:修改断面数据
	// Parameter: void
	//************************************
	static void WRQ_ZDM_EditDM(void)
	{
		// Add your code for command ZwForWHRQY._EditDM here
		DrawDMXProcess dmxProcess;
		dmxProcess.Mod();
	}

	// - WRQ_ZDM._DELDATA command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_DELDATA
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_DELDATA
	// Access:    public static 
	// Returns:   void
	// Qualifier:删除桩号等数据
	// Parameter: void
	//************************************
	static void WRQ_ZDM_DELDATA(void)
	{
		// Add your code for command WRQ_ZDM._DELDATA here
		DrawDMXProcess dmxProcess;
		dmxProcess.Del();
	}

	// - WRQ_ZDM._XJGD command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_XJGD
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_XJGD
	// Access:    public static 
	// Returns:   void
	// Qualifier:新建管道
	// Parameter: void
	//************************************
	static void WRQ_ZDM_XJGD(void)
	{
		// Add your code for command WRQ_ZDM._XJGD here
		CString strCount =  CDMXUtils::getNumCount();
		int nCount = MyTransFunc::StringToInt(strCount);
		if (nCount <= 1)
		{
			AfxMessageBox(_T("没有地面线数据"));
			return;
		}
		CStartDrawRQGD	gdProcess;
		gdProcess.start();
	}

	// - WRQ_ZDM._ZJJSD command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_ZJJSD
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_ZJJSD
	// Access:    public static 
	// Returns:   void
	// Qualifier:增加警示带功能
	// Parameter: void
	//************************************
	static void WRQ_ZDM_ZJJSD(void)
	{
		// Add your code for command WRQ_ZDM._ZJJSD here
		CAcModuleResourceOverride rs;
		CDlgDrawJSD dlg;
		dlg.DoModal();
	}

	// - WRQ_ZDM._CRQT command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_CRQT
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_CRQT
	// Access:    public static 
	// Returns:   void
	// Qualifier:插入其它图例，如附属配件
	// Parameter: void
	//************************************
	static void WRQ_ZDM_CRQT(void)
	{
		// Add your code for command WRQ_ZDM._CRQT here
		CAcModuleResourceOverride rs;
		CDlgInsertOther dlg;
		dlg.DoModal();
	}

	// - WRQ_ZDM._SCTL command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_SCTL
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_SCTL
	// Access:    public static 
	// Returns:   void
	// Qualifier: 生成图例
	// Parameter: void
	//************************************
	static void WRQ_ZDM_SCTL(void)
	{
		// Add your code for command WRQ_ZDM._SCTL here
		CString strFileName;
		CString strBlkName;
		TCHAR szKword [132];
		int nRet = acedGetString(1, _T("\n请输入块名："), szKword);
		if (nRet != RTNORM)
		{
			return;
		}
		strBlkName = szKword;
		strFileName = MyBaseUtils::GetAppPath() + _T("附属配件\\") + strBlkName + _T(".dwg"); 
		CMakeBlkFile mFile;
		mFile.SetFileName(strFileName);
		mFile.Save();
	}

	// - WRQ_ZDM._JC command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_JC
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_JC
	// Access:    public static 
	// Returns:   void
	// Qualifier: 插入障碍物
	// Parameter: void
	//************************************
	static void WRQ_ZDM_JC(void)
	{
		// Add your code for command WRQ_ZDM._JC here
		CAcModuleResourceOverride rs;
		CDlgObstacle dlg;
		dlg.DoModal();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CZwForWHRQYApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _TKSZ, TKSZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _XJDMX, XJDMX, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _INSERTDATA, CRSJ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _EditDM, EDDM, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _DELDATA, SCSJ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _XJGD, XJGD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _ZJJSD, ZJJSD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _CRQT, CRQT, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _SCTL, SCTL, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _JC, JC, ACRX_CMD_TRANSPARENT, NULL)
