#include "GameScene.h"
#include <DxLib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "ResultScene.h"

namespace
{
	
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller)
{
	
}

GameScene::~GameScene()
{
	
}

void GameScene::Update(Input& input)
{
	if (input.IsTriggered("Pause"))
	{
		//éüÇÃÉVÅ[ÉìÇ÷
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
}

void GameScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Game Scene", 0xffffff);
#endif
}