#pragma once
#include <map>
using namespace std;
#ifdef GWDESIGN_MODULE //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define WRQ_GWDESING_DLL __declspec(dllexport)
#else
#define WRQ_GWDESING_DLL __declspec(dllimport)
#endif
class WRQ_GWDESING_DLL CSerialNoUtils
{
public:
	CSerialNoUtils(void);
	~CSerialNoUtils(void);
	
	map<int, vector<AcDbObjectId> > getAllData();

	bool delAllNo();

	bool del(AcDbObjectId objId);
	bool add(CString strNo);
	

	bool removeId(AcDbObjectId noId, AcDbObjectId removeId, AcDbObjectId addId = AcDbObjectId::kNull);

	bool addId(AcDbObjectId noId, AcDbObjectId addId);
	AcDbObjectId getIdByNo(CString strNo);
	vector<AcDbObjectId> getdataById(CString strNo);

private:
	map<int, AcDbObjectId> getAllNo();

	AcGePoint3d basePt(AcDbObjectId objId);

	CString getPipeStr(AcDbObjectId plineId, CString strNo, AcDbObjectId addId = AcDbObjectId::kNull);

	bool EraseNo(CString strNo);
	bool EditPipe(CString strNo);
private:
	//��ţ�pline��
	map<int, vector<AcDbObjectId> > m_dataInfo;
};
