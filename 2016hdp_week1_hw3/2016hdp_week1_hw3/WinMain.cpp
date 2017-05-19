#include <windows.h> //필수
#include <time.h>

#include "resource.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam );

int APIENTRY WinMain( HINSTANCE hInstance, // H : (handle) OS가 컨트롤하기 위한 핸들 32or64 bit 정수
					  HINSTANCE hPrevInstance, // 이전 인스턴스 신경쓸 필요 x
					  LPSTR lpszCmdParam, // 롱포인터스트링 커맨드
					  int nCmdShow) // 
					  //메인함수
					  //대문자로 써져있음 -> 메크로

{
	WNDCLASSEX wcex; //WindowClassExtension

	wcex.cbSize		= sizeof(WNDCLASSEX);
	wcex.style		= CS_HREDRAW | CS_VREDRAW; //class style Horizental Vertical
	wcex.lpfnWndProc	= (WNDPROC)WndProc; //관례적으로 통일해서 감
	wcex.cbClsExtra		= 0; // 안씀
	wcex.cbWndExtra		= 0; // 안씀
	wcex.hInstance		= hInstance;
	wcex.hIcon	= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //어플리케이션 디폴트 값
	wcex.hIconSm= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //아이콘스몰
	wcex.hCursor= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1)); //커서 
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); //색
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1); //쓸꺼면 셋팅
	wcex.lpszClassName	= szWindowClass; 

	if( !RegisterClassEx(&wcex) ) //등록됬는지 검사. 안전장치
		return 0;

	//윈도우 생성
	HWND	hWnd = CreateWindowEx( WS_EX_APPWINDOW //클래스이름
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
		, NULL ); // 생성파라마터, 여분의 데이터

	if( !hWnd )
		return 0;

	WCHAR appTitle[256];
	LoadString(hInstance, IDS_TITLE, appTitle, 256);
	SetWindowText(hWnd, appTitle);

	ShowWindow(hWnd, nCmdShow); //윈도우를 보여주는 함수

	//이벤트로 받아서 전송
	MSG			msg;
	while( GetMessage(&msg, NULL, 0, 0) ) //메세지 획득
	{
		TranslateMessage(&msg); //이벤트 번역
		DispatchMessage(&msg); //결과 발송
	}

	return (int)msg.wParam;
}

// 메시지 처리 함수

// 윈도우 프로시저
LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam )
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE: //window message 초기화하는부분
		srand((unsigned)time(NULL));
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

	case WM_MOUSEMOVE:
	{
		//POINT mousepoint;
		//GetCursorPos(mousepoint);

		hdc = GetDC(hWnd);

		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		wchar_t str[255];

		wsprintf(str, L"X : %d", mouseX);
		TextOut(hdc, 20, 15, str, wcslen(str));

		wsprintf(str, L"Y : %d", mouseY);
		TextOut(hdc, 20, 35, str, wcslen(str));

		//OutputDebugString(L"=================MOVE===================\n");
		
		DeleteDC(hdc);
	}
		break;

	case WM_LBUTTONDOWN:
	{
		hdc = GetDC(hWnd);

		int shape = rand() % 2; // rect or ellipse
		int brush = rand() % 2; // solid or hatch

		int x1 = rand() % 800;
		int y1 = rand() % 600;

		int x2 = rand() % 800;
		int y2 = rand() % 600;

		struct rgb
		{
			int red;
			int green;
			int blue;
		};

		rgb color;

		color.red = rand() % 256;
		color.green = rand() % 256;
		color.blue = rand() % 256;

		HBRUSH myBrush;
		HBRUSH oldBrush;

		if (brush == 0)
		{
			myBrush = CreateSolidBrush(RGB(color.red, color.green, color.blue));
			oldBrush = (HBRUSH)SelectObject(hdc, myBrush); //옛날 브러쉬를 반환, 인자로 받은걸 브러쉬로 만듬
		}
		else
		{
			myBrush = CreateHatchBrush(HS_FDIAGONAL, RGB(color.red, color.green, color.blue));
			oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
		}

		if (brush == 0)
		{
			Rectangle(hdc, x1, y1, x2, y2);
		}
		else
		{
			Ellipse(hdc, x1, y1, x2, y2);
		}

		DeleteObject(myBrush);
		DeleteDC(hdc);

	}
		break;

	case WM_RBUTTONDOWN:
		OutputDebugString(L"=================오른쪽버튼이클릭되었습니다===================\n");
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) //id 구분 LOWORD : 그냥 번역해주는거
			{
			case ID_FILE_OPEN:

				break;
			case ID_FILE_SAVE:
			
				m_myImage.Save(_T("c:\\temp\\image2"), Gdiplus::ImageFormatBMP);
				break;
			case ID_FILE_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				break;
			}
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}