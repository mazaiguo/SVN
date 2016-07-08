#include "StdAfx.h"
#include "SerialNoUtils.h"
#include "SerialNo.h"
#include "GWDesingUtils.h"
#include "COperatePL.h"
#include "PipeUtils.h"

CSerialNoUtils::CSerialNoUtils(void)
{
	m_dataInfo.clear();
}

CSerialNoUtils::~CSerialNoUtils(void)
{
	m_dataInfo.clear();
}

map<int, vector<AcDbObjectId> > CSerialNoUtils::getAllData()
{
	ads_name ssname;
	struct resbuf* filter = acutBuildList(-4,_T("<and"), RTDXF0, _T("ÐòºÅ"), -4,_T("and>"), RTNONE);
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return m_dataInfo;
	}
	
	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	CSerialNo* pSerialNo = NULL;
	int nCount = 0;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pSerialNo, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		CString strNo = pSerialNo->strText();
		nCount = MyTransFunc::StringToInt(strNo);
		vector<AcDbObjectId> tmpVec = pSerialNo->getAllId();
		pSerialNo->close();
		m_dataInfo.insert(make_pair(nCount, tmpVec));
	}
	acedSSFree(ssname);
	return m_dataInfo;
}

bool CSerialNoUtils::delAllNo()
{
	int nCur = MyTransFunc::StringToInt(CGWDesingUtils::getCurNum());

	ads_name ssname;
	struct resbuf* filter = acutBuildList(-4,_T("<and"), RTDXF0, _T("ÐòºÅ"), -4,_T("and>"), RTNONE);
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return false;
	}

	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	CSerialNo* pSerialNo = NULL;
	COperatePL pl;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pSerialNo, objId, AcDb::kForWrite) != Acad::eOk)
		{
			continue;
		}
		CString strNo = pSerialNo->strText();
		AcGePoint3d pt = pSerialNo->basePt();
		vector<AcDbObjectId> plinevec = pSerialNo->getAllId();
		pSerialNo->erase();
		pSerialNo->close();
		
		for (vector<AcDbObjectId>::iterator itr = plinevec.begin();
			itr != plinevec.end();
			++itr)
		{
			AcDbObjectId plineId = *itr;
			pl.restorePolyline(plineId, pt);
		}
	}
	acedSSFree(ssname);

	CGWDesingUtils::SetCurNum(_T("1"));
	int nNumCount = MyTransFunc::StringToInt(CGWDesingUtils::getNumCount()) - nCur;
	CString strTmp;
	strTmp.Format(_T("%d"), nNumCount);
	CGWDesingUtils::setNumCount(strTmp);
	return true;
}

bool CSerialNoUtils::del(AcDbObjectId objId)
{
	CSerialNo* pNo = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, objId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	vector<AcDbObjectId> vecInfo = pNo->getAllId();
	AcGePoint3d pt = pNo->basePt();
	CString strNo = pNo->strText();
	pNo->close();

	int nNo = MyTransFunc::StringToInt(strNo);
	if (vecInfo.size() == 1)
	{
		AcDbObjectId plineId = vecInfo.at(0);
		CString strStart = getPipeStr(plineId, strNo);
		
		removeId(objId, plineId);
		MyEditEntity::EraseObj(plineId);
		vector<AcDbObjectId> startVec = getdataById(strStart);
		if (startVec.size() < 1)
		{
			EraseNo(strStart);
		}
	}
	else if (vecInfo.size() == 2)
	{
		AcDbObjectId preId,nexId;
		preId = vecInfo.at(0);
		nexId = vecInfo.at(1);
		CString strStart,strEnd;

		COperatePL pl;
		preId = pl.restorePolyline(preId, pt);
		nexId = pl.restorePolyline(nexId, pt);

		vector<PlineInfo> vecPline= pl.JoinTwoPline(preId, nexId);
		AcDbObjectId enId = pl.drawPlineBydata(vecPline);
		enId = MyEditEntity::openEntChangeLayer(enId, CGWDesingUtils::getGlobalPipeLayerId());
		
		strStart = getPipeStr(preId, strNo, enId);
		strEnd = getPipeStr(nexId, strNo, enId);

		MyEditEntity::EraseObj(preId);
		MyEditEntity::EraseObj(nexId);

		int nCount = MyTransFunc::StringToDouble(strStart);
		if (nCount > nNo)
		{
			nCount--;
		}
		strStart.Format(_T("%d"), nCount);

		nCount = MyTransFunc::StringToDouble(strEnd);
		if (nCount > nNo)
		{
			nCount--;
		}
		strEnd.Format(_T("%d"), nCount);

		
		MyEditEntity::OpenObjAppendStrToXdata(enId, START_ENT, strStart);
		MyEditEntity::OpenObjAppendStrToXdata(enId, END_ENT, strEnd);

		


	}
	else if (vecInfo.size() == 0)
	{
		
	}
	else
	{
		AfxMessageBox(_T("´Ë´¦ÐòºÅ²»ÄÜÉ¾³ý"));
		return false;
	}
	bool bre = EraseNo(strNo);
	EditPipe(strNo);
	return bre;
}

bool CSerialNoUtils::add(CString strNo)
{
	return true;
}

bool CSerialNoUtils::removeId(AcDbObjectId noId, AcDbObjectId removeId, AcDbObjectId addId)
{
	CSerialNo* pNo = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity((AcDbEntity*&)pNo, noId, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		return false;
	}
	pNo->upgradeOpen();
	pNo->removeId(removeId);
	if (!addId.isNull())
	{
		pNo->addObjId(addId);
	}
	pNo->close();
	return true;
}

bool CSerialNoUtils::addId(AcDbObjectId noId, AcDbObjectId addId)
{
	CSerialNo* pNo = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity((AcDbEntity*&)pNo, noId, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		return false;
	}
	pNo->upgradeOpen();
	if (!addId.isNull())
	{
		pNo->addObjId(addId);
	}
	pNo->close();
	return true;
}

map<int, AcDbObjectId> CSerialNoUtils::getAllNo()
{
	map<int, AcDbObjectId> mapInfo;
	ads_name ssname;
	struct resbuf* filter = acutBuildList(-4,_T("<and"), RTDXF0, _T("ÐòºÅ"), -4,_T("and>"), RTNONE);
	int nRet = acedSSGet(_T("X"), NULL, NULL, filter, ssname);
	acutRelRb(filter);
	if (nRet != RTNORM)
	{
		return mapInfo;
	}

	long sslen;
	acedSSLength(ssname, &sslen);
	ads_name ename;
	AcDbObjectId objId = AcDbObjectId::kNull;
	CSerialNo* pSerialNo = NULL;
	for (int i=0; i<sslen; i++)
	{
		acedSSName(ssname, i, ename);
		acdbGetObjectId(objId, ename);
		if (acdbOpenAcDbEntity((AcDbEntity*&)pSerialNo, objId, AcDb::kForRead) != Acad::eOk)
		{
			continue;
		}
		CString strNo = pSerialNo->strText();
		int nCount = MyTransFunc::StringToInt(strNo);
		pSerialNo->close();

		mapInfo.insert(make_pair(nCount, objId));
	}
	acedSSFree(ssname);
	return mapInfo;
}

AcGePoint3d CSerialNoUtils::basePt(AcDbObjectId objId)
{
	AcGePoint3d basePt;
	CSerialNo* pNo = NULL;
	if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, objId, AcDb::kForRead) != Acad::eOk)
	{
		return basePt;
	}
	basePt = pNo->basePt();
	pNo->close();
	return basePt;
}

CString CSerialNoUtils::getPipeStr(AcDbObjectId plineId, CString strNo, AcDbObjectId addId)
{
	CString strStartNo, strEndNo, strRet;
	strStartNo = MyEditEntity::GetObjStrXdata(plineId, START_ENT);
	strEndNo = MyEditEntity::GetObjStrXdata(plineId, END_ENT);
	if (strStartNo.CompareNoCase(strNo) == 0)
	{
		AcDbObjectId entId = getIdByNo(strEndNo);
		strRet = strEndNo;
		removeId(entId, plineId, addId);
	}
	else
	{
		AcDbObjectId entId = getIdByNo(strStartNo);
		strRet = strStartNo;
		removeId(entId, plineId, addId);
	}
	return strRet;
}

bool CSerialNoUtils::EraseNo(CString strNo)
{
	int nNo = MyTransFunc::StringToInt(strNo);
	int nCur = MyTransFunc::StringToInt(CGWDesingUtils::getCurNum());

	//ÐÞ¸ÄÐòºÅ
	map<int, AcDbObjectId> NoInfo = getAllNo();
	map<int, AcDbObjectId>::iterator iT = NoInfo.find(nNo);
	if (iT == NoInfo.end())
	{
		return false;
	}
	AcDbObjectId entId = AcDbObjectId::kNull;
	AcGePoint3d basePt;
	vector<AcDbObjectId> vecInfo;
	bool bEarse = true;
	for (; iT != NoInfo.end();
		++iT)
	{
		entId = iT->second;
		CSerialNo* pNo = NULL;
		if (acdbOpenAcDbEntity((AcDbEntity*&)pNo, entId, AcDb::kForWrite) != Acad::eOk)
		{
			continue;
		}
		if (bEarse)
		{
			basePt = pNo->basePt();
			vecInfo = pNo->getAllId();
			pNo->erase();
		}
		else
		{			
			CString strTmp = pNo->strText();
			int nCount = MyTransFunc::StringToInt(strTmp);
			nCount--;
			strTmp.Format(_T("%d"), nCount);
			pNo->setstrText(strTmp);
		}
		pNo->close();

		bEarse = false;
	}

	nCur--;
	CString strTmp;
	strTmp.Format(_T("%d"), nCur);
	CGWDesingUtils::SetCurNum(strTmp);
	return true;
}

bool CSerialNoUtils::EditPipe(CString strNo)
{
	int nCount = MyTransFunc::StringToInt(strNo);
	CPipeUtils pipe;
	vector<AcDbObjectId> pipeInfo = pipe.getAllPipe();
	CString strStart,strEnd;
	for (vector<AcDbObjectId>::iterator iter = pipeInfo.begin();
		iter != pipeInfo.end();
		++iter)
	{
		AcDbObjectId plineId = *iter;
		strStart = MyEditEntity::GetObjStrXdata(plineId, START_ENT);
		int nCur = MyTransFunc::StringToInt(strStart);
		if (nCur > nCount)
		{
			nCur--;
			strStart.Format(_T("%d"), nCur);
			MyEditEntity::OpenObjAppendStrToXdata(plineId, START_ENT, strStart);
		}
		strEnd = MyEditEntity::GetObjStrXdata(plineId, END_ENT);
		nCur = MyTransFunc::StringToInt(strEnd);
		if (nCur > nCount)
		{
			nCur--;
			strEnd.Format(_T("%d"), nCur);
			MyEditEntity::OpenObjAppendStrToXdata(plineId, END_ENT, strEnd);
		}
	}

	return true;
}

AcDbObjectId CSerialNoUtils::getIdByNo(CString strNo)
{
	int nCount = MyTransFunc::StringToInt(strNo);
	map<int, AcDbObjectId> NoInfo = getAllNo();
	map<int, AcDbObjectId>::iterator iter = NoInfo.find(nCount);
	if (iter == NoInfo.end())
	{
		return AcDbObjectId::kNull;
	}
	AcDbObjectId entId = iter->second;
	return entId;
	
}

vector<AcDbObjectId> CSerialNoUtils::getdataById(CString strNo)
{
	int nCount = MyTransFunc::StringToInt(strNo);
	map<int, vector<AcDbObjectId> > mInfo = getAllData();
	vector<AcDbObjectId> info;
	map<int, vector<AcDbObjectId> >::iterator iter = mInfo.find(nCount);
	if (iter != mInfo.end())
	{
		info = iter->second;
	}

	return info;
}	
