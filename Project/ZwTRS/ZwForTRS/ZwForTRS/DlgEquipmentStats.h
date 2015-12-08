

//-----------------------------------------------------------------------------
//----- DlgEquipmentStats.h : Declaration of the CDlgEquipmentStats
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "BlkInfo.h"
#include "IniFile.h"
#include "CEBExcelOperate.h"
#include <map>
#include <vector>
#include <utility>
#include "DataSort.h"
#include "afxwin.h"
using namespace std;
//-----------------------------------------------------------------------------
class CDlgEquipmentStats : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgEquipmentStats)

public:
	CDlgEquipmentStats (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgEquipmentStats();

	enum { IDD = IDD_DLGEQUIPMENTSTATS} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	bool SelectEnt();
	void GetEntInfoById(AcDbObjectId objId);

	//输出文件
	bool OutPutFile();
	//将数据写入XLS中
	bool WriteData();

	
private:
	//第一个参数记录流程图编号，第二个参数记录块里的信息及数量
	//map<CString, pair<CDataBaseInfo*, int> > m_Data;
	vector<pair<CString, CDataBaseInfo*> > m_Data;
	int m_nStart;//excel表中起始位置
	int m_nCombine;//excel合并行数
	CString m_strFileName;//文件名
	CDataSort m_dataSort;//
	//Excel的数据
	vector<pair<CString, DataVec> > m_ExcelData;
	//数据分类
	map<CString, vector<CString> > m_strData;

	//vector<pair<CString, CDataBaseInfo*> > m_DataTmp;
public:
	// 模板
	CComboBox m_CombTemplate;
	CString m_strTemplate;
	afx_msg void OnCbnSelchangeComboModel();
} ;
