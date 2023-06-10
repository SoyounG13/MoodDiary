
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <atlimage.h>
#include <shlobj.h>
#include <gdiplusheaders.h>
using namespace std;
using namespace cv;

// CChildView 창
struct Data
{
	int mood = 6;

	int weather[4];
	int emotion[11];
	int hobby[7];
	int food[7];
	int housework[3];

	string picture;
};

enum ElementName
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

class Element2
{
public:
	CImage Img;

	int xPixel;
	int yPixel;
	int xPrintPixel;
	int yPrintPixel;

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

	void draw(CPaintDC& dc, CPoint point)
	{
		Img.Draw(dc, point);
		setPrintPixel(point.x, point.y);
	}
};

class MonthElement
{
public:
	Element2 month[12];
	Element2 letter;

	void Load()
	{
		CString dir;

		dir.Format(L"image/Month/month.png");
		letter.Img.Load(dir);
		letter.setPixel(50, 65);
		for (int i = 1; i <= 12; i++)
		{
			dir.Format(L"image/Month/month%d.png", i);
			month[i - 1].Img.Load(dir);
			month[i - 1].setPixel(50, 65);
		}
	}

	void draw(CPaintDC& dc, int num)
	{
		CPoint p(900 / 2 - 50, 0);
		month[num - 1].draw(dc, p);
		p.x = 900 / 2;
		letter.draw(dc, p);
	}
};

class MonthButtomElement : public Element2
{
public:
	bool isLeft = false;
	bool isRight = false;

	void setLeftRight(bool isLeft)
	{
		if (isLeft)
			this->isLeft = true;
		else
			this->isRight = true;
	}

	void Load()
	{
		CString dir;

		if (isLeft)
			dir.Format(L"image/Month/left.png");
		else if (isRight)
			dir.Format(L"image/Month/right.png");

		Img.Load(dir);
		setPixel(65, 65);
	}

	void draw(CPaintDC& dc)
	{
		CPoint point;

		if (isLeft)
			point = { 900 / 2 - 50 - 65, 0 };
		else if (isRight)
			point = { 900 / 2 + 50, 0 };

		Img.Draw(dc, point);
		setPrintPixel(point.x, point.y);
	}

	bool checkClick(int x, int y)
	{
		if (xPrintPixel <= x && x <= xPrintPixel + xPixel)
			if (yPrintPixel <= y && y <= yPrintPixel + yPixel)
				return true;
		return false;
	}

	bool click(CPoint point, int& month)
	{
		if (checkClick(point.x, point.y))
		{
			if (isLeft)
			{
				if (month > 1)
					month--;
			}
			else if (isRight)
			{
				if (month < 12)
					month++;
			}
			return true;
		}
		return false;
	}
};

class DateElement
{
public:
	Element2 date[31];

	void Load()
	{
		CString dir;

		for (int i = 1; i <= 31; i++)
		{
			dir.Format(L"image/Date/date%d.png", i);
			date[i - 1].Img.Load(dir);
			date[i - 1].setPixel(128, 22);
		}
	}

	void draw(CPaintDC& dc, int num)
	{
		int size;
		switch (num)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			size = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			size = 30;
			break;
		case 2:
			size = 28;
			break;
		}

		CPoint point;
		switch (num)
		{
		case 1:
		case 10:
			point = { 6, 0 };
			break;
		case 2:
		case 3:
		case 11:
			point = { 2, 0 };
			break;
		case 4:
		case 7:
			point = { 5, 0 };
			break;
		case 6:
			point = { 3, 0 };
			break;
		case 5:
		case 8:
			point = { 1, 0 };
			break;
		case 9:
		case 12:
			point = { 4, 0 };
			break;
		}
		point.x = point.x * 128 + 2;
		point.y = point.y * 150 + 65 + 128;

		for (int i = 1; i <= size; i++)
		{
			date[i - 1].draw(dc, point);

			point.x += date[i - 1].xPixel;
			if (point.x == 898)
			{
				point.x = 2;
				point.y += 150;
			}
		}
	}
};

class FlowerElement : public Element2
{
public:
	int num;
	int date;

	bool checkClick(int x, int y)
	{
		if (xPrintPixel <= x && x <= xPrintPixel + xPixel)
			if (yPrintPixel <= y && y <= yPrintPixel + yPixel)
				return true;
		return false;
	}

	bool click(CPoint point)
	{
		if (checkClick(point.x, point.y))
		{
			
			return true;
		}
		return false;
	}
};

class FlowerElements
{
public:
	FlowerElement fe[31];
	CImage Img[7];
	int size;

	void Load()
	{
		CString dir;

		for (int i = 0; i <= 6; i++)
		{
			dir.Format(L"image/Flower/flower%d.png", i);
			Img[i].Load(dir);
		}

		for (int i = 0; i < 31; i++)
		{
			fe[i].setPixel(128, 128);
			fe[i].date = i + 1;
		}
	}

	void draw(CPaintDC& dc, int num, Data data[12][31])
	{
		switch (num)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			size = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			size = 30;
			break;
		case 2:
			size = 28;
			break;
		}

		CPoint point;
		switch (num)
		{
		case 1:
		case 10:
			point = { 6, 0 };
			break;
		case 2:
		case 3:
		case 11:
			point = { 2, 0 };
			break;
		case 4:
		case 7:
			point = { 5, 0 };
			break;
		case 6:
			point = { 3, 0 };
			break;
		case 5:
		case 8:
			point = { 1, 0 };
			break;
		case 9:
		case 12:
			point = { 4, 0 };
			break;
		}
		point.x = point.x * 128 + 2;
		point.y = point.y * 150 + 65;

		for (int i = 1; i <= size; i++)
		{
			fe[i - 1].num = data[num - 1][i - 1].mood;
			
			Img[fe[i - 1].num].Draw(dc, point);
			fe[i - 1].setPrintPixel(point.x, point.y);

			point.x += 128;
			if (point.x == 898)
			{
				point.x = 2;
				point.y += 150;
			}
		}
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

	/* ------------------- */

	Element2 background2;
	MonthElement me;
	MonthButtomElement left;
	MonthButtomElement right;
	DateElement de;
	FlowerElements fe;

	Data data[12][31];

	int month;
	int date;

	/* ------------------- */

	bool b_calendarMenu;
	bool b_dailyCheckMenu;

// 함수 생성
	void calendarMenu();
	void dailyCheckMenu();
	void ReadDataFromFile(string filename);
	void UpdateData(string filename, Data newData, int monthData, int dateData);
	void CaptureWebcamImage(CImage& image);
	void SaveImageToPNG(CImage& image, CString filename);

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

