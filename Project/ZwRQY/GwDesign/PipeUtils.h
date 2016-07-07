#pragma once
#include <vector>
using namespace std;
#ifdef GWDESIGN_MODULE //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define WRQ_GWDESING_DLL __declspec(dllexport)
#else
#define WRQ_GWDESING_DLL __declspec(dllimport)
#endif
class WRQ_GWDESING_DLL CPipeUtils
{
public:
	CPipeUtils(void);
	~CPipeUtils(void);
	vector<AcDbObjectId> getAllPipe();

	bool ShowGuanduanText();

	bool del(AcDbObjectId plineId);

private:
	bool doGuandaoXdata(bool bIsShow = false);
	
private:
	vector<AcDbObjectId> m_PipeInfo;

};
