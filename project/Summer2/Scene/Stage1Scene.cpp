#include "Stage1Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

Stage1Scene::Stage1Scene(SceneController& controller):
	SceneBase(controller)
{
}

Stage1Scene::~Stage1Scene()
{
}

void Stage1Scene::Update(Input& input)
{
#if _DEBUG
	//�f�o�b�O�V�[��
	if (CheckHitKey(KEY_INPUT_D))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTriggered("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
}

void Stage1Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
#endif
}
