
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
	static void WRQ_ZDM_TKSZ(void)//ͼ������
	{
		// Add your code for command ZwForWHRQY._TKSZ here
		CAcModuleResourceOverride rs;
		CDlgTKSZ dlg;
		dlg.DoModal();
	}

	// - ZwForWHRQY._XJDMX command (do not rename)
	static void WRQ_ZDM_XJDMX(void)//�½���߼�������
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
	// Qualifier: �������ݲ��޸����е�ͼ�Σ���Ҫ����״ˮ���ߺ�ʵ��ˮ���߼����ӵ��¶���
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
	// Qualifier:�޸Ķ�������
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
	// Qualifier:ɾ��׮�ŵ�����
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
	// Qualifier:�½��ܵ�
	// Parameter: void
	//************************************
	static void WRQ_ZDM_XJGD(void)
	{
		// Add your code for command WRQ_ZDM._XJGD here
		CString strCount =  CDMXUtils::getNumCount();
		int nCount = MyTransFunc::StringToInt(strCount);
		if (nCount <= 1)
		{
			AfxMessageBox(_T("û�е���������"));
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
	// Qualifier:���Ӿ�ʾ������
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
	// Qualifier:��������ͼ�����總�����
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
	// Qualifier: ����ͼ��
	// Parameter: void
	//************************************
	static void WRQ_ZDM_SCTL(void)
	{
		// Add your code for command WRQ_ZDM._SCTL here
		CString strFileName;
		CString strBlkName;
		TCHAR szKword [132];
		int nRet = acedGetString(1, _T("\n�����������"), szKword);
		if (nRet != RTNORM)
		{
			return;
		}
		strBlkName = szKword;
		strFileName = MyBaseUtils::GetAppPath() + _T("zdm\\�������\\") + strBlkName + _T(".dwg"); 
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
	// Qualifier: �����ϰ���
	// Parameter: void
	//************************************
	static void WRQ_ZDM_JC(void)
	{
		// Add your code for command WRQ_ZDM._JC here
		CAcModuleResourceOverride rs;
		CDlgObstacle dlg;
		dlg.DoModal();
	}

	// - WRQ_ZDM._DELJC command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_DELJC
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_DELJC
	// Access:    public static 
	// Returns:   void
	// Qualifier:ѡ�񽻹ܣ�Ȼ��ɾ������
	// Parameter: void
	//************************************
	static void WRQ_ZDM_DELJC(void)
	{
		// Add your code for command WRQ_ZDM._DELJC here
		AcDbObjectId objId = AcDbObjectId::kNull;

		ads_name ename;
		AcGePoint3d pt;
		int nRet = acedEntSel(_T("\n��ѡ��Ҫɾ���Ľ��ܣ�"), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(objId, ename);
		CString strGroupName = MyEditEntity::openObjAndGetGroupName(objId);
		MyEditEntity::EraseEntByGroupName(strGroupName);
	}

	// - WRQ_ZDM._ZJFM command (do not rename)
	//************************************
	// Method:    WRQ_ZDM_ZJFM
	// FullName:  CZwForWHRQYApp::WRQ_ZDM_ZJFM
	// Access:    public static 
	// Returns:   void
	// Qualifier:���ӷ���
	// Parameter: void
	//************************************
	static void WRQ_ZDM_ZJFM(void)
	{
		// Add your code for command WRQ_ZDM._ZJFM here
		bool bDrawBc = CDMXUtils::getcreateBc();
		if (!bDrawBc)
		{
			AfxMessageBox(_T("\n�������ӵ�����"));
			return;
		}

		if (!CDMXUtils::getcreateGw())
		{
			AfxMessageBox(_T("\n���Ȼ��ƹܵ�"));
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		AcGePoint3d startPt,midPt,endPt;
		int nRet = acedGetPoint(NULL, _T("\n��ָ����ʾ�����"), asDblArray(startPt));
		if (nRet != RTNORM)
		{
			return;
		}
		nRet = acedGetPoint(asDblArray(startPt), _T("\n��ָ����ʾ���յ�"), asDblArray(midPt));
		if (nRet != RTNORM)
		{
			return;
		}
		acedGrDraw(asDblArray(startPt), asDblArray(midPt), 7, 1);
		int nOrtho;
		MyBaseUtils::GetVar(_T("ORTHOMODE"), &nOrtho);
		if (nOrtho == 0)
		{
			MyBaseUtils::SetVar(_T("ORTHOMODE"), 1);
		}
		nRet = acedGetPoint(asDblArray(midPt), _T("\n��ָ������"), asDblArray(endPt));
		if (nRet != RTNORM)
		{
			MyBaseUtils::SetVar(_T("ORTHOMODE"), nOrtho);
			return;
		}
		MyBaseUtils::SetVar(_T("ORTHOMODE"), nOrtho);

		AcGeVector3d vec = endPt - midPt;
		double dAng = vec.angleOnPlane(AcGePlane::kXYPlane);
		AcDbObjectId textId = AcDbObjectId::kNull;
		AcDbObjectId plineId = AcDbObjectId::kNull;
		AcDbObjectId textStyleId = MySymble::CreateTextStyle(_T("FSHZ"), _T("fszf.shx"), _T("fshz.shx"), 0.8, 3000.0/(CDMXUtils::getXScale()));
		AcGePoint3dArray points;
		double dLen;
		/*if (vec.x > 0)
		{*/
			//textId = MyDrawEntity::DrawText(midPt, m_strType, 3.0, textStyleId, AcDb::kTextLeft);
			textId = MyEditEntity::openEntChangeRotation(textId, dAng);
			dLen = MyEditEntity::OpenObjAndGetLength(textId);
			acutPolar(asDblArray(midPt), dAng, dLen, asDblArray(endPt));
			points.append(startPt);
			points.append(midPt);
			points.append(endPt);
			plineId = MyDrawEntity::DrawPlineByPoints(points);
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
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _DELJC, SCJC, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _ZJFM, ZJFM, ACRX_CMD_TRANSPARENT, NULL)
