
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once


// CChildView 창
typedef enum ElementName
{
	mood,
	weather,
	emotion,
	hobby,
	food,
	housework
};

class Element
{
public:
	CImage colorImg;
	CImage grayImg;

	int x;
	int y;
	int xPixel;
	int yPixel;
	int xPrintPixel;
	int yPrintPixel;
	bool select;

	Element() { select = false; };

	void setPixel(int xPixel, int yPixel)
	{
		this->xPixel = xPixel;
		this->yPixel = yPixel;
	}

	void setPrintPixel(int xPrintPixel, int yPrintPixel)
	{
		this->xPrintPixel = xPrintPixel;
		this->yPrintPixel = yPrintPixel;
	}

	bool checkClick(int x, int y)
	{
		if (xPrintPixel <= x && x <= xPrintPixel + xPixel)
			if (yPrintPixel <= y && y <= yPrintPixel + yPixel)
				return true;
		return false;
	}

	void changeSelect()
	{
		select = !select;
	}

	int draw(CPaintDC& dc, CPoint point)
	{
		colorImg.Draw(dc, point);
		setPrintPixel(point.x, point.y);

		return yPixel;
	}

	bool click(CPoint point)
	{
		if (checkClick(point.x, point.y))
		{
			changeSelect();
			return true;
		}
		return false;
	}
};

class Elements
{
public:
	Element title;
	Element element[12]{};

	CString name;

	int size;

	Elements() {};

	void setNameSize(CString name, int size)
	{
		this->name = name;
		this->size = size;
	}

	void Load()
	{
		CString dir;

		dir.Format(L"image/%sTitle.png", name);
		title.colorImg.Load(dir);
		title.setPixel(200, 75);
		for (int i = 0; i < size; i++)
		{
			dir.Format(L"image/%s/%s%d.png", name, name, i);
			element[i].colorImg.Load(dir);
			dir.Format(L"image/%sGray/%s%d_gray.png", name, name, i);
			element[i].grayImg.Load(dir);
			element[i].setPixel(90, 100);
		}
	}

	int draw(CPaintDC& dc, CPoint point)
	{
		CPoint startPoint;
		startPoint = point;

		title.colorImg.Draw(dc, point.x, point.y);
		point.y += title.yPixel;
		for (int i = 0; i < size; i++)
		{
			if (element[i].select)
				element[i].colorImg.Draw(dc, point.x, point.y);
			else
				element[i].grayImg.Draw(dc, point.x, point.y);

			element[i].setPrintPixel(point.x, point.y);

			point.x += element[i].xPixel;
			if ((i + 1) % 5 == 0)
			{
				point.x = startPoint.x;
				point.y += element[i].yPixel;
			}
		}

		return point.y - startPoint.y + element[0].yPixel;
	}

	bool moodClick(CPoint point, bool& bms)
	{
		for (int i = 0; i < size; i++)
		{
			if (bms)
			{
				if (element[i].checkClick(point.x, point.y) && element[i].select)
				{
					element[i].changeSelect();
					bms = false;
					return true;
				}
			}
			else
			{
				if (element[i].checkClick(point.x, point.y))
				{
					element[i].changeSelect();
					bms = true;
					return true;
				}
			}
		}
		return false;
	}

	bool click(CPoint point)
	{
		for (int i = 0; i < size; i++)
		{
			if (element[i].checkClick(point.x, point.y))
			{
				element[i].changeSelect();
				return true;
			}
		}
		return false;
	}
};

class CChildView : public CWnd
{
// 변수 생성
	Element background;
	Element camera;
	Element save;

	Elements em[6];
	
	bool b_moodSelect;
	bool b_calendarMenu;
	bool b_dailyCheckMenu;

// 함수 생성
	void calendarMenu();
	void dailyCheckMenu();

// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

