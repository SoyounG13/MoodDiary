
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MoodDiary.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	em[mood].setNameSize(L"Mood", 6);
	em[weather].setNameSize(L"Weather", 4);
	em[emotion].setNameSize(L"Emotion", 11);
	em[hobby].setNameSize(L"Hobby", 7);
	em[food].setNameSize(L"Food", 7);
	em[housework].setNameSize(L"Housework", 3);
	camera.setPixel(450, 300);
	save.setPixel(200, 150);

	background.colorImg.Load(L"image/Background.png");
	camera.colorImg.Load(L"image/Camera.png");
	save.colorImg.Load(L"image/save.png");

	for (int i = mood; i <= housework; i++)
		em[i].Load();

	b_moodSelect = false;
	b_calendarMenu = false;
	b_dailyCheckMenu = true;

	dailyCheckMenu();
}

CChildView::~CChildView()
{
}

void CChildView::calendarMenu()
{
	b_calendarMenu = true;
	b_dailyCheckMenu = false;
}

void CChildView::dailyCheckMenu()
{
	b_calendarMenu = false;
	b_dailyCheckMenu = true;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (b_calendarMenu)
	{
		return;
	}

	else if (b_dailyCheckMenu)
	{
		CPoint pointer;
		pointer.x = 0;
		pointer.y = 0;

		background.draw(dc, pointer);

		for (int i = mood; i <= hobby; i++)
			pointer.y += em[i].draw(dc, pointer);

		pointer.x = 450;
		pointer.y = 0;

		for (int i = food; i <= housework; i++)
			pointer.y += em[i].draw(dc, pointer);

		pointer.y += camera.draw(dc, pointer);

		pointer.x = 575;

		save.draw(dc, pointer);
	}

	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int xClick, yClick;
	xClick = point.x;
	yClick = point.y;

	if (b_calendarMenu)
	{
		return;
	}

	else if (b_dailyCheckMenu)
	{
		if (em[mood].moodClick(point, b_moodSelect))
			return;
		for (int i = weather; i <= housework; i++)
			if (em[i].click(point))
				return;
		if (camera.click(point))
			return;
		if (save.click(point))
			return;
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RedrawWindow();
	CWnd::OnLButtonUp(nFlags, point);
}
