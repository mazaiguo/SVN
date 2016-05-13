#include "stdafx.h"
#include "MySplite.H"

CStringSplite::CStringSplite() {
  m_pSpt = _T(" \t,;");
}

CStringSplite::CStringSplite(TCHAR *src) {
	m_pSpt = _T(" \t,;");
	Splite(src);
}

void CStringSplite::Splite(TCHAR *src) {
	this -> RemoveAll();

	int i, j, nLen;
	nLen = _tcslen(src);
	TCHAR sItem[512];//[64];
	j = 0;
	for (i = 0; i < nLen; i++) {
		if ( m_pSpt.Find(src[i]) == -1) sItem[j++] = src[i];
		else if (sItem[0] != '\0'){
			sItem[j] = '\0';
			j = 0;
			this->AddTail(sItem);
			sItem[0] = '\0';
		}
	}
	if (sItem[0] != '\0'){
		sItem[j] = '\0';
		this->AddTail(sItem);
	}
}

void CStringSplite::Separator(TCHAR *scr) {
	m_pSpt = scr;
}

LPCTSTR CStringSplite::operator[](int nIndex) const {
	return (LPCTSTR) this->GetAt(this->FindIndex( nIndex ));
}