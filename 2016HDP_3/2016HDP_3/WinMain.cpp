#include <windows.h> //�ʼ�
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
			//�� ���� ���μ��� ó��
			//������ ����
			HDC hdc = GetDC(hWnd);

			HDC memoryDC = CreateCompatibleDC(hdc);
			HBITMAP memhbitmap = CreateCompatibleBitmap(hdc, 640, 480);
 			HBITMAP oldbitmap = (HBITMAP)SelectObject(memoryDC, memhbitmap);

			Rectangle(memoryDC, 0, 0, 640, 480);

			//���׸���
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

// �޽��� ó�� �Լ�

// ������ ���ν���
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