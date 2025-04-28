#include "TestCollScene.h"
#include "ResultScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/Collidable.h"
#include "../General/Rigidbody.h"
#include "../General/Collision/CollisionManager.h"

//アクター
#include "../Game/Actors/Actor.h"
#include "../Game/Actors/Player/Player.h"
#include "../Game/Actors/TestColls/TestSphere.h"
#include "../Game/Actors/TestColls/TestCapsule.h"
//カメラ
#include "../Game/Camera/Camera.h"

//デバッグモード
#include "DebugScene.h"

namespace
{
	const Vector3 kPlayerPos = { 0.0f, 0.0f, 0.0f };
	const Vector3 kCameraPos = { 0.0f, 100.0f, 200.0f };
}

TestCollScene::TestCollScene(SceneController& controller) :
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Data/Model/Player.mv1"))
{
	//プレイヤーの初期化
	m_player = std::make_shared<Player>(m_playerHandle, kPlayerPos);
	m_actors.push_back(m_player);
	//カメラの初期化
	m_camera = std::make_unique<Camera>(kCameraPos, m_player);
	//コリジョンマネージャー
	m_collManager = std::make_unique<CollisionManager>();

	//当たり判定のテストを行うので適当にオブジェクトを追加
	//動く
	m_actors.push_back(std::make_shared<TestSphere>(Vector3{ 100.0f,0.0f,0.0f }, 20.0f,false));//球
	m_actors.push_back(std::make_shared<TestSphere>(Vector3{ -300.0f,10.0f,0.0f }, 50.0f,false));//球
	m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ 0.0f,0.0f,300.0f }, Vector3{ 0.0f,100.0f,300.0f }, 50.0f,false));//カプセル
	m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ -500.0f,10.0f,-500.0f }, Vector3{ -700.0f,10.0f, -500.0f }, 50.0f,false));//カプセル
	//動かない
	m_actors.push_back(std::make_shared<TestSphere>(Vector3{ 500.0f,0.0f,0.0f }, 20.0f, true));//球
	m_actors.push_back(std::make_shared<TestSphere>(Vector3{ -700.0f,10.0f,0.0f }, 50.0f, true));//球
	m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ 0.0f,0.0f,700.0f }, Vector3{ 0.0f,100.0f,700.0f }, 50.0f, true));//カプセル
	m_actors.push_back(std::make_shared<TestCapsule>(Vector3{ 500.0f,10.0f,-300.0f }, Vector3{ 800.0f,10.0f, -300.0f }, 50.0f, true));//カプセル
}

TestCollScene::~TestCollScene()
{
}

void TestCollScene::Update(Input& input)
{
	//デバッグシーン
	if (CheckHitKey(KEY_INPUT_D))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
	if (input.IsTriggered("Pause"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(input, m_camera);
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

void TestCollScene::Draw()
{
	DrawString(0, 0, "Stage1 Scene", 0xffffff);
	DrawString(0, 16, "[D]キーで Debug Scene", 0xffffff);
	DrawLine3D(VGet(0, 0, 500), VGet(0, 0, -500), 0x00ff00);
	DrawLine3D(VGet(500, 0, 0), VGet(-500, 0, 00), 0x00ffff);
	DrawLine3D(VGet(0, 500, 0), VGet(0, -500, 0), 0xff0000);
	//アクターの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
}
