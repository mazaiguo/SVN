
//-----------------------------------------------------------------------------
//----- DlgFindBlock.h : Declaration of the CDlgFindBlock
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include <vector>
#include <map>
#include "DoEntColor.h"
#include "afxwin.h"
using namespace std;
//-----------------------------------------------------------------------------
class CDlgFindBlock : public CAcUiDialog {
	DECLARE_DYNAMIC (CDlgFindBlock)

public:
	CDlgFindBlock (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CDlgFindBlock();

	enum { IDD = IDD_DLGFINDBLOCK} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCancel();
	BOOL Create(CWnd* pParent);

	afx_msg void OnBnClickedButtonFind();
	bool GetData();

	//»æÖÆÍâ°üÎ§¿ò
	void DrawExts(AcDbExtents exts);
private:
	std::map<CString, std::vector<AcDbExtents> > m_AllData;
	//CString m_strText;
	int m_nDeFault;
	double m_dScale;
	CDoEntColor m_DoEnt;
public:
	CComboBox m_CombBlockName;
	CString m_strBlockName;
	CString m_strPreName;
	afx_msg void OnCbnSelchangeComboBlock();
} ;
