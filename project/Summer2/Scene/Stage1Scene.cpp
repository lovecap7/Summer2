#include "Stage1Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/Collidable.h"
#include "../General/Rigidbody.h"

//�A�N�^�[
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/Player/Player.h"
//�J����
#include "../Game/Camera/Camera.h"

#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

Stage1Scene::Stage1Scene(SceneController& controller):
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player.mv1"))
{
	//�v���C���[�̏����ʒu
	Position3 firstPos(0.0f, 0.0f, 0.0f);
	//�v���C���[�̏�����
	m_player = std::make_shared<Player>(m_playerHandle, firstPos);
	m_actors.push_back(m_player);
	//�J�����̏�����
	m_camera = std::make_unique<Camera>(Vector3(100.0f,100.0f,300.0f), firstPos);
}

Stage1Scene::~Stage1Scene()
{
}

void Stage1Scene::Update(Input& input)
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
	if (input.IsTriggered("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(input,m_camera);
	}
	//�A�N�^�[�̏Փˏ���

	//�X�V�m��
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}

	//�J�����̍X�V
	m_camera->Update(m_player->GetCollidable()->GetRb()->GetPos());
}

void Stage1Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
	DrawLine3D(VGet(0,0,500), VGet(0, 0, -500), 0x00ff00);
	DrawLine3D(VGet(500,0,0), VGet(-500, 0, 00), 0x00ffff);
	DrawLine3D(VGet(0,500,0), VGet(0, -500, 0), 0xff0000);
#endif
	//�A�N�^�[�̕`��
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
}
