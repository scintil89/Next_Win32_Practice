#include <windows.h> //�ʼ�
#include <time.h>

#include "resource.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")

LRESULT CALLBACK WndProc( HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam );

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
	wcex.hIcon	= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //���ø����̼� ����Ʈ ��
	wcex.hIconSm= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //�����ܽ���
	wcex.hCursor= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1)); //Ŀ�� 
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
	while( GetMessage(&msg, NULL, 0, 0) ) //�޼��� ȹ��
	{
		TranslateMessage(&msg); //�̺�Ʈ ����
		DispatchMessage(&msg); //��� �߼�
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
		DeleteDC(hdc);

	}
		break;

	case WM_RBUTTONDOWN:
		OutputDebugString(L"=================�����ʹ�ư��Ŭ���Ǿ����ϴ�===================\n");
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) //id ���� LOWORD : �׳� �������ִ°�
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