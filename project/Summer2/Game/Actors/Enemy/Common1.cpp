#include "Common1.h"
#include "../../EnemyManager.h"
#include <memory>
#include "../../../General/Model.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/MeleeAttack.h"

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 40.0f; //カプセルの半径
	//トリガーの半径
	constexpr float kSearchTriggerRadius = 500.0f;
	//プレイヤーを追いかける距離
	constexpr float kRunDistance = 150.0f;
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 5.0f;
	
	//体力
	constexpr int kHp = 500;
	//攻撃のクールタイム
	constexpr int kAttackCoolTime = 180;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = 3;

	//重力
	constexpr float kMaxGravity = -10.0f;//落下スピードが大きくなりすぎないように
	//減速
	constexpr float kMoveDeceRate = 0.8f;

	//左腕と左手のインデックス
	constexpr int kLeftArmIndex = 13;
	constexpr int kLeftHandIndex = 17;
	//左腕の当たり判定の大きさ(攻撃の大きさ)
	constexpr float kLeftArmRadius = 10.0f;

	//アニメーションの名前
	const char* kIdleAnim = "CharacterArmature|Idle";//待機
	const char* kWalkAnim = "CharacterArmature|Walk";//歩く
	const char* kRunAnim = "CharacterArmature|Run";//走る
	const char* kAttackAnim = "CharacterArmature|Weapon";//攻撃
	const char* kHitAnim = "CharacterArmature|HitReact";//小喰らい
	const char* kDeadAnim = "CharacterArmature|Death";//大喰らい
}

Common1::Common1(std::unique_ptr<EnemyManager>& enemyManager, int modelHandle, Vector3 pos):
	EnemyBase(enemyManager),
	m_update(&Common1::UpdateIdle),
	m_lastUpdate(&Common1::UpdateAttack),
	m_isBattleMode(false),
	m_isHitSearch(false),
	m_attackCoolTime(kAttackCoolTime)
{
	//モデルの初期化
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//モデルの大きさ調整
	m_model->SetScale(VGet(0.5f, 0.5f, 0.5f));
	//衝突判定
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(pos));
	//やられ判定(衝突判定と同じにする)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, kHp, *this);
	//索敵範囲
	m_searchTrigger = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kSearchTriggerRadius), std::make_shared<Rigidbody>(pos));
	//攻撃の判定
	CreateAttack();
}

Common1::~Common1()
{
}

void Common1::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//状態に合わせて初期化
	InitState();
	//更新
	(this->*m_update)(input, camera);
	//アニメーションの更新
	m_model->Update();
	//戦闘に関する更新処理
	BattleUpdate();
}

void Common1::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= kMaxGravity)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Common1::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void Common1::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
	//やられ判定
	DrawCapsule3D(
		m_hurtPoint->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetRadius(),
		32,
		0x0000ff,
		0x0000ff,
		m_hurtPoint->IsNoDamege()//無敵の時は塗りつぶされる
	);
	//索敵範囲
	DrawSphere3D(
		m_searchTrigger->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_searchTrigger->GetColl())->GetRadius(),
		16,
		0xff00ff,
		0xff00ff,
		false
	);
	//左腕
	DrawCapsule3D(
		m_leftArm->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->GetRadius(),
		16,
		0xffff00,
		0xffff00,
		false
	);
#endif
	m_model->Draw();
}

void Common1::Complete()
{
	//コライダー
	m_collidable->GetRb()->SetNextPos();//次の座標へ
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//カプセルの移動
	//プレイヤーを探すトリガー
	m_searchTrigger->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());

	//戦闘状態解除
	m_isBattleMode = false;
	//探知解除
	m_isHitSearch = false;
}

void Common1::OnHitSearch(const Vector3& playerPos)
{
	if (m_update == &Common1::UpdateDead)return;//死亡中は無視
	if (m_update == &Common1::UpdateHit)return;//やられ中は無視
	//探知したのでtrue
	m_isHitSearch = true;
	//距離をチェック
	Vector3 dist = playerPos - m_searchTrigger->GetRb()->GetPos();
	//遠いなら
	if (dist.Magnitude() > kRunDistance)
	{
		//攻撃中じゃなければ
		if (m_update != &Common1::UpdateAttack)
		{
			//移動
			dist.y = 0.0f;
			m_collidable->GetRb()->SetMoveVec(dist.Normalize() * kChaseSpeed);
			//状態変化
			if (m_update != &Common1::UpdateMove)
			{
				m_update = &Common1::UpdateMove;
			}
		}
	}
	//攻撃の範囲内なら
	else
	{
		//戦闘状態
		m_isBattleMode = true;
		//状態変化
		if (m_update != &Common1::UpdateAttack)
		{
			//向きの更新
			m_model->SetDir(VGet(dist.Normalize().x, 0.0f, dist.Normalize().z));
		}
	}
}

void Common1::HitReaction()
{
	printfDx("ID:%dの敵のHP = %d\n", m_id,m_hurtPoint->GetHp());
	m_collidable->SetState(State::None);
	//アニメーションを削除
	m_model->DeleteAnim();
	//やられ
	m_model->SetAnim(kHitAnim, false);
	//やられリアクション
	m_update = &Common1::UpdateHit;
	return;
}

void Common1::UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//死んでるなら
	if (m_hurtPoint->IsDead())
	{
		m_update = &Common1::UpdateDead;
		return;
	}

	//攻撃のクールタイム
	--m_attackCoolTime;
	if (m_attackCoolTime <= 0 && m_isBattleMode)
	{
		m_attackCoolTime = kAttackCoolTime;
		m_update = &Common1::UpdateAttack;
		return;
	}
	//減速
	SpeedDown();
}

void Common1::UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//死んでるなら
	if (m_hurtPoint->IsDead())
	{
		m_update = &Common1::UpdateDead;
		return;
	}

	//戦闘状態
	if (m_isBattleMode || !m_isHitSearch)
	{
		m_update = &Common1::UpdateIdle;
		return;
	}
	//減速
	SpeedDown();
	//向きの更新
	m_model->SetDir(VGet(m_collidable->GetRb()->GetVec().x, 0.0f, m_collidable->GetRb()->GetVec().z));
}

void Common1::UpdateAttack(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//死んでるなら
	if (m_hurtPoint->IsDead())
	{
		m_update = &Common1::UpdateDead;
		return;
	}

	//アニメーション終了後
	if (m_model->IsFinishAnim())
	{
		m_update = &Common1::UpdateIdle;
		return;
	}

	//減速
	SpeedDown();
}

void Common1::UpdateHit(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//死んでるなら
	if (m_hurtPoint->IsDead())
	{
		m_update = &Common1::UpdateDead;
		return;
	}

	//アニメーション終了後
	if (m_model->IsFinishAnim())
	{
		m_update = &Common1::UpdateIdle;
		return;
	}

	//減速
	SpeedDown();
}

void Common1::UpdateDead(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//アニメーション終了後
	if (m_model->IsFinishAnim())
	{
		m_isDead = true;//死亡
	}
	//減速
	SpeedDown();
}

void Common1::InitState()
{
	//状態が変わっていないなら早期リターン
	if (m_lastUpdate == m_update)return;
	if (m_update == &Common1::UpdateIdle)
	{
		//待機状態
		m_model->SetAnim(kIdleAnim, true);
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Common1::UpdateMove)
	{
		//走る
		m_model->SetAnim(kRunAnim, true);
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Common1::UpdateAttack)
	{
		m_collidable->SetState(State::None);
		//攻撃
		m_model->SetAnim(kAttackAnim, false);
	}
	else if (m_update == &Common1::UpdateDead)
	{
		m_collidable->SetState(State::Dead);
		//死亡
		m_model->SetAnim(kDeadAnim, false);
	}
	m_lastUpdate = m_update;
}

void Common1::SpeedDown()
{
	//減速
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	m_collidable->GetRb()->SetVec(vec);
}

void Common1::BattleUpdate()
{
	//やられ判定の位置更新
	UpdateHurtPoint();
	//左腕の位置の更新
	UpdateLeftArm();
}

void Common1::UpdateHurtPoint()
{
	//移動量を取得
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//座標更新
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}

void Common1::CreateLeftArm()
{
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(m_model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(m_model->GetModelHandle(), kLeftHandIndex);//手の指先
	m_leftArm = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(leftArm.x, leftArm.y, leftArm.z), kLeftArmRadius), 
		std::make_shared<Rigidbody>(Vector3(leftHand.x, leftHand.y, leftHand.z)));
}

void Common1::UpdateLeftArm()
{
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(m_model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(m_model->GetModelHandle(), kLeftHandIndex);//手の指先
	//左腕を形成するカプセルの座標をセット
	m_leftArm->GetRb()->SetPos(Position3(leftArm.x, leftArm.y, leftArm.z));
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->SetEndPos(Position3(leftHand.x, leftHand.y, leftHand.z));
}

void Common1::CreateAttack()
{
	//左腕の判定を作る
	CreateLeftArm();
	//攻撃の判定と左腕の判定を同じにする
	m_punch = std::make_shared<MeleeAttack>(m_leftArm, kAttackDamage, kAttackKeepFrame, *this);
}
