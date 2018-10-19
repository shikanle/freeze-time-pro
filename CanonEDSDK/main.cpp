#include "edsdk.h"

#include <iostream>


EDSDK::Camera *MainCamera;


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
CHAR szTitle[MAX_LOADSTRING];                  // �������ı�
CHAR szWindowClass[MAX_LOADSTRING];            // ����������

												// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = NULL; // hInstance;
	wcex.hIcon = NULL; // LoadIcon(hInstance, );
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
int  timer_x = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_F1:
			MainCamera->OpenSession();
			MainCamera->SetSaveTo();
			{
				MainCamera->SetWhiteBalance(EDSDK::WhiteBalance::Daylight);
			}
			break;
		case VK_F5:
			MainCamera->PressShutterButton(EDSDK::ShutterButton::Completely_NonAF);
			MainCamera->PressShutterButton(EDSDK::ShutterButton::Off);
			//MainCamera->TakePicture();
			break;
		case VK_F4:
			MainCamera->PressShutterButton(EDSDK::ShutterButton::Halfway);
			MainCamera->PressShutterButton(EDSDK::ShutterButton::Off);
			break;
		case VK_F9:
			timer_x = 0;
			SetTimer(hWnd, 0, 1000, NULL);
			break;
		}
	}
	case WM_TIMER:
	{
		timer_x++;
		char buf[100];
		_itoa_s(timer_x, buf, 54, 10);
		SetWindowText(hWnd, buf);
		if (timer_x % 10 == 0) {
			MainCamera->PressShutterButton(EDSDK::ShutterButton::Completely_NonAF);
			MainCamera->PressShutterButton(EDSDK::ShutterButton::Off);

			//MainCamera->Async(&(EDSDK::Camera::PressShutterWithoutAF));
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void main() {

	for (auto c : EDSDK::GetCameraList()) {
		std::cout << c->ToString() << std::endl;
		MainCamera = c;
	}

	strcpy_s(szTitle, 64, "AA");
	strcpy_s(szWindowClass, 64, "AA");

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	MyRegisterClass();

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(NULL, SW_SHOW))
	{
		return;
	}

	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// ����Ϣѭ��: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//return (int)msg.wParam;
	/*
	for (auto c : EDSDK::GetCameraList()) {
		std::cout << c.ToString() << std::endl;
		c.OpenSession();
		c.SetSaveTo();
		c.TakePicture();
		for (int i = 0; i < 10000; i++) {
			Sleep(1);
		}
		c.CloseSession();
	}*/

}