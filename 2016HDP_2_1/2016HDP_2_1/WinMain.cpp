#include <windows.h> //필수
#include "MyTime.h"
#include "MyInput.h"
#include <atlimage.h>


#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

CImage g_bitmap[3];
CImage g_Airplane;
CImage g_lightning;
WCHAR g_szPath[256];

CMyTime mytime;
CMyInput myinput;

double sky1 = 0, sky2 = 0, sky3 = 0;

int mouseX = 0;
int mouseY = 0;
float x = 320, y = 240;
int iAirplane = 0;

float moveSpeed = 200.0f;

LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam);

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
			myinput.KeyboardProcess();

			mytime.ProcessTime();
			float deltatime = mytime.GetElapsedTime();

			HDC		hdc			= GetDC(hWnd);
			HDC		memoryDC = CreateCompatibleDC(hdc);
			HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, 640, 480);
			HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

			// 640 , 480
			// img Moving
			g_bitmap[0].BitBlt(memoryDC, 640 - sky1, 0, 640, 124, 0, 0, SRCCOPY); //124
			g_bitmap[0].BitBlt(memoryDC, 0, 0, 640 - sky1, 124, sky1, 0, SRCCOPY);

			g_bitmap[1].BitBlt(memoryDC, 640 - sky2, 124, 640, 338, 0, 0, SRCCOPY);//214
			g_bitmap[1].BitBlt(memoryDC, 0, 124, 640 - sky2, 338, sky2, 0, SRCCOPY);

			g_bitmap[2].BitBlt(memoryDC, 640 - sky3, 338, 640, 480, 0, 0, SRCCOPY); //142
			g_bitmap[2].BitBlt(memoryDC, 0, 338, 640 - sky3, 480, sky3, 0, SRCCOPY);
			
			g_lightning.TransparentBlt(memoryDC, 0, 0, 129, 209, 0, 0, 129, 209, RGB(255, 0, 255));

			sky1 = sky1 + deltatime * 10;
			if (sky1 >= 640)
			{
				sky1 = 0;
			}
			sky2 = sky2 + deltatime * 30;
			if (sky2 >= 640)
			{
				sky2 = 0;
			}
			sky3 = sky3 + deltatime * 40;
			if (sky3 >= 640)
			{
				sky3 = 0;
			}
			
			if (myinput.GetKey(VK_A))
			{
				x = x - deltatime * moveSpeed;
			}
			if (myinput.GetKey(VK_W))
			{
				y = y - deltatime * moveSpeed;
			}
			if (myinput.GetKey(VK_D))
			{
				x = x + deltatime * moveSpeed;
			}
		    if (myinput.GetKey(VK_S))
			{
				y = y + deltatime * moveSpeed;
			}

			if (iAirplane < 5)
			{
				g_Airplane.BitBlt(memoryDC, x, y, 50, 15, 0, 0, SRCAND);
				g_Airplane.BitBlt(memoryDC, x, y, 50, 15, 0, 15, SRCPAINT);
				iAirplane++;
			}
			else if (5 <= iAirplane < 10)
			{
				g_Airplane.BitBlt(memoryDC, x, y, 50, 15, 50, 0, SRCAND);
				g_Airplane.BitBlt(memoryDC, x, y, 50, 15, 50, 15, SRCPAINT);
				iAirplane++;
				if (iAirplane >= 10)
				{
					iAirplane = 0;
				}
			}
			

			// memoryDC에 복사된 내용을 화면에 복사
			BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);

			SelectObject(memoryDC, oldBitmap); 
			// memoryDC와 관련된 내용 
			DeleteObject(memoryBitmap);
			DeleteObject(oldBitmap);
			DeleteDC(memoryDC);

			ReleaseDC(hWnd, hdc);
		}
	}

	g_bitmap[0].Destroy();
	g_bitmap[1].Destroy();
	g_bitmap[2].Destroy();
	g_Airplane.Destroy();
	g_lightning.Destroy();

	return (int)msg.wParam;
}

// 메시지 처리 함수

// 윈도우 프로시저
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch(message)
	{
	case WM_CREATE: //window 
	{
		mytime.Init();
		mytime.ProcessTime();

		GetCurrentDirectory(sizeof(g_szPath), g_szPath);

		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		WCHAR temp1[256];
		wcscpy_s(temp1, g_szPath);
		wcscat_s(temp1, L"/image/sky1.bmp");
		g_bitmap[0].Load(temp1);

		WCHAR temp2[256];
		wcscpy_s(temp2, g_szPath);
		wcscat_s(temp2, L"/image/sky2.bmp");
		g_bitmap[1].Load(temp2);

		WCHAR temp3[256];
		wcscpy_s(temp3, g_szPath);
		wcscat_s(temp3, L"/image/sky3.bmp");
		g_bitmap[2].Load(temp3);

		WCHAR temp[256];
		wcscpy_s(temp, g_szPath);
		wcscat_s(temp, L"/image/airplane.bmp");
		g_Airplane.Load(temp);

		WCHAR temp_l[256];
		wcscpy_s(temp, g_szPath);
		wcscat_s(temp, L"/image/45601.bmp");
		g_lightning.Load(temp); // 255 0 255

	}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint( hWnd, &ps );
		g_bitmap[0].BitBlt(hdc, 0, 0, SRCCOPY); // 0~123
		g_bitmap[1].BitBlt(hdc, 0, 124, SRCCOPY); //124~338
		g_bitmap[2].BitBlt(hdc, 0, 338, SRCCOPY); //
		EndPaint( hWnd, &ps );
	}
		break;
	case WM_MOUSEMOVE:
	{
// 		mytime.ProcessTime();
// 		float deltatime = mytime.GetElapsedTime();
// 		
// 		HDC hdc = GetDC(hWnd);
// 		HDC	  memoryDC = CreateCompatibleDC(hdc);
// 		HBITMAP  memoryBitmap = CreateCompatibleBitmap(hdc, 640, 480);
// 		HBITMAP  oldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);
// 
// 		g_bitmap[0].BitBlt(memoryDC, 640 - sky1, 0, 640, 124, 0, 0, SRCCOPY); //124
// 		g_bitmap[0].BitBlt(memoryDC, 0, 0, 640 - sky1, 124, sky1, 0, SRCCOPY);
// 
// 		g_bitmap[1].BitBlt(memoryDC, 640 - sky2, 124, 640, 338, 0, 0, SRCCOPY);//214
// 		g_bitmap[1].BitBlt(memoryDC,	0, 124, 640 - sky2, 338, sky2, 0, SRCCOPY);
// 
// 		g_bitmap[2].BitBlt(memoryDC, 640 - sky3, 338, 640, 480, 0, 0, SRCCOPY); //142
// 		g_bitmap[2].BitBlt(memoryDC, 0, 338, 640 - sky3, 480, sky3, 0, SRCCOPY);
// 
// 		sky1 = sky1 + deltatime * 10;
// 		if (sky1 >= 640)
// 		{
// 			sky1 = 0;
// 		}
// 		sky2 = sky2 + deltatime * 30;
// 		if (sky2 >= 640)
// 		{
// 			sky2 = 0;
// 		}
// 		sky3 = sky3 + deltatime * 40;
// 		if (sky3 >= 640)
// 		{
// 			sky3 = 0;
// 		}
// 
// 		g_lightning.TransparentBlt(memoryDC, 0, 0, 129, 209, 0, 0, 129, 209, RGB(255, 0, 255));
// 
// 		mouseX = LOWORD(lParam);
// 		mouseY = HIWORD(lParam);
// 
// 		if (iAirplane < 5)
// 		{
// 			g_Airplane.BitBlt(memoryDC, mouseX - 25, mouseY - 7, 50, 15, 0, 0, SRCAND);
// 			g_Airplane.BitBlt(memoryDC, mouseX - 25, mouseY - 7, 50, 15, 0, 15, SRCPAINT);
// 			iAirplane++;
// 		}
// 		else if (5 <= iAirplane < 10)
// 		{
// 			g_Airplane.BitBlt(memoryDC, mouseX - 25, mouseY - 7, 50, 15, 50, 0, SRCAND);
// 			g_Airplane.BitBlt(memoryDC, mouseX - 25, mouseY - 7, 50, 15, 50, 15, SRCPAINT);
// 			iAirplane++;
// 			if (iAirplane >= 10)
// 			{
// 				iAirplane = 0;
// 			}
// 		}
// 
// 		BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);
// 
// 		SelectObject(memoryDC, oldBitmap);
// 		DeleteObject(memoryBitmap);
// 		DeleteDC(memoryDC);
// 
// 		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

