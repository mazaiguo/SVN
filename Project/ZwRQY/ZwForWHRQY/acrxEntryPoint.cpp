
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
#include "SpecialText.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

Acad::ErrorStatus AddtoModelSpace(AcDbEntity*pEnt, AcDbObjectId&retId, AcDbDatabase* pDb/* =NULL */)
{
	Acad::ErrorStatus es;
	AcDbBlockTable* pBlockTable;	//块表指针
	if(pDb==NULL)
		pDb=acdbCurDwg();

	es=pDb->getBlockTable(pBlockTable,AcDb::kForRead);	
	if(es!=Acad::eOk)
		return es;

	AcDbBlockTableRecord* pBlockTableRecord;	//块表记录指针
	es=pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,
		AcDb::kForWrite);
	if(es!=Acad::eOk)
	{
		pBlockTable->close();
		return es;
	}
	pBlockTable->close();	//关闭块表指针

	es=pBlockTableRecord->appendAcDbEntity(retId,pEnt);
	if(es!=Acad::eOk)
	{
		pBlockTableRecord->close();
		return es;
	}

	pBlockTableRecord->close();	//关闭模型空间块表记录指针
	return Acad::eOk;
}

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

		CSpecialText::rxInit();
		acrxBuildClassHierarchy();	
		
		

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		
		deleteAcRxClass(CSpecialText::desc());
		

		return (retCode) ;
	}

	virtual void RegisterServerComponents () 
	{
	}

	// - ZwForWHRQY._TKSZ command (do not rename)
	static void ZwForWHRQY_TKSZ(void)//图框设置
	{
		// Add your code for command ZwForWHRQY._TKSZ here
		CAcModuleResourceOverride rs;
		CDlgTKSZ dlg;
		dlg.DoModal();
	}

	// - ZwForWHRQY._XJDMX command (do not rename)
	static void ZwForWHRQY_XJDMX(void)//新建标尺及地面线
	{
		// Add your code for command ZwForWHRQY._XJDMX here
		CBiaochiInfo info;
		info.start();
	}

	// - ZwForWHRQY._TEST command (do not rename)
	static void ZwForWHRQY_TEST(void)
	{
		// Add your code for command ZwForWHRQY._TEST here
		CBcUtils utls;
		CZdmDataInfo m_pZDM;
		utls.add(m_pZDM.label(), m_pZDM);
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
	static void ZwForWHRQY_INSERTDATA(void)
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
	static void ZwForWHRQY_EditDM(void)
	{
		// Add your code for command ZwForWHRQY._EditDM here
	}

	// - ZwForWHRQY._Test command (do not rename)
	static void ZwForWHRQY_Test1(void)
	{
		// Add your code for command ZwForWHRQY._Test here
		CSpecialText* pText = new CSpecialText();
		pText->setPos(AcGePoint3d(0, 0, 0));
		pText->setHeig(3);
		pText->setTxtString(_T("ABCD"));
		//MyBaseUtils::addToCurrentSpaceAndClose(pText);
		AcDbObjectId objId;
		AddtoModelSpace(pText, objId, acdbCurDwg());
		pText->close();
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
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CZwForWHRQYApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, ZwForWHRQY, _TKSZ, TKSZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, ZwForWHRQY, _XJDMX, XJDMX, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, ZwForWHRQY, _TEST, TEST, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, ZwForWHRQY, _INSERTDATA, CRSJ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, ZwForWHRQY, _EditDM, EDDM, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, ZwForWHRQY, _Test1, Test1, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CZwForWHRQYApp, WRQ_ZDM, _DELDATA, SCSJ, ACRX_CMD_TRANSPARENT, NULL)
