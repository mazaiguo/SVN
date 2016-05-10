#pragma once
#ifdef MYBASEDLL_EXPORTS //�ڴ�������DLL�Ĺ������Ȱ� MYBASEDLL_EXPORTS Ԥ������
#define SERVERDLL_API __declspec(dllexport)
#else
#define SERVERDLL_API __declspec(dllimport)
#endif

class SERVERDLL_API MyLayout
{
public:
	MyLayout(void);
	~MyLayout(void);
	// --------------------------------------------------------------------------------------------
	// Summary: ���Ʋ��ֲ�ͼ
	// Parameters:
	//     pt1                -    ��ͼ�������ڵ������С��
	//     pt2                -    ��ͼ�������ڵ��������
	//     ptInsertTopLeft    -    �ӿڵ����½ŵ�
	//     dScaleFactor        -    ���ű���
	//     dAngle                -    ��ͼ�Ƕ�
	//     pBoundary            -    ��ͼ�߽�
	// Returns: 
	//     �ɹ����� Acad::eOk, ���򷵻� Acad::ErrorStatus ����״̬
	// --------------------------------------------------------------------------------------------
	static Acad::ErrorStatus DrawLayout(const AcGePoint3d& pt1, const AcGePoint3d& pt2, const AcGePoint3d& ptInsertBottomLeft, LPCTSTR lpszLayoutName, double dScaleFactor, double dAngle, AcDbPolyline* pBoundary);

	// --------------------------------------------------------------------------------------------
	// Summary: �����²���
	// Parameters:
	//     lpszName        -    ���ֵ�����
	//     layoutId        -    ����id
	//     blkTabRecId    -    ���ֿ���¼
	// Returns: 
	//     �ɹ����� Acad::eOk, ���򷵻� Acad::ErrorStatus ����״̬
	// --------------------------------------------------------------------------------------------
	static Acad::ErrorStatus CreateNewLayout(LPCTSTR lpszName, AcDbObjectId& layoutId, AcDbObjectId& blkTabRecId);

	// --------------------------------------------------------------------------------------------
	// Summary: �������в���
	// --------------------------------------------------------------------------------------------
	Acad::ErrorStatus AllLayout();

	// --------------------------------------------------------------------------------------------
	// Summary: ����ָ�����ƵĲ���
	// Parameters:
	//     lpszName        -    ���ֵ�����
	//     layoutId        -    ����id
	//     blkTabRecId    -    ���ֿ���¼
	// Returns: 
	//     �ɹ����� Acad::eOk, ���򷵻� Acad::ErrorStatus ����״̬
	// --------------------------------------------------------------------------------------------
	static Acad::ErrorStatus FindLayout(LPCTSTR lpszName, AcDbObjectId& layoutId, AcDbObjectId& blkTabRecId);
};
