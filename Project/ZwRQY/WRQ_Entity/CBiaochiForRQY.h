#ifndef CBIAOCHIFORRQY_H
#define CBIAOCHIFORRQY_H

#ifdef WRQ_ENTITY_MODULE //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
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
	CString		m_strCount;//����
	//////////////////////////////////////////////////////////////////////////
	double m_dDesignDmx;//��Ƶ�����
	double m_dRealDmx;//ʵ�ʵ�����
	CString m_strJiedian;//�ڵ�
	CString m_strPipeType;//�ܵ�����
	double m_dcurData;//��ǰ׮������
	double m_dGuanDi;//�ܵ�
	double m_dWaShen;//����
	double m_dPoDu;//�¶�
	double m_dJuli;//����
	//////////////////////////////////////////////////////////////////////////
	//��ͬһ��͹��
	bool   m_bHasBulge;//�Ƿ���͹��
	double m_dDesignDmxS;//�ڶ�����Ƶ�����
	double m_dRealDmxS;//�ڶ���ʵ�ʵ�����
	double m_dJiedianS;

    static Adesk::Int16 m_version;
	static LPCTSTR		m_dictName;

	//AcDbHardPointerId   m_GroupId;//���߶���¼��dictionary��
        // Dxf Codes
    enum 
	{
		kDxfLabel       	= 1,
		kDxfNumCount		= 300,//90~99 32λ����ֵ
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
		kDxfHasBulge		= 290,//290-299 ������־ֵ
		kDxfGroupId			= 340,//kDxf//320-329		���������������ԭ������ȡ�ľ��ֵ�������� INSERT �� XREF �����ڼ䲻����ת��
    };

public:
	static LPCTSTR		dictName()	{ return m_dictName; }
	static AcDbObjectId	getStandardStyle(AcDbDatabase* db, bool makeIfNotThere);

};


#endif    // ARXDBGDBADESKLOGOSTYLE_H
