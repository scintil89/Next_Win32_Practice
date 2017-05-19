#include <windows.h> //�ʼ�
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
	wcex.hIcon	= LoadIcon(hInstance, IDI_APPLICATION); //���ø����̼� ����Ʈ ��
	wcex.hIconSm= LoadIcon(hInstance, IDI_APPLICATION); //�����ܽ���
	wcex.hCursor= LoadCursor(NULL, IDC_ARROW); //Ŀ�� 
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); //��
	wcex.lpszMenuName	= NULL; //������ ����
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
		, 640 //width
		, 480 //height
		, NULL //�θ� ������
		, NULL //�޴�
		, hInstance //�ν��Ͻ� �ڵ�
		, NULL ); // �����Ķ���, ������ ������

	if( !hWnd )
		return 0;

	ShowWindow(hWnd, nCmdShow); //�����츦 �����ִ� �Լ�

	//�̺�Ʈ�� �޾Ƽ� ����
	MSG			msg;
	while (true) //�޼��� ȹ��
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
			

			// memoryDC�� ����� ������ ȭ�鿡 ����
			BitBlt(hdc, 0, 0, 640, 480, memoryDC, 0, 0, SRCCOPY);

			SelectObject(memoryDC, oldBitmap); 
			// memoryDC�� ���õ� ���� 
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

// �޽��� ó�� �Լ�

// ������ ���ν���
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

