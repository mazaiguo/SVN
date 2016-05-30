#ifndef CBASEDATAFORZDDWG_H
#define CBASEDATAFORZDDWG_H
//#include "MyBase.h"

#ifdef WRQ_ENTITY_MODULE //在创建产生DLL的工程中先把 MYBASEDLL_EXPORTS 预定义上
#define WRQ_ENTITY_DLL __declspec(dllexport)
#else
#define WRQ_ENTITY_DLL __declspec(dllimport)
#endif

class WRQ_ENTITY_DLL CBaseDataForZdDwg : public AcDbObject 
{

public:
                CBaseDataForZdDwg();
    virtual     ~CBaseDataForZdDwg();

    ACRX_DECLARE_MEMBERS(CBaseDataForZdDwg);

        // access to data members

    LPCTSTR             label() const;
    Acad::ErrorStatus   setLabel(LPCTSTR newLabel);

	CString					NumCount();
	Acad::ErrorStatus	setNumCount(CString nCount);

	CString					CurNum();
	Acad::ErrorStatus	setCurNum(CString nCount);

	CString					JdNum();
	Acad::ErrorStatus	setJdNum(CString nCount);

	CString					JcNum();
	Acad::ErrorStatus	setJcNum(CString nCount);

	CString				startZH() const;
	Acad::ErrorStatus	setStartZH(CString dHx); 

	CString				pipeType() const;
	Acad::ErrorStatus	setPipeType(CString dHx); 

	double				XScale() const;
	Acad::ErrorStatus	setXScale(double dHx);

	double				YScale() const;
	Acad::ErrorStatus	setYScale(double dHx);

	double				minElavation() const;
	Acad::ErrorStatus	setMinElavation(double dElavation);

	double				maxElavation() const;
	Acad::ErrorStatus	setMaxElavation(double dElavation);

	bool				createBc() const;
	Acad::ErrorStatus	setcreateBc(bool bDrawBc);

	bool				createJiedian() const;
	Acad::ErrorStatus	setcreateJieidan(bool bDrawBc);

	bool				createGw() const;
	Acad::ErrorStatus	setcreateGw(bool bDrawBc);

	AcGePoint3d			basePt() const;
	Acad::ErrorStatus	setbasePt(AcGePoint3d startPt);


	// overridden from AcDbObject
    virtual Acad::ErrorStatus   dwgInFields(AcDbDwgFiler* filer);
    virtual Acad::ErrorStatus   dwgOutFields(AcDbDwgFiler* filer) const;
    virtual Acad::ErrorStatus   dxfInFields(AcDbDxfFiler* filer);
    virtual Acad::ErrorStatus   dxfOutFields(AcDbDxfFiler* filer) const;

private:
    // data members
    CString             m_label;
	CString				m_strNumCount;//图纸中数字的总数
	CString				m_strCurNum;//当前图纸的数据
	CString             m_strJdNum;//当前图纸的节点数
	CString				m_strJcNum;//当前交管的数量

	CString				m_strStartZH;//起始桩号
	CString				m_strPipeType;
	double				m_dXScale;//x比例
	double				m_dYScale;//y比例

	double				m_dminElavation;//起始标高
	double				m_dmaxElavation;//终止标高

	bool				m_bDrawBc;//绘制标尺
	bool				m_bDrawJiedian;//绘制节点
	bool				m_bDrawGW;//绘制管网
	AcGePoint3d			m_basePt;//基点

    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;

	//AcGePoint3d			m_endPt;
	//double				m_dHengxiang;
	//double				m_dZongxiang;


        // Dxf Codes
    enum 
	{
        kDxfLabel       	= 1,
		kDxfNumCount		= 300,//90~99 32位整数值
		kDxfCurNum			= 301,
		kDxfJdNum			= 302,
		kDxfJcNum			= 303,
		kDxfXScale			= 40,//4~59
		kDxfYScale			= 41,
		kDxfminElavation 	= 42,
		kDxfmaxElavation 	= 43,
		kDxfStartZH			= 303,
		kDxfPipeType		= 304,
		kDxfDrawBc			= 290,//290-299 布尔标志值
		kDxfDrawJiedian		= 291,
		kDxfDrawGw			= 292,
		
		kDxfBasePt			= 10,//基点
		/*kDxfHengxiang		= 140,
		kDxfZongxiang		= 141,
		kDxfStartPt			= 10,
		kDxfEndPt			= 11,*/
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
