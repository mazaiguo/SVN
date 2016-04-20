#include "StdAfx.h"
#include "ZdmDataInfo.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CZdmDataInfo::CZdmDataInfo(void)
{
	m_bHasBulge = false;
	m_strCount = _T("1");//����
	m_label =  BC_DICT + m_strCount;
	//////////////////////////////////////////////////////////////////////////
	m_dDesignDmx = 0.0;//��Ƶ�����
	m_dRealDmx = 0.0;//ʵ�ʵ�����
	m_strJiedian = _T("");//�ڵ�
	m_dcurData = 0.0;//��ǰ׮������
	m_dGuanDi = 0.0;//�ܵ�
	m_dWaShen = 0.0;//����
	m_dPoDu = 0.0;//�¶�
	m_dJuli = 0.0;//����
	//////////////////////////////////////////////////////////////////////////
	//��ͬһ��͹��
	//m_bHasBulge;//�Ƿ���͹��
	m_dDesignDmxS = 0.0;//�ڶ�����Ƶ�����
	m_dRealDmxS = 0.0;//�ڶ���ʵ�ʵ�����
	m_dJiedianS = 0.0;
}

CZdmDataInfo::~CZdmDataInfo(void)
{
}


LPCTSTR CZdmDataInfo::label() const
{
	return m_label;
}

Acad::ErrorStatus CZdmDataInfo::setLabel( LPCTSTR newLabel )
{
	m_label = newLabel;
	return Acad::eOk;
}

CString CZdmDataInfo::getCount() const
{
	return m_strCount;
}

double CZdmDataInfo::getDesignDmx() const
{
	return m_dDesignDmx;
}

double CZdmDataInfo::getRealDmx() const
{	
	return m_dRealDmx;
}

LPCTSTR CZdmDataInfo::getJiedian() const
{
	return m_strJiedian;
}

double CZdmDataInfo::getGuanDi() const
{
	return m_dGuanDi;
}

double CZdmDataInfo::getWaShen() const
{
	return m_dWaShen;
}

double CZdmDataInfo::getPoDu() const
{
	return m_dPoDu;
}

double CZdmDataInfo::getJuli() const
{
	return m_dJuli;
}

double CZdmDataInfo::getcurData() const
{
	return m_dcurData;
}

bool CZdmDataInfo::getHasBulge() const
{
	return m_bHasBulge;
}

double CZdmDataInfo::getDesingDmxS() const
{
	return m_dDesignDmxS;
}

double CZdmDataInfo::getRealDmxS() const
{
	return m_dRealDmxS;
}

double CZdmDataInfo::getJiedianS() const
{
	return m_dJiedianS;
}

Acad::ErrorStatus CZdmDataInfo::setCount( LPCTSTR nCount )
{
	m_strCount = nCount;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setDesignDmx( double strText )
{
	m_dDesignDmx = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setRealDmx( double strText )
{
	m_dRealDmx = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setJiedian( LPCTSTR strText )
{
	m_strJiedian = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setGuanDi( double strText )
{
	m_dGuanDi = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setWaShen( double strText )
{
	m_dWaShen = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setPoDu( double strPoDu )
{
	m_dPoDu = strPoDu;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setJuli( double strJuli )
{
	m_dJuli = strJuli;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setcurData( double dcurData )
{
	m_dcurData = dcurData;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setHasBulge( bool bHasBulge )
{
	m_bHasBulge = bHasBulge;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setDesingDmxS( double strText )
{
	m_dDesignDmxS = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setRealDmxS( double strText )
{
	m_dRealDmxS = strText;
	return Acad::eOk;
}

Acad::ErrorStatus CZdmDataInfo::setJiedianS( double strText )
{
	m_dJiedianS = strText;
	return Acad::eOk;
}