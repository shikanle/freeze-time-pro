// FreezeTimePro.cpp : ����Ӧ�ó������ڵ㡣
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
			"��ѡ�� FreezeTime Pro ������ģʽ��\nѡ�� ���ǡ� ����������ģʽ��ѡ�� ���� �����ͻ���ģʽ", 
			"FreezeTime ѡ������ģʽ", MB_ICONQUESTION | MB_YESNO) == IDYES) {
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
