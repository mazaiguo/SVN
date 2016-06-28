#pragma once
//绘制套管或者管沟
class CDrawTGAndGG
{
public:
	CDrawTGAndGG(void);
	~CDrawTGAndGG(void);

	bool drawTG();
	bool drawGG();//埋深不足

	bool del();
	
private:
	bool midProcess(); 
	bool getStartZH();
	bool getEndZH();
	bool drawlineAndText();
	bool getDownText();
	bool getUpText();
	CString CurNumPosition( double dValue, bool& bIsExisted);

	bool getSpecailData();

private: 
	AcDbObjectIdArray m_idArrs;
	double m_dStartZH;
	double m_dEndZH;
	CString m_strDownText;
	CString m_strUpText;
	bool m_bIsTG;//是否套管
};
