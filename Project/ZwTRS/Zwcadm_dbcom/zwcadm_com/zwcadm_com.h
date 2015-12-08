// zwcadm_com.h : zwcadm_com DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "zwcadm_com_i.h"


// Czwcadm_comApp
// 有关此类实现的信息，请参阅 zwcadm_com.cpp
//

class Czwcadm_comApp : public CWinApp
{
public:
	Czwcadm_comApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
