//-----------------------------------------------------------------------------
//----- CBaseEntDoubleClick.cpp : Implementation of CBaseEntDoubleClick
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CBaseEntDoubleClick.h"
#include "ZDMUtils.h"
#include "BcUtils.h"
#include "ZdmDataInfo.h"
#include "DrawZDM.h"
#include "DrawDMXProcess.h"
#include "CDlgEditZhuangHao.h"

//-----------------------------------------------------------------------------
CBaseEntDoubleClick::CBaseEntDoubleClick (const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acedEditor )
			acedEditor->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CBaseEntDoubleClick::~CBaseEntDoubleClick () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CBaseEntDoubleClick::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CBaseEntDoubleClick::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *CBaseEntDoubleClick::Subject () const {
	return (acedEditor) ;
}

bool CBaseEntDoubleClick::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

// -----------------------------------------------------------------------------
void CBaseEntDoubleClick::beginDoubleClick(const AcGePoint3d & clickPoint)
{
	acDocManager->lockDocument(curDoc(),AcAp::kWrite,NULL,NULL,true); 
	
	ads_name ssname,ename;
	AcDbObjectId objId;
	AcDbObject* pObj = NULL;
	int nResult = acedSSGet(NULL, asDblArray(clickPoint), NULL, NULL, ssname);
	if (nResult == RTNORM)
	{
		long len;
		acedSSLength(ssname, &len);
		if (len > 1L)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 
			return;
		}

		acedSSName(ssname, 0, ename);
		Acad::ErrorStatus es = acdbGetObjectId(objId, ename);


		if (acdbOpenObject(pObj, objId, AcDb::kForWrite) != Acad::eOk)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 
			return;
		}
		AcDbObjectIdArray hardPointerIds, softPointerIds, hardOwnershipIds, softOwnershipIds;
		ArxDbgReferenceFiler filer;
		pObj->dwgOut(&filer);
		pObj->close();

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		hardPointerIds = filer.m_hardPointerIds;
		softPointerIds = filer.m_softPointerIds;
		hardOwnershipIds = filer.m_hardOwnershipIds;
		softOwnershipIds = filer.m_softOwnershipIds;
		if (softPointerIds.length() < 1)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 

			return;
		}

		AcDbObjectId entId = softPointerIds.at(0);
		AcDbGroup* pGroup = NULL;
		if (acdbOpenObject(pGroup, entId, AcDb::kForRead) != Acad::eOk)
		{
			acedSSFree(ssname);
			acDocManager->unlockDocument(curDoc()); 

			return;
		}
		TCHAR* name;
		pGroup->getName(name);
		pGroup->close();
		CString strGroupName = name;
		int nFind = strGroupName.Find(BC_DICT);
		//如果找到就要弹出对话框进行下一步的处理操作
		if (nFind >= 0)
		{
			//AfxMessageBox(_T("取到有用的数据!"));
			CBcUtils utils;
			CZdmDataInfo data;
			utils.get(strGroupName, data);

			CDlgEditZhuangHao dlg;
			dlg.setZDMData(&data);
			if (dlg.DoModal() == IDOK)
			{
				data = dlg.getData();
				MyEditEntity::EraseEntByGroupName(strGroupName);

				CDrawZDM zdm;
				zdm.setData(&data);
				bool bRet = zdm.mod(strGroupName);
			}
			else	
			{
				MyBaseUtils::flushGraphics(objId);
			}

			//if (pObj->isKindOf(AcDbText::desc()))
			//{
			//	AcDbText* pText = AcDbText::cast(pObj);
			//	CString strLayerName = pText->layer();
			//	pObj->close();
			//	if (strLayerName.CompareNoCase(_T("DM-TEXT")) == 0)
			//	{
			//		AcGePoint3d pt = pText->position();
			//		AcGePoint3d basePt;
			//		basePt = CDMXUtils::getbasePt();
			//		double dLen = basePt.y - pt.y;
			//		double dYScale = 1000/(CDMXUtils::getYScale());
			//		int nSize = ceil(dLen/(dYScale*1.5));
			//		CString strText = pText->textString();

			//		CString strTmp = BC_DICT;
			//		CString strCur;

			//		int nLen = strTmp.GetLength();
			//		strCur = strName.Right(strName.GetLength() - nLen);
			//		int nCount = MyTransFunc::StringToInt(strCur);

			//		CBcUtils utils;
			//		CZdmDataInfo data;
			//		utils.get(strName, data);				

			//		double dValue = MyTransFunc::StringToDouble(strText);
			//		if (nSize == 1)//设计地面高文字
			//		{
			//			dValue += 1;
			//			data.setDesignDmx(dValue);
			//		}
			//		else if (nSize == 2)//现状地面高文字
			//		{
			//			dValue += 1;
			//			data.setRealDmx(dValue);
			//		}
			//		else if (nSize == 4)//桩号文字
			//		{
			//			dValue -= 10;
			//			data.setcurData(dValue);
			//		}
			//		else
			//		{
			//			return;
			//		}
			//		data.setCount(strCur);
			//		data.setJiedian(strCur);
			//		MyEditEntity::EraseEntByGroupName(strName);

			//		CDrawZDM zdm;
			//		zdm.setData(&data);
			//		bool bRet = zdm.mod(strName);
			//	}
			//}
		}

		acutDelString(name);
	}
	acedSSFree(ssname);
	//MyBaseUtils::SetVar(_T("dblclkedit"), nCurDbl);
	acDocManager->unlockDocument(curDoc()); 
}

// -----------------------------------------------------------------------------

void CBaseEntDoubleClick::commandEnded(const ACHAR * cmdStr)
{
	AcEditorReactor::commandEnded (cmdStr) ;
	//if (_tcscmp(cmdStr, _T("REFEDIT")) == 0)
	//{
	//	if (!acdbHostApplicationServices()->workingDatabase())
	//		return;

	//	// get entities just operated on
	//	//
	//	ads_name sset;
	//	int err = acedSSGet(_T("P"), NULL, NULL, NULL, sset);
	//	if (err != RTNORM)
	//	{
	//		return;
	//	}

	//	long length;
	//	acedSSLength(sset, &length);

	//	ads_name en;
	//	AcDbObjectId eId;
	//	AcDbEntity *pEnt;
	//	bIsEdit = false;
	//	for (long i=0; i < length; i++) 
	//	{
	//		acedSSName(sset, i, en);

	//		Acad::ErrorStatus es = acdbGetObjectId(eId, en);
	//		if (es != Acad::eOk)
	//		{
	//			acedSSFree(sset);
	//			return;
	//		}
	//		es = acdbOpenObject(pEnt, eId, AcDb::kForRead);
	//		if (es != Acad::eOk)
	//		{
	//			continue;
	//		}
	//		if (pEnt->isKindOf(AcDbBlockReference::desc()))
	//		{
	//			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
	//			CString strBlkName;
	//			AcDbObjectId blkId = pBlkRef->blockTableRecord();
	//			pBlkRef->close();

	//			ArxDbgUtils::symbolIdToName(blkId, strBlkName);	
	//			CString strTmpName = strBlkName.Left(3);
	//			if (strTmpName.CompareNoCase(_T("DIE")) == 0)
	//			{
	//				bDefault = TRUE;
	//				bIsEdit = true;
	//				strDescrip = _T("DIE");
	//			}	
	//			else
	//			{
	//				int nAlingerCount = gGlobal.GetAlignerCount();
	//				std::map<CString, AcDbObjectId> dieIdMap = gGlobal.GetDieIdMap();
	//				for (int i=0; i<nAlingerCount; i++)
	//				{
	//					CString strAligner;
	//					strDescrip.Format(_T("Aligner%d"), i+1);
	//					std::map<CString, AcDbObjectId>::iterator iter = dieIdMap.find(strDescrip);
	//					AcDbObjectId objId = iter->second;
	//					CDieEntity* pDieEnt = NULL;
	//					if (acdbOpenAcDbEntity((AcDbEntity*&)pDieEnt, objId, AcDb::kForRead) == Acad::eOk)
	//					{
	//						strAligner = pDieEnt->GetBlockName();
	//						pDieEnt->close();
	//					}
	//					if (strBlkName.CompareNoCase(strAligner) == 0)
	//					{
	//						bIsEdit = true;
	//						bDefault = FALSE;
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			pEnt->close();
	//		}
	//	}
	//}
	//if (_tcscmp(cmdStr, _T("REFCLOSE")) == 0)//如果命令为REFCLOSE
	//{
	//	CDieData dieData;
	//	if (bIsEdit)
	//	{
	//		if (bDefault)
	//		{
	//			dieData.ModifiedDie(gGlobal.GetDeFaultDieId());
	//			if (gGlobal.GetCreateLayerGraph())
	//			{
	//				dieData.CreateDeFaultLayerGraph(TRUE);
	//			}
	//		}
	//		else
	//		{
	//			dieData.RegenAlignerDie(strDescrip);
	//		}
	//		//添加更新计算面积的功能
	//		double dDieCount = gGlobal.GetDieCount();
	//		if (dDieCount < 0.1)
	//		{
	//			AfxMessageBox(_T("请先生成扫描图谱"));
	//			return;
	//		}
	//	
	//		CTextForExMap txt;
	//		txt.DrawTextForMap();
	//	}
	//}

	//if ((_tcscmp(cmdStr, _T("CMD")) == 0) || (_tcscmp(cmdStr, _T("_CMDLOOP")) == 0))
	//{
	//	acDocManager->sendStringToExecute(curDoc(), _T("showinfo \n"), false, true);
	//}
	
}

// -----------------------------------------------------------------------------
void CBaseEntDoubleClick::commandCancelled(const ACHAR * cmdStr)
{
	AcEditorReactor::commandCancelled (cmdStr) ;
	/*if ((_tcscmp(cmdStr, _T("CMD")) == 0) || (_tcscmp(cmdStr, _T("_CMDLOOP")) == 0))
	{
		acDocManager->sendStringToExecute(curDoc(), _T("showinfo \n"), false, true);
	}*/
}

// -----------------------------------------------------------------------------
void CBaseEntDoubleClick::commandWillStart(const ACHAR * cmdStr)
{
	AcEditorReactor::commandWillStart (cmdStr) ;
}
