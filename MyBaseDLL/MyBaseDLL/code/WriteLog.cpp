#include "StdAfx.h"
#include "WriteLog.h"

CWriteLog::CWriteLog(void)
{

}

CWriteLog::~CWriteLog(void)
{

}

CWriteLog::CWriteLog(LPCTSTR strFile)
{
	m_strFilePath = strFile;
}

//设置文件路径
void CWriteLog::setPath(LPCTSTR strFile)
{
	m_strFilePath = strFile;
}
//写数据
bool CWriteLog::writeLog(LPCTSTR strLog)
{
	if (m_strFilePath.IsEmpty())
	{
		AfxMessageBox(_T("还没有制定log路径"));
		return false;
	}
	CString strFile = m_strFilePath + _T("\\error.log");
	CStdioFile File;
	if (!File.Open(strFile, CFile::modeCreate | CFile::modeReadWrite| CFile::typeText)) 
	{
		return false;
	}
	CTime tCurrentTime = CTime::GetCurrentTime() ;
	CString strError;
	strError.Format(_T("%d-%d-%d %d:%d:%d -----%s\n"), tCurrentTime.GetYear(), tCurrentTime.GetMonth(),
		tCurrentTime.GetDay(), tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond(),
		strLog);
	File.SeekToEnd();
	File.WriteString(strError);
	File.Close();
	return true;
}

