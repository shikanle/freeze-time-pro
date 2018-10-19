// FreezeTimePro.cpp : 定义应用程序的入口点。
//

#include "form.h"
#include "server_form.h"
#include "client_form.h"

using namespace FreezeTimePro;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow)
{
	SetProcessDPIAware();
	CommandLine.Parse(__argc, __argv);
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	auto mode = CommandLine.GetMode();
	if (mode.length() == 0) {
		if (MessageBox(NULL, 
			"请选择 FreezeTime Pro 的运行模式：\n选择 “是” 启动服务器模式，选择 “否” 启动客户端模式", 
			"FreezeTime 选择运行模式", MB_ICONQUESTION | MB_YESNO) == IDYES) {
			mode = "server";
		} else {
			mode = "client";
		}
	}
	if (mode == "server") {
		ServerForm().ShowModal();
	} else {
		ClientForm().ShowModal();
	}

}
