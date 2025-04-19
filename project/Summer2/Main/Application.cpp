#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"

Application& Application::GetInstance()
{
    //���̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
    static Application app;
    return app;
}

bool Application::Init()
{
    //�t���X�N���[���łȂ��A�E�B���h�E���[�h�ŊJ���悤�ɂ���
    //�����������֐��̓E�B���h�E���J���O��(Dxlib.Init()�̑O)�ɏ������Ă����K�v������
	ChangeWindowMode(Game::kDefaultWindowMode);
	//��ʃT�C�Y�ύX
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	//�Q�[���^�C�g��
	SetWindowText("�^�C�g��");
	//�Q�[���A�C�R��
	//SetWindowIconID(IDI_ICON1);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return false;			// �G���[���N�����璼���ɏI��
	}

	//�����ɏ����ƈ�u�t���X�N���[���ɂȂ�̂ŏ����Ȃ�
	//ChangeWindowMode(true);
	//SetGraphMode(1280, 720, 32);

	//�`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

    return true;
}

void Application::Run()
{
	//�A�v���P�[�V�����ȊO�͂����Ő錾�Ə�����
	SceneController sceneController;
	//�R���g���[���[
	Input input;
	

	//�Q�[�����[�v
	while (ProcessMessage() == 0) // Windows���s��������҂�
	{
		//����̃��[�v���n�܂������Ԃ��o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		//��ʑS�̂��N���A
		ClearDrawScreen();

		////�����ɃQ�[���̏���������
		input.Update();
		sceneController.Update(input);
		sceneController.Draw();

		//��ʂ̐؂�ւ���҂K�v������
		ScreenFlip();//1/60�b�o�߂���܂ő҂�

		//FPS��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}
	}
}

void Application::Terminate()
{
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

