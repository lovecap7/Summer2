#include "GameScene.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>

#include  "../General/Input.h"
#include "SceneController.h"
#include "../Game/Score/ScoreManager.h"
#include "ResultScene.h"
#include "../Game/Actor.h"
#include "../Game/Player.h"
#include "../General/GameCamera.h"
#include "../General/Collision/Collidable.h"
#include "../General/Collision/Trigger.h"
#include "../General/Collision/ColliderBase.h"
#include "../General/game.h"
//当たり判定
#include "../General/Collision/CollisionChecker.h"
#include "../General/Collision/CollisionProcess.h"
//UI
#include "../General/UI/UIManager.h"
//敵
#include "../Game/Enemy1.h"
#include "../Game/Enemy2.h"
//ステージ
#include "../Game/Stage/StageGround.h"
//ゴール
#include "../Game/Stage/Goal.h"

namespace
{
	constexpr float kWorldSize = 10000.0f;
	constexpr float kSpaceInterval = 100.0f;
	// 画面に表示できる最大パーティクル
	constexpr int kEffectParticleLimit = 20000;
	//透明度
	constexpr float kMaxAlpha = 255.0f;
	constexpr float kAddAlpha = 5.0f;
	//スコア座標
	constexpr int kScorePosX = 500;
	constexpr int kScorePosY = 0;

	//エフェクトパス
	const char* kHitEffect = "../SonicHomingGame/Effects/efk/HitBom.efkefc";//ヒットエフェクト
	const char* kHomigEffect = "../SonicHomingGame/Effects/efk/Homing.efkefc";//ホーミング中のエフェクト
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller),
	m_playerHandle(MV1LoadModel("Model/Player/Player.mv1")),
	//敵
	m_beeHandle(MV1LoadModel("Model/Enemy/Bee.mv1")),
	m_crabHandle(MV1LoadModel("Model/Enemy/Crab.mv1")),
	//床
	m_groundOpenHandle(MV1LoadModel("Model/Stage/Plane/Plane_Open.mv1")),
	//真っすぐな道
	m_roadStraightHandle(MV1LoadModel("Model/Stage/Road/Road_Straight.mv1")),
	//コーナー
	m_roadCornerHandle(MV1LoadModel("Model/Stage/Road/Road_Corner.mv1")),
	//坂
	m_roadSlopeHandle(MV1LoadModel("Model/Stage/Road/Road_Slider.mv1")),
	//空
	m_skyHandle(MV1LoadModel("Model/Sky/Sky_Night01.pmx")),
	m_skyAngle(0.0f),
	//ゴール
	m_goalHandle(MV1LoadModel("Model/Stage/Goal/Goal.mv1")),
	//フェードイン
	m_fadeAlpha(kMaxAlpha)
{
	//プレイヤーのポインタをカメラとアクターに渡す
	std::shared_ptr<Player> player = std::make_shared<Player>(m_playerHandle,kHomigEffect);
	m_actors.push_back(player);

	//カメラ
	m_camera = std::make_shared<GameCamera>(player);
	//当たり判定をチェックするクラス
	m_collChecker = std::make_shared<CollisionChecker>();
	m_collProcess = std::make_shared<CollisionProcess>();

	//UI
	m_uiManager = std::make_shared<UIManager>();

	//ステージを作る
	StageCreate();

	//ライトの向き
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 0.0f));

	//フェードインしてからスタート
	m_update = &GameScene::FadeInUpdate;
	m_draw = &GameScene::FadeDraw;
}

void GameScene::StageCreate()
{
	//空
	MV1SetPosition(m_skyHandle, VGet(0.0f, -200.0f, 0.0f));
	MV1SetScale(m_skyHandle, VGet(30.0f, 30.0f, 30.0f));

	//スタート地点
	std::shared_ptr<StageGround> startGround = std::make_shared<StageGround>(MV1DuplicateModel(m_groundOpenHandle));
	startGround->SetScale(VGet(50.0f, 10.0f, 50.0f));
	startGround->SetPos(Vector3{ 0.0f,0.0f,0.0f });
	m_actors.push_back(startGround);

	//道
	std::shared_ptr<StageGround> roadS1 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS1->SetScale(VGet(20.0f, 30.0f, 20.0f));
	roadS1->SetPos(Vector3{ 0.0f,0.0f,-2000.0f });
	m_actors.push_back(roadS1);
	//道2
	std::shared_ptr<StageGround> roadS2 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS2->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS2->SetPos(Vector3{ -23000.0f,2000.0f,-700.0f });
	m_actors.push_back(roadS2);
	//道3
	std::shared_ptr<StageGround> roadS3 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS3->SetScale(VGet(50.0f, 50.0f, 50.0f));
	roadS3->SetPos(Vector3{ 5000.0f,2000.0f,10000.0f });
	roadS3->SetAngle(VGet(0.0f, 90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS3);
	//道4
	std::shared_ptr<StageGround> roadS4 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS4->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS4->SetPos(Vector3{ -12000.0f,2000.0f,5000.0f });
	roadS4->SetAngle(VGet(0.0f, 45.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS4);
	//道5
	std::shared_ptr<StageGround> roadS5 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS5->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS5->SetPos(Vector3{ -7000.0f,2000.0f,7000.0f });
	roadS5->SetAngle(VGet(0.0f, 45.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS5);
	//道6
	std::shared_ptr<StageGround> roadS6 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadStraightHandle));
	roadS6->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadS6->SetPos(Vector3{ -15000.0f,2000.0f,7000.0f });
	roadS6->SetAngle(VGet(0.0f, 45.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadS6);

	//カーブ
	std::shared_ptr<StageGround> roadC1 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadCornerHandle));
	roadC1->SetScale(VGet(20.0f, 30.0f, 20.0f));
	roadC1->SetPos(Vector3{ 0.0f,0.0f,-10000.0f });
	m_actors.push_back(roadC1);
	//カーブ2
	std::shared_ptr<StageGround> roadC2 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadCornerHandle));
	roadC2->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadC2->SetPos(Vector3{ -15900.0f,2000.0f,-18000.0f });
	roadC2->SetAngle(VGet(0.0f, 90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadC2);
	//カーブ3
	std::shared_ptr<StageGround> roadC3 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadCornerHandle));
	roadC3->SetScale(VGet(50.0f, 30.0f, 30.0f));
	roadC3->SetPos(Vector3{ -14000.0f,5000.0f,8000.0f });
	roadC3->SetAngle(VGet(0.0f / 180.0f * DX_PI_F, 180.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadC3);

	//坂道
	std::shared_ptr<StageGround> roadSl1 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadSlopeHandle));
	roadSl1->SetScale(VGet(20.0f, 20.0f, 20.0f));
	roadSl1->SetPos(Vector3{ -8000.0f,100.0f,-18000.0f });
	roadSl1->SetAngle(VGet(0.0f, 90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadSl1);
	//坂道2
	std::shared_ptr<StageGround> roadSl2 = std::make_shared<StageGround>(MV1DuplicateModel(m_roadSlopeHandle));
	roadSl2->SetScale(VGet(50.0f, 50.0f, 50.0f));
	roadSl2->SetPos(Vector3{ 4000.0f,-100.0f,10000.0f });
	roadSl2->SetAngle(VGet(20.0f / 180.0f * DX_PI_F, -90.0f / 180.0f * DX_PI_F, 0.0f));
	m_actors.push_back(roadSl2);

	//敵1bee
	std::shared_ptr<Enemy1> bee1 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle),Vector3{ 0.0f,2000.0f,-12000.0f },kHitEffect);
	m_actors.push_back(bee1);
	//敵2bee
	std::shared_ptr<Enemy1> bee2 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle),Vector3{ -16300.0f,3000.0f,-18000.0f }, kHitEffect);
	m_actors.push_back(bee2);
	//敵3bee
	std::shared_ptr<Enemy1> bee3 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -16300.0f,3000.0f,-19000.0f },kHitEffect);
	m_actors.push_back(bee3);
	//敵4bee
	std::shared_ptr<Enemy1> bee4 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -16300.0f,3000.0f,-17000.0f },kHitEffect);
	m_actors.push_back(bee4);
	//敵5bee
	std::shared_ptr<Enemy1> bee5 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -10000.0f,4000.0f,5000.0f },kHitEffect);
	m_actors.push_back(bee5);
	//敵6bee
	std::shared_ptr<Enemy1> bee6 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -5000.0f,4000.0f,5000.0f },kHitEffect);
	m_actors.push_back(bee6);
	for (int i = 1;i <= 12;++i)
	{
		//敵7~17bee
		std::shared_ptr<Enemy1> bee7to17 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ 2000.0f * i ,2500.0f + 1000 * i,10000.0f },kHitEffect);
		m_actors.push_back(bee7to17);

	}
	for (int i = 1;i <= 5;++i)
	{
		//敵18~22bee
		std::shared_ptr<Enemy1> bee18to22 = std::make_shared<Enemy1>(MV1DuplicateModel(m_beeHandle), Vector3{ -20000.0f + 3000.0f * i,4000.0f,-7000.0f + 3000.0f * i },kHitEffect);
		m_actors.push_back(bee18to22);

	}
	
	for (int i = 1;i <= 3;++i)
	{
		//敵1~3crab
		std::shared_ptr<Enemy2> crab1To3 = std::make_shared<Enemy2>(MV1DuplicateModel(m_beeHandle), Vector3{ 0.0f,200.0f,-5000.0f + -2000.0f * i },kHitEffect);
		m_actors.push_back(crab1To3);
	}
	

	//ゴール前
	std::shared_ptr<StageGround> endGround = std::make_shared<StageGround>(MV1DuplicateModel(m_groundOpenHandle));
	endGround->SetScale(VGet(50.0f, 50.0f, 50.0f));
	endGround->SetPos(Vector3{	23600.0f,11000.0f,10000.0f });
	m_actors.push_back(endGround);

	//ゴール
	std::shared_ptr<Goal> goal = std::make_shared<Goal>(MV1DuplicateModel(m_goalHandle));
	goal->SetScale(VGet(10.0f, 10.0f, 10.0f));
	goal->SetPos(Vector3{ 23600.0f,12000.0f,10000.0f });
	m_actors.push_back(goal);
}


GameScene::~GameScene()
{
	//モデルの削除
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_beeHandle);
	MV1DeleteModel(m_crabHandle);
	MV1DeleteModel(m_groundOpenHandle);
	MV1DeleteModel(m_roadStraightHandle);
	MV1DeleteModel(m_roadCornerHandle);
	MV1DeleteModel(m_roadSlopeHandle);
	MV1DeleteModel(m_skyHandle);
	MV1DeleteModel(m_goalHandle);
	//エフェクトの削除
	DeleteEffekseerEffect(LoadEffekseerEffect(kHitEffect));
	DeleteEffekseerEffect(LoadEffekseerEffect(kHomigEffect));
}

void GameScene::Update(Input& input)
{
	//状態に応じた処理
	(this->*m_update)(input);
}

void GameScene::Draw()
{
	//状態に応じた処理
	(this->*m_draw)();
}

void GameScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("Pause"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}

	//空の回転
	m_skyAngle += 0.01f / 180.0f * DX_PI_F;
	MV1SetRotationXYZ(m_skyHandle, VGet(0.0f, m_skyAngle, 0.0f));

	//オブジェクトの更新処理
	for (auto& actor : m_actors)
	{
		actor->Update(input, m_camera);
	}

	//トリガーの判定チェック
	TriggerCheck();

	//当たり判定のチェック
	CollisionCheck();

	//カメラからレイを飛ばしてホーミングのターゲットを探す
	m_camera->SearchHomingTarget(m_actors, m_collChecker);

	//オブジェクトの移動
	for (auto& actor : m_actors)
	{
		actor->CompleteMove();
	}

	//死亡中のオブジェクトからスコアを加算
	for (auto& actor : m_actors)
	{
		//敵以外ならスキップ
		if (actor->GetCollidable()->GetObjectType() != ObjectType::Enemy)continue;
		//死亡中じゃないならスキップ
		if (!std::dynamic_pointer_cast<EnemyBase>(actor)->IsDeading())continue;

		//ここまで来たら処理
		//スコアを加算
		int score = std::dynamic_pointer_cast<EnemyBase>(actor)->GetScere();
		m_controller.GetScoreManager()->AddTotalScere(score);
		//渡し終わったので0にする
		std::dynamic_pointer_cast<EnemyBase>(actor)->ResetScore();
	}

	//死んだオブジェクトを消す
	//remove_ifで条件に合うものを配列の後ろに動かしその先頭アドレスを返す
	auto remIt = std::remove_if(
		m_actors.begin(), //要素の頭
		m_actors.end(), //要素の終わり
		//ラムダ式(無名関数)
		[](const std::shared_ptr<Actor>& actor)->bool {
			return actor->IsDead();//死んでるオブジェクトを削除
		});
	//上で処理して後ろに動かした要素の先頭アドレスをゲットしたのでその要素から終わりの要素までを削除する
	m_actors.erase(remIt, m_actors.end());


	//カメラの更新処理
	m_camera->Update(input);

	for (auto& actor : m_actors)
	{
		//ゴールしてるかチェック
		if (actor->GetTrigger()->GetTriggerType() == TriggerType::Goal)
		{
			if (std::dynamic_pointer_cast<Goal>(actor)->IsGoal())
			{
				m_update = &GameScene::FadeOutUpdate;
				m_draw = &GameScene::FadeDraw;
			}
			break;
		}
	}

	//UIの更新
	m_uiManager->Update(m_actors, m_camera);

	//スコア
	m_controller.GetScoreManager()->Update();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void GameScene::NormalDraw()
{
	MV1DrawModel(m_skyHandle);
#if _DEBUG

	DrawString(0, 0, "Game Scene", 0xffffff);

	for (float i = -kWorldSize / 2.0f;i <= kWorldSize / 2.0f;i += kSpaceInterval)
	{
		DrawLine3D(VGet(-kWorldSize / 2.0f, 0.0f, i), VGet(kWorldSize / 2.0f, 0.0f, i), 0xffffff);//X軸
		DrawLine3D(VGet(i, 0.0f, -kWorldSize / 2.0f), VGet(i, 0.0f, kWorldSize / 2.0f), 0xffffff);//Z軸
	}
	DrawLine3D(VGet(-kWorldSize / 2.0f, 0.0f, 0.0f), VGet(kWorldSize / 2.0f, 0.0f, 0.0f), 0x0000ff);//X軸
	DrawLine3D(VGet(0.0f, -kWorldSize / 2.0f, 0.0f), VGet(0.0f, kWorldSize / 2.0f, 0.0f), 0xff0000);//Y軸
	DrawLine3D(VGet(0.0f, 0.0f, -kWorldSize / 2.0f), VGet(0.0f, 0.0f, kWorldSize / 2.0f), 0x00ff00);//Z軸
#endif
	//オブジェクトの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}

	//UIの描画
	m_uiManager->Draw();

	//スコア
	m_controller.GetScoreManager()->Draw(kScorePosX, kScorePosY);

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void GameScene::FadeInUpdate(Input& input)
{
	//空の回転
	m_skyAngle += 0.01f / 180.0f * DX_PI_F;
	MV1SetRotationXYZ(m_skyHandle, VGet(0.0f, m_skyAngle, 0.0f));

	//オブジェクトの更新処理
	for (auto& actor : m_actors)
	{
		actor->Update(input, m_camera);
	}

	//当たり判定のチェック
	CollisionCheck();

	//オブジェクトの移動
	for (auto& actor : m_actors)
	{
		actor->CompleteMove();
	}

	//カメラの更新処理
	m_camera->Update(input);

	if (m_fadeAlpha <= 0)
	{
		m_update = &GameScene::NormalUpdate;
		m_draw = &GameScene::NormalDraw;
		return;
	}
	m_fadeAlpha -= kAddAlpha;

	//UIの更新
	m_uiManager->Update(m_actors, m_camera);

	//スコア
	m_controller.GetScoreManager()->Update();
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (m_fadeAlpha >= kMaxAlpha)
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;
	}
	m_fadeAlpha += kAddAlpha;
	//UIの更新
	m_uiManager->Update(m_actors, m_camera);

	//スコア
	m_controller.GetScoreManager()->Update();
}

void GameScene::FadeDraw()
{
	MV1DrawModel(m_skyHandle);

	//オブジェクトの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}

	//UIの描画
	m_uiManager->Draw();

	//スコア
	m_controller.GetScoreManager()->Draw(kScorePosX, kScorePosY);

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

//トリガー
void GameScene::TriggerCheck()
{
	//当たったかチェックするオブジェクト
	for (auto& actorA : m_actors)
	{
		//死んだオブジェクトならスキップ
		if (actorA->IsDead())continue;
		//何もないならスキップ
		if (actorA->GetCollidable()->GetObjectType() == ObjectType::None)continue;

		//トリガー
		for (auto& actorB : m_actors)
		{
			//死んだオブジェクトならスキップ
			if (actorB->IsDead())continue;
			//同じオブジェクトタイプなら判定をしない
			if (actorA->GetCollidable()->GetObjectType() == actorB->GetCollidable()->GetObjectType())continue;
			//トリガーの当たり判定の情報を取得
			auto trigger = actorB->GetTrigger();
			//なにもないならスキップ
			if (trigger->GetTriggerType() == TriggerType::None)continue;

			//当たっているならtrue;
			bool isHit = false;

			//球との当たり判定
			if (actorA->GetCollidable()->GetCollider()->GetCollKind() == ColliderKind::Sphere)
			{
				//当たっているならtrue
				isHit = m_collChecker->CheckSCollTrigger(actorA->GetCollidable(), actorB->GetTrigger());
			}
			//カプセル
			else if (actorA->GetCollidable()->GetCollider()->GetCollKind() == ColliderKind::Capsule)
			{
				//当たっているならtrue
				isHit = m_collChecker->CheckCCollTrigger(actorA->GetCollidable(), actorB->GetTrigger());
			}
			//ポリゴン
			else if (actorA->GetCollidable()->GetCollider()->GetCollKind() == ColliderKind::Polygon)
			{
				//当たっているならtrue
				isHit = m_collChecker->CheckPCollTrigger(actorA->GetCollidable(), actorB->GetTrigger());
			}

			//当たっているなら
			if (isHit)
			{
				actorB->OnHitTrigger(actorA);
				actorA->BeHitTrigger(actorB);
			}
		}
	}
}

//衝突処理
void GameScene::CollisionCheck()
{
	for (auto& actorA : m_actors)
	{
		//死んだオブジェクトならスキップ
		if (actorA->IsDead())continue;
		//当たり判定の情報を取得
		auto& collA = actorA->GetCollidable();

		//何もないならスキップ
		if (collA->GetObjectType() == ObjectType::None)continue;


		auto kindA = collA->GetCollider()->GetCollKind();

		for (auto& actorB : m_actors)
		{
			//死んだオブジェクトならスキップ
			if (actorB->IsDead())continue;
			//同じオブジェクトと2回以上当たり判定のチェックを行うのを防ぐ
			if (actorB <= actorA)continue;
			//当たり判定の情報を取得
			auto& collB = actorB->GetCollidable();
			//なにもないならスキップ
			if (collB->GetObjectType() == ObjectType::None)continue;

			//当たっているならtrue;
			bool isHit = false;
			//当たり判定の種類
			auto kindB = collB->GetCollider()->GetCollKind();

			//球
			if (kindA == ColliderKind::Sphere)
			{
				//球と球の当たり判定
				if (kindB == ColliderKind::Sphere)
				{
					//当たっているならtrue
					isHit = m_collChecker->CheckCollSS(collA, collB);
					if (isHit)
					{
						//衝突処理
						m_collProcess->CollisionSS(collA, collB);
					}
				}
				//球とカプセル
				else if (kindB == ColliderKind::Capsule)
				{
					//当たっているならtrue
					isHit = m_collChecker->CheckCollCS(collB, collA);//左がカプセルで右が球
					if (isHit)
					{
						//衝突処理
						m_collProcess->CollisionSC(collA, collB);
					}
				}
				//球とポリゴン
				else if (kindB == ColliderKind::Polygon)
				{
					//当たっているならtrue
					isHit = m_collChecker->CheckCollSP(collA, collB);//左が球で右がポリゴン
					if (isHit)
					{
						//衝突処理
						m_collProcess->CollisionSP(collA, collB);
					}
				}
			}
			//カプセル
			else if (kindA == ColliderKind::Capsule)
			{
				//カプセルと球の当たり判定
				if (kindB == ColliderKind::Sphere)
				{
					//当たっているならtrue
					isHit = m_collChecker->CheckCollCS(collA, collB);//左がカプセルで右が球
					if (isHit)
					{
						//衝突処理
						m_collProcess->CollisionSC(collB, collA);
					}
				}
				//カプセル同士
				else if (kindB == ColliderKind::Capsule)
				{
					//当たっているならtrue
					isHit = m_collChecker->CheckCollCC(collA, collB);
				}
			}
			//ポリゴン
			else if (kindA == ColliderKind::Polygon)
			{
				if (kindB == ColliderKind::Sphere)
				{
					//当たっているならtrue
					isHit = m_collChecker->CheckCollSP(collB, collA);//左が球で右がポリゴン
					if (isHit)
					{
						//衝突処理
						m_collProcess->CollisionSP(collB, collA);
					}
				}
			}

			//当たったならそれぞれのイベントを起こす
			if (isHit)
			{
				actorA->OnHitColl(actorB);
				actorB->OnHitColl(actorA);
			}
		}
	}
}
