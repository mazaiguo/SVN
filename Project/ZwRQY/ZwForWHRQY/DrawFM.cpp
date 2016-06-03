#include "StdAfx.h"
#include "DrawFM.h"

CDrawFM::CDrawFM(void)
{
}

CDrawFM::~CDrawFM(void)
{
}


bool CDrawFM::GetZhuanghao()
{
	CString strPrompt;
	double dZhuanghao;
	strPrompt.Format(_T("\nָ������׮��ֵ<m> <%.2f>:"), dZhuanghao);
	
	int nRet = acedGetReal(strPrompt, &dZhuanghao);
	if (nRet != RTNORM)
	{
		return false;
	}

}

bool CDrawFM::GetDescription()
{
	TCHAR tempBuf[133];
	CString strNo;

	int nRet = acedGetString(1.  _T("\n��������:"),tempBuf);
	if (nRet != RTNORM)
	{
		return false;
	}
	strNo = tempBuf;
	
	return true;
}

//����WipeOut����
Acad::ErrorStatus CDrawFM::CreateWipeout (AcDbObjectId &wipeoutId,AcGePoint3dArray point3ds)
{
	Acad::ErrorStatus es;
#ifdef ARX
	if (RTNORM != acedArxLoad(_T("AcWipeoutObj18.dbx"))) return Acad::eNotImplementedYet;
	es = AcDbWipeout::createImageDefinition();
	if (es != Acad::eOk)
	{
		return es;
	}
#else
	if (RTNORM != acedArxLoad(_T("ZrxDraw.zrx")))
	{
		return Acad::eNotImplementedYet;
	}
#endif	
	if (!point3ds.at(0).isEqualTo(point3ds.last()))
	{
		point3ds.append(point3ds.at(0));
	}
	AcDbWipeout *pWipeout = new AcDbWipeout;
	if(pWipeout == NULL) return Acad::eNotImplementedYet;
	pWipeout->setDatabaseDefaults();
	AcGePoint3d originPnt(AcGePoint3d::kOrigin);
	AcGeVector3d Udirection(1,0,0);
	AcGeVector3d Vdirection(0,-1,0);
	pWipeout->setOrientation(originPnt,Udirection, Vdirection); 

	pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent,Adesk::kTrue);
	pWipeout->setDisplayOpt(AcDbRasterImage::kShow, false);
	es = pWipeout->setClipBoundaryToWholeImage();
	AcGeMatrix3d PixelToModel,ModelToPixel;
	pWipeout->getPixelToModelTransform(PixelToModel);
	ModelToPixel=PixelToModel.invert();
	for(int i=0;i< point3ds.length();i++)
	{
		point3ds.at(i).transformBy(ModelToPixel);
	}   
	AcGePoint2dArray point2ds = MyTransFunc::My3d22d(point3ds);
	es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly,point2ds);
	AcGePoint2d pt2d;
	double scale;
	AcDbObjectId id;
	pWipeout->append(id);
	wipeoutId = pWipeout->objectId();
	pWipeout->close();
	return Acad::eOk;
}