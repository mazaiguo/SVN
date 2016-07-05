#pragma once
#include <vector>
using namespace std;

class CPipeUtils
{
public:
	CPipeUtils(void);
	~CPipeUtils(void);
	vector<AcDbObjectId> getAllPipe();

	bool ShowGuanduanText();

private:
	bool doGuandaoXdata(bool bIsShow = false);
	
private:
	vector<AcDbObjectId> m_PipeInfo;

};
