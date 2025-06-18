#include "Stage3Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

Stage3Scene::Stage3Scene(SceneController& controller):
	SceneBase(controller)
{
}

Stage3Scene::~Stage3Scene()
{
}

void Stage3Scene::Init()
{
	//�Ȃ�
}

void Stage3Scene::Update(Input& input)
{
#if _DEBUG
	//�f�o�b�O�V�[��
	if (input.IsTrigger("SceneChange"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTrigger("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
}

void Stage3Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage3 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
#endif
}

void Stage3Scene::End()
{
}
