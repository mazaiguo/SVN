#pragma once

class CDoEduDWG
{
public:
	CDoEduDWG(void);
	~CDoEduDWG(void);
	bool RemoveEduPlotStamp_Force(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_13(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_10(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_09(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_08(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_07(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_06(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_05(const AcDbDatabase* pDb);
	bool RemoveEduPlotStamp_04(const AcDbDatabase* pDb);
	void RemoveEduPlotStamp(const AcDbDatabase* pDb);
	bool RemoveZWEduPlotStamp(const AcDbDatabase* pDb);

	bool Split(CString& src, CString token, CStringArray& resultArray,bool ingoreEmptyString=false);
	BOOL IsValidLong(const CString value,long &lValue);
	int _getHostAppMinorVersion();
	int getHostAppMinorVersion();



};
