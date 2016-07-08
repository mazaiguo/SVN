//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DistinguishData.h"
#include "DlgSettings.h"
#include "SerialNoJig.h"
//#include "CGasPipe.h"
#include "GWDesingUtils.h"
#include "DrawUtility.h"
#include "COperatePL.h"
#include "PipeUtils.h"
#include "SerialNoUtils.h"
#include "DlgModPipe.h"
#include "BlkInfo.h"
#include "DlgModQYD.h"
//#include "DlgBasicSettings.h"
//#include "DlgTextSettings.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CGwDesignApp : public AcRxArxApp {

public:
	CGwDesignApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - GwDesign._SBGW command (do not rename)
	static void GwDesign_SBGW(void)
	{
		// Add your code for command GwDesign._SBGW here
		CGWDesingUtils::SetCurNum(_T("1"));
		CGWDesingUtils::setNumCount(_T("1"));
		CDistinguishData dData;
		dData.doIt();
	}

	// - WRQ_GwDesign._SZ command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_SZ
	// FullName:  CGwDesignApp::WRQ_GwDesign_SZ
	// Access:    public static 
	// Returns:   void
	// Qualifier: ��������
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_SZ(void)
	{
		// Add your code for command WRQ_GwDesign._SZ here
		CAcModuleResourceOverride rs;
		CDlgSettings dlg;
		dlg.DoModal();
	}

	// - WRQ_GwDesign._CRXH command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_CRXH
	// FullName:  CGwDesignApp::WRQ_GwDesign_CRXH
	// Access:    public static 
	// Returns:   void
	// Qualifier:�������
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_XH(void)
	{
		// Add your code for command WRQ_GwDesign._CRXH here
		CSerialNoJig jig;
		jig.startJig();
	}

	// - WRQ_GwDesign._ZJGD command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_ZJGD
	// FullName:  CGwDesignApp::WRQ_GwDesign_ZJGD
	// Access:    public static 
	// Returns:   void
	// Qualifier: ���ӹܵ�
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_GD(void)
	{
		// Add your code for command WRQ_GwDesign._ZJGD here
		ads_name ename;
		AcDbObjectId startId,endId;
		AcGePoint3d startPt,endPt,tmpPt;
		int nRet = acedEntSel(_T("\n��ѡ�����:"), ename, asDblArray(tmpPt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(startId, ename);
		CSerialNo* pNo = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, startId, AcDb::kForRead) != Acad::eOk)
		{
			AfxMessageBox(_T("û��ѡ�����ʵ��"));
			return;
		}
		startPt = pNo->basePt();
		CString strStart = pNo->strText();
		pNo->close();

		nRet = acedEntSel(_T("\n��ѡ�����:"), ename, asDblArray(tmpPt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(endId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, endId, AcDb::kForRead) != Acad::eOk)
		{
			AfxMessageBox(_T("û��ѡ�����ʵ��"));
			return;
		}
		endPt = pNo->basePt();
		CString strEnd = pNo->strText();
		pNo->close();
		//���pline
		AcDbObjectId enId = MyDrawEntity::DrawPlineByTwoPoint(startPt, endPt);
		enId = MyEditEntity::openEntChangeLayer(enId, CGWDesingUtils::getGlobalPipeLayerId());
		enId = MyEditEntity::openPlineChangeWidth(enId, 1.0);
		COperatePL pl;
		enId = pl.trimbycircle(enId);
		MyEditEntity::OpenObjAppendStrToXdata(enId, START_ENT, strStart);
		MyEditEntity::OpenObjAppendStrToXdata(enId, END_ENT, strEnd);
	}

	// - WRQ_GwDesign._CRXH command (do not rename)
	//************************************
	// Method:    WRQ_GwDesign_CRXH
	// FullName:  CGwDesignApp::WRQ_GwDesign_CRXH
	// Access:    public static 
	// Returns:   void
	// Qualifier:	�������
	// Parameter: void
	//************************************
	static void WRQ_GwDesign_CRXH(void)
	{
		// Add your code for command WRQ_GwDesign._CRXH here
		ads_name ename;
		AcGePoint3d pt;
		AcDbObjectId objId = AcDbObjectId::kNull;
		int nRet = acedEntSel(_T("\n��ѡ��ȼ���ܵ���"), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		acdbGetObjectId(objId, ename);
		AcDbPolyline* pPoly = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pPoly, objId, AcDb::kForWrite) != Acad::eOk)
		{
			return;
		}
		double dLength = 0;
		AcGePoint3d startPt, endPt, midPt;
		pPoly->getStartPoint(startPt);
		pPoly->getEndPoint(endPt);
		pPoly->getDistAtPoint(endPt, dLength);
		if (dLength < 3*CGWDesingUtils::getGlobalRadius())
		{
			AfxMessageBox(_T("̫���ˣ������ڴ˴��������"));
			pPoly->close();
			return;
		}
		pPoly->close();

		bool bContinued = true;
		bool bErase = true;
		while (bContinued)
		{
			bContinued = false;
			nRet = acedGetPoint(asDblArray(startPt), _T("\n��ѡ������"), asDblArray(midPt));
			if (nRet != RTNORM)
			{
				bErase = false;
				break;
			}				
			
			if ((acutDistance(asDblArray(startPt), asDblArray(midPt)) < 2*CGWDesingUtils::getGlobalRadius())
				||(acutDistance(asDblArray(endPt), asDblArray(midPt)) < 2*CGWDesingUtils::getGlobalRadius()))
			{
				AfxMessageBox(_T("�����ڴ˴�������ţ���˵�̫������Ż��ص�"));
				bContinued = true;
				bErase = true;
				continue;
			}
		}	
		if (bErase)
		{
			COperatePL pl;
			pl.HalfPlByPt(objId, midPt);

		}
	}

	// - WRQ_GwDesign._Test command (do not rename)
	static void WRQ_GwDesign_HY(void)
	{
		// Add your code for command WRQ_GwDesign._Test here
		CSerialNoUtils no;
		no.delAllNo();
	}

	// - WRQ_GwDesign._HBDDX command (do not rename)
	//static void WRQ_GwDesign_HBDDX(void)
	//{
	//	// Add your code for command WRQ_GwDesign._HBDDX here
	//	ads_name ename;
	//	AcGePoint3d pt;
	//	int nRet = acedEntSel(_T("\n��ѡ��һ�������"), ename, asDblArray(pt));
	//	if (nRet != RTNORM)
	//	{
	//		return;
	//	}
	//	AcGePoint3d startPt,endPt;
	//	nRet = acedGetPoint(NULL, _T("\nѡ����ʼ�㣺"), asDblArray(startPt));
	//	if (nRet != RTNORM)
	//	{
	//		return;
	//	}

	//	AcDbObjectId objId = AcDbObjectId::kNull;
	//	acdbGetObjectId(objId, ename);
	//	COperatePL pl;
	//	AcDbObjectId entId = AcDbObjectId::kNull;
	//	entId = pl.restorePolyline(objId, startPt);
	//}

	// - WRQ_GwDesign._XSGD command (do not rename)
	static void WRQ_GwDesign_XSGD(void)
	{
		// Add your code for command WRQ_GwDesign._XSGD here
		CPipeUtils pipe;
		pipe.ShowGuanduanText();
	}

	// - WRQ_GwDesign._SCXH command (do not rename)
	static void WRQ_GwDesign_SCXH(void)
	{
		// Add your code for command WRQ_GwDesign._SCXH here
		ads_name ename;
		AcGePoint3d pt;
		int nRet = acedEntSel(_T("\n��ѡ����ţ�"), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		AcDbObjectId objId = AcDbObjectId::kNull;
		acdbGetObjectId(objId, ename);
		CSerialNo* pNo = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, objId, AcDb::kForRead) != Acad::eOk)
		{
			return;
		}
		pNo->close();

		CSerialNoUtils cNo;
		bool bRet = cNo.del(objId);
		if (!bRet)
		{
			return;
		}
	}

	// - WRQ_GwDesign._SCGD command (do not rename)
	static void WRQ_GwDesign_SCGD(void)
	{
		// Add your code for command WRQ_GwDesign._SCGD here
		ads_name ename;
		AcGePoint3d pt;
		int nRet = acedEntSel(_T("\n��ѡ��Ҫɾ���Ĺܵ���"), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		AcDbPolyline* pLine = NULL;
		AcDbObjectId objId = AcDbObjectId::kNull;
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pLine, objId, AcDb::kForRead) != Acad::eOk)
		{
			return;
		}

		CString strLayer = pLine->layer();
		if (strLayer.CompareNoCase(_T("ȼ���ܵ�")) != 0)
		{
			pLine->close();
			return;
		}
		pLine->close();

		CPipeUtils pipe;
		pipe.del(objId);
	}

	// - WRQ_GwDesign._XGGD command (do not rename)
	static void WRQ_GwDesign_XGGD(void)//�޸Ĺܵ�
	{
		// Add your code for command WRQ_GwDesign._XGGD here
		CAcModuleResourceOverride rs;
		CDlgModPipe mod;
		mod.DoModal();
	}

	// - WRQ_GwDesign._LIGD command (do not rename)
	static void WRQ_GwDesign_LIGD(void)
	{
		// Add your code for command WRQ_GwDesign._LIGD here
		resbuf *rb = acutBuildList( -4,_T("<and"),RTDXF0,_T("LWPOLYLINE"), 8, _T("ȼ���ܵ�"), -4,_T("and>"), RTNONE);
		ads_name ssname;
		int nRet = acedSSGet(NULL, NULL, NULL, rb, ssname);
		acutRelRb(rb);
		if (nRet != RTNORM)
		{
			return;
		}
		AcDbObjectId objId = AcDbObjectId::kNull;
		ads_name ename;
		long sslen;
		acedSSLength(ssname, &sslen);
		CString strStartNo,strEndNo,strDiameter,strTexture,strAmount;
		for (int i=0; i<sslen; i++)
		{
			acedSSName(ssname, i, ename);
			acdbGetObjectId(objId, ename);
			strStartNo = MyEditEntity::GetObjStrXdata(objId, START_ENT);
			strEndNo = MyEditEntity::GetObjStrXdata(objId, END_ENT);
			strDiameter = MyEditEntity::GetObjStrXdata(objId, GD_DIAMETER);
			strTexture = MyEditEntity::GetObjStrXdata(objId, GD_TEXTURE);
			strAmount = MyEditEntity::GetObjStrXdata(objId, GD_AMOUNT);
			acutPrintf(_T("\n��ʼ���:%s \n��ֹ��ţ�%s \n�ܾ���%s \n���ʣ�%s \n������%s"), strStartNo, strEndNo, strDiameter, strTexture, strAmount);
		}


		acedSSFree(ssname);
	}

	// - WRQ_GwDesign._CRQYD command (do not rename)
	static void WRQ_GwDesign_ZJQYD(void)
	{
		// Add your code for command WRQ_GwDesign._CRQYD here
		ads_name ename;
		AcGePoint3d pt,insertPt;
		int nRet = acedEntSel(_T("\n��ѡ����ţ�"), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		AcDbObjectId objId = AcDbObjectId::kNull;
		acdbGetObjectId(objId, ename);

		CSerialNo* pNo = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, objId, AcDb::kForRead) != Acad::eOk)
		{
			return;
		}
		AcGePoint3d basePt = pNo->basePt();
		CString strNo = pNo->strText();
		pNo->close();

		nRet = acedGetPoint(asDblArray(basePt), _T("\n��ָ���յ㣺"), asDblArray(insertPt));
		if (nRet != RTNORM)
		{
			return;
		}
		
		
		CBlkBaseInFo blkInfo;
		CString strPath = MyBaseUtils::GetAppPath() + _T("qyd.dwg");
		blkInfo.SetBlkName(_T("qyd"));
		blkInfo.SetFileName(strPath);
		blkInfo.SetInsertPt(insertPt);
		blkInfo.SetScale(AcGeScale3d(CGWDesingUtils::getGlobalScale(), CGWDesingUtils::getGlobalScale(), CGWDesingUtils::getGlobalScale()));

		CBlkInsert blkInsert;
		blkInsert.SetBlkInfo(blkInfo);
		blkInsert.SetModified(true);
		blkInsert.SetDrawWP(false);
		if (blkInsert.Insert()) 
		{
			AcDbObjectId blkId = blkInsert.GetObjectId();
			CString strCurNum = CGWDesingUtils::getCurNum();
			int nCurNum = MyTransFunc::StringToInt(strCurNum);
			
			int nQydNum = 1;
			MyBaseUtils::GetVar(_T("USERI3"), &nQydNum);//��¼��Դ��ĸ���
			nCurNum += nQydNum;
			strCurNum.Format(_T("%d"), nCurNum);

			AcDbObjectId pLine = MyDrawEntity::DrawPlineByTwoPoint(basePt, insertPt);
			COperatePL pL;
			pLine = pL.trimstartBycircle(pLine);
			pLine = MyEditEntity::openEntChangeLayer(pLine, CGWDesingUtils::getGlobalPipeLayerId());


			CSerialNoUtils sUtils;
			sUtils.addId(objId, pLine);

			MyEditEntity::OpenObjAppendStrToXdata(pLine, START_ENT, strNo);
			MyEditEntity::OpenObjAppendStrToXdata(pLine, END_ENT, strCurNum);
		
			AcDbHandle pHan = pLine.handle();
			MyEditEntity::OpenObjAppendHandleToXdata(blkId, GD_HANDLE, pHan);
			nQydNum++;
			MyBaseUtils::SetVar(_T("USERI3"), nQydNum);//��¼��Դ�����ʼ���	
			CGWDesingUtils::setNumCount(strCurNum);
		}
	}

	// - WRQ_GwDesign._SCQYD command (do not rename)
	static void WRQ_GwDesign_SCQYD(void)
	{
		// Add your code for command WRQ_GwDesign._SCQYD here
		ads_name ename;
		AcGePoint3d pt;
		int nRet = acedEntSel(_T("\n��ѡ����Դ�㣺"), ename, asDblArray(pt));
		if (nRet != RTNORM)
		{
			return;
		}
		AcDbObjectId objId = AcDbObjectId::kNull;
		acdbGetObjectId(objId, ename);
		AcDbBlockReference* pBlkRef = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pBlkRef, objId, AcDb::kForRead) != Acad::eOk)
		{
			return;
		}
		AcDbObjectId blockId = pBlkRef->blockTableRecord();
		pBlkRef->close();
		CString strBlockName;
		MyBaseUtils::symbolIdToName(blockId, strBlockName);
		if (strBlockName.CompareNoCase(_T("qyd")) != 0)
		{
			acutPrintf(_T("\nѡ��Ĳ�����Դ��."));
			return;
		}
		
		AcDbHandle pHan = MyEditEntity::GetObjHandleXdata(objId, GD_HANDLE);
		AcDbObjectId entId;
		AcDbDatabase* pDb = acdbCurDwg();
		Acad::ErrorStatus es = pDb->getAcDbObjectId(entId, true, pHan);

		CPipeUtils pipe;
		pipe.del(entId);

		MyEditEntity::EraseObj(objId);
		int nQydNum;
		MyBaseUtils::GetVar(_T("USERI3"), &nQydNum);//��¼��Դ�����ʼ���	
		nQydNum--;
		MyBaseUtils::SetVar(_T("USERI3"), nQydNum);
		CString strCurNum = CGWDesingUtils::getCurNum();
		int nCurNum = MyTransFunc::StringToInt(strCurNum);
		nCurNum += nQydNum;
		strCurNum.Format(_T("%d"), nCurNum);
		CGWDesingUtils::setNumCount(strCurNum);
	}

	// - WRQ_GwDesign._XGQYD command (do not rename)
	static void WRQ_GwDesign_XGQYD(void)
	{
		// Add your code for command WRQ_GwDesign._XGQYD here
		CAcModuleResourceOverride rs;
		CDlgModQYD dlg;
		dlg.DoModal();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGwDesignApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, GwDesign, _SBGW, SBGW, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _SZ, SZ, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _XH, XH, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _GD, ZJGD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _CRXH, CRXH, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _HY, HY, ACRX_CMD_TRANSPARENT, NULL)
//ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _HBDDX, HBDDX, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _XSGD, XSGD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _SCXH, SCXH, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _SCGD, SCGD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _XGGD, XGGD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _LIGD, LIGD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _ZJQYD, ZJQYD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _SCQYD, SCQYD, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGwDesignApp, WRQ_GwDesign, _XGQYD, XGQYD, ACRX_CMD_TRANSPARENT, NULL)
