#pragma once
#include "DataBaseInfo.h"
#ifdef _DLL_SAMPLE
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif

class DLL_SAMPLE_API CExternalCall
{
public:
	CExternalCall(void);
	~CExternalCall(void);

	CDataBaseInfo GetData();
};

