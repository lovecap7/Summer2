#include "Stage1Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
//UI
#include "../Game/UI/UIManager.h"
#include "../Game/UI/UIPlayerHP.h"
#include "../Game/UI/UIPlayerUltGage.h"
//配置データ
#include "../General/TransformDataLoader.h"
//アクター
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/ActorManager.h"
//プレイヤー
#include "../Game/Actors/Player/Player.h"
//敵
#include "../Game/Actors/Enemy/EnemyManager.h"
#include "../Game/Actors/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "../Game/Actors/Enemy/SmallDragon/SmallDragon.h"
#include "../Game/Actors/Enemy/BossDragon/BossDragon.h"
#include "../Game/Actors/Enemy/EnemyBase.h"
//ステージ
#include "../Game/Actors/Stage/InvisibleWall.h"
#include "../Game/Actors/Stage/StageObjectCollision.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../Game/Actors/Stage/Sky.h"
//カメラ
#include "../Game/Camera/Camera.h"
//デバッグモード
#include "DebugScene.h"
#include <cassert>

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 600.0f, -600.0f };
}


Stage1Scene::Stage1Scene(SceneController& controller) :
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player/Player.mv1")),
	m_wallHandle(MV1LoadModel("Data/Model/Stage/InvisibleWall.mv1")),
	m_purpleDinosaurHandle(MV1LoadModel("Data/Model/Enemy/PurpleDinosaur.mv1")),
	m_smallDragonHandle(MV1LoadModel("Data/Model/Enemy/SmallDragon.mv1")),
	m_bossDragonHandle(MV1LoadModel("Data/Model/Enemy/BossDragon.mv1")),
	m_pathHandle(MV1LoadModel("Data/Model/Stage/1/Path.mv1")),
	m_cubeHandle(MV1LoadModel("Data/Model/Collision/Cube.mv1")),
	m_cylinderHandle(MV1LoadModel("Data/Model/Collision/Cylinder.mv1")),
	m_skyHandle(MV1LoadModel("Data/Model/Stage/Sky/Sky_Daylight02.pmx"))
{
	assert(m_playerHandle >= 0);
	assert(m_wallHandle >= 0);
	assert(m_purpleDinosaurHandle >= 0);
	assert(m_smallDragonHandle >= 0);
	assert(m_bossDragonHandle >= 0);
	assert(m_pathHandle >= 0);
	assert(m_cubeHandle >= 0);
	assert(m_cylinderHandle >= 0);
	assert(m_skyHandle >= 0);

	//登場するオブジェクトをセットしていく

	//キャラクターの作成
	CreateCharacter(m_actors);
	//ステージのオブジェクト配置
	CreateStage(m_actors);

	auto floor = std::make_shared<InvisibleWall>(m_wallHandle, Vector3{ 0.0f,-10.0f,0.0f }, VGet(1000.0f, 1.0f, 1000.0f), VGet(0.0f, 0.0f, 0.0f));
	m_actors.emplace_back(floor);
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>(m_player);
	//カメラの初期化
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	//UIの準備
	m_uiManager = std::make_shared<UIManager>();
}

Stage1Scene::~Stage1Scene()
{

}

void Stage1Scene::Init()
{
	//初期化処理
	m_actorManager->Init(std::move(m_actors));
	//UIの作成
	//プレイヤーの体力と必殺ゲージ
	auto playerHpUI = std::make_shared<UIPlayerHP>(m_player->GetHurtPoint());
	auto playerUltUI = std::make_shared<UIPlayerUltGage>(m_player->GetUltGage());
	m_uiManager->Entry(playerHpUI);
	m_uiManager->Entry(playerUltUI);
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


	//デバッグで一時停止されてないなら
#if _DEBUG
	if (!m_isUpdateStop || (input.IsTrigger("StopUpdate") && m_isUpdateStop))
#endif
	{
		//アクターの更新処理
		m_actorManager->Update(input, m_camera, m_uiManager);
		//カメラの更新
		m_camera->Update();
		//UIの更新
		m_uiManager->Update();
	}
}

void Stage1Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);

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
	//アクターの描画
	m_actorManager->Draw();
	//UIの描画
	m_uiManager->Draw();
}

void Stage1Scene::End()
{
	//登録解除
	m_actorManager->End();
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_purpleDinosaurHandle);
	MV1DeleteModel(m_smallDragonHandle);
	MV1DeleteModel(m_bossDragonHandle);
	MV1DeleteModel(m_pathHandle);
	MV1DeleteModel(m_cubeHandle);
	MV1DeleteModel(m_cylinderHandle);
	MV1DeleteModel(m_skyHandle);
}

void Stage1Scene::CreateCharacter(std::vector<std::shared_ptr<Actor>>& actors)
{
	//配置データを取得
	auto characterData = TransformDataLoader::LoadDataCSV("Data/CSV/CharacterTransformData.csv");
	//名前からオブジェクトを配置していく
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
	//空を作成
	actors.emplace_back(std::make_shared<Sky>(m_skyHandle));

	//描画用
	//配置データを取得
	auto stageDrawData = TransformDataLoader::LoadDataCSV("Data/CSV/StageTransformData.csv");
	//名前からオブジェクトを配置していく
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
	////当たり判定用
	////配置データを取得
	//auto stageCollData = TransformDataLoader::LoadDataCSV("Data/CSV/StageCollisionTransformData.csv");
	////名前からコリジョンを配置していく
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
