#include <windows.h> //필수
#include <time.h>
#include <string>
#include "resource.h"


#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam );

//애니메이션 커서 함수
HCURSOR LoadAnimatedCursor(HINSTANCE hInstance, UINT nID, LPCTSTR pszResouceType)
{
	HCURSOR hCursor = NULL; //커서를 다루는 핸들

	if (hInstance) //예외처리
	{
		HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(nID), pszResouceType); //리소스 핸들
		DWORD dwResourceSize = SizeofResource(hInstance, hResource); //리소스 크기
		if (dwResourceSize > 0) //예외처리
		{
			HGLOBAL hRsrcGlobal = LoadResource(hInstance, hResource); //리소스를 로드하는 함수
			if (hRsrcGlobal) //예외처리
			{
				LPBYTE pResource = (LPBYTE)LockResource(hRsrcGlobal); //로드리소스로 불러온 리소스가 저장된 메모리의 포인터를 반환
				if (pResource) //예외처리
				{
					//커서를 만듬
					hCursor = (HCURSOR)CreateIconFromResource(pResource, dwResourceSize, FALSE, 0x00030000);
					UnlockResource(pResource);
				}
				FreeResource(hRsrcGlobal); //LockRecource -> FreeResource
			}
		}
	}
	return hCursor; //커서핸들 반환
}

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
	wcex.hIcon	= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //어플리케이션
	wcex.hIconSm= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //아이콘스몰
	//wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //기본커서 
	wcex.hCursor = LoadAnimatedCursor(hInstance, IDR_MYTYPE2, L"MyType"); //애니메이션 커서

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
	//GetMessage(&msg, NULL, 0, 0)
	while(true) //메세지 획득
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
		
			HDC imgDC = CreateCompatibleDC(hdc);
			HBITMAP hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP1));
			HBITMAP oldBitmap = (HBITMAP)SelectObject(imgDC, hBitmap);
		
			BitBlt(hdc, 0, 0, 800, 600, imgDC, 0, 0, SRCCOPY);
		
			SelectObject(imgDC, oldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(imgDC);
		
			DeleteDC(hdc);
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

	switch (message)
	{
	case WM_CREATE: //window message 초기화하는부분
	{
		srand((unsigned)time(NULL));
		break;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
		break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		//hdc = GetDC(hWnd);

		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

// 		wchar_t str[255];
// 
// 		wsprintf(str, L"X : %d", mouseX);
// 		TextOut(hdc, 20, 15, str, wcslen(str));
// 
// 		wsprintf(str, L"Y : %d", mouseY);
// 		TextOut(hdc, 20, 35, str, wcslen(str));
// 
// 		//OutputDebugString(L"=================MOVE===================\n");
// 		
// 		Rectangle(hdc, mouseX - 50, mouseY - 50, mouseX + 50, mouseY + 50);
// 
// 		DeleteDC(hdc);
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
		ReleaseDC(hWnd, hdc);
	}
		break;

	// HW - 3
	// 클릭한 지점을 기준으로 가로 / 세로 100인 사각형 출력
	case WM_RBUTTONDOWN:
	{
		hdc = GetDC(hWnd);
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		Rectangle(hdc, mouseX-50, mouseY-50, mouseX+50, mouseY+50);
		ReleaseDC(hWnd, hdc);
	}
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) //id 구분 LOWORD : 그냥 번역해주는거
			{
			case ID_FILE_OPEN:
			{
				hdc = GetDC(hWnd);
				HDC bmpdc = CreateCompatibleDC(hdc);
				
				//이미지 로드
				HBITMAP hLoadbitmap = (HBITMAP)LoadImage(NULL, L"saveimg.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
				//HBITMAP hLoadbitmap = LoadBitmap((HINSTANCE)GetWindowLong );

				//윈도의 크기를 받아옴
				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);

				SelectObject(bmpdc, hLoadbitmap);

				//이미지를 dc로 복사
				BitBlt(hdc, 0, 0, windowRect.right, windowRect.bottom, bmpdc, 0, 0, SRCCOPY);

				DeleteDC(bmpdc);
				DeleteObject(hLoadbitmap);
								
				ReleaseDC(hWnd, hdc);
			}
				break;

			case ID_FILE_SAVE:
			{
				hdc = GetDC(hWnd);
				HDC hMemDC = CreateCompatibleDC(hdc);
				HBITMAP hbmp = NULL;
				BITMAP bmp;
				
				//윈도의 크기를 받아옴
				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);

				//hdc로부터 비트맵을 생성
				hbmp = CreateCompatibleBitmap(hdc, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
				//hbmp = CreateCompatibleBitmap(hdc, windowRect.right, windowRect.bottom);

				SelectObject(hMemDC, hbmp);
				
				//dc 간의 고속 복사
				BitBlt(hMemDC, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,	hdc, 0, 0, SRCCOPY);
				//BitBlt(hMemDC, 0, 0, windowRect.right, windowRect.bottom, hdc, 0, 0, SRCCOPY);

				// Get the BITMAP from the HBITMAP
				GetObject(hbmp, sizeof(BITMAP), &bmp);

				//비트맵 파일 생성 준비
				BITMAPFILEHEADER   bmfHeader;
				BITMAPINFOHEADER   bi;

				bi.biSize = sizeof(BITMAPINFOHEADER);
				bi.biWidth = bmp.bmWidth;
				bi.biHeight = bmp.bmHeight;
				bi.biPlanes = 1;
				bi.biBitCount = 32;
				bi.biCompression = BI_RGB;
				bi.biSizeImage = 0;
				bi.biXPelsPerMeter = 0;
				bi.biYPelsPerMeter = 0;
				bi.biClrUsed = 0;
				bi.biClrImportant = 0;

				DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;

				HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
				char *lpbitmap = (char *)GlobalLock(hDIB);

				GetDIBits(hdc, hbmp, 0,	(UINT)bmp.bmHeight,	lpbitmap,
					(BITMAPINFO *)&bi, DIB_RGB_COLORS);

				//CreateFile
				HANDLE hFile = CreateFile(L"saveimg.bmp",
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, NULL);

				DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

				bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

				bmfHeader.bfSize = dwSizeofDIB;

				bmfHeader.bfType = 0x4D42;

				DWORD dwBytesWritten = 0;
				WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
				WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
				WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

				GlobalUnlock(hDIB);
				GlobalFree(hDIB);

				CloseHandle(hFile);

				DeleteObject(hbmp);
				DeleteObject(hMemDC);

				ReleaseDC(hWnd, hdc);
			}
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