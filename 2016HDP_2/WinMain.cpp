#include <windows.h> //�ʼ�
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

int APIENTRY WinMain(HINSTANCE hInstance, // H : (handle) OS�� ��Ʈ���ϱ� ���� �ڵ� 32or64 bit ����
	HINSTANCE hPrevInstance, // ���� �ν��Ͻ� �Ű澵 �ʿ� x
	LPSTR lpszCmdParam, // �������ͽ�Ʈ�� Ŀ�ǵ�
	int nCmdShow) // 
	//�����Լ�
	//�빮�ڷ� �������� -> ��ũ��

{
	WNDCLASSEX wcex; //WindowClassExtension

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; //class style Horizental Vertical
	wcex.lpfnWndProc = (WNDPROC)WndProc; //���������� �����ؼ� ��
	wcex.cbClsExtra = 0; // �Ⱦ�
	wcex.cbWndExtra = 0; // �Ⱦ�
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); //���ø����̼� ����Ʈ ��
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION); //�����ܽ���
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� 
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //��
	wcex.lpszMenuName = NULL; //������ ����
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex)) //��ω���� �˻�. ������ġ
		return 0;

	//������ ����
	HWND	hWnd = CreateWindowEx(WS_EX_APPWINDOW //Ŭ�����̸�
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
		, NULL); // �����Ķ���, ������ ������

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow); //�����츦 �����ִ� �Լ�

	//�̺�Ʈ�� �޾Ƽ� ����
	MSG			msg;

	float elapsedTime = 0;

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

// �޽��� ó�� �Լ�

// ������ ���ν���
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
// 		BitBlt(hdc, 0, 0, 800, 600, memoryDC, 0, 0, SRCCOPY); // memDC�� hdc�� ����
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