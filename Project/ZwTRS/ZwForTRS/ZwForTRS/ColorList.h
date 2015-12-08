#if !defined(AFX_COLORLIST_H)
#define AFX_COLORLIST_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditList.h : header file
//
typedef int (* fGetType) (int);

// CEditList window
class CColorList : public CListCtrl
{
	// Construction
public:
	CColorList();

// Attributes
public:
	enum eType{
		eEdit,
		eCombo,
		eComboColor,
		eComboLineType,
		eComboLineWeight,
		eLast,
		eColor
	};

	fGetType m_fGetType;

	CStringList m_strList;
// Operations
public:
	protected:
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColumnType(fGetType func) { m_fGetType = func;}
	virtual ~CColorList();	
	afx_msg void SetItemColor(DWORD iItem, COLORREF color);
	afx_msg void SetAllItemColor(DWORD iItem,COLORREF TextBkColor);	

	// Generated message map functions
protected:

	//CComboBox * ComboItem( int nItem,  int nSubItem);
	//CEdit * EditItem( int nItem, int nSubItem);
	//{{AFX_MSG(CEditList)
	//afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

	CMap<DWORD, DWORD&, COLORREF, COLORREF&> MapItemColor;

	DECLARE_MESSAGE_MAP()

public:
	//afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};
#endif 
