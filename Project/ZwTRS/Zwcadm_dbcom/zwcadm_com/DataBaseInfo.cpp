#include "StdAfx.h"
#include "DataBaseInfo.h"

CDataBaseInfo::CDataBaseInfo(void)
{	
	m_strShuliang = _T("1");
	m_strDanzhong = _T("");
}

CDataBaseInfo::CDataBaseInfo(const CDataBaseInfo& scl)
{
	*this = scl;
}


CDataBaseInfo::~CDataBaseInfo(void)
{
}

//代号
CString CDataBaseInfo::GetFItemId() const
{
	return m_strDaihao;
}
//名称
CString CDataBaseInfo::GetFName() const
{
	return m_strMingcheng;
}
//数量
CString CDataBaseInfo::GetShuliang() const
{
	return m_strShuliang;
}
//材料
CString CDataBaseInfo::GetCailiao() const
{
	return m_strCailiao;
}
//单重
CString CDataBaseInfo::GetDanzhong() const
{
	return m_strDanzhong;
}
//总重
CString CDataBaseInfo::GetZongzhong() const
{
	return m_strZongzhong;
}
//备注即执行标准
CString CDataBaseInfo::GetBeizhu() const
{
	return m_strBeiZhu;
}
//单位
CString CDataBaseInfo::GetDanwei() const
{
	return m_strDanwei;
}
//零件类型
CString CDataBaseInfo::GetFErpCls() const
{
	return m_strLingjianLeixing;
}
//热处理
CString CDataBaseInfo::GetReChuli() const
{
	return m_strReChuli;
}
//表面处理
CString CDataBaseInfo::GetBiaoMianChuli() const
{
	return m_strBiaoMianChuli;
}
//物料代码
CString CDataBaseInfo::GetFNumber() const
{
	return m_strFNumber;
}

//CString CDataBaseInfo::GetChartNum() const
//{
//	return m_strChartNum;
//}
//设置信息

//代号
void CDataBaseInfo::SetFItemId(LPCTSTR str)
{
	m_strDaihao = str;
}
//名称
void CDataBaseInfo::SetFName(LPCTSTR str)
{
	m_strMingcheng = str;
}
//数量
void CDataBaseInfo::SetShuliang(LPCTSTR str)
{
	m_strShuliang = str;
}
//材料
void CDataBaseInfo::SetCailiao(LPCTSTR str)
{
	m_strCailiao = str;
}
//单重
void CDataBaseInfo::SetDanzhong(LPCTSTR str)
{
	m_strDanzhong = str;
}
//总重
void CDataBaseInfo::SetZongzhong(LPCTSTR str)
{
	m_strZongzhong = str;
}
//备注即执行标准
void CDataBaseInfo::SetBeizhu(LPCTSTR str)
{
	m_strBeiZhu = str;
}
//单位
void CDataBaseInfo::SetDanwei(LPCTSTR str)
{
	m_strDanwei = str;
}
//零件类型
void CDataBaseInfo::SetFErpCls(LPCTSTR str)
{
	m_strLingjianLeixing = str;
}

//热处理
void CDataBaseInfo::SetReChuli(LPCTSTR str)
{
	m_strReChuli = str;
}
//表面处理
void CDataBaseInfo::SetBiaoMianChuli(LPCTSTR str)
{
	m_strBiaoMianChuli = str;
}
//物料代码
void CDataBaseInfo::SetFNumber(LPCTSTR str)
{
	m_strFNumber = str;
}

//void CDataBaseInfo::SetChartNum(LPCTSTR str)
//{
//	m_strChartNum = str;
//}

CDataBaseInfo&   CDataBaseInfo::operator = (const CDataBaseInfo& scl)
{
	m_strDaihao = scl.GetFItemId();//代号
	m_strMingcheng = scl.GetFName();//名称
	m_strShuliang = scl.GetShuliang();//数量
	m_strCailiao = scl.GetCailiao();//材料
	m_strDanzhong = scl.GetDanzhong();//单重
	m_strZongzhong = scl.GetZongzhong();//总重
	m_strBeiZhu = scl.GetBeizhu();//备注
	m_strDanwei = scl.GetDanwei();//单位
	m_strLingjianLeixing = scl.GetFErpCls();//零件类型
	m_strReChuli = scl.GetReChuli();//热处理
	m_strBiaoMianChuli = scl.GetBiaoMianChuli();//表面处理
	m_strFNumber = scl.GetFNumber();//物料代码
	//m_strChartNum = scl.GetChartNum();//流程图编号
	return *this;
}
