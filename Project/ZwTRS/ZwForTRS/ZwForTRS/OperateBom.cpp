#include "StdAfx.h"
#include "OperateBom.h"
#include "DataBaseInfo.h"
#include "MyEditEntity.h"

COperateBom::COperateBom(void)
{
	for (map<CString, pair<CDataBaseInfo*, int> >::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		CDataBaseInfo* pData = NULL;
		pData = iter->second.first;
		if (pData != NULL)
		{
			delete pData;
			pData = NULL;
		}
	}
	m_Data.clear();
}

COperateBom::~COperateBom(void)
{
	for (map<CString, pair<CDataBaseInfo*, int> >::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		CDataBaseInfo* pData = NULL;
		pData = iter->second.first;
		if (pData != NULL)
		{
			delete pData;
			pData = NULL;
		}
	}
	m_Data.clear();
}


bool COperateBom::DoIt()
{
	if (!GetBom())
	{
		return false;
	}
	if (!SelectEnt())
	{
		return false;
	}

	pair<CDataBaseInfo*, int> tmpPair;
	int nIndex = 1;
	for (map<CString, pair<CDataBaseInfo*, int> >::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		CDataBaseInfo* pData = NULL;
		tmpPair = iter->second;
		pData = tmpPair.first;
		SetBom(pData, nIndex);
		nIndex++;

	}
	return true;
}

//选择实体
bool COperateBom::SelectEnt()
{
	bool bRet = false;

	ads_name ssname;
	struct resbuf*	filter=acutBuildList(-4,_T("<and"), RTDXF0, _T("INSERT"), -4, _T("and>"), RTNONE);
	int nRet = acedSSGet(NULL, NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return false;
	}
	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		AcDbEntity* pEnt = NULL;
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		int nCount =1;
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlkRef = AcDbBlockReference::cast(pEnt);
			
			pBlkRef->close();

			GetEntInfoById(objId);
		}
		else
		{
			pEnt->close();
		}
	}
	
	acedSSFree(ssname);
	return true;
}
//将数据添加到bom
bool COperateBom::AddDataToBom()
{
	return true;
}
//获取Bom表并将bom表的数据清空
bool COperateBom::GetBom()
{
	m_pZwmDb = m_appZwm.GetDb();
	if (m_pZwmDb)
	{
		m_pZwmDb->OpenFile(_T(""));
	}
	if (m_pZwmDb == NULL)
	{
		return false;
	}
	m_pBom = m_pZwmDb->GetBom();
	if (m_pBom == NULL)
	{
		return false;
	}
	////test
	//ZwcadmBomRow objBomRow;
	//m_pBom->GetItem(0, objBomRow);


	//CStringArray strArr;
	//strArr.RemoveAll();
	//objBomRow.GetLabels(strArr);
	//vector<CString> vec1;
	//vector<CString> vec2;
	//for (int i=0; i<strArr.GetSize(); i++)
	//{
	//	vec1.push_back(strArr.GetAt(i));
	//}
	//strArr.RemoveAll();
	//objBomRow.GetNames(strArr);
	//for (int i=0; i<strArr.GetSize(); i++)
	//{
	//	vec2.push_back(strArr.GetAt(i));
	//}
	//int nItemCount = objBomRow.GetItemCount();
	//for (int i=0; i<nItemCount; i++)
	//{		
	//	CString strLabel,strName,strValue;
	//	objBomRow.GetItem(i, strLabel, strName, strValue);

	//}
	//清空bom表里的数据
	
	//m_pBom->RomoveAll();
	return true;
}

//设置Bom表数据
bool COperateBom::SetBom(CDataBaseInfo* pData, int nCount)
{
	ZwcadmBomRow objBomRow;

	CString strXuhao;
	strXuhao.Format(_T("%d"), nCount);
	objBomRow.SetItem(_T("序号"), strXuhao);
	objBomRow.SetItem(_T("名称"), pData->GetFName());
	objBomRow.SetItem(_T("代号"), pData->GetFNumber());
	objBomRow.SetItem(_T("数量"), pData->GetShuliang());
	objBomRow.SetItem(_T("材料"), pData->GetCailiao());
	objBomRow.SetItem(_T("单重"), pData->GetDanzhong());
	objBomRow.SetItem(_T("总重"), pData->GetZongzhong());
	objBomRow.SetItem(_T("备注"), pData->GetBeizhu());
	objBomRow.SetItem(_T("单位"), pData->GetDanwei());
	objBomRow.SetItem(_T("零件类型"), pData->GetFErpCls());
	objBomRow.SetItem(_T("热处理"), pData->GetReChuli());
	objBomRow.SetItem(_T("表面处理"), pData->GetBiaoMianChuli());
	m_pBom->AddItem(objBomRow);
	return true;
}


ZwcadmBomRow COperateBom::SetBomCell()
{
	//COMPNAME 代号 COMPDES--名称
	//RELQTY--数量 COMPMAT--材料
	//COMPWT--单重 COMPTWT--总重 COMPREMARK--备注
	//COMPUMIS--单位 COMPITEM--零件类型
	///COMPTSUP--表面处理 COMPTTERM--热处理
	ZwcadmBomRow objBomRow;

	return objBomRow;
}

void COperateBom::GetEntInfoById(AcDbObjectId objId)
{
	CDataBaseInfo* dataInfo = new CDataBaseInfo();
	dataInfo->SetFItemId(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_DAIHAO));
	dataInfo->SetFName(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_MINGCHENG));
	//dataInfo->SetShuliang(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_SHULIANG));
	dataInfo->SetCailiao(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_CAILIAO));
	dataInfo->SetDanzhong(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_DANZHONG));
	//dataInfo->SetZongzhong(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_ZONGZHONG));
	dataInfo->SetBeizhu(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_BEIZHU));
	dataInfo->SetDanwei(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_DANWEI));
	dataInfo->SetFErpCls(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_LINGJIANLEIXING));
	//dataInfo->SetReChuli(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_RECHULI));
	//dataInfo->SetBiaoMianChuli(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_BIAOMIANCHULI));
	dataInfo->SetFNumber(MyEditEntity::GetObjStrXdata(objId, ZRXDBG_FNUMBER));
	CString strChartNum = MyEditEntity::GetObjStrXdata(objId, ZRXDBG_LIUCHENGTU_NUM);
	pair<CDataBaseInfo*, int> tmpPair;
	int nCount = 1;
	tmpPair.first = dataInfo;
	tmpPair.second = nCount;
	pair<map<CString, pair<CDataBaseInfo*, int> >::iterator, bool> bRet = m_Data.insert(make_pair(strChartNum, tmpPair));
	if (!bRet.second)
	{
		bRet.first->second.second++;
	}
}

