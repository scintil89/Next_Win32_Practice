#include <windows.h> //필수
#include <time.h>
#include <math.h>
#include "MyTime.h"
#include <vector>
#include <algorithm>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define PI	3.14

int mouseX = 0;
int mouseY = 0;

float dist = 0;

wchar_t str[50] = { 0 };

CMyTime myt;

double degree = 0;
double degreesum = 0;
double cos_ = 0;
double sin_ = 0;

double rect1_x1 = 60;
double rect1_y1 = 80;
double rect1_x2 = -60;
double rect1_y2 = 80;
double rect1_x3 = -60;
double rect1_y3 = -80;
double rect1_x4 = 60;
double rect1_y4 = -80;

double rect2_x1 = 60;
double rect2_y1 = 80;
double rect2_x2 = -60;
double rect2_y2 = 80;
double rect2_x3 = -60;
double rect2_y3 = -80;
double rect2_x4 = 60;
double rect2_y4 = -80;

double getMax(double a, double b, double c, double d);
double getMin(double a, double b, double c, double d);

// double rect1_x1 = 60;
// double rect1_y1 = -80;
// 
// double rect1_x2 = -60;
// double rect1_y2 = -80;
// 
// double rect1_x3 = 60;
// double rect1_y3 = -80;
// 
// double rect1_x4 = 60;
// double rect1_y4 = 80;

LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, // H : (handle) OS가 컨트롤하기 위한 핸들 32or64 bit 정수
	HINSTANCE hPrevInstance, // 이전 인스턴스 신경쓸 필요 x
	LPSTR lpszCmdParam, // 롱포인터스트링 커맨드
	int nCmdShow) // 
	//메인함수
	//대문자로 써져있음 -> 메크로

{
	WNDCLASSEX wcex; //WindowClassExtension

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; //class style Horizental Vertical
	wcex.lpfnWndProc = (WNDPROC)WndProc; //관례적으로 통일해서 감
	wcex.cbClsExtra = 0; // 안씀
	wcex.cbWndExtra = 0; // 안씀
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); //어플리케이션 디폴트 값
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION); //아이콘스몰
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //색
	wcex.lpszMenuName = NULL; //쓸꺼면 셋팅
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex)) //등록됬는지 검사. 안전장치
		return 0;

	//윈도우 생성
	HWND	hWnd = CreateWindowEx(WS_EX_APPWINDOW //클래스이름
		, szWindowClass //윈도우이름
		, szTitle //캡션 바 문자열
		, WS_OVERLAPPEDWINDOW //윈도우 스타일
		, CW_USEDEFAULT //윈도우의 좌표
		, CW_USEDEFAULT //윈도우의 좌표
		, 640 //width
		, 480 //height
		, NULL //부모 윈도우
		, NULL //메뉴
		, hInstance //인스턴스 핸들
		, NULL); // 생성파라마터, 여분의 데이터

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow); //윈도우를 보여주는 함수

	//이벤트로 받아서 전송
	MSG			msg;
	while (true) //메세지 획득
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg); //이벤트 번역
			DispatchMessage(&msg); //결과 발송
		}
		else
		{
			//주 게임 프로세스 처리
			//프레임 영역
			myt.ProcessTime();

			HDC hdc = GetDC(hWnd);
			//memdc
			HDC memoryDC = CreateCompatibleDC(hdc);
			HBITMAP memhbitmap = CreateCompatibleBitmap(hdc, 640, 480);
			HBITMAP oldbitmap = (HBITMAP)SelectObject(memoryDC, memhbitmap);
			//brush
			HBRUSH mybrush1 = CreateSolidBrush(RGB(255, 0, 0)); //red
			HBRUSH oldbrush1 = nullptr;
			HBRUSH mybrush2 = CreateSolidBrush(RGB(255, 255, 0)); //yellow
			HBRUSH oldbrush2 = nullptr;
			HBRUSH mybrush3 = CreateSolidBrush(RGB(0, 255, 0)); //green
			HBRUSH oldbrush3 = nullptr;
			HBRUSH mybrush4 = CreateSolidBrush(RGB(0, 0, 255)); //blue
			HBRUSH oldbrush4 = nullptr;
			//memdc 색칠
			Rectangle(memoryDC, 0, 0, 640, 480);

			//delta time
			degree = myt.GetElapsedTime() * 80 * PI / 180;
			degreesum += degree;

			sin_ = sin(degree);
			cos_ = cos(degree);

			//rect1
			// x2 x1
			// x3 x4
			rect1_x1 = rect1_x1*cos_ - rect1_y1*sin_;
			rect1_y1 = rect1_x1*sin_ + rect1_y1*cos_;

			rect1_x2 = rect1_x2*cos_ - rect1_y2*sin_;
			rect1_y2 = rect1_x2*sin_ + rect1_y2*cos_;

			rect1_x3 = rect1_x3*cos_ - rect1_y3*sin_;
			rect1_y3 = rect1_x3*sin_ + rect1_y3*cos_;

			rect1_x4 = rect1_x4*cos_ - rect1_y4*sin_;
			rect1_y4 = rect1_x4*sin_ + rect1_y4*cos_;
//			//이상한 회전
// 			rect1_x1 = ((rect1_x1 - mouseX)*cos_ - (rect1_y1 - mouseY)*sin_) + mouseX;
// 			rect1_y1 = ((rect1_x1 - mouseX)*sin_ + (rect1_y1 - mouseY)*cos_) + mouseY;
// 
// 			rect1_x2 = ((rect1_x2 - mouseX)*cos_ - (rect1_y2 - mouseY)*sin_) + mouseX;
// 			rect1_y2 = ((rect1_x2 - mouseX)*sin_ + (rect1_y2 - mouseY)*cos_) + mouseY;
// 
// 			rect1_x3 = ((rect1_x3 - mouseX)*cos_ - (rect1_y3 - mouseY)*sin_) + mouseX;
// 			rect1_y3 = ((rect1_x3 - mouseX)*sin_ + (rect1_y3 - mouseY)*cos_) + mouseY;
// 
// 			rect1_x4 = ((rect1_x4 - mouseX)*cos_ - (rect1_y4 - mouseY)*sin_) + mouseX;
// 			rect1_y4 = ((rect1_x4 - mouseX)*sin_ + (rect1_y4 - mouseY)*cos_) + mouseY;
			MoveToEx(memoryDC, rect1_x1 + mouseX, rect1_y1 + mouseY, NULL);
			LineTo(memoryDC, rect1_x2 + mouseX, rect1_y2 + mouseY);
			LineTo(memoryDC, rect1_x3 + mouseX, rect1_y3 + mouseY);
			LineTo(memoryDC, rect1_x4 + mouseX, rect1_y4 + mouseY);
			LineTo(memoryDC, rect1_x1 + mouseX, rect1_y1 + mouseY);
			//test
// 			oldbrush1 = (HBRUSH)SelectObject(memoryDC, mybrush1);
// 			Rectangle(memoryDC, rect1_x2 + mouseX, rect1_y2 + mouseY, rect1_x2 + mouseX + 5, rect1_y2 + mouseY + 5);
// 			SelectObject(memoryDC, oldbrush1);
// 
// 			oldbrush2 = (HBRUSH)SelectObject(memoryDC, mybrush2);
// 			Rectangle(memoryDC, rect1_x3 + mouseX, rect1_y3 + mouseY, rect1_x3 + mouseX + 5, rect1_y3 + mouseY + 5);
// 			SelectObject(memoryDC, oldbrush2);
// 
// 			oldbrush3 = (HBRUSH)SelectObject(memoryDC, mybrush3);
// 			Rectangle(memoryDC, rect1_x4 + mouseX, rect1_y4 + mouseY, rect1_x4 + mouseX + 5, rect1_y4 + mouseY + 5);
// 			SelectObject(memoryDC, oldbrush3);
// 
// 			oldbrush4 = (HBRUSH)SelectObject(memoryDC, mybrush4);
// 			Rectangle(memoryDC, rect1_x1 + mouseX, rect1_y1 + mouseY, rect1_x1 + mouseX + 5, rect1_y1 + mouseY + 5);
// 			SelectObject(memoryDC, oldbrush4);

			//rect2
			rect2_x1 = rect1_x1 + 300;
			rect2_x2 = rect1_x2 + 300;
			rect2_x3 = rect1_x3 + 300;
			rect2_x4 = rect1_x4 + 300;
			rect2_y1 = rect1_y1 + 300;
			rect2_y2 = rect1_y2 + 300;
			rect2_y3 = rect1_y3 + 300;
			rect2_y4 = rect1_y4 + 300;

			MoveToEx(memoryDC, rect2_x1, rect2_y1, NULL);
			LineTo(memoryDC, rect2_x2, rect2_y2);
			LineTo(memoryDC, rect2_x3, rect2_y3);
			LineTo(memoryDC, rect2_x4, rect2_y4);
			LineTo(memoryDC, rect2_x1, rect2_y1);

			double transX = (cos(-degreesum) * (rect1_x3 + mouseX - 300) - sin(-degreesum) * (rect1_y3 + mouseY - 300));
			double transY = (sin(-degreesum) * (rect1_x3 + mouseX - 300) + cos(-degreesum) * (rect1_y3 + mouseY - 300));

// 			double rect1xMax = getMax(rect1_x1, rect1_x2, rect1_x3, rect1_x4) + mouseX;
// 			double rect1xMin = getMin(rect1_x1, rect1_x2, rect1_x3, rect1_x4) + mouseX;
// 			double rect1yMax = getMax(rect1_y1, rect1_y2, rect1_y3, rect1_y4) + mouseY;
// 			double rect1yMin = getMin(rect1_y1, rect1_y2, rect1_y3, rect1_y4) + mouseY;
// 
// 			double rect2xMax = getMax(rect2_x1, rect2_x2, rect2_x3, rect2_x4);
// 			double rect2xMin = getMin(rect2_x1, rect2_x2, rect2_x3, rect2_x4);
// 			double rect2yMax = getMax(rect2_y1, rect2_y2, rect2_y3, rect2_y4);
// 			double rect2yMin = getMin(rect2_y1, rect2_y2, rect2_y3, rect2_y4);
			//|| (rect2xMax > rect1xMin > rect2xMin) || (rect2yMax > rect1yMax > rect2yMin) || (rect2yMax > rect1yMin > rect2yMin)
			if ((transX > 60) || (transX + 120 < -50) || (transY > 80) || (transY + 160 < -80))
			{
				wsprintf(str, L"비충돌");
				TextOut(memoryDC, 20, 55, str, wcslen(str));
			}
			else
			{
				wsprintf(str, L"충돌");
				TextOut(memoryDC, 20, 55, str, wcslen(str));
			}

			//
			wsprintf(str, L"X : %d", mouseX);
			TextOut(memoryDC, 20, 15, str, wcslen(str));

			wsprintf(str, L"Y : %d", mouseY);
			TextOut(memoryDC, 20, 35, str, wcslen(str));
			


			BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);

			DeleteObject(mybrush1);
			DeleteObject(oldbrush1);
			DeleteObject(mybrush2);
			DeleteObject(oldbrush2);
			DeleteObject(mybrush3);
			DeleteObject(oldbrush3);
			DeleteObject(mybrush4);
			DeleteObject(oldbrush4);

			DeleteObject(oldbitmap);
			DeleteObject(memhbitmap);
			DeleteDC(memoryDC);
			ReleaseDC(hWnd, hdc);

		}
	}

	return (int)msg.wParam;
}

// 메시지 처리 함수

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE: //window message
	{
		myt.Init();
	}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_MOUSEMOVE:
	{
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

// 		HDC hdc = GetDC(hWnd);
// 		HDC memoryDC = CreateCompatibleDC(hdc);
// 		//HBITMAP memhbitmap = CreateCompatibleBitmap(hdc, 640, 480);
// 		//HBITMAP oldbitmap = (HBITMAP)SelectObject(memoryDC, memhbitmap);
// 
// 		HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
// 		HBRUSH oldbrush = (HBRUSH)SelectObject(memoryDC, hbrush);
// 		Rectangle(memoryDC, 0, 0, 640, 480);
// 		SelectObject(memoryDC, oldbrush);
// 
// 		Rectangle(memoryDC, 200, 200, 400, 400);
// 
// 		Rectangle(memoryDC, mouseX - 50, mouseY - 50, mouseX + 50, mouseY + 50);
// 
// 		wchar_t str[255];
// 
// 		wsprintf(str, L"X : %d", mouseX);
// 		TextOut(memoryDC, 20, 15, str, wcslen(str));
// 
// 		wsprintf(str, L"Y : %d", mouseY);
// 		TextOut(memoryDC, 20, 35, str, wcslen(str));
// 
// 		BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);
// 
// 		//SelectObject(memoryDC, oldbitmap);
// 
// 		DeleteDC(memoryDC);
// 		ReleaseDC(hWnd, hdc);
	}
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

double getMax(double a, double b, double c, double d)
{
	std::vector<double> v;

	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(d);

	return *std::max_element(v.begin(), v.end());
}

double getMin(double a, double b, double c, double d)
{
	std::vector<double> v;

	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(d);

	return *std::min_element(v.begin(), v.end());
}
