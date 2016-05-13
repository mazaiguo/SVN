#pragma once
#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

#include <vector>
using namespace std;
class SERVERDLL_API MyParserString 
{
public:
	MyParserString(void);
	~MyParserString(void);

	
	//����strPile ����strPileǰ����ַ����ַ����int�ʹ�С
	static int GetPileLength(CString strPile, CString& strPrefix);

	static CString GetNameAndStandard(CString strPile, CString& strPrefix);
	//����strPile,����strPileǰ����ַ�strPrefix��������ַ�strNext�������ַ��м������int�ʹ�С
	static int GetPileLengthEx(CString strPile, CString& strPrefix, CString& strNext);
	//����λ�������ַ���
	//nflag=0��ʾ����
	//1��ʾ�ݼ�
	//2��ʾ����
	static int GetPrePileLengthEx(CString strPile, CString& strPrefix, CString& strNext);

	static CString OutPutStringByNumericalDigit(CString strPile, int nWeishu, int nFlag = 0);
	//���ַ�תΪlineweight
	static AcDb::LineWeight StrToLineType(CString& str);

	//�������������ַ�����������浽һ��cstringarray��
	static void GetCommaFromString(CString strInput, CStringArray& strResult);

	//��strPath���ҳ������ļ������浽stringarray��
	static void GetFileNameFromThePath(CString& strPath, CStringArray& strFileNameArr, const CString strTypeName, bool bIsFullPath = false);
	//��������������ַ���������
	static void GetParenthesesFromString(CString strInput, CStringArray& strResult);

	static CString GetParenthesesFromString(CString strInput);

	//������+���ַ�����������浽һ��cstringarray��
	static void GetPlusFromString(CString strInput, CStringArray& strResult);
	//�ж��ַ��Ƿ�Ϊ������
	static bool JudgeStr(CString str);

};
