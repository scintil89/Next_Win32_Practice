#include <windows.h> //필수
#include <atlimage.h>
#include "resource.h"
#include "MyTime.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

CImage g_bitmap;
WCHAR g_szPath[256];

CMyTime* mytime = new CMyTime();

static int x = 0;

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
		, 800 //width
		, 600 //height
		, NULL //부모 윈도우
		, NULL //메뉴
		, hInstance //인스턴스 핸들
		, NULL); // 생성파라마터, 여분의 데이터

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow); //윈도우를 보여주는 함수

	//이벤트로 받아서 전송
	MSG			msg;

	float elapsedTime = 0;

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
			mytime->ProcessTime();
			elapsedTime = mytime->GetElapsedTime();

			HDC		hdc = GetDC(hWnd);
			HDC		memoryDC = CreateCompatibleDC(hdc);
			HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, 800, 600);
			HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

			g_bitmap.BitBlt(memoryDC, 800 - x, 0, 800, 600, 0, 0, SRCCOPY);
			g_bitmap.BitBlt(memoryDC, 0, 0, 800 - x, 600, x, 0, SRCCOPY);

			// img Moving
			x = x + 800 * elapsedTime;
			if (x>=800) 
			{
				x = 0;
			}

			// memDC -> hDC
			// Double Buffering
			BitBlt(hdc, 0, 0, 800, 600, memoryDC, 0, 0, SRCCOPY);

			DeleteDC(memoryDC);

			ReleaseDC(hWnd, hdc);
		}
	}

	g_bitmap.Destroy();

	delete mytime;
	mytime = nullptr;

	return (int)msg.wParam;
}

// 메시지 처리 함수

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE: //window 
	{
		GetCurrentDirectory(sizeof(g_szPath), g_szPath);

		WCHAR temp[256];
		wcscpy_s(temp, g_szPath);
		wcscat_s(temp, L"/image/BackGround.bmp");
		g_bitmap.Load(temp);

		mytime->Init();

	}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		//g_bitmap.BitBlt(hdc, 0, 0, SRCCOPY); // 0~123
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_MOUSEMOVE:
	{
// 		hdc = GetDC(hWnd);
// 		HDC memoryDC = CreateCompatibleDC(hdc);
// 		HDC imgDC = CreateCompatibleDC(hdc);
// 
// 		HBITMAP hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1));
// 		HBITMAP oldBitmap = (HBITMAP)SelectObject(imgDC, hBitmap);	
// 
// 		BitBlt(memoryDC, 800 - x, 0, 800, 600, imgDC, 0, 0, SRCCOPY);
// 		BitBlt(memoryDC, 0, 0, 800 - x, 600, imgDC, x, 0, SRCCOPY);
// 		SelectObject(imgDC, oldBitmap);
// 
// 		x++;
// 		if (x >= 800)
// 		{
// 			x = 0;
// 		}
// 		
// 		int mouseX = LOWORD(lParam); 
// 		int mouseY = HIWORD(lParam);
// 
// 		
// 		HBRUSH mybrush = CreateSolidBrush(RGB(255, 255, 255));
// 		HBRUSH oldbrush = (HBRUSH)SelectObject(memoryDC, mybrush);
// 
// 		wchar_t str[255];
// 		
// 		wsprintf(str, L"X : %d", mouseX);
// 		TextOut(hdc, 20, 15, str, wcslen(str));
// 		 
// 		wsprintf(str, L"Y : %d", mouseY);
// 		TextOut(hdc, 20, 35, str, wcslen(str));
// 
// 		Rectangle(memoryDC, mouseX - 50, mouseY - 50, mouseX + 50, mouseY + 50);
// 
// 		SelectObject(hdc, oldbrush);
// 
// 		BitBlt(hdc, 0, 0, 800, 600, memoryDC, 0, 0, SRCCOPY); // memDC를 hdc에 복사
// 
// 		DeleteObject(hBitmap);
// 		DeleteObject(oldBitmap);
// 
// 		DeleteDC(imgDC);
// 		DeleteDC(memoryDC);
// 
// 		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
	{
	}
		break;
	case WM_RBUTTONDOWN:
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}