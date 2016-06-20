#pragma once
//绘制套管或者管沟
class CDrawTGAndGG
{
public:
	CDrawTGAndGG(void);
	~CDrawTGAndGG(void);

	bool drawTG();
	bool drawGG();//埋深不足
	bool getStartZH();
	bool getEndZH();
	bool drawlineAndText();
	bool getDescription();
private:
	

private: 
	AcDbObjectIdArray m_idArrs;
	double m_dStartZH;
	double m_dEndZH;
	CString m_strDescription;
};
