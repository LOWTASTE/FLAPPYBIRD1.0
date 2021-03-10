
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "FLAPPYBIRD1.0.h"
#include "ChildView.h"

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")//导入声音头文件库

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_START 1
#define TIMER_UP 2
//#define TIMER_BLOCK 3
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 1000
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView 消息处理程序
//计分函数



BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	level = -1;
	process = 0;
	//载入开始界面
	m_bgstart.Load(_T("bgstart.png"));
	//载入SOUND按钮
	m_picsound.Load(_T("sound.png"));
	//载入help按钮
	m_pichelp.Load(_T("help.png"));
	m_bghelp.Load(_T("bghelp.png"));
	//载入BGM
	BGMSetting();
	//载入开始按钮图片
	m_startbutton.Load(_T("start.png"));
	//载入难度选择
	m_bgselect.Load(_T("bgselect.png"));
	//载入完成画面
	m_bgcomplete.Load(_T("bgcomplete.png"));
	//载入天空背景
	m_bg[SIMPLE].Load(_T("bgsimple.png"));
	m_bg[COMMON].Load(_T("bgcommon.png"));
	m_bg[HARD].Load(_T("bghard.png"));
	m_bg[HELL].Load(_T("bghell.png"));
	//载入地面背景
	m_grand[SIMPLE].Load(_T("bggsimple.png"));
	m_grand[COMMON].Load(_T("bggcommon.png"));
	m_grand[HARD].Load(_T("bgghard.png"));
	m_grand[HELL].Load(_T("bgghell.png"));
	//载入鸟
	bird.m_pic.Load(_T("bird.png"));
	bird.m_body.SetRect(200, 200, 280, 280);
	//载入柱子
	pillar_pic[SIMPLE].Load(_T("pillar0.png"));
	pillar_pic[COMMON].Load(_T("pillar1.png"));
	pillar_pic[HARD].Load(_T("pillar2.png"));
	pillar_pic[HELL].Load(_T("pillar3.png"));
	setrandpillar();
	//载入数字

	number[0].Load(_T("0.png"));
	number[1].Load(_T("1.png"));
	number[2].Load(_T("2.png"));
	number[3].Load(_T("3.png"));
	number[4].Load(_T("4.png"));
	number[5].Load(_T("5.png"));
	number[6].Load(_T("6.png"));
	number[7].Load(_T("7.png"));
	number[8].Load(_T("8.png"));
	number[9].Load(_T("9.png"));
	//载入火焰
	for (size_t i = 0; i <NUMBER_BLOCK; i++)
	{
		if (i % 3 == 0)
			fire[i].pic.Load(_T("fire0.png"));
		if (i % 3 == 1)
			fire[i].pic.Load(_T("fire1.png"));
		if (i % 3 == 2)
			fire[i].pic.Load(_T("fire2.png"));

		fire[i].x = rand() % 700+300;
		fire[i].y = rand() % 600;		
		fire[i].height = rand() % 50;
		fire[i].width = fire[i].height;

	}
	//载入星星
	for (size_t i = 0; i <NUMBER_BLOCK; i++)
{
	star[i].pic.Load(_T("star.png"));
	star[i].x = rand() % 800+200;
	star[i].y = rand() % 600;
	star[i].height = rand() % 50;
	star[i].width = star[i].height;
}

	//载入结束界面
	m_bgover.Load(_T("over.png"));
	return TRUE;
}


void CChildView::OnPaint() 
{
	CDC *cDC = this->GetDC();
	//获取窗口大小
	GetClientRect(&m_client);
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	//开始界面
	if (process == 0)
	{
		m_bgstart.Draw(m_cacheDC, 0, 0);
		m_startbutton.Draw(m_cacheDC, 400, 300);
		m_pichelp.Draw(m_cacheDC, 860, 520);
	}
	//选择界面
	if (process == 1)
	{
		m_bgselect.Draw(m_cacheDC, 0, 0);
	}
	//游戏界面
	if (process == 2) 
	{
		//天空背景
		m_bg[level].Draw(m_cacheDC, 0, 0);
		//地面背景
		m_grand[level].Draw(m_cacheDC, grandmove, 400);
		m_grand[level].Draw(m_cacheDC, grandmove+1000, 400);
		//鸟
		bird.m_pic.Draw(m_cacheDC, bird.m_body.left, bird.m_body.top, 100, 100, bird.UP*100,0,100, 100);
		//柱子
		for (size_t i = 0; i < NUMBER_PILLAR; i++)
		{
			pillar_pic[level].Draw(m_cacheDC, pillar[i].left, pillar[i].top,100,pillar[i].bottom-pillar[i].top );
		}
		//星辰特效
		if (level ==HARD)
		{
			for (size_t i = 0; i < NUMBER_BLOCK; i++)
			{
				star[i].pic.Draw(m_cacheDC, star[i].x, star[i].y);
			}
		}
		//火焰特效
		if (level == HELL) 
		{
			for (size_t i = 0; i < NUMBER_BLOCK; i++)
			{
				fire[i].pic.Draw(m_cacheDC, fire[i].x, fire[i].y);
			}
		}
		if (point == 98)
		{
			process = 4;
		}
	}
	//结束界面
	if (process == 3)
	{
		m_bgover.Draw(m_cacheDC, 0, 0);
		GetPoint();
	}

	if (process == 4) {
		m_bgcomplete.Draw(m_cacheDC, 0, 0);
	}

	if (process == 5)
	{
		m_bghelp.Draw(m_cacheDC, 0, 0);
	}
	m_picsound.Draw(m_cacheDC,920,520);
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);

	ValidateRect(&m_client);
	//释放缓冲DC
	m_cacheDC.DeleteDC();
	//释放对象
	m_cacheCBitmap.DeleteObject();
	//释放DC
	ReleaseDC(cDC);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_START:
	{
		if (level == -1 && process == 0)
		{
			OnPaint();
		}
		if (level!=-1 && process==1)
		{
			process = 2;
		}
		if (process == 2)
		{
			//地面场景移动			
			grandmove-=5*(level+1);
			if (grandmove == -1000)
				grandmove = 0;			

			//鸟下降,障碍物
			if(level==SIMPLE)
			{ 
				bird.m_body.top ++;
				bird.m_body.bottom ++;
			}
			if (level == COMMON)
			{
				bird.m_body.top+=2;
				bird.m_body.bottom+=2;
			}
			if (level == HARD)
			{
				bird.m_body.top+=3;
				bird.m_body.bottom+=3;
				//STAR
				for (size_t i = 0; i <NUMBER_BLOCK; i++)
				{
					star[i].x -= 10;
					star[i].y += 10;
					if (star[i].x <= 0)
						star[i].x = 1000;
					if (star[i].y >= 600)
						star[i].y = 0;
				}

			}
			if (level == HELL)
			{
				bird.m_body.top+=5;
				bird.m_body.bottom+=5;
				//FIRE

				for (size_t i = 0; i <NUMBER_BLOCK; i++)
				{
					fire[i].x -= 15;
					fire[i].y += 15;

					if (fire[i].x <= 0)
						fire[i].x = 1000;
					if (fire[i].y >= 600)
						fire[i].y = 0;

				}

			}

			//柱子移动 以及 计分
			for (size_t i = 0; i < NUMBER_PILLAR; i++)
			{
				if (level == SIMPLE)
				{
					pillar[i].left-=6;
					pillar[i].right-=6;	
					if (pillar[i].left<=200)
					{
						point = i/2+1;
					}
				}
				if (level == COMMON)
				{
					pillar[i].left-=10;
					pillar[i].right-=10;
					if (pillar[i].left <= 200)
					{
						point = i/2+1;
					}
				}
				if (level == HARD)
				{
					pillar[i].left-=15;
					pillar[i].right-=15;
					if (pillar[i].left <= 200)
					{
						point = i/2+1;
					}
				}			
				if (level == HELL)
				{
					pillar[i].left-=25;
					pillar[i].right-=25;
					if (pillar[i].left <= 200)
					{
						point = i/2+1;
					}

				}

			}			
			OnPaint();

			//判断
			if (!Canpass())
			{

				OnPaint();
				process = 3;
			}

		}	
		OnPaint();
	}
	break;
	case TIMER_UP:
	{
		bird.UP = 0;
	}
	break;
	/*case TIMER_BLOCK:
	{
		if(process==2&&level==HARD)
			//STAR
		for (size_t i = 0; i <NUMBER_BLOCK; i++)
			{
				star[i].x-=10;
				star[i].y+=10;
				if (star[i].x <= 0)
					star[i].x = 1000;
				if (star[i].y >= 600)
					star[i].y = 0;
			}
		//FIRE
		for (size_t i = 0; i <NUMBER_BLOCK; i++)
		{
			fire[i].x -= 30;
			fire[i].y += 20;

			if (fire[i].x <= 0)
				fire[i].x = 1000;
			if (fire[i].y >= 600)
				fire[i].y = 0;

		}

	}
	break;*/
	}
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(TIMER_START, 1, NULL);
	SetTimer(TIMER_UP, 200, NULL);
	//SetTimer(TIMER_BLOCK, 200, NULL);
	return 0;
}


BOOL CChildView::Canpass() {
	for (size_t i = 0; i < NUMBER_PILLAR; i++)
	{
		if (i%2==0)
			if (abs(bird.m_body.left - pillar[i].left) <= 100 && bird.m_body.top <= pillar[i].bottom)
				return FALSE;
		if (i%2==1)
			if (abs(bird.m_body.left - pillar[i].left) <= 100 && bird.m_body.bottom >= pillar[i].top)
				return FALSE;
	}
	if (bird.m_body.top <= 0 || bird.m_body.bottom >= 600)
		return FALSE;
	return  TRUE;
}

//重新设置柱子和障碍物
void CChildView::setrandpillar()
{
	grandmove = 0;
	bird.m_body.SetRect(200, 200, 280, 280);
	for (size_t i = 0; i < NUMBER_PILLAR; i++)
	{
		//top
		if (i % 2 == 0)
		{
			pillar[i].top = 0;
			pillar[i].left = 900 + (i + 1) * 150;
			//
			pillar[i].bottom = rand() % 200;
			pillar[i].right = pillar[i].left + 50;
		}
		//bottom
		if (i % 2 == 1)
		{
			//
			//pillar[i].top = 400 + rand() % 200;
			pillar[i].top = 300 + pillar[i-1].bottom;
			pillar[i].left = 900 + i * 150;
			pillar[i].bottom = WINDOW_HEIGHT;
			pillar[i].right = pillar[i].left + 50;

			//if(pillar[i].top-pillar[i-1].bottom<200)
			//{
				//i--;
			//}
		}

	}
	for (size_t i = 0; i < NUMBER_BLOCK; i++)
	{
		fire[i].x = rand() % 700 + 300;
		fire[i].y = rand() % 600;
		star[i].x = rand() % 800 + 200;
		star[i].y = rand() % 600;

	}
}



void CChildView::GetPoint()
{

	int ten=point/10;
	int one=point-ten*10;
	number[ten].Draw(m_cacheDC, 250, 0);
	number[one].Draw(m_cacheDC, 500, 0);

}

//选择难度 重开 菜单
void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (process == 1&&level == -1)
	{
		if (point.y >= 100 && point.y < 200 && point.x >= 400 && point.x <= 600)
			level = SIMPLE;
		if (point.y >= 200 && point.y < 300 && point.x >= 400 && point.x <= 600)
			level = COMMON;
		if (point.y >= 300 && point.y < 400 && point.x >= 400 && point.x <= 600)
			level = HARD;
		if (point.y >= 400 && point.y < 500 && point.x >= 400 && point.x <= 600)
			level = HELL;
		if (point.y >= 0 && point.y < 120 && point.x >= 0 && point.x <= 260)
			process = 0;
		//BGM
		BGMSetting();
	}
	if (process == 4)
	{
		process = 3;
	}
	if (point.x >= 920 && point.x <= 1020 && point.y > 520 && point.y < 580)
	{
		if (soundcontrol == 0)
		{
			mciSendString(_T("stop bgmstart.mp3"), NULL, 0, NULL);
			mciSendString(_T("stop bgmsimple.mp3"), NULL, 0, NULL);
			mciSendString(_T("stop bgmcommon.mp3"), NULL, 0, NULL);
			mciSendString(_T("stop bgmhard.mp3"), NULL, 0, NULL);
			mciSendString(_T("stop bgmhell.mp3"), NULL, 0, NULL);

			soundcontrol = 1;
		}
		else if (soundcontrol == 1)
		{			
			BGMSetting();
			soundcontrol = 0;
		}
	}
	if (process == 5)
	{
		process = 0;
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (process == 0 && point.x >= 400 && point.x <= 600 && point.y >= 300 && point.y <= 400)
	{
		process = 1;
	}
	if (process == 0 && point.x >= 860 && point.x <= 920 && point.y > 520 && point.y < 580)
	{
		process = 5;
	}
	if (process == 2)
	{
		if (level==SIMPLE)
		{
			bird.m_body.bottom -= 100;
			bird.m_body.top -= 100;
		}
		if (level == COMMON)
		{
			bird.m_body.bottom -= 120;
			bird.m_body.top -= 120;
		}
		if (level == HARD)
		{
			bird.m_body.bottom -= 140;
			bird.m_body.top -= 140;
		}
		if (level == HELL)
		{
			bird.m_body.bottom -= 150;
			bird.m_body.top -= 150;
		}

		bird.UP = 1;
		//mciSendString(_T("play jump.mp3"), NULL, 0, NULL);
		if (soundcontrol == 0)
		PlaySound(_T("jump.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	if (process == 3)
	{
		if (point.x <= 250 && point.x >= 0 && point.y >= 0 && point.y <= 200)
		{

			setrandpillar();
			CChildView::point = 0;
			process = 2;

		}
		if (point.x <= 1000 && point.x >= 750 && point.y >= 0 && point.y <= 200)
		{
			level = -1;
			process = 0;
			CChildView::point = 0;
			BGMSetting();
			setrandpillar();

		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::BGMSetting() {
	if (level == -1)
	{
		mciSendString(_T("play bgmstart.mp3 repeat"), NULL, 0, NULL);
		mciSendString(_T("stop bgmsimple.mp3"), NULL, 0, NULL);
		mciSendString(_T("stop bgmcommon.mp3"), NULL, 0, NULL);
		mciSendString(_T("stop bgmhard.mp3"), NULL, 0, NULL);
		mciSendString(_T("stop bgmhell.mp3"), NULL, 0, NULL);
	}
	else
	{
		mciSendString(_T("stop bgmstart.mp3"), NULL, 0, NULL);

		if (level == 0)
			mciSendString(_T("play bgmsimple.mp3 repeat"), NULL, 0, NULL);
		if (level == 1)
			mciSendString(_T("play bgmcommon.mp3 repeat"), NULL, 0, NULL);
		if (level == 2)
			mciSendString(_T("play bgmhard.mp3 repeat"), NULL, 0, NULL);
		if (level == 3)
			mciSendString(_T("play bgmhell.mp3 repeat"), NULL, 0, NULL);
	}
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//nChar表示按下的键值  
	switch (nChar)
	{
	case ' ':
	{
		if (level == SIMPLE)
		{
			bird.m_body.bottom -= 100;
			bird.m_body.top -= 100;
		}
		if (level == COMMON)
		{
			bird.m_body.bottom -= 120;
			bird.m_body.top -= 120;
		}
		if (level == HARD)
		{
			bird.m_body.bottom -= 100;
			bird.m_body.top -= 100;
		}
		if (level == HELL)
		{
			bird.m_body.bottom -= 150;
			bird.m_body.top -= 150;
		}
		if(soundcontrol==0)
			PlaySound(_T("jump.wav"), NULL, SND_FILENAME | SND_ASYNC);

		bird.UP = 1;
		//mciSendString(_T("play jump.mp3"), NULL, 0, NULL);
	}
	break;
	case 'q':
	case'Q':
	{
		bird.m_body.bottom += 10;
		bird.m_body.top +=10;
	}
	break;
	case'e':
	case'E':
	{
		bird.m_body.bottom -=50;
		bird.m_body.top -= 50;
		bird.UP = 1;
	}
	break;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
