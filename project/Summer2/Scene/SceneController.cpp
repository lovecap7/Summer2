#include "SceneController.h"
#include <memory>
//�p�����̊֐����F�����Ă����
#include "TitleScene.h"

SceneController::SceneController()
{
	//��ԍŏ��̃V�[�������͊��蓖�Ă�
	//�������g�̃C���X�^���X��n���Ă�����
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
