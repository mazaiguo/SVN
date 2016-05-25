#include "StdAfx.h"
#include "ZdmDataInfo.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CZdmDataInfo::CZdmDataInfo(void)
{
	m_bHasBulge = false;
	m_strCount = _T("1");//数量
	m_label =  BC_DICT + m_strCount;
	//////////////////////////////////////////////////////////////////////////
	m_dDesignDmx = 0.0;//设计地面线
	m_dRealDmx = 0.0;//实际地面线
	m_strJiedian = _T("");//节点
	m_dcurData = 0.0;//当前桩号数据
	m_dGuanDi = 0.0;//管底
	m_dWaShen = 0.0;//挖深
	m_dPoDu = 0.0;//坡度
	m_dJuli = 0.0;//距离
	//////////////////////////////////////////////////////////////////////////
	//在同一处凸起
	//m_bHasBulge;//是否有凸起
	m_dDesignDmxS = 0.0;//第二个设计地面线
	m_dRealDmxS = 0.0;//第二个实际地面线
	m_dJiedianS = 350;
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

LPCTSTR CZdmDataInfo::getPipeType() const
{
	return m_strPipeType;
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

double CZdmDataInfo::getDesignDmxS() const
{
	return m_dDesignDmxS;
}

double CZdmDataInfo::getRealDmxS() const
{
	return m_dRealDmxS;
}

double CZdmDataInfo::getPipeDiameter() const
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

Acad::ErrorStatus CZdmDataInfo::setPipeType(LPCTSTR strText)
{
	m_strPipeType = strText;
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

Acad::ErrorStatus CZdmDataInfo::setPipeDiameter( double strText )
{
	m_dJiedianS = strText;
	return Acad::eOk;
}

CZdmDataInfo& CZdmDataInfo::operator=(const CZdmDataInfo& zdm)
{
	m_label = zdm.label();
	m_strCount = zdm.getCount();//数量
	//////////////////////////////////
	m_dDesignDmx = zdm.getDesignDmx();//设计地面线
	m_dRealDmx = zdm.getRealDmx();//实际地面线
	m_strJiedian = zdm.getJiedian();//节点
	m_strPipeType = zdm.getPipeType();
	m_dcurData = zdm.getcurData();//当前桩号数据
	m_dGuanDi = zdm.getGuanDi();//管底
	m_dWaShen = zdm.getWaShen();//挖深
	m_dPoDu = zdm.getPoDu();//坡度
	m_dJuli = zdm.getJuli();//距离
	//////////////////////////////////
	//一处凸起
	m_bHasBulge = zdm.getHasBulge();//是否有凸起
	m_dDesignDmxS = zdm.getDesignDmxS();//第二个设计地面线
	m_dRealDmxS = zdm.getRealDmxS();//第二个实际地面线
	m_dJiedianS = zdm.getPipeDiameter();
	return *this;
}
