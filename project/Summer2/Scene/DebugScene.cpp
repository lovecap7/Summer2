#include "DebugScene.h"
#include "TitleScene.h"
#include "SelectStageScene.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "ResultScene.h"
#include <DxLib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include <memory>

DebugScene::DebugScene(SceneController& controller):
	SceneBase(controller)
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Update(Input& input)
{
#if _DEBUG
	if (CheckHitKey(KEY_INPUT_Q))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller));
		return;
	}
	if (CheckHitKey(KEY_INPUT_E))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<Stage1Scene>(m_controller));
		return;
	}
	if (CheckHitKey(KEY_INPUT_R))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<Stage2Scene>(m_controller));
		return;
	}
	if (CheckHitKey(KEY_INPUT_T))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<Stage3Scene>(m_controller));
		return;
	}
	if (CheckHitKey(KEY_INPUT_Y))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
#endif
}

void DebugScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Debug Scene", 0xff0000);
	DrawString(100, 50, "Title Scene", 0xffff00);
	DrawString(100, 70, "SelectStage Scene", 0xffff00);
	DrawString(100, 90, "Stage1 Scene", 0xffff00);
	DrawString(100, 110, "Staeg2 Scene", 0xffff00);
	DrawString(100, 130, "Stage3 Scene", 0xffff00);
	DrawString(100, 150, "Result Scene", 0xffff00);
	DrawString(300, 50, ": [Q]�L�[", 0xffffff);
	DrawString(300, 70, ": [W]�L�[", 0xffffff);
	DrawString(300, 90, ": [E]�L�[", 0xffffff);
	DrawString(300, 110, ": [R]�L�[", 0xffffff);
	DrawString(300, 130, ": [T]�L�[", 0xffffff);
	DrawString(300, 150, ": [Y]�L�[", 0xffffff);
#endif
}
