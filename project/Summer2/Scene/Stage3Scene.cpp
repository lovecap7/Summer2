#include "Stage3Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//デバッグモード
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
	//なし
}

void Stage3Scene::Update(Input& input)
{
#if _DEBUG
	//デバッグシーン
	if (input.IsTrigger("SceneChange"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTrigger("Pause"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
}

void Stage3Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage3 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
#endif
}

void Stage3Scene::End()
{
}
