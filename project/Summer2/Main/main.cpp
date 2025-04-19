#include <DxLib.h>
#include "Application.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//C++��static�����o�̌Ăяo���́@::�@�Ȃ̂Œ���
	Application& app = Application::GetInstance();
	if (!app.Init())
	{
		return -1;
	}
	app.Run();
	app.Terminate();

	return 0;				// �\�t�g�̏I�� 
}