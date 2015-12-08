// CZwcadmCom.cpp : CCZwcadmCom 的实现

#include "stdafx.h"

#include "CZwcadmCom.h"
#include "adui.h"
#include "DataBaseInfo.h"
//#include "ExternalCall.h"

//#include "acuiDialog.h"
 //CCZwcadmCom

#if 0
#pragma comment(lib, "ZwForTRS.lib") 
#endif

STDMETHODIMP CCZwcadmCom::CallTitleMenuFunction(BSTR bstrCommandName, IPETitleCollection** ppPETitleCollection, BSTR bstrFileName, BSTR* bstrErrorMsg, long *lSucceeded)
{
	return S_OK;
}

CString GetAppPath()
{
	CString path;
	TCHAR buf[1000];

	DWORD pathLength =GetModuleFileName(GetModuleHandle(_T("zwcadm_com.dll")),buf,1000);

	if (pathLength)
	{
		path = buf;
		path = path.Left(path.ReverseFind('\\'));
		path+=_T("\\");
	}
	else
	{
		path =_T("");
	}
	return path;
}

CString SubString(CString string, CString Split, int index)
{
	int Num=string.Find(Split);
	if(Num==-1) return _T("");
	if(index==0)
		return string.Left(Num);
	int i=1;
	CString retStr=_T("");
	int nCount = 0;
#ifdef UNICODE
	nCount = 0;
#else
	nCount = 1;
#endif
	while(i<index)
	{
		Num=string.Find(Split,Num+1+nCount);
		if(Num==-1) return _T("");
		i++;
	}
	int Num2=string.Find(Split,Num+1+nCount);
	if(Num2==-1){
		retStr=string.Mid(Num+1+nCount);
	}
	else
	{
		retStr=string.Mid(Num+1+nCount,Num2-Num-1-nCount);

	}
	return retStr;
}


CString GetResultFromName(CString strName, const CDataBaseInfo Info)
{

	CString strOut;
	bool bRet = true;
	//COMPNAME 代号 COMPDES--名称
	//RELQTY--数量 COMPMAT--材料
	//COMPWT--单重 COMPTWT--总重 COMPREMARK--备注
	//COMPUMIS--单位 COMPITEM--零件类型
	///COMPTSUP--表面处理 COMPTTERM--热处理
	//
	if (strName.CompareNoCase(_T("COMPNAME")) == 0)//COMPNAME 代号
	{
		strOut = Info.GetFNumber();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPDES")) == 0)// COMPDES--名称
	{
		strOut = Info.GetFName();
		return strOut;
	}
	if (strName.CompareNoCase(_T("RELQTY")) == 0)//RELQTY--数量
	{
		strOut = Info.GetShuliang();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPMAT")) == 0)//COMPMAT--材料
	{
		strOut = Info.GetCailiao();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPWT")) == 0)//COMPWT--单重
	{
		strOut = Info.GetDanzhong();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPTWT")) == 0)//COMPTWT--总重
	{
		//strOut = Info.GetZongzhong();
		//bRet = false;
		strOut = Info.GetZongzhong();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPREMARK")) == 0)//COMPREMARK--备注
	{
		strOut = Info.GetBeizhu();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPUMIS")) == 0)//COMPUMIS--单位
	{
		strOut = Info.GetDanwei();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPITEM")) == 0)//COMPITEM--零件类型
	{
		strOut = Info.GetFErpCls();
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPTSUP")) == 0)//COMPTSUP--表面处理
	{
		//strOut = Info.GetBiaoMianChuli();
		//bRet = false;
		strOut = _T("");
		return strOut;
	}
	if (strName.CompareNoCase(_T("COMPTTERM")) == 0)//COMPTTERM--热处理
	{
		//strOut = Info.GetReChuli();
		//bRet = false;
		strOut = _T("");
		return strOut;
	}
	//else
	//{
	//	//bRet = false;
	//	strOut = _T("");
	//}
	return strOut;
}

//读取csv文件数据
CDataBaseInfo ReadCsv()
{
	CDataBaseInfo dbInfo;
	CString strFileName = GetAppPath() + _T("tmp.txt");
	CStdioFile File;
	if (!File.Open(strFileName, CFile::modeReadWrite  | CFile::typeText)) 
	{
		return dbInfo;
	}
	CString buffer;
	while(File.ReadString(buffer))
	{
		dbInfo.SetFItemId(SubString(buffer, _T("？"), 0));
		dbInfo.SetFName(SubString(buffer, _T("？"), 1));
		dbInfo.SetCailiao(SubString(buffer, _T("？"), 2));
		dbInfo.SetBeizhu(SubString(buffer, _T("？"), 3));
		dbInfo.SetDanzhong(SubString(buffer, _T("？"), 4));
		dbInfo.SetFNumber(SubString(buffer, _T("？"), 5));
		dbInfo.SetFErpCls(SubString(buffer, _T("？"), 6));
		dbInfo.SetDanwei(SubString(buffer, _T("？"), 7));
		dbInfo.SetShuliang(SubString(buffer, _T("？"), 8));
		dbInfo.SetZongzhong(SubString(buffer, _T("？"), 9));
	}
	File.Close();
	return dbInfo;
}

typedef bool (_stdcall * AddProc)(); 

STDMETHODIMP CCZwcadmCom::CallBomMenuFunction(BSTR bstrCommandName, BSTR FileName, IPETitleCollection** ppPETitleCollection, IPEBomCollections** ppPEBomCollections, BSTR* ErrorMsg, long *lSucceeded)
{

	//AfxMessageBox(_T("hello, call your name"));
	_bstr_t t(bstrCommandName, FALSE);
	CString strCommand = t;
	if (strCommand.CompareNoCase(_T("Function1")) == 0)
	{
		//内容
		CDataBaseInfo info;
		HINSTANCE hd=::LoadLibrary("ZwForTRS.zrx"); 

		AddProc Add=(AddProc)GetProcAddress(hd,"GetData");
		
		//if(!Add)
		//{
		//	//MessageBox(_T("获取Add函数地址失败！"));
		//}
		//CExternalCall eCall;
		//eCall = Add();
		//
		////CExternalCall eCall;
		//info = eCall.GetData();
		bool bRet = Add();
		if (bRet)
		{
			info = ReadCsv();
			CString strTmp;
			strTmp.Format(_T("FItemID=%s"), info.GetFItemId());
			if (strTmp.IsEmpty())
			{
				return S_OK;
			}
			else
			{
				if (ppPEBomCollections)
				{
					long lBomCollectionsCounts = -1;
					(*ppPEBomCollections)->get_Counts(&lBomCollectionsCounts);
					if (lBomCollectionsCounts > 0)
					{
						for (long i = 0; i < lBomCollectionsCounts; i++)
						{
							CString str_key, str_Value, strTemp, strTemp1;

							IPEBomCollection *pBomCollection = NULL;
							(*ppPEBomCollections)->GetItem(i + 1, &pBomCollection);

							if (pBomCollection)
							{
								long lBomCounts;
								pBomCollection->get_Counts(&lBomCounts);

								for (int j=0; j<lBomCounts; j++)
								{
									//////////////////////////////////////////////////////////////////////////
									IPEBom *pPEBom = NULL;
									pBomCollection->GetItem(j+1 , &pPEBom);//代号
									CComBSTR bstrTitleName;
									pPEBom->get_BomName(&bstrTitleName);
									CString strTmp, str_Value;
									strTmp = LPCSTR(_bstr_t(bstrTitleName));
									str_Value = GetResultFromName(strTmp, info);
									if (!str_Value.IsEmpty())
									{	
										pPEBom->put_BomValue(_bstr_t(str_Value));	
									}
								}
							}
						}
					}
				}

			}
		}
		//释放lib
		FreeLibrary(hd);
		//AfxMessageBox(strTmp);
	}

	*lSucceeded = 1;
	return S_OK;
}


