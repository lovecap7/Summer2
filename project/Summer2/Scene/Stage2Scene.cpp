#include "Stage2Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

Stage2Scene::Stage2Scene(SceneController& controller):
	SceneBase(controller)
{
}

Stage2Scene::~Stage2Scene()
{
}

void Stage2Scene::Init()
{
	//�Ȃ�
}

void Stage2Scene::Update(Input& input)
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

void Stage2Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage2 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
#endif
}

void Stage2Scene::End()
{
}
