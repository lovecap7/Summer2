#include "Stage2Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//デバッグモード
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
	//なし
}

void Stage2Scene::Update(Input& input)
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

void Stage2Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage2 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
#endif
}

void Stage2Scene::End()
{
}
