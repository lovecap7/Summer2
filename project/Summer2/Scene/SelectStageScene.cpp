#include "SelectStageScene.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

SelectStageScene::SelectStageScene(SceneController& controller):
	SceneBase(controller)
{
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Update(Input& input)
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
	if (input.IsTriggered("A"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<Stage1Scene>(m_controller));
		return;
	}
	if (input.IsTriggered("B"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<Stage2Scene>(m_controller));
		return;
	}
	if (input.IsTriggered("X"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<Stage3Scene>(m_controller));
		return;
	}
}

void SelectStageScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SelectStage Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
	DrawString(0, 32, "A Bottun : Stage 1", 0xffff00);
	DrawString(0, 48, "B Bottun : Stage 2", 0xffff00);
	DrawString(0, 64, "X Bottun : Stage 3", 0xffff00);
#endif
}
