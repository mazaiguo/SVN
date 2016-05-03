#pragma once
//标尺数据
class CBiaochiInfo
{
public:
	CBiaochiInfo(void);
	~CBiaochiInfo(void);
	bool start();
	//创建新的地面线
	bool newLine();
	//在原有的数据上继续
	bool oldLine();
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
private:

	//************************************
	// Method:    initData
	// FullName:  CBiaochiInfo::initData
	// Access:    private 
	// Returns:   void
	// Qualifier:初始化数据
	//************************************
	void initData();

private:
	bool m_bIsNode;//是否有节点
	AcGePoint3d m_basePt;//基点
};
