#include "StdAfx.h"
#include "BcUtils.h"

CBcUtils::CBcUtils(void)
{
	m_Data.clear();
}

CBcUtils::~CBcUtils(void)
{
	m_Data.clear();
}

bool CBcUtils::add( LPCTSTR strLabel, CZdmDataInfo data)
{
	//将数据写入dictionary中
	getAllData();
	map<CString, CZdmDataInfo>::iterator iter = m_Data.find(strLabel);	
	AcDbObjectId StyleId;

	if (iter == m_Data.end())//如果dictionary中没有就添加记录
	{
		AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
			CBiaochiForRQY::dictName(), true,
			acdbHostApplicationServices()->workingDatabase());

		Acad::ErrorStatus es;
		if (testDict->getAt(strLabel, StyleId) != Acad::eOk)
		{
			CBiaochiForRQY* newRec = new CBiaochiForRQY;

			newRec->setLabel(data.label());
			newRec->setCount(data.getCount());
			newRec->setDesignDmx(data.getDesignDmx());
			newRec->setRealDmx(data.getRealDmx());
			newRec->setJiedian(data.getJiedian());
			newRec->setGuanDi(data.getGuanDi());
			newRec->setWaShen(data.getWaShen());
			newRec->setPoDu(data.getPoDu());
			newRec->setJuli(data.getJuli());
			newRec->setcurData(data.getcurData());
			////////////////////
			newRec->setHasBulge(data.getHasBulge());
			newRec->setDesingDmxS(data.getDesignDmxS());
			newRec->setRealDmxS(data.getRealDmxS());
			newRec->setJiedianS(data.getJiedianS());

			es = testDict->setAt(strLabel, newRec, StyleId);
			if (es == Acad::eOk) 
			{
				newRec->close();
			}
			else 
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}

		testDict->close();
	}
	else//如果有，就表示要修改当前数据
	{
		modify(strLabel, data);
	}
	return true;
}

bool CBcUtils::insert( LPCTSTR strLabel, CZdmDataInfo data)
{
	getAllData();
	map<CString, CZdmDataInfo>::iterator iter = m_Data.find(strLabel);
	if (iter == m_Data.end())
	{
		//没有找到，说明插入的数据不对，不能在那个位置插入数据
		return false;
	}
	else
	{
		//倒序查找
		vector<pair<CString, CZdmDataInfo> > tmpVec;
		bool bIsCreased = true;
		for (map<CString, CZdmDataInfo>::reverse_iterator iRevers = m_Data.rbegin();
			iRevers != m_Data.rend(); ++iRevers)
		{
			CZdmDataInfo tmpData = iRevers->second;
			CString strName = iRevers->first;
			CString strTmpName;

			if (strName.CompareNoCase(strLabel) == 0)
			{
				CString strCount = iRevers->second.getCount();
				int nCount = MyTransFunc::StringToInt(strCount);
				nCount++;
				strCount.Format(_T("%d"), nCount);
				strTmpName = BC_DICT + strCount;
				tmpData.setCount(strCount);
				tmpData.setJiedian(strCount);
				tmpData.setLabel(strTmpName);
				tmpVec.push_back(make_pair(strTmpName, tmpData));
				bIsCreased = false;
				continue;
			}
			CString strCount = iRevers->second.getCount();
			int nCount = MyTransFunc::StringToInt(strCount);
			if (bIsCreased)
			{
				nCount++;
			}
			strCount.Format(_T("%d"), nCount);
			strTmpName = BC_DICT + strCount;
			tmpData.setCount(strCount);
			tmpData.setJiedian(strCount);
			tmpData.setLabel(strTmpName);
			tmpVec.push_back(make_pair(strTmpName, tmpData));
		}

		//m_Data.insert(make_pair(strLabel, data));
		tmpVec.push_back(make_pair(strLabel, data));
		m_Data.clear();
		for (vector<pair<CString, CZdmDataInfo> >::iterator iter = tmpVec.begin();
			iter != tmpVec.end(); ++iter)
		{
			m_Data.insert(make_pair(iter->first, iter->second));
		}
		//删除所有，然后添加
		delAll();
		addAll();
	}
	return true;
}

bool CBcUtils::modify( LPCTSTR strLabel, CZdmDataInfo pData )
{
	getAllData();
	map<CString, CZdmDataInfo>::iterator iter = m_Data.find(strLabel);
	if (iter == m_Data.end())
	{
		//没有找到
		return false;
	}
	else	
	{
		//CZdmDataInfo tmpData = iter->second;
		iter->second.setLabel(pData.label());
		iter->second.setCount(pData.getCount());
		iter->second.setJiedian(pData.getJiedian());
		iter->second.setDesignDmx(pData.getDesignDmx());
		iter->second.setRealDmx(pData.getRealDmx());
		iter->second.setJiedian(pData.getJiedian());
		iter->second.setGuanDi(pData.getGuanDi());
		iter->second.setWaShen(pData.getWaShen());
		iter->second.setPoDu(pData.getPoDu());
		iter->second.setJuli(pData.getJuli());
		iter->second.setcurData(pData.getcurData());
		////////////////////
		iter->second.setHasBulge(pData.getHasBulge());
		iter->second.setDesingDmxS(pData.getDesignDmxS());
		iter->second.setRealDmxS(pData.getRealDmxS());
		iter->second.setJiedianS(pData.getJiedianS());

		//删除所有，然后添加
		delAll();
		addAll();
	}
	return true;
}
//
bool CBcUtils::del( LPCTSTR strLabel )
{
	getAllData();
	map<CString, CZdmDataInfo>::iterator iter = m_Data.find(strLabel);
	if (iter != m_Data.end())
	{
		//倒序查找
		bool bIsDeCreased = true;
		vector<pair<CString, CZdmDataInfo> > tmpVec;
		for (map<CString, CZdmDataInfo>::reverse_iterator iRevers = m_Data.rbegin();
			iRevers != m_Data.rend(); ++iRevers)
		{
			CZdmDataInfo tmpData = iRevers->second;
			CString strName = iRevers->first;
			CString strTmpName;

			if (strName.CompareNoCase(strLabel) == 0)
			{
				bIsDeCreased = false;
				continue;
			}
			CString strCount = iRevers->second.getCount();
			int nCount = MyTransFunc::StringToInt(strCount);
			if (bIsDeCreased)
			{
				nCount--;
			}
			strCount.Format(_T("%d"), nCount);
			strTmpName = BC_DICT + strCount;
			tmpData.setCount(strCount);
			tmpData.setJiedian(strCount);
			tmpData.setLabel(strTmpName);
			tmpVec.push_back(make_pair(strTmpName, tmpData));
		}

		m_Data.clear();
		for (vector<pair<CString, CZdmDataInfo> >::iterator iter = tmpVec.begin();
			iter != tmpVec.end(); ++iter)
		{
			m_Data.insert(make_pair(iter->first, iter->second));
		}
		//删除所有，然后添加
		delAll();
		addAll();
	}
	else
	{
		//没有这个元素
		return false;
	}
	return true;
}

bool CBcUtils::get( LPCTSTR strLabel ,CZdmDataInfo& bcData)
{
	getAllData();
	map<CString, CZdmDataInfo>::iterator iter = m_Data.find(strLabel);
	if (iter != m_Data.end())
	{ 
		bcData = iter->second;
		return true;
	}
	return false;
}

//************************************
// Method:    getCount
// FullName:  CBcUtils::getCount
// Access:    public 
// Returns:   int
// Qualifier:
//************************************
//int CBcUtils::getCount()
//{
//	getAllData();
//	int nCount = m_Data.size();
//	return nCount;
//}

map<CString, CZdmDataInfo> CBcUtils::getAllData()
{
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForRead(
		CBiaochiForRQY::dictName(),	acdbHostApplicationServices()->workingDatabase());
	Acad::ErrorStatus es;
	//如果没有数据直接退出
	if (testDict == NULL)
	{
		return m_Data;
	}

	AcDbDictionaryIterator* pIter = testDict->newIterator();
	for (; !pIter->done(); pIter->next())
	{
		AcDbObject* pObj = NULL;
		es = pIter->getObject(pObj, AcDb::kForRead);
		if (pObj->isKindOf(CBiaochiForRQY::desc()))
		{
			CZdmDataInfo data;
			CBiaochiForRQY* pEnt = CBiaochiForRQY::cast(pObj);
			data.setLabel(pEnt->label());
			data.setCount(pEnt->getCount());
			data.setJiedian(pEnt->getJiedian());
			data.setDesignDmx(pEnt->getDesignDmx());
			data.setRealDmx(pEnt->getRealDmx());
			data.setJiedian(pEnt->getJiedian());
			data.setGuanDi(pEnt->getGuanDi());
			data.setWaShen(pEnt->getWaShen());
			data.setPoDu(pEnt->getPoDu());
			data.setJuli(pEnt->getJuli());
			data.setcurData(pEnt->getcurData());
			////////////////////
			data.setHasBulge(pEnt->getHasBulge());
			data.setDesingDmxS(pEnt->getDesingDmxS());
			data.setRealDmxS(pEnt->getRealDmxS());
			data.setJiedianS(pEnt->getJiedianS());
			m_Data.insert(make_pair(pEnt->label(), data));
		}
		es = pObj->close();	
	}
	delete pIter;
	
	testDict->close();
	return m_Data;
}

void CBcUtils::addAll()
{
	AcDbObjectId StyleId;
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBiaochiForRQY::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());

	Acad::ErrorStatus es;

	for (map<CString, CZdmDataInfo>::iterator iter = m_Data.begin();
		iter != m_Data.end();
		++iter)
	{
		CString strLable = iter->first;
		CZdmDataInfo data = iter->second;
		if (testDict->getAt(strLable, StyleId) != Acad::eOk)
		{
			CBiaochiForRQY* newRec = new CBiaochiForRQY;

			newRec->setLabel(data.label());
			newRec->setCount(data.getCount());
			newRec->setDesignDmx(data.getDesignDmx());
			newRec->setRealDmx(data.getRealDmx());
			newRec->setJiedian(data.getJiedian());
			newRec->setGuanDi(data.getGuanDi());
			newRec->setWaShen(data.getWaShen());
			newRec->setPoDu(data.getPoDu());
			newRec->setJuli(data.getJuli());
			newRec->setcurData(data.getcurData());
			////////////////////
			newRec->setHasBulge(data.getHasBulge());
			newRec->setDesingDmxS(data.getDesignDmxS());
			newRec->setRealDmxS(data.getRealDmxS());
			newRec->setJiedianS(data.getJiedianS());

			es = testDict->setAt(strLable, newRec, StyleId);
			if (es == Acad::eOk) 
			{
				newRec->close();
			}
			else 
			{
				delete newRec;
				StyleId = AcDbObjectId::kNull;
			}
		}
	}
	testDict->close();
}

void CBcUtils::delAll()
{
	AcDbDictionary* testDict = MyBaseUtils::openDictionaryForWrite(
		CBiaochiForRQY::dictName(), true,
		acdbHostApplicationServices()->workingDatabase());
	Acad::ErrorStatus es;

	AcDbDictionaryIterator* pIter = testDict->newIterator();
	for (; !pIter->done(); pIter->next())
	{
		AcDbObjectId entId = pIter->objectId();
		testDict->remove(entId);
	}
	delete pIter;

	testDict->close();
}
