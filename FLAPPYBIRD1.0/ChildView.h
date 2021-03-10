
// ChildView.h : CChildView 类的接口
//


#pragma once


// CChildView 窗口
#define LEVEL_EVENT 4
#define SIMPLE 0
#define COMMON 1
#define HARD 2
#define HELL 3

#define NUMBER_BLOCK 9
#define NUMBER_PILLAR 198
#define NUMBER 10
class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	CRect m_client;    //保存客户区大小
	CDC m_cacheDC;   //缓冲DC
	CBitmap m_cacheCBitmap;//缓冲位图	

	struct BIRD
	{
		CImage m_pic;
		CRect m_body;
		int UP = 0;
	}bird;

	struct BLOCK
	{
		CImage pic;
		int x;
		int y;
		int width;
		int height;
	}fire[NUMBER_BLOCK], star[NUMBER_BLOCK];
	CImage pillar_pic[LEVEL_EVENT];
	CRect pillar[NUMBER_PILLAR];
	
	CImage m_picsound;
	CImage number[NUMBER];
	CImage m_bg[LEVEL_EVENT];
	CImage m_bgstart;
	CImage	m_bgover;
	CImage m_bgcomplete;
	CImage m_startbutton;
	CImage m_bgselect;
	CImage m_grand[LEVEL_EVENT];
	CImage m_pichelp;
	CImage m_bghelp;
	int grandmove;
	int process;
	int level;
	int point;
	int soundcontrol;
// 操作
public:
	void GetPoint();
	void setrandpillar();

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void BGMSetting();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL Canpass();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

