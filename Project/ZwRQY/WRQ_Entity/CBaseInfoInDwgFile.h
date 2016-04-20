#ifndef CBASEINFOINDWGFILE_H
#define CBASEINFOINDWGFILE_H
#include "MyBase.h"
#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
class SERVERDLL_API CBaseInfoInDwgFile : public AcDbObject 
{

public:
                CBaseInfoInDwgFile();
    virtual     ~CBaseInfoInDwgFile();

    ACRX_DECLARE_MEMBERS(CBaseInfoInDwgFile);

        // access to data members

    LPCTSTR             label() const;
    Acad::ErrorStatus   setLabel(LPCTSTR newLabel);

	CString					NumCount();
	Acad::ErrorStatus	setNumCount(CString nCount);

	CString					CurNum();
	Acad::ErrorStatus	setCurNum(CString nCount);
	/*AcGePoint3d			startPt() const;
	Acad::ErrorStatus	setStartPt(AcGePoint3d startPt);


	AcGePoint3d			endPt() const;
	Acad::ErrorStatus	setEndPt(AcGePoint3d endPt);*/

	/*double				hengxiang() const;
	Acad::ErrorStatus	setHengxiang(double dHx);

	double				zongxiang() const;
	Acad::ErrorStatus	setZongxiang(double dZx);*/
        // overridden from AcDbObject
    virtual Acad::ErrorStatus   dwgInFields(AcDbDwgFiler* filer);
    virtual Acad::ErrorStatus   dwgOutFields(AcDbDwgFiler* filer) const;
    virtual Acad::ErrorStatus   dxfInFields(AcDbDxfFiler* filer);
    virtual Acad::ErrorStatus   dxfOutFields(AcDbDxfFiler* filer) const;

private:
    // data members
    CString             m_label;
	CString				m_strNumCount;//ͼֽ�����ֵ�����
	CString				m_strCurNum;//��ǰͼֽ������
    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;

	//AcGePoint3d			m_startPt;
	//AcGePoint3d			m_endPt;
	//double				m_dHengxiang;
	//double				m_dZongxiang;


        // Dxf Codes
    enum 
	{
        kDxfLabel       = 1,
		kDxfNumCount	= 300,//90~99 32λ����ֵ
		kDxfCurNum		= 301,
		/*kDxfHengxiang	= 140,
		kDxfZongxiang	= 141,
		kDxfStartPt		= 10,
		kDxfEndPt		= 11,*/
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
