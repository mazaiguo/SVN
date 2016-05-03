#pragma once

#include "ZDMUtils.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//临时的数据中转
#ifdef WRQ_ZDM_MODULE //在创建产生DLL的工程中先把 
#define ZDM_DLL_API __declspec(dllexport)
#else
#define ZDM_DLL_API __declspec(dllimport)
#endif

class ZDM_DLL_API CZdmDataInfo
{
public:
	CZdmDataInfo(void);
	~CZdmDataInfo(void);

	//设置数据
	LPCTSTR             label() const;
	Acad::ErrorStatus   setLabel(LPCTSTR newLabel);

	CString	getCount() const;
	double getDesignDmx() const;
	double getRealDmx() const;
	LPCTSTR getJiedian() const;
	double getGuanDi() const;
	double getWaShen() const;
	double getPoDu() const;
	double getJuli() const;
	double getcurData() const;
	//////////////////////////////////////////////////////////////////////////
	bool   getHasBulge() const;
	double getDesignDmxS() const;
	double getRealDmxS() const;
	double getPipeDiameter() const;
	//double getcurDataS() const;

	Acad::ErrorStatus setCount(LPCTSTR nCount);
	Acad::ErrorStatus setDesignDmx(double strText);
	Acad::ErrorStatus setRealDmx(double strText);
	Acad::ErrorStatus setJiedian(LPCTSTR strText);
	Acad::ErrorStatus setGuanDi(double strText);
	Acad::ErrorStatus setWaShen(double strText);
	Acad::ErrorStatus setPoDu(double strPoDu);
	Acad::ErrorStatus setJuli(double strJuli);
	Acad::ErrorStatus setcurData(double dcurData);
	//////////////////////////////////////////////////////////////////////////
	Acad::ErrorStatus setHasBulge(bool bHasBulge);
	Acad::ErrorStatus setDesingDmxS(double strText);
	Acad::ErrorStatus setRealDmxS(double strText);
	Acad::ErrorStatus setPipeDiameter(double strText);

	CZdmDataInfo& operator=(const CZdmDataInfo& zdm);
private:
	CString             m_label;
	CString		m_strCount;//数量
	//////////////////////////////////////////////////////////////////////////
	double m_dDesignDmx;//设计地面线
	double m_dRealDmx;//实际地面线
	CString m_strJiedian;//节点
	double m_dcurData;//当前桩号数据
	double m_dGuanDi;//管底
	double m_dWaShen;//挖深
	double m_dPoDu;//坡度
	double m_dJuli;//距离
	//////////////////////////////////////////////////////////////////////////
	//在同一处凸起
	bool   m_bHasBulge;//是否有凸起
	double m_dDesignDmxS;//第二个设计地面线
	double m_dRealDmxS;//第二个实际地面线
	double m_dJiedianS;
};


