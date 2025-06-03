#include "TestCollScene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
//�A�N�^�[
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/ActorManager.h"
//�v���C���[
#include "../Game/Actors/Player/Player.h"
//�G
#include "../Game/Actors/Enemy/EnemyManager.h"
#include "../Game/Actors/Enemy/Common1.h"
#include "../Game/Actors/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "../Game/Actors/Enemy/EnemyBase.h"
//�X�e�[�W
#include "../Game/Actors/Stage/InvisibleWall.h"
#include "../Game/Actors/TestColls/TestPolygon.h"
//�J����
#include "../Game/Camera/Camera.h"

//�f�o�b�O���[�h
#include "DebugScene.h"

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 400.0f, 500.0f };
}

TestCollScene::TestCollScene(SceneController& controller) :
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player/Player.mv1")),
	m_polygonHandle(MV1LoadModel("Data/Model/Stage/Stage1.mv1")),
	m_wallHandle(MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1")),
	m_common1Handle(MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1"))
{
	//�o�ꂷ��I�u�W�F�N�g���Z�b�g���Ă���
	std::vector<std::shared_ptr<Actor>> actors;

	//�v���C���[�̏�����
	m_player = std::make_shared<Player>(m_playerHandle, kPlayerPos);
	actors.push_back(m_player);
	//�J�����̏�����
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	std::shared_ptr<EnemyBase> enemy = std::make_shared<Common1>(MV1DuplicateModel(m_common1Handle), Vector3{ -200.0f,-50.0f,0.0f });
	actors.push_back(enemy);
	std::shared_ptr<EnemyBase> enemy2 = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_common1Handle), Vector3{ -300.0f,-50.0f,0.0f });
	actors.push_back(enemy2);
	actors.push_back(std::make_shared<TestPolygon>(Vector3{ 0.0f,-100.0f,0.0f }, m_polygonHandle));
	actors.push_back(std::make_shared<InvisibleWall>(m_wallHandle,Vector3{ -100.0f,-50.0f,0.0f },VGet(1.0f,1.0f,1.0f), VGet(0.0f, 0.0f, 0.0f)));//������
	m_actorManager = std::make_shared<ActorManager>(actors, m_player);
}

TestCollScene::~TestCollScene()
{
	//�o�^����
	m_actorManager->Exit();

	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_common1Handle);
	MV1DeleteModel(m_polygonHandle);
}

void TestCollScene::Init()
{
	m_actorManager->Init();
}

void TestCollScene::Update(Input& input)
{
#if _DEBUG
	if (input.IsTrigger("StopUpdate"))
	{
		m_isUpdateStop = true;
	}
	if (input.IsTrigger("Enter"))
	{
		m_isUpdateStop = false;
	}
	

	//�f�o�b�O�V�[��
	if (input.IsTrigger("SceneChange"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTrigger("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	if (input.IsTrigger("Start"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<TestCollScene>(m_controller));
		return;
	}

	//�f�o�b�O�ňꎞ��~����ĂȂ��Ȃ�
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//�A�N�^�[�̍X�V����
		m_actorManager->Update(input,m_camera);
		//�J�����̍X�V
		m_camera->Update();
#if _DEBUG
		m_camera->RotaCamera(input);
		//���̓e�X�g
		if (input.IsLowPowerLeftStick())
		{
			printfDx("���������|���Ă܂�\n");
		}
		if (input.IsMediumPowerLeftStick())
		{
			printfDx("���������|���Ă܂�\n");
		}
		if (input.IsHighPowerLeftStick())
		{
			printfDx("�ő�܂œ|���Ă܂�\n");
		}
#endif
	}
}

void TestCollScene::Draw()
{
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
	DrawString(0, 40, "Start�{�^���� ���Z�b�g", 0xffffff);
	DrawLine3D(VGet(0, 0, 500), VGet(0, 0, -500), 0x00ff00);
	DrawLine3D(VGet(500, 0, 0), VGet(-500, 0, 00), 0x00ffff);
	DrawLine3D(VGet(0, 500, 0), VGet(0, -500, 0), 0xff0000);
	//�A�N�^�[�̕`��
	m_actorManager->Draw();
}
