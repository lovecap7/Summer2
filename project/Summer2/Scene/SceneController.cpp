#include "SceneController.h"
#include <memory>
//継承元の関数も認識してくれる
#include "TitleScene.h"

SceneController::SceneController()
{
	//一番最初のシーンだけは割り当てる
	//自分自身のインスタンスを渡してあげる
	m_scene = std::make_shared<TitleScene>(*this);
}

void SceneController::Update(Input& input)
{
	m_scene->Update(input);
}

void SceneController::Draw()
{
	m_scene->Draw();
}

void SceneController::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	m_scene = scene;
}
