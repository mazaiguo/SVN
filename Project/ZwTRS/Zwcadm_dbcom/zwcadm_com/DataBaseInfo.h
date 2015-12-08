#pragma once

//#ifdef _DLL_SAMPLE
//#define DLL_SAMPLE_API __declspec(dllexport)
//#else
//#define DLL_SAMPLE_API __declspec(dllimport)
//#endif

class  /*DLL_SAMPLE_API*/ CDataBaseInfo//插入块的基本信息
{
public:
	CDataBaseInfo(void);
	CDataBaseInfo(const CDataBaseInfo& scl);
	~CDataBaseInfo(void);
	//获取信息
	CString GetFItemId() const;
	CString GetFName() const;
	CString GetShuliang() const;
	CString GetCailiao() const;
	CString GetDanzhong() const;
	CString GetZongzhong() const;
	CString GetBeizhu() const;
	CString GetDanwei() const;
	CString GetFErpCls() const;
	CString GetReChuli() const;
	CString GetBiaoMianChuli() const;
	CString GetFNumber() const;//物料代码
	//CString GetChartNum() const;//流程图编号
	//设置信息
	void SetFItemId(LPCTSTR str);
	void SetFName(LPCTSTR str);
	void SetShuliang(LPCTSTR str);
	void SetCailiao(LPCTSTR str);
	void SetDanzhong(LPCTSTR str);
	void SetZongzhong(LPCTSTR str);
	void SetBeizhu(LPCTSTR str);
	void SetDanwei(LPCTSTR str);
	void SetFErpCls(LPCTSTR str);
	void SetReChuli(LPCTSTR str);
	void SetBiaoMianChuli(LPCTSTR str);
	void SetFNumber(LPCTSTR str);
	//void SetChartNum(LPCTSTR str);

	CDataBaseInfo&   operator = (const CDataBaseInfo& scl);

private:
	CString m_strDaihao;//代号
	CString m_strMingcheng;//名称
	CString m_strShuliang;//数量
	CString m_strCailiao;//材料
	CString m_strDanzhong;//单重
	CString m_strZongzhong;//总重
	CString m_strBeiZhu;//备注
	CString m_strDanwei;//单位
	CString m_strLingjianLeixing;//零件类型
	CString m_strReChuli;//热处理
	CString m_strBiaoMianChuli;//表面处理
	CString m_strFNumber;//物料代码
	//CString m_strChartNum;//流程图编号
};