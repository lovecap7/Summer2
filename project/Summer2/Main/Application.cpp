#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"
// EffekseerForDXLib.h���C���N���[�h���܂��B
#include <EffekseerForDXLib.h>

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

	//�����ɏ����ƈ�u�t���X�N���[���ɂȂ�̂ŏ����Ȃ�
	//ChangeWindowMode(true);
	//SetGraphMode(1280, 720, 32);


	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return false;			// �G���[���N�����璼���ɏI��
	}

	//�`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//Effekseer�֘A

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	const int particleMax = 20000;
	if (Effekseer_Init(particleMax) == -1) { DxLib_End(); }

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//3D�`��̂��߂̏���
	//Z�o�b�t�@���g�p����
	SetUseZBuffer3D(true);
	//Z�o�b�t�@�ɏ������݂��s��
	SetWriteZBuffer3D(true);
	//�o�b�N�J�����O���s��(�|���S���̗��ʂ�`�悵�Ȃ��悤�ɂ���)
	SetUseBackCulling(true);

    return true;
}

void Application::Run()
{
	//�A�v���P�[�V�����ȊO�͂����Ő錾�Ə�����
	SceneController* sceneController = new SceneController();
	//�R���g���[���[
	Input* input = new Input(DX_INPUT_PAD1);
	Input* input2 = new Input(DX_INPUT_PAD2);

	//�Q�[�����[�v
	while (ProcessMessage() == 0) // Windows���s��������҂�
	{
		//����̃��[�v���n�܂������Ԃ��o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		//��ʑS�̂��N���A
		ClearDrawScreen();

		//�����ɃQ�[���̏���������
		input->Update();
		input2->Update();
		sceneController->Update(*input);
		sceneController->Draw();

		//��ʂ̐؂�ւ���҂K�v������
		ScreenFlip();//1/60�b�o�߂���܂ő҂�

		//FPS��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}

		//ESC�L�[�ŏI��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			//����
			delete input;
			input = nullptr;
			delete input2;
			input2 = nullptr;
			delete sceneController;
			sceneController = nullptr;
			break;
		}
	}
}

void Application::Terminate()
{
	// Effekseer���I������B
	Effkseer_End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

