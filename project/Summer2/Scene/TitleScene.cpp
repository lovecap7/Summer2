#include "TitleScene.h"
#include "Dxlib.h"
#include  "../General/Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include <memory>

namespace
{
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update(Input& input)
{
	if (input.IsTriggered("Pause"))
	{
		//éüÇÃÉVÅ[ÉìÇ÷
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
		return;
	}
}

void TitleScene::Draw()
{
	DrawString(0, 0, "Title Scene", 0xffffff);
}
