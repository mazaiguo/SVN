#pragma once

#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif
//дlog��־
class SERVERDLL_API CWriteLog
{
public:
	CWriteLog(void);
	CWriteLog(LPCTSTR strFile);

	virtual ~CWriteLog(void);

	//�����ļ�·��
	void setPath(LPCTSTR strFile);
	//д����
	bool writeLog(LPCTSTR strLog);
	//
private:
	CString m_strFilePath;
};
