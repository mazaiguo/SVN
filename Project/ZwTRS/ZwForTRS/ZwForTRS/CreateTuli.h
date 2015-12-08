#pragma once
#include <map>
#include <utility>
using namespace std;

//控制块的信息
class CBlkRefInFo
{
public:
	CBlkRefInFo(void);
	~CBlkRefInFo();
	
	//获取信息
	//获取插入点坐标
	AcGePoint3d GetBasePt() const;
	AcDbExtents GetExts() const;
	AcDbObjectId GetBlkId() const;
	//LPCTSTR GetBlkName() const;
	double GetBlkScale() const;

	//获取数据
	double GetLength() const;
	double GetWidth() const;
	AcGePoint3d GetMidPt() const;

	//设置信息
	void SetBasePt(AcGePoint3d basePt);
	void SetExts(AcDbExtents exts);
	void SetBlkId(AcDbObjectId blkId);
	//void SetBlkName(LPCTSTR strName);
	void SetBlkScale(double dScale);

	CBlkRefInFo&   operator = (const CBlkRefInFo& scl);
private:
	AcGePoint3d m_BasePt;//基础点
	AcDbExtents m_Exts;//外框
	AcDbObjectId m_blkId;//id
	//CString		m_strBlkName;//名字
	double		m_dScale;//比例
};

//控制生成图例框架的数据，包含长度，宽度等
typedef map<CString, pair<CBlkRefInFo*, int> > DataMap;
class CCreateFrame
{
public:
	CCreateFrame(void);
	~CCreateFrame(void);
	//获取数据
	AcGePoint3d GetBasePt() const;
 	//设置数据
	void SetBasePt(AcGePoint3d basePt);	
	
	//获取所有的块数据
	DataMap GetData() const;
	//设置所有的块数据
	void SetData(DataMap data);

	bool DoIt();
private:
	//操作
	//绘制外框
	bool DrawFrame();
	//绘制特殊文字
	void DrawSpecialText(AcGePoint3d basePt, double dLen, double dWidth, CString strText, bool bIsNum = false);
	//绘制外部参照
	void DrawSpecialBlkRef(AcGePoint3d basePt, CBlkRefInFo* blkInfo);

	//清理垃圾
	void Clear();
	//添加到选择集中
	void AppendToSS(AcDbObjectId objId);

	//移动实体
	void MoveEntity();
private:
	double m_dXuhao;	//序号长度
	double m_dTuli;//图例长度
	double m_dSBMC;//设备名称
	double m_dCount;//数量
	double m_dBeizhu;//备注
	double m_dTextHeight;//文字所在行的高度
	double m_dHeight;//图例所在行的高度
	double m_dTxtHeight;//文字高度
	double m_dScale;
	double m_dNumTextHeight;//数字文字高度
	AcGePoint3d m_basePt;//左下角点的插入点

	ads_name m_ssname;//选择集
	DataMap m_Data;
	AcDbObjectIdArray m_IdArrs;
};


class CCreateTuli
{
public:
	CCreateTuli(void);
	~CCreateTuli(void);

	void doIt();
	
	double GetBlkLength() const;
	double GetBlkHeight() const;
	//double GetTextLength() const;
	//double GetTextHeight() const;

	void SetBlkLength(double dBlkLength);
	void SetBlkHeight(double dBlkHeight);
	//void SetTextLength(double dTextLength);
	//void SetTextHeight(double dTextHeight);

private:
	bool AddEntity();

	void MoveEntity();
	void CloneAndMoveEntity(AcDbObjectId entId, AcGeMatrix3d m);
	bool ChangeBlkRef(AcDbObjectId objId);//修改块参照的位置
	void CalcScale(AcDbObjectId objId);//计算比例
	void Clear();
private:
	double m_BlkLength;//块参照所占的长度
	double m_BlkHeight;//块参照所占的高度
	double m_TextLength;//文字所占长度
	double m_TextHeight;//文字所占宽度
	double m_dScale;//比例
	//std::map<CString, std::pair<AcGePoint3d ,AcDbObjectId> > m_EntitySet;
	DataMap m_EntitySet;
	ads_name m_ssname;
};
