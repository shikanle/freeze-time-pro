#include "edsdk.h"

#include <iostream>


EDSDK::Camera *MainCamera;


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
CHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
CHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

												// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
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
		// 分析菜单选择: 
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
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
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

	// TODO: 在此放置代码。

	// 初始化全局字符串
	MyRegisterClass();

	// 执行应用程序初始化: 
	if (!InitInstance(NULL, SW_SHOW))
	{
		return;
	}

	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// 主消息循环: 
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