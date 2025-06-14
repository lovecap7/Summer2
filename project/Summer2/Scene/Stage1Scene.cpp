#include "Stage1Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
//�z�u�f�[�^
#include "../../General/TransformDataLoader.h"
//�A�N�^�[
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/ActorManager.h"
//�v���C���[
#include "../Game/Actors/Player/Player.h"
//�G
#include "../Game/Actors/Enemy/EnemyManager.h"
#include "../Game/Actors/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "../Game/Actors/Enemy/SmallDragon/SmallDragon.h"
#include "../Game/Actors/Enemy/BossDragon/BossDragon.h"
#include "../Game/Actors/Enemy/EnemyBase.h"
//�X�e�[�W
#include "../Game/Actors/Stage/InvisibleWall.h"
#include "../Game/Actors/TestColls/TestPolygon.h"
#include "../Game/Actors/Stage/StageObjectCollision.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
//�J����
#include "../Game/Camera/Camera.h"
//�f�o�b�O���[�h
#include "DebugScene.h"

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 400.0f, -500.0f };
}


Stage1Scene::Stage1Scene(SceneController& controller):
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player/Player.mv1")),
	m_wallHandle(MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1")),
	m_purpleDinosaurHandle(MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1")),
	m_smallDragonHandle(MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1")),
	m_bossDragonHandle(MV1LoadModel("Data/Model/Enemy/BossDragon.mv1")),
	m_pathHandle(MV1LoadModel("Data/Model/Stage/1/Path.mv1")),
	m_cubeHandle(MV1LoadModel("Data/Model/Collision/Cube.mv1")),
	m_cylinderHandle(MV1LoadModel("Data/Model/Collision/Cylinder.mv1"))
{
	//�o�ꂷ��I�u�W�F�N�g���Z�b�g���Ă���
	std::vector<std::shared_ptr<Actor>> actors;
	CreateCharacter(actors);
	CreateStage(actors);
	//�J�����̏�����
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	actors.emplace_back(std::make_shared<InvisibleWall>(m_wallHandle, Vector3{ 0.0f,-200.0f,0.0f }, VGet(1000.0f, 1.0f, 1000.0f), VGet(0.0f, 0.0f, 0.0f)));
	m_actorManager = std::make_shared<ActorManager>(actors, m_player);
}

Stage1Scene::~Stage1Scene()
{
	//�o�^����
	m_actorManager->Exit();
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_purpleDinosaurHandle);
	MV1DeleteModel(m_smallDragonHandle);
	MV1DeleteModel(m_bossDragonHandle);
	MV1DeleteModel(m_pathHandle);
	MV1DeleteModel(m_cubeHandle);
	MV1DeleteModel(m_cylinderHandle);
}

void Stage1Scene::Init()
{
	m_actorManager->Init();
}

void Stage1Scene::Update(Input& input)
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
	

	//�f�o�b�O�ňꎞ��~����ĂȂ��Ȃ�
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//�A�N�^�[�̍X�V����
		m_actorManager->Update(input, m_camera);
		//�J�����̍X�V
		m_camera->Update();
	}
}

void Stage1Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]�L�[�� Debug Scene", 0xffffff);
	
	for (int z = -500; z <= 500; z += 100)
	{
		DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	}
	for (int x = -500; x <= 500; x += 100)
	{
		DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	}
	VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	DrawString(screenPos.x, screenPos.y, "X-", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	DrawString(screenPos.x, screenPos.y, "Z+", 0xffffff);
	screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	DrawString(screenPos.x, screenPos.y, "Z-", 0xffffff);
#endif
	//�A�N�^�[�̕`��
	m_actorManager->Draw();
}

void Stage1Scene::CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors)
{
	//�z�u�f�[�^���擾
	auto characterData = TransformDataLoader::LoadDataCSV("Data/CSV/CharacterTransformData.csv");
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& charaData : characterData)
	{
		if (charaData.name == "Player")
		{
			m_player = std::make_shared<Player>(m_playerHandle, charaData.pos);
			m_player->GetModel()->SetScale(charaData.scale);
			m_player->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(m_player);
		}
		else if (charaData.name == "SmallDragon")
		{
			std::shared_ptr<EnemyBase> smallDragon = std::make_shared<SmallDragon>(MV1DuplicateModel(m_smallDragonHandle),
				charaData.pos);
			smallDragon->GetModel()->SetScale(charaData.scale);
			smallDragon->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(smallDragon);
		}
		else if (charaData.name == "BossDragon")
		{
			std::shared_ptr<EnemyBase> bossDragon = std::make_shared<BossDragon>(MV1DuplicateModel(m_bossDragonHandle),
				charaData.pos);
			bossDragon->GetModel()->SetScale(charaData.scale);
			bossDragon->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(bossDragon);
		}
		else if (charaData.name == "PurpleDinosaur")
		{
			std::shared_ptr<EnemyBase> purpleDinosaur = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_purpleDinosaurHandle),
				charaData.pos);
			purpleDinosaur->GetModel()->SetScale(charaData.scale);
			purpleDinosaur->GetModel()->SetRot(charaData.rot);
			actors.emplace_back(purpleDinosaur);
		}
	}
}

void Stage1Scene::CreateStage(std::vector<std::shared_ptr<Actor>>& actors)
{
	//�`��p
	//�z�u�f�[�^���擾
	auto stageDrawData = TransformDataLoader::LoadDataCSV("Data/CSV/StageTransformData.csv");
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> path = 
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_pathHandle), stageData.pos, stageData.scale, stageData.rot);
			actors.emplace_back(path);
		}
		/*else if (stageData.name == "Plane")
		{
			std::shared_ptr<StageObjectDraw> plane =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_cubeHandle), stageData.pos, stageData.scale, stageData.rot);
			actors.emplace_back(plane);
		}*/
	}
	////�����蔻��p
	////�z�u�f�[�^���擾
	//auto stageCollData = TransformDataLoader::LoadDataCSV("Data/CSV/StageCollisionTransformData.csv");
	////���O����R���W������z�u���Ă���
	//for (auto& stageData : stageCollData)
	//{
	//	if (stageData.name == "Cube")
	//	{
	//		std::shared_ptr<StageObjectCollision> Cube =
	//			std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_cubeHandle), stageData.pos, stageData.scale, stageData.rot);
	//		actors.emplace_back(Cube);
	//	}
	//	else if (stageData.name == "Cylinder")
	//	{
	//		std::shared_ptr<StageObjectCollision> cylinder =
	//			std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_cylinderHandle), stageData.pos, stageData.scale, stageData.rot);
	//		actors.emplace_back(cylinder);
	//	}
	//}
}
