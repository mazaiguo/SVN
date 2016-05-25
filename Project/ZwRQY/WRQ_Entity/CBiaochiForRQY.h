#ifndef CBIAOCHIFORRQY_H
#define CBIAOCHIFORRQY_H

#ifdef WRQ_ENTITY_MODULE //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif
class WRQ_ENTITY_DLL CBiaochiForRQY : public AcDbObject 
{

public:
                CBiaochiForRQY();
    virtual     ~CBiaochiForRQY();

    ACRX_DECLARE_MEMBERS(CBiaochiForRQY);

        // access to data members

    LPCTSTR             label() const;
    Acad::ErrorStatus   setLabel(LPCTSTR newLabel);

	CString	 	getCount() const;
	double   	getDesignDmx() const;
	double   	getRealDmx() const;
	LPCTSTR  	getJiedian() const;
	LPCTSTR  	getPipeType() const;
	double 		getGuanDi() const;
	double 		getWaShen() const;
	double 		getPoDu() const;
	double 		getJuli() const;
	double 		getcurData() const;
	//////////////////////////////////////////////////////////////////////////
	bool   		getHasBulge() const;
	double 		getDesingDmxS() const;
	double 		getRealDmxS() const;
	double 		getPipeDiameter() const;
	//double getcurDataS() const;

	Acad::ErrorStatus setCount(LPCTSTR nCount);
	Acad::ErrorStatus setDesignDmx(double strText);
	Acad::ErrorStatus setRealDmx(double strText);
	Acad::ErrorStatus setJiedian(LPCTSTR strText);
	Acad::ErrorStatus setPipeType(LPCTSTR strText);
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


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/*AcDbObjectId        groupId() const;
	Acad::ErrorStatus   setGroupId(const AcDbObjectId& styleId);*/

        // overridden from AcDbObject
    virtual Acad::ErrorStatus   dwgInFields(AcDbDwgFiler* filer);
    virtual Acad::ErrorStatus   dwgOutFields(AcDbDwgFiler* filer) const;
    virtual Acad::ErrorStatus   dxfInFields(AcDbDxfFiler* filer);
    virtual Acad::ErrorStatus   dxfOutFields(AcDbDxfFiler* filer) const;

private:
    // data members
    CString             m_label;
	CString		m_strCount;//数量
	//////////////////////////////////////////////////////////////////////////
	double m_dDesignDmx;//设计地面线
	double m_dRealDmx;//实际地面线
	CString m_strJiedian;//节点
	CString m_strPipeType;//管道类型
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

    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;

	//AcDbHardPointerId   m_GroupId;//将线都记录在dictionary中
        // Dxf Codes
    enum 
	{
		kDxfLabel       	= 1,
		kDxfNumCount		= 300,//90~99 32位整数值
		kDxfJieDian			= 301,
		kDxfPipeType		= 302,
		kDxfDesignDmx		= 40,//4~59
		kDxfRealDmx			= 41,
		kDxfCurData 		= 42,
		kDxfGuandi 			= 43,
		kDxfWashen  		= 44,
		kDxfPodu    		= 45,
		kDxfJuli    		= 46,
		kDxfDesignDmxS		= 47,
		kDxfRealDmxS		= 48,
		kDxfJieDianS		= 49,
		kDxfHasBulge		= 290,//290-299 布尔标志值
		kDxfGroupId			= 340,//kDxf//320-329		任意对象句柄；“按原样”获取的句柄值。它们在 INSERT 和 XREF 操作期间不进行转换
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
