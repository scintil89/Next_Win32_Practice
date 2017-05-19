#include <windows.h> //�ʼ�
#include <time.h>
#include <string>
#include "resource.h"


#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam );

//�ִϸ��̼� Ŀ�� �Լ�
HCURSOR LoadAnimatedCursor(HINSTANCE hInstance, UINT nID, LPCTSTR pszResouceType)
{
	HCURSOR hCursor = NULL; //Ŀ���� �ٷ�� �ڵ�

	if (hInstance) //����ó��
	{
		HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(nID), pszResouceType); //���ҽ� �ڵ�
		DWORD dwResourceSize = SizeofResource(hInstance, hResource); //���ҽ� ũ��
		if (dwResourceSize > 0) //����ó��
		{
			HGLOBAL hRsrcGlobal = LoadResource(hInstance, hResource); //���ҽ��� �ε��ϴ� �Լ�
			if (hRsrcGlobal) //����ó��
			{
				LPBYTE pResource = (LPBYTE)LockResource(hRsrcGlobal); //�ε帮�ҽ��� �ҷ��� ���ҽ��� ����� �޸��� �����͸� ��ȯ
				if (pResource) //����ó��
				{
					//Ŀ���� ����
					hCursor = (HCURSOR)CreateIconFromResource(pResource, dwResourceSize, FALSE, 0x00030000);
					UnlockResource(pResource);
				}
				FreeResource(hRsrcGlobal); //LockRecource -> FreeResource
			}
		}
	}
	return hCursor; //Ŀ���ڵ� ��ȯ
}

int APIENTRY WinMain( HINSTANCE hInstance, // H : (handle) OS�� ��Ʈ���ϱ� ���� �ڵ� 32or64 bit ����
					  HINSTANCE hPrevInstance, // ���� �ν��Ͻ� �Ű澵 �ʿ� x
					  LPSTR lpszCmdParam, // �������ͽ�Ʈ�� Ŀ�ǵ�
					  int nCmdShow) // 
					  //�����Լ�
					  //�빮�ڷ� �������� -> ��ũ��

{
	WNDCLASSEX wcex; //WindowClassExtension

	wcex.cbSize		= sizeof(WNDCLASSEX);
	wcex.style		= CS_HREDRAW | CS_VREDRAW; //class style Horizental Vertical
	wcex.lpfnWndProc	= (WNDPROC)WndProc; //���������� �����ؼ� ��
	wcex.cbClsExtra		= 0; // �Ⱦ�
	wcex.cbWndExtra		= 0; // �Ⱦ�
	wcex.hInstance		= hInstance;
	wcex.hIcon	= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //���ø����̼�
	wcex.hIconSm= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //�����ܽ���
	//wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //�⺻Ŀ�� 
	wcex.hCursor = LoadAnimatedCursor(hInstance, IDR_MYTYPE2, L"MyType"); //�ִϸ��̼� Ŀ��

	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); //��
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1); //������ ����
	wcex.lpszClassName	= szWindowClass; 

	if( !RegisterClassEx(&wcex) ) //��ω���� �˻�. ������ġ
		return 0;

	//������ ����
	HWND	hWnd = CreateWindowEx( WS_EX_APPWINDOW //Ŭ�����̸�
		, szWindowClass //�������̸�
		, szTitle //ĸ�� �� ���ڿ�
		, WS_OVERLAPPEDWINDOW //������ ��Ÿ��
		, CW_USEDEFAULT //�������� ��ǥ
		, CW_USEDEFAULT //�������� ��ǥ
		, 800 //width
		, 600 //height
		, NULL //�θ� ������
		, NULL //�޴�
		, hInstance //�ν��Ͻ� �ڵ�
		, NULL ); // �����Ķ���, ������ ������

	if( !hWnd )
		return 0;

	WCHAR appTitle[256];
	LoadString(hInstance, IDS_TITLE, appTitle, 256);
	SetWindowText(hWnd, appTitle);

	ShowWindow(hWnd, nCmdShow); //�����츦 �����ִ� �Լ�

	//�̺�Ʈ�� �޾Ƽ� ����
	MSG			msg;
	//GetMessage(&msg, NULL, 0, 0)
	while(true) //�޼��� ȹ��
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg); //�̺�Ʈ ����
			DispatchMessage(&msg); //��� �߼�
		}
		else
		{
			//�� ���� ���μ��� ó��
			//������ ����
			
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

// �޽��� ó�� �Լ�

// ������ ���ν���
LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam )
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE: //window message �ʱ�ȭ�ϴºκ�
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
			oldBrush = (HBRUSH)SelectObject(hdc, myBrush); //���� �귯���� ��ȯ, ���ڷ� ������ �귯���� ����
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
	// Ŭ���� ������ �������� ���� / ���� 100�� �簢�� ���
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
			switch (LOWORD(wParam)) //id ���� LOWORD : �׳� �������ִ°�
			{
			case ID_FILE_OPEN:
			{
				hdc = GetDC(hWnd);
				HDC bmpdc = CreateCompatibleDC(hdc);
				
				//�̹��� �ε�
				HBITMAP hLoadbitmap = (HBITMAP)LoadImage(NULL, L"saveimg.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
				//HBITMAP hLoadbitmap = LoadBitmap((HINSTANCE)GetWindowLong );

				//������ ũ�⸦ �޾ƿ�
				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);

				SelectObject(bmpdc, hLoadbitmap);

				//�̹����� dc�� ����
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
				
				//������ ũ�⸦ �޾ƿ�
				RECT windowRect;
				GetWindowRect(hWnd, &windowRect);

				//hdc�κ��� ��Ʈ���� ����
				hbmp = CreateCompatibleBitmap(hdc, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
				//hbmp = CreateCompatibleBitmap(hdc, windowRect.right, windowRect.bottom);

				SelectObject(hMemDC, hbmp);
				
				//dc ���� ��� ����
				BitBlt(hMemDC, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,	hdc, 0, 0, SRCCOPY);
				//BitBlt(hMemDC, 0, 0, windowRect.right, windowRect.bottom, hdc, 0, 0, SRCCOPY);

				// Get the BITMAP from the HBITMAP
				GetObject(hbmp, sizeof(BITMAP), &bmp);

				//��Ʈ�� ���� ���� �غ�
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