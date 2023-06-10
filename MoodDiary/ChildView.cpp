
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

	/* ----------------------------------------------- */
	
	ReadDataFromFile("data.txt");

	month = 1;

	left.setLeftRight(true);
	right.setLeftRight(false);

	background2.Img.Load(L"image/Background2.png");
	me.Load();
	left.Load();
	right.Load();
	de.Load();
	fe.Load();

	calendarMenu();
}

CChildView::~CChildView()
{
}

void CChildView::calendarMenu()
{
	b_calendarMenu = true;
	b_dailyCheckMenu = false;

	for(int i = 0; i < 6; i++)
		em[mood].element[i].select = false;
	b_moodSelect = false;
	for (int i = 0; i < 4; i++)
		em[weather].element[i].select = false;
	for (int i = 0; i < 11; i++)
		em[emotion].element[i].select = false;
	for (int i = 0; i < 7; i++)
		em[hobby].element[i].select = false;
	for (int i = 0; i < 7; i++)
		em[food].element[i].select = false;
	for (int i = 0; i < 3; i++)
		em[housework].element[i].select = false;
}

void CChildView::dailyCheckMenu()
{
	b_calendarMenu = false;
	b_dailyCheckMenu = true;
	
	if (data[month - 1][date - 1].mood != 6)
	{
		em[mood].element[data[month - 1][date - 1].mood].select = true;
		b_moodSelect = true;
		for (int i = 0; i < 4; i++)
			em[weather].element[i].select = data[month - 1][date - 1].weather[i];
		for (int i = 0; i < 11; i++)
			em[emotion].element[i].select = data[month - 1][date - 1].emotion[i];
		for (int i = 0; i < 7; i++)
			em[hobby].element[i].select = data[month - 1][date - 1].hobby[i];
		for (int i = 0; i < 7; i++)
			em[food].element[i].select = data[month - 1][date - 1].food[i];
		for (int i = 0; i < 3; i++)
			em[housework].element[i].select = data[month - 1][date - 1].housework[i];
	}
}

void CChildView::ReadDataFromFile(string filename)
{
	const int MAX_MONTH = 12;
	const int MAX_DATE = 31;

	ifstream infile(filename);

	string line;
	while (getline(infile, line))
	{
		stringstream ss(line);
		string token;

		getline(ss, token, ' ');
		int month = stoi(token);

		getline(ss, token, ' ');
		int date = stoi(token);

		Data& d = data[month - 1][date - 1];

		getline(ss, token, ' ');
		d.mood = stoi(token);

		for (int i = 0; i < 4; i++)
		{
			getline(ss, token, ' ');
			d.housework[i] = stoi(token);
		}

		for (int i = 0; i < 11; i++)
		{
			getline(ss, token, ' ');
			d.emotion[i] = stoi(token);
		}

		for (int i = 0; i < 7; i++)
		{
			getline(ss, token, ' ');
			d.hobby[i] = stoi(token);
		}
		
		for (int i = 0; i < 7; i++)
		{
			getline(ss, token, ' ');
			d.food[i] = stoi(token);
		}

		for (int i = 0; i < 3; i++)
		{
			getline(ss, token, ' ');
			d.housework[i] = stoi(token);
		}

		getline(ss, token, ' ');
		d.picture = token;
	}

	infile.close();
}

// 웹캠으로부터 사진을 찍고 CImage에 저장하는 함수
void CChildView::CaptureWebcamImage(CImage& image)
{
	VideoCapture capture(0);  // 웹캠을 엽니다.

	if (!capture.isOpened())
	{
		AfxMessageBox(_T("웹캠을 열 수 없습니다."));
		return;
	}

	Mat frame;
	capture.read(frame);  // 웹캠으로부터 프레임을 읽어옵니다.

	// 윈도우 생성 및 이미지 표시
	cv::namedWindow("이미지");
	cv::imshow("이미지", frame);

	// 키 입력 대기
	cv::waitKey(0);

	// 윈도우 닫기
	cv::destroyAllWindows();


	// OpenCV의 Mat을 CImage로 변환합니다.
	image.Create(frame.cols, frame.rows, 24);
	uchar* imgBuffer = (uchar*)image.GetBits();
	int imgPitch = image.GetPitch();

	Mat imgRGB(frame.rows, frame.cols, CV_8UC3, imgBuffer, imgPitch);
	cvtColor(frame, imgRGB, COLOR_BGR2RGB);

	capture.release();  // 웹캠 사용이 끝났으므로 해제합니다.
}

// CImage를 PNG 파일로 저장하는 함수
void CChildView::SaveImageToPNG(CImage& image, CString filename)
{
	// GDI+ 초기화
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// CImage를 GDI+ Bitmap으로 변환
	Gdiplus::Bitmap bitmap((HBITMAP)image, NULL);

	// 저장할 폴더 경로
	CString folderPath;
	SHGetFolderPath(NULL, CSIDL_MYPICTURES, NULL, 0, folderPath.GetBuffer(MAX_PATH));
	folderPath.ReleaseBuffer();

	// 저장할 파일 경로
	CString filePath = folderPath + _T("\\picture\\") + filename;

	// 폴더가 없는 경우 폴더를 생성합니다.
	SHCreateDirectoryEx(NULL, folderPath + _T("\\picture"), NULL);

	// PNG로 저장
	bitmap.Save(filePath, &Gdiplus::ImageFormatPNG);

	// GDI+ 종료
	Gdiplus::GdiplusShutdown(gdiplusToken);
}


void CChildView::UpdateData(string filename, Data newData, int monthData, int dateData)
{
	ifstream infile(filename);
	ofstream outfile("temp.txt");

	string line;
	bool foundDate = false;
	while (getline(infile, line))
	{
		stringstream ss(line);
		string token;

		getline(ss, token, ' ');
		int month = stoi(token);

		getline(ss, token, ' ');
		int date = stoi(token);

		if (month == monthData && date == dateData)
		{
			// 이미 존재하는 날짜인 경우 정보를 수정
			outfile << monthData << " " << dateData << " "
				<< newData.mood << " ";
			for (int i = 0; i < 4; i++)
			{
				outfile << newData.weather[i] << " ";
			}
			for (int i = 0; i < 11; i++)
			{
				outfile << newData.emotion[i] << " ";
			}
			for (int i = 0; i < 7; i++)
			{
				outfile << newData.hobby[i] << " ";
			}
			for (int i = 0; i < 7; i++)
			{
				outfile << newData.food[i] << " ";
			}
			for (int i = 0; i < 3; i++)
			{
				outfile << newData.housework[i] << " ";
			}
			outfile << endl;

			foundDate = true;
		}
		else
		{
			// 다른 날짜는 그대로 유지
			outfile << line << endl;
		}
	}

	if (!foundDate)
	{
		// 새로운 날짜인 경우 추가
		outfile << monthData << " " << dateData << " "
			<< newData.mood << " ";
		for (int i = 0; i < 4; i++)
		{
			outfile << newData.weather[i] << " ";
		}
		for (int i = 0; i < 11; i++)
		{
			outfile << newData.emotion[i] << " ";
		}
		for (int i = 0; i < 7; i++)
		{
			outfile << newData.hobby[i] << " ";
		}
		for (int i = 0; i < 7; i++)
		{
			outfile << newData.food[i] << " ";
		}
		for (int i = 0; i < 3; i++)
		{
			outfile << newData.housework[i] << " ";
		}
		outfile << endl;
	}

	infile.close();
	outfile.close();

	// 원본 파일 삭제 후 임시 파일을 원본 파일명으로 변경
	remove(filename.c_str());
	rename("temp.txt", filename.c_str());
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
		CPoint pointer;
		pointer.x = 0;
		pointer.y = 0;

		background2.draw(dc, pointer);
		me.draw(dc, month);
		left.draw(dc);
		right.draw(dc);
		de.draw(dc, month);
		fe.draw(dc, month, data);
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
		if (left.click(point, month))
			return;
		if (right.click(point, month))
			return;
		for (int i = 0; i < fe.size; i++)
			if (fe.fe[i].click(point))
			{
				date = fe.fe[i].date;
				dailyCheckMenu();
				return;
			}
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
		{
			for (int i = 0; i < 6; i++)
				if (em[mood].element[i].select)
					data[month - 1][date - 1].mood = i;
			for (int i = 0; i < 4; i++)
				data[month - 1][date - 1].weather[i] = em[weather].element[i].select;
			for (int i = 0; i < 11; i++)
				data[month - 1][date - 1].emotion[i] = em[emotion].element[i].select;
			for (int i = 0; i < 7; i++)
				data[month - 1][date - 1].hobby[i] = em[hobby].element[i].select;
			for (int i = 0; i < 7; i++)
				data[month - 1][date - 1].food[i] = em[food].element[i].select;
			for (int i = 0; i < 3; i++)
				data[month - 1][date - 1].housework[i] = em[housework].element[i].select;
			UpdateData("data.txt", data[month - 1][date - 1], month, date);
			calendarMenu();
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RedrawWindow();
	CWnd::OnLButtonUp(nFlags, point);
}
