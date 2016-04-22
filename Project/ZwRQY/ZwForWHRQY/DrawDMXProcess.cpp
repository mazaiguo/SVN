#include "StdAfx.h"
#include "DrawDMXProcess.h"
#include "DrawZDM.h"

DrawDMXProcess::DrawDMXProcess(void)
{
	CString strCount = CDMXUtils::getNumCount();
	m_nCout = MyTransFunc::StringToInt(strCount);
	m_bDrawJiedian = CDMXUtils::getcreateJiedian();
	m_pZdmInfo = new CZdmDataInfo();
	m_pZdmInfo->setCount(strCount);
	m_pZdmInfo->setJiedian(strCount);
	CString strLabel = BC_DICT + strCount;
	m_pZdmInfo->setLabel(strLabel);
	if (m_nCout == 1)
	{
		m_dZhuanghao = 0.00;
		m_dSJDmHeight = 0.00;
	}
	else
	{
		//�����Ϊ1����Ҫȡ��һ���洢�����е�����
		int nTmp = m_nCout - 1;
		strCount.Format(_T("%d"), nTmp);
		CString strTmpLabel = BC_DICT + strCount;
		CBcUtils bcUtils;

		CZdmDataInfo pBiaoChi = bcUtils.get(strTmpLabel);
		m_dZhuanghao = pBiaoChi.getcurData();
		m_dSJDmHeight = pBiaoChi.getDesignDmx();
	}
	m_dminElavation = CDMXUtils::getMinElavation();
	m_dmaxElavation = CDMXUtils::getMaxElavation();
}

DrawDMXProcess::~DrawDMXProcess(void)
{
	if (m_pZdmInfo != NULL)
	{
		delete m_pZdmInfo;
		m_pZdmInfo = NULL;
	}
}

bool DrawDMXProcess::Draw()
{
	if (m_nCout == 1)
	{
		if (!GetStartZhuanghao())
		{
			return false;
		}	
	}

	int nRet = GetZhuanghao();
	if (nRet == RTNORM)
	{
		if (m_bDrawJiedian)
		{
			GetIsJiedian();	
		}

		if (!GetSJDmHeight())
		{
			return false;
		}

		if (!GetXzDmHeight())
		{
			return false;
		}


		CDrawZDM zdm;
		zdm.setData(m_pZdmInfo);
		AcDbObjectId groupId = zdm.Draw();
		return true;
	}
	else if (nRet == RTKWORD)
	{
		return true;
	}
	else
	{
		return false;
	}
	
	//m_GroupIds.append(groupId);
	return true;
}

bool DrawDMXProcess::Insert()
{
	CString strPrompt;
	strPrompt.Format(_T("\n��ʼ׮��ֵ<m> <%.2f>:"), m_dZhuanghao);
	int nRet = acedGetReal(strPrompt, &m_dZhuanghao);
	if (nRet == RTNORM)
	{
		//return true;
	}
	/*else if (nRet == RTNONE)
	{
		m_dZhuanghao = m_dZhuanghao;
	}*/
	else
	{
		return false;
	}
	CString strCur = CurNumPosition(m_dZhuanghao);
	if (strCur.CompareNoCase(_T("0")) == 0)
	{
		return false;
	}
	CString strLabel = BC_DICT + strCur;
	m_pZdmInfo->setLabel(strLabel);
	m_pZdmInfo->setCount(strCur);
	m_pZdmInfo->setJiedian(strCur);
	m_pZdmInfo->setcurData(m_dZhuanghao);

	if (nRet == RTNORM)
	{
		if (m_bDrawJiedian)
		{
			GetIsJiedian();	
		}

		if (!GetSJDmHeight())
		{
			return false;
		}

		if (!GetXzDmHeight())
		{
			return false;
		}


		CDrawZDM zdm;
		zdm.setData(m_pZdmInfo);
		AcDbObjectId groupId = zdm.insert();
		return true;
	}
	else if (nRet == RTKWORD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DrawDMXProcess::Del()
{
	ads_name ename;
	AcGePoint3d pt;
	int nRet = acedEntSel(_T("\nѡ��Ҫɾ����׮��"), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	AcDbObjectId objId = AcDbObjectId::kNull;
	acdbGetObjectId(objId, ename);
	AcDbObjectIdArray objIdArrs;
	objIdArrs.removeAll();
	objIdArrs = MyEditEntity::openObjAndGetGroupIds(objId);
	if ((objIdArrs.length() <=0) || (objIdArrs.length() > 1))
	{
		AfxMessageBox(_T("û��ѡ��׮��"));
		return false;
	}
	AcDbObjectId groupId;
	groupId = objIdArrs.at(0);
	AcDbGroup* pGroup = NULL;
	if (acdbOpenAcDbObject((AcDbObject*&)pGroup, groupId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	CString strGroupName;
	strGroupName = pGroup->name();
	pGroup->close();
	
	CString strCur = CurNumPosition(strGroupName);
	int nCount = MyTransFunc::StringToInt(strCur);
	if (nCount == 1)
	{
		AfxMessageBox(_T("����ɾ����һ��׮��"));
		return false;
	}
	else if (nCount == 0)
	{
		AfxMessageBox(_T("û��ѡ��׮��"));
		return false;
	}
	
	//ɾ��dictionary
	CBcUtils utils;
	CString strNextGroupName;
	nCount++;
	CString strTmpCur;
	strTmpCur.Format(_T("%d"), nCount);
	strNextGroupName = BC_DICT + strTmpCur;
	CZdmDataInfo data = utils.get(strNextGroupName);
	data.setLabel(strGroupName);
	data.setCount(strCur);
	data.setJiedian(strCur);

	EraseEntFromDict(strGroupName);

	CDrawZDM zdm;
	zdm.setData(&data);
	bool bRet = zdm.mod(strGroupName);
	return bRet;
}

bool DrawDMXProcess::Mod()
{
	ads_name ename;
	AcGePoint3d pt;
	int nRet = acedEntSel(_T("\nѡ��Ҫ�༭��׮��"), ename, asDblArray(pt));
	if (nRet != RTNORM)
	{
		return false;
	}
	AcDbObjectId objId = AcDbObjectId::kNull;
	acdbGetObjectId(objId, ename);
	AcDbObjectIdArray objIdArrs;
	objIdArrs.removeAll();
	objIdArrs = MyEditEntity::openObjAndGetGroupIds(objId);
	if ((objIdArrs.length() <=0) || (objIdArrs.length() > 1))
	{
		AfxMessageBox(_T("û��ѡ��׮��"));
		return false;
	}
	AcDbObjectId groupId;
	groupId = objIdArrs.at(0);
	AcDbGroup* pGroup = NULL;
	if (acdbOpenAcDbObject((AcDbObject*&)pGroup, groupId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	CString strGroupName;
	strGroupName = pGroup->name();
	pGroup->close();

	CString strCur = CurNumPosition(strGroupName);
	int nCount = MyTransFunc::StringToInt(strCur);

	CBcUtils utils;
	CZdmDataInfo data = utils.get(strGroupName);
	double dZhuanghao = data.getcurData();
	data.setcurData(dZhuanghao - 10);
	
	CDrawZDM zdm;
	zdm.setData(&data);
	bool bRet = zdm.mod(strGroupName);
	return true;
}

//�������
bool DrawDMXProcess::GetStartZhuanghao()
{
	int nRet = acedGetReal(_T("\n���õ�����(��״�����)...\n׮�Ż�׼<0>"), &m_dstartZhuanghao);
	if (nRet == RTNORM)
	{
		//return true;
	}
	else if (nRet == RTNONE)
	{
		m_dstartZhuanghao = 0;
	}
	else
	{
		return false;
	}
	return true;
}

int DrawDMXProcess::GetZhuanghao()
{
	CString strPrompt;
	if (m_nCout == 1)
	{
		strPrompt.Format(_T("\n��ʼ׮��ֵ<m> <%.2f>:"), m_dZhuanghao);
	}
	else
	{
		acedInitGet(0, _T("Undo"));
		strPrompt.Format(_T("\n����һ��(U)/<��һ��׮��ֵ> <m> <%.2f>:"), m_dZhuanghao);
	}
	int nRet = acedGetReal(strPrompt, &m_dZhuanghao);
	if (nRet == RTNORM)
	{
		//return true;
	}
	else if (nRet == RTNONE)
	{
		//m_dZhuanghao = m_dZhuanghao;
		return RTERROR;
	}
	else if (nRet == RTKWORD)
	{
		doUndo();
		return RTKWORD;
	}
	else
	{
		return RTERROR;
	}
	m_pZdmInfo->setcurData(m_dZhuanghao);

	return RTNORM;
}

//�Ƿ�ڵ�
bool DrawDMXProcess::GetIsJiedian()
{
	acedInitGet(0 , _T("Yes No"));
	TCHAR szKword [132];
	szKword[0] = _T('N');	//��szKwordһ��Ĭ��ֵN
	szKword[1] = _T('\0');
	int nRet = acedGetKword(_T("\n�Ƿ񶨽ڵ��?>>��(Y)/��(N) <N>:"), szKword);
	if (nRet == RTNORM)	//����õ�����Ĺؼ���
	{
		if (_tcscmp(szKword, _T("Yes")) == 0)
			m_bDrawJiedian = true;
		else if (_tcscmp(szKword, _T("No")) == 0)
			m_bDrawJiedian = false;
	}
	else if (nRet == RTNONE)	//����û�����Ϊ��ֵ
	{
		m_bDrawJiedian = false;
	}
	
	CDMXUtils::SetcreateJiedian(m_bDrawJiedian);
	return m_bDrawJiedian;
}
//
bool DrawDMXProcess::GetSJDmHeight()
{
	CString strPrompt;
	strPrompt.Format(_T("\n��Ƶ�����<m> <%.2f>:"), m_dSJDmHeight);

	bool bRet = false;
	while(!bRet)
	{
		int nRet = acedGetReal(strPrompt, &m_dSJDmHeight);
		if (nRet == RTNORM)
		{
			//return true;
			if (verifyHeight(m_dSJDmHeight))
			{
				bRet = true;
			}
		}
		else if (nRet == RTNONE)
		{
			m_dSJDmHeight = m_dSJDmHeight;
			if (verifyHeight(m_dSJDmHeight))
			{
				bRet = true;
			}
		}
		else
		{
			bRet = false;
			//return false;
			break;
		}
	}
	
	m_pZdmInfo->setDesignDmx(m_dSJDmHeight);
	return bRet;
}

bool DrawDMXProcess::GetXzDmHeight()
{
	CString strPrompt;
	strPrompt.Format(_T("\n��״������<m> <%.2f>:"), m_dSJDmHeight);
	bool bRet = false;
	while(!bRet)
	{
	int nRet = acedGetReal(strPrompt, &m_dXzDmHeight);
	if (nRet == RTNORM)
	{
		//return true;
		if (verifyHeight(m_dXzDmHeight))
		{
			bRet = true;
		}
	}
	else if (nRet == RTNONE)
	{
		m_dXzDmHeight = m_dSJDmHeight;
		if (verifyHeight(m_dXzDmHeight))
		{
			bRet = true;
		}
	}
	else
	{
		bRet = false;
		break;
	}
	}
	m_pZdmInfo->setRealDmx(m_dXzDmHeight);
	return bRet;
}

bool DrawDMXProcess::verifyHeight( double dHeight )
{
	if (dHeight < m_dminElavation)
	{
		AfxMessageBox(_T("���������ݱȻ��������߱�߻��ͣ���������������"));
		return false;
	}
	if (dHeight > m_dmaxElavation)
	{
		AfxMessageBox(_T("���������ݱȻ��������߱�߻��ߣ���������������"));
		return false;
	}
	return true;
}

bool DrawDMXProcess::doUndo()
{
	int nTmp = m_nCout - 1;
	CString strCount;
	strCount.Format(_T("%d"), nTmp);
	CString strTmpLabel = BC_DICT + strCount;
		
	EraseEntFromDict(strTmpLabel);

	CBcUtils utils;
	utils.del(strTmpLabel);

	CDMXUtils::setNumCount(strCount);
	return true;
}

bool DrawDMXProcess::EraseEntFromDict( CString strGroupName )
{
	AcDbDictionary *pGroupDict;	
	AcDbGroup* pGroup = NULL;
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	if (pGroupDict->getAt(strGroupName, (AcDbObject*&)pGroup, AcDb::kForWrite) != Acad::eOk)
	{
		pGroupDict->close();
		return false;
	}

	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	AcDbObjectId objId;
	AcDbObjectIdArray objIds;
	objIds.removeAll();
	int nLength = 0;
	nLength = pGroup->allEntityIds(objIds);
	for (int i=0; i<objIds.length(); i++)
	{
		objId = objIds.at(i);
		es = acdbOpenAcDbEntity((AcDbEntity*&)pEnt, objId, AcDb::kForWrite);
		if (es!= Acad::eOk)
		{
			pEnt->close();
		}
		else
		{
			pEnt->erase();
			pEnt->close();
		}
	}
	pGroup->erase();
	pGroup->close();

	pGroupDict->close();
	return true;
}

CString DrawDMXProcess::CurNumPosition( double dValue )
{
	CString strCur = _T("0");
	double dZhuanghao;
	CBcUtils bcUtils;
	map<CString, CZdmDataInfo> data = bcUtils.getAllData();

	for (map<CString, CZdmDataInfo>::iterator iter = data.begin();
		iter != data.end();
		++iter)
	{
		CZdmDataInfo pData = iter->second;
		dZhuanghao = pData.getcurData();
		//��׮�ű���ʵֵ��ʱ��˵��λ�þ�������ط�
		if (dZhuanghao > dValue)
		{
			strCur = pData.getCount();
			break;
		}
	}
	return strCur;
}

CString DrawDMXProcess::CurNumPosition(CString strlabel)
{
	CString strTmp = BC_DICT;
	int nFind = strlabel.Find(BC_DICT);	
	CString strCur;

	if (nFind < 0)
	{
		strCur = _T("0");
		return strCur;
	}
	int nLen = strTmp.GetLength();
	strCur = strlabel.Right(strlabel.GetLength() - nLen);
	return strCur;
}
