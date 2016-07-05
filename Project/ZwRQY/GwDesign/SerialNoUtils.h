#pragma once
#include <map>
using namespace std;

class CSerialNoUtils
{
public:
	CSerialNoUtils(void);
	~CSerialNoUtils(void);
	
	map<int, vector<AcDbObjectId> > getAllData();

	bool delAllNo();

	bool del(AcDbObjectId objId);
	bool add(CString strNo);
	

	bool removeId(AcDbObjectId noId, AcDbObjectId removeId, AcDbObjectId addId = AcDbObjectId::kNull);
	AcDbObjectId getIdByNo(CString strNo);
	vector<AcDbObjectId> getdataById(CString strNo);

private:
	map<int, AcDbObjectId> getAllNo();

	AcGePoint3d basePt(AcDbObjectId objId);

	CString getPipeStr(AcDbObjectId plineId, CString strNo, AcDbObjectId addId = AcDbObjectId::kNull);

	bool EraseNo(CString strNo);
	bool EditPipe(CString strNo);
private:
	//ÐòºÅ£¬pline¼¯
	map<int, vector<AcDbObjectId> > m_dataInfo;
};
