#include "Stage1Scene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/Collidable.h"
#include "../General/Rigidbody.h"
#include "../General/Collision/CollisionManager.h"
#include "../Game/Attack/AttackManager.h"

//アクター
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/Player/Player.h"
//カメラ
#include "../Game/Camera/Camera.h"

#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 100.0f, 100.0f };
}

Stage1Scene::Stage1Scene(SceneController& controller):
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player.mv1"))
{
	//プレイヤーの初期化
	m_player = std::make_shared<Player>(m_playerHandle, kPlayerPos);
	m_actors.push_back(m_player);
	//カメラの初期化
	m_camera = std::make_unique<Camera>(kCameraPos,m_player);
	//コリジョンマネージャー
	m_collManager = std::make_unique<CollisionManager>();
	//攻撃の処理
	m_attackManger = std::make_unique<AttackManager>();
}

Stage1Scene::~Stage1Scene()
{
	MV1DeleteModel(m_playerHandle);
}

void Stage1Scene::Update(Input& input)
{
#if _DEBUG
	//デバッグシーン
	if (CheckHitKey(KEY_INPUT_D))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	if (input.IsTriggered("Pause"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(input,m_camera, m_attackManger);
	}
	//アクターの衝突処理
	m_collManager->Update(m_actors);
	//更新確定
	for (auto& actor : m_actors)
	{
		actor->Complete();
	}

	//カメラの更新
	m_camera->Update();
}

void Stage1Scene::Draw()
{
#if _DEBUG
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
	DrawLine3D(VGet(0,0,500), VGet(0, 0, -500), 0x00ff00);
	DrawLine3D(VGet(500,0,0), VGet(-500, 0, 00), 0x00ffff);
	DrawLine3D(VGet(0,500,0), VGet(0, -500, 0), 0xff0000);
#endif
	//アクターの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
}
