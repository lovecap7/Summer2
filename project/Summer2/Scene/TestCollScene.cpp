#include "TestCollScene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/Collidable.h"
#include "../General/Rigidbody.h"
#include "../General/Collision/CollisionManager.h"
#include "../Game/Attack/AttackManager.h"

//�A�N�^�[
#include "../Game/Actors/Actor.h"
//�v���C���[
#include "../Game/Actors/Player/Player.h"
//�G
#include "../Game/EnemyManager.h"
#include "../Game/Actors/Enemy/Common1.h"
#include "../Game/Actors/Enemy/EnemyBase.h"
//�e�X�^�[
#include "../Game/Actors/TestColls/TestSphere.h"
#include "../Game/Actors/TestColls/TestCapsule.h"
#include "../Game/Actors/TestColls/TestPolygon.h"
//�X�e�[�W
#include "../Game/Actors/Stage/InvisibleWall.h"
//�J����
#include "../Game/Camera/Camera.h"

//�f�o�b�O���[�h
#include "DebugScene.h"

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 400.0f, 500.0f };
	//�d��
	const Vector3 kGravity = { 0.0f, -0.5f, 0.0f };
}

TestCollScene::TestCollScene(SceneController& controller) :
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player/Player.mv1")),
	m_polygonHandle(MV1LoadModel("Data/Model/Stage/Stage1.mv1")),
	m_wallHandle(MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1")),
	m_common1Handle(MV1LoadModel("Data/Model/Enemy/Common1.mv1"))
{
	//�v���C���[�̏�����
	m_player = std::make_shared<Player>(m_playerHandle, kPlayerPos);
	m_actors.push_back(m_player);
	//�J�����̏�����
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	//�R���W�����}�l�[�W���[
	m_collManager = std::make_unique<CollisionManager>();
	//�G�l�~�[�}�l�[�W���[
	m_enemyManager = std::make_unique<EnemyManager>(m_player);

	std::shared_ptr<EnemyBase> enemy = std::make_shared<Common1>(m_enemyManager,m_common1Handle, Vector3{ -200.0f,-50.0f,0.0f });
	m_actors.push_back(enemy);
	//�����蔻��̃e�X�g���s���̂œK���ɃI�u�W�F�N�g��ǉ�
	////����
	//m_actors.push_back(std::make_shared<TestSphere>(Vector3{ 100.0f,0.0f,0.0f }, 20.0f,false));//��
	//m_actors.push_back(std::make_shared<TestSphere>(Vector3{ 200.0f,10.0f,0.0f }, 50.0f,false));//��
	//m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ 100.0f,0.0f,100.0f }, Vector3{ 100.0f,100.0f,100.0f }, 50.0f,false));//�J�v�Z��
	//m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ 300.0f,10.0f,0.0f }, Vector3{ 500.0f,10.0f, 0.0f }, 50.0f,false));//�J�v�Z��
	////�����Ȃ�
	//m_actors.push_back(std::make_shared<TestSphere>(Vector3{ -100.0f,0.0f,0.0f }, 20.0f, true));//��
	//m_actors.push_back(std::make_shared<TestSphere>(Vector3{ -200.0f,10.0f,0.0f }, 50.0f, true));//��
	//m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ -100.0f,0.0f,100.0f }, Vector3{ -100.0f,100.0f,-100.0f }, 50.0f, true));//�J�v�Z��
	//m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ -300.0f,10.0f,0.0f }, Vector3{ -500.0f,10.0f, 0.0f }, 50.0f, true));//�J�v�Z��
	m_actors.push_back(std::make_shared<TestPolygon>(Vector3{ 0.0f,-100.0f,0.0f }, m_polygonHandle));
	m_actors.push_back(std::make_shared<InvisibleWall>(m_wallHandle,Vector3{ -100.0f,-50.0f,0.0f },VGet(1.0f,1.0f,1.0f), VGet(0.0f, 0.0f, 0.0f)));//������

	//�U���̏���
	m_attackManger = std::make_unique<AttackManager>();
}

TestCollScene::~TestCollScene()
{
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_common1Handle);
	MV1DeleteModel(m_polygonHandle);
}

void TestCollScene::Update(Input& input)
{
	//�f�o�b�O�V�[��
	if (CheckHitKey(KEY_INPUT_D))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
	if (input.IsTriggered("Pause"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	if (input.IsTriggered("Start"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<TestCollScene>(m_controller));
		return;
	}

	//�G�̑O����
	m_enemyManager->Update(m_actors);

	//�A�N�^�[�̍X�V
	for (auto& actor : m_actors)
	{
		actor->Update(input, m_camera, m_attackManger);
		actor->Gravity(kGravity);
	}
	//�U���̏���
	m_attackManger->Update(m_actors);

	//���Ńt���O�`�F�b�N
	auto remIt = std::remove_if(m_actors.begin(), m_actors.end(), [](std::shared_ptr<Actor> actor) {return actor->IsDead();});
	m_actors.erase(remIt, m_actors.end());//�폜

	//�A�N�^�[�̏Փˏ���
	m_collManager->Update(m_actors);
	//�X�V�m��
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}

	//�J�����̍X�V
	m_camera->Update();
#if _DEBUG
	m_camera->RotaCamera(input);
#endif
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
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
	//�U���̕`��
	m_attackManger->Draw();
}
