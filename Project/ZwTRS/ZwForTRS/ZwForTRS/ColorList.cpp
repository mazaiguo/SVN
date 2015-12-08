// EditList.cpp : implementation file
//

#include "stdafx.h"
#include "ColorList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditList

CColorList::CColorList()
{
	m_fGetType = NULL;
	/*if (m_ComboxColor != NULL)
	{
		m_ComboxColor->ResetContent();
	}*/
}

CColorList::~CColorList()
{
	/*if (m_ComboxColor != NULL)
	{
		m_ComboxColor->ResetContent();
	}*/
}


BEGIN_MESSAGE_MAP(CColorList, CListCtrl)
	//{{AFX_MSG_MAP(CEditList)
	//ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	//ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	//ON_NOTIFY_REFLECT(NM_DBLCLK, &CColorList::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditList message handlers

//CEdit *CColorList::EditItem(int nItem, int nSubItem)
//{
//	CRect rect;
//	int offset = 0;
//	if(!EnsureVisible(nItem, TRUE))
//	{ 
//		return NULL;	
//	}
//
//	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
//	// Now scroll if we need to expose the column
//	CRect rcClient;
//	GetClientRect(rcClient);
//	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
//	{
//		CSize size(offset + rect.left,0);		
//		Scroll(size);
//		rect.left -= size.cx;
//	}
//	rect.left += offset;	
//	rect.right = rect.left + GetColumnWidth(nSubItem);
//	if(rect.right > rcClient.right) 
//	   rect.right = rcClient.right;
//
//	// Get Column alignment	
//	LV_COLUMN lvcol;
//	lvcol.mask = LVCF_FMT;
//	GetColumn(nSubItem, &lvcol);
//
//	DWORD dwStyle;
//	if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
//		dwStyle = ES_LEFT;
//	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
//		dwStyle = ES_RIGHT;
//	else 
//		dwStyle = ES_CENTER;	
//
//	
//	dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
//	CEdit *pEdit = new CEditItem(nItem, nSubItem, GetItemText(nItem, nSubItem));
//
//#define IDC_EDITCTRL 0x1234
//	pEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);	
//	//	pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
//	//funny thing happend here, uncomment this, 
//	//and then edit an item, 
//	//enter a long text so that the ES_AUTOHSCROLL comes to rescue
//	//yes that's look funny, ???.
//	return pEdit;
//}

//void CColorList::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
// 	LV_ITEM *plvItem = &plvDispInfo->item;
//
//	if( plvItem->iItem != -1 &&  // valid item
//		plvItem->pszText )		// valid text
//	{
//		SetItemText( plvItem->iItem, plvItem->iSubItem, plvItem->pszText);				
//		
//		// this will invoke an ItemChanged handler in parent
//		if ( plvItem->iSubItem != 0 )
//		{
//			TCHAR szText[0x10+1];
//			GetItemText( plvItem->iItem, 0, szText, 0x10);				
//			SetItemText( plvItem->iItem, 0, szText);				
//		}	
//	}
//
//	*pResult = 0;
//}

//LPCTSTR lineWeightTypeToStr(AcDb::LineWeight type, CString& str)
//{
//	if (type == AcDb::kLnWt000)
//		str = _T("0.00 mm");
//	else if (type == AcDb::kLnWt005)
//		str = _T("0.05 mm");
//	else if (type == AcDb::kLnWt009)
//		str = _T("0.09 mm");
//	else if (type == AcDb::kLnWt013)
//		str = _T("0.13 mm");
//	else if (type == AcDb::kLnWt015)
//		str = _T("0.15 mm");
//	else if (type == AcDb::kLnWt018)
//		str = _T("0.18 mm");
//	else if (type == AcDb::kLnWt020)
//		str = _T("0.20 mm");
//	else if (type == AcDb::kLnWt025)
//		str = _T("0.25 mm");
//	else if (type == AcDb::kLnWt030)
//		str = _T("0.30 mm");
//	else if (type == AcDb::kLnWt035)
//		str = _T("0.35 mm");
//	else if (type == AcDb::kLnWt040)
//		str = _T("0.40 mm");
//	else if (type == AcDb::kLnWt050)
//		str = _T("0.50 mm");
//	else if (type == AcDb::kLnWt053)
//		str = _T("0.53 mm");
//	else if (type == AcDb::kLnWt060)
//		str = _T("0.60 mm");
//	else if (type == AcDb::kLnWt070)
//		str = _T("0.70 mm");
//	else if (type == AcDb::kLnWt080)
//		str = _T("0.80 mm");
//	else if (type == AcDb::kLnWt090)
//		str = _T("0.90 mm");
//	else if (type == AcDb::kLnWt100)
//		str = _T("1.00 mm");
//	else if (type == AcDb::kLnWt106)
//		str = _T("1.06 mm");
//	else if (type == AcDb::kLnWt120)
//		str = _T("1.20 mm");
//	else if (type == AcDb::kLnWt140)
//		str = _T("1.40 mm");
//	else if (type == AcDb::kLnWt158)
//		str = _T("1.58 mm");
//	else if (type == AcDb::kLnWt200)
//		str = _T("2.00 mm");
//	else if (type == AcDb::kLnWt211)
//		str = _T("2.11 mm");
//	else if (type == AcDb::kLnWtByLayer)
//		str = _T("ByLayer");
//	else if (type == AcDb::kLnWtByBlock)
//		str = _T("ByBlock");
//	else if (type == AcDb::kLnWtByLwDefault)
//		str = _T("ByLwDefault");
//	else {
//		ASSERT(0);
//		str = _T("*Unknown*");
//	}
//
//	return str;
//}
//
//
//BOOL CColorList::PreCreateWindow(CREATESTRUCT& cs) 
//{
//	// TODO: Add your specialized code here and/or call the base class
//	cs.style |= WS_CHILD |
//				LVS_REPORT |
//				LVS_SINGLESEL |
//				LVS_SHOWSELALWAYS;	
//	return CListCtrl::PreCreateWindow(cs);
//}


//
//void CColorList::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
//
//	switch(pLVKeyDow->wVKey)
//	{
//		case VK_DELETE: 
//			{
//				int nItem = GetSelectionMark();
//				if(nItem!=-1) // valid item 					
//				{
//					DeleteItem( nItem );
//				}
//			}	break;
//
//		default :break;
//	}
//	*pResult = 0;
//
//}

//int CColorList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
//{
//	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
//		return -1;
//	
//	// TODO: Add your specialized creation code here	
//	SetExtendedStyle( GetExtendedStyle() | LVS_EX_CHECKBOXES );
//	
//	return 0;
//}

//CComboBox * CColorList::ComboItem(int nItem, int nSubItem)
//{
//#define IDC_COMBOBOXINLISTVIEW 0x1235
//
//	CString strFind = GetItemText(nItem, nSubItem);
//
//	//basic code start
//	CRect rect;
//	int offset = 0;
//	// Make sure that the item is visible
//	if( !EnsureVisible(nItem, TRUE)) 
//		return NULL;
//
//	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
//	// Now scroll if we need to expose the column
//	CRect rcClient;
//	GetClientRect(rcClient);
//	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
//	{
//		CSize size;
//		size.cx = offset + rect.left;
//		size.cy = 0;
//		Scroll(size);
//		rect.left -= size.cx;
//	}
//
//	rect.left += offset;	
//	rect.right = rect.left + GetColumnWidth(nSubItem);
//	if(rect.right > rcClient.right) 
//		rect.right = rcClient.right;
//	//basic code end
//
//	rect.bottom += 30 * rect.Height();//dropdown area
//
//	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL|CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
//	CComboBox *pList = new CComboItem(nItem, nSubItem, &m_strList);
//	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
//	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
//
//	pList->ShowDropDown();
//	pList->SelectString(-1, strFind.GetBuffer(1));
//	// The returned pointer should not be saved
//	return pList;
//}

void CColorList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	switch(lplvdr->nmcd.dwDrawStage)//判断状态
	{
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}
	case CDDS_ITEMPREPAINT://如果为画ITEM之前就要进行颜色的改变
		{
			COLORREF ItemColor;
			if(MapItemColor.Lookup((unsigned long&)nmcd.dwItemSpec, ItemColor))
				// 根据在 SetItemColor(DWORD iItem, COLORREF color) 设置的
				// ITEM号和COLORREF 在摸板中查找，然后进行颜色赋值。
			{
				//lplvdr->clrText = RGB(0,0,0);//ItemColor;
				lplvdr->clrTextBk = ItemColor;
				*pResult = CDRF_DODEFAULT;
			}
		}
		break;
	}
}

void CColorList::SetItemColor(DWORD iItem, COLORREF color)
{
	MapItemColor.SetAt(iItem, color);//设置某行的颜色。
	this->RedrawItems(iItem, iItem);//重新染色
	this->SetFocus();    //设置焦点
	UpdateWindow();
}

void CColorList::SetAllItemColor(DWORD iItem,COLORREF TextBkColor)
{
	if(iItem > 0)
	{
		for(DWORD numItem = 0; numItem < iItem ;numItem ++)
		{
			MapItemColor.SetAt(numItem, TextBkColor);
			this->RedrawItems(numItem, numItem);
		}
	}
	return;

}


//
//void CColorList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	//// TODO: 在此添加控件通知处理程序代码
//	if( GetFocus() != this) 
//		SetFocus();
//
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//	if (m_fGetType && m_fGetType( pNMListView->iSubItem ) == eEdit)
//	{
//		//EditItem (pNMListView->iItem, pNMListView->iSubItem);
//	}
//	*pResult = 0;
//}
