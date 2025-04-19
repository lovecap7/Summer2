#include <DxLib.h>
#include <memory>
#include "ResultScene.h"
#include "TitleScene.h"
#include  "../General/Input.h"
#include "SceneController.h"

namespace
{

}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update(Input& input)
{
	if (input.IsTriggered("Pause"))
	{
		//éüÇÃÉVÅ[ÉìÇ÷
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}

void ResultScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Result Scene", 0xffffff);
#endif
}
