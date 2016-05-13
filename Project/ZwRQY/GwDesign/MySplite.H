#ifndef __MYJ_CStringSplite_H__
#define __MYJ_CStringSplite_H__

#include <memory.h>
#include <afx.h>

class CStringSplite : public CStringList {

public:
	CString m_pSpt;   // the actual array of data

// Construction
	CStringSplite();
	CStringSplite(TCHAR *src);

// Operations
	void Splite(TCHAR *src);
	void Separator(TCHAR *scr);
	LPCTSTR operator[](int nIndex) const;

};
#endif	//__MYJ_CStringSplite_H__
