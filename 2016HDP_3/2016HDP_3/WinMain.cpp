#include <windows.h> //필수
#include <time.h>
#include <math.h>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

int mouseX = 0;
int mouseY = 0;
float dist = 0;

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
	wcex.hIcon	= LoadIcon(hInstance, IDI_APPLICATION); //어플리케이션 디폴트 값
	wcex.hIconSm= LoadIcon(hInstance, IDI_APPLICATION); //아이콘스몰
	wcex.hCursor= LoadCursor(NULL, IDC_ARROW); //커서 
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); //색
	wcex.lpszMenuName	= NULL; //쓸꺼면 셋팅
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
		, 640 //width
		, 480 //height
		, NULL //부모 윈도우
		, NULL //메뉴
		, hInstance //인스턴스 핸들
		, NULL ); // 생성파라마터, 여분의 데이터

	if( !hWnd )
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
			HDC hdc = GetDC(hWnd);

			HDC memoryDC = CreateCompatibleDC(hdc);
			HBITMAP memhbitmap = CreateCompatibleBitmap(hdc, 640, 480);
 			HBITMAP oldbitmap = (HBITMAP)SelectObject(memoryDC, memhbitmap);

			Rectangle(memoryDC, 0, 0, 640, 480);

			//선그리기
			MoveToEx(memoryDC, 0, 480, NULL);
			LineTo(memoryDC, 640, 0);



			//
			wchar_t str1[255];
			wsprintf(str1, L"X : %d", mouseX);
			TextOut(memoryDC, 20, 15, str1, wcslen(str1));

			wchar_t str2[255];
			wsprintf(str2, L"Y : %d", mouseY);
			TextOut(memoryDC, 20, 35, str2, wcslen(str2));

			

			Rectangle(memoryDC, mouseX - 50, mouseY - 50, mouseX + 50, mouseY + 50);

			BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);

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
LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam )
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch(message)
	{
	case WM_CREATE: //window message
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
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

		HDC hdc = GetDC(hWnd);
		HDC memoryDC = CreateCompatibleDC(hdc);
		HBITMAP memhbitmap = CreateCompatibleBitmap(hdc, 640, 480);
		HBITMAP oldbitmap = (HBITMAP)SelectObject(memoryDC, memhbitmap);

		Rectangle(memoryDC, 0, 0, 640, 480);

		MoveToEx(memoryDC, 0, 480, NULL);
		LineTo(memoryDC, 640, 0);

		//
		
		dist = abs( (6 / 5)*mouseX - (1 / 4)*mouseY - 480) / ( ((6 / 5) ^ 2 + (-1 / 4) ^ 2))^(1/2);

		wchar_t str[255];

		wsprintf(str, L"X : %d", mouseX);
		TextOut(memoryDC, 20, 15, str, wcslen(str));

		wsprintf(str, L"Y : %d", mouseY);
		TextOut(memoryDC, 20, 35, str, wcslen(str));


		
		//Rectangle(memoryDC, mouseX - 50, mouseY - 50, mouseX + 50, mouseY + 50);

		BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);

		SelectObject(memoryDC, oldbitmap);

		DeleteDC(memoryDC);
		ReleaseDC(hWnd, hdc);
		}
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}