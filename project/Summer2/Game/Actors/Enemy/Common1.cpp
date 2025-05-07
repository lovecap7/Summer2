#include "Common1.h"
#include <memory>
#include "../../../General/Model.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/SphereCollider.h"

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,100.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 20.0f; //カプセルの半径
	//トリガーの半径
	constexpr float kSearchTriggerRadius = 500.0f;
	//プレイヤーを追いかける距離
	constexpr float kRunDistance = 200.0f;
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 5.0f;
	//プレイヤーを攻撃距離
	constexpr float kAttackDistance = 100.0f;
	//攻撃のクールタイム
	constexpr int kAttackCoolTime = 180;

	//重力
	constexpr float kMaxGravity = -10.0f;//落下スピードが大きくなりすぎないように
	//減速
	constexpr float kMoveDeceRate = 0.8f;

	//アニメーションの名前
	const char* kIdleAnim = "CharacterArmature|Idle";//待機
	const char* kWalkAnim = "CharacterArmature|Walk";//歩く
	const char* kRunAnim = "CharacterArmature|Run";//走る
	const char* kAttackAnim = "CharacterArmature|Weapon";//攻撃
	const char* kSmallHitAnim = "CharacterArmature|HitReact";//小喰らい
	const char* kBigHitAnim = "CharacterArmature|Death";//大喰らい
}

Common1::Common1(int modelHandle, Vector3 pos):
	m_update(&Common1::IdleUpdate),
	m_lastUpdate(&Common1::AttackUpdate),
	m_isBattleMode(false),
	m_isHitSearch(false),
	m_attackCoolTime(kAttackCoolTime)
{
	//モデルの初期化
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//モデルの大きさ調整
	m_model->SetScale(VGet(0.5f, 0.5f, 0.5f));
	//初期位置
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(pos));
	//トリガー
	m_searchTrigger = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kSearchTriggerRadius), std::make_shared<Rigidbody>(pos));
}

Common1::~Common1()
{
}

void Common1::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//状態に合わせて初期化
	StateInit();
	//更新
	(this->*m_update)(input, camera);
	//アニメーションの更新
	m_model->Update();
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
	DrawSphere3D(
		m_searchTrigger->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_searchTrigger->GetColl())->GetRadius(),
		16,
		0xff00ff,
		0xff00ff,
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
	//探知したのでtrue
	m_isHitSearch = true;
	//距離をチェック
	Vector3 dist = playerPos - m_searchTrigger->GetRb()->GetPos();
	//遠いなら
	if (dist.Magnitude() > kRunDistance)
	{
		//攻撃中じゃなければ
		if (m_update != &Common1::AttackUpdate)
		{
			//移動
			dist.y = 0.0f;
			m_collidable->GetRb()->SetMoveVec(dist.Normalize() * kChaseSpeed);
			//状態変化
			if (m_update != &Common1::MoveUpdate)
			{
				m_update = &Common1::MoveUpdate;
			}
		}
	}
	//攻撃の範囲内なら
	else if (dist.Magnitude() > kAttackDistance)
	{
		//戦闘状態
		m_isBattleMode = true;
		//状態変化
		if (m_update != &Common1::AttackUpdate)
		{
			//向きの更新
			m_model->SetDir(VGet(dist.Normalize().x, 0.0f, dist.Normalize().z));
		}
	}
}

void Common1::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//攻撃のクールタイム
	--m_attackCoolTime;
	if (m_attackCoolTime <= 0 && m_isBattleMode)
	{
		m_attackCoolTime = kAttackCoolTime;
		m_update = &Common1::AttackUpdate;
		return;
	}
	//減速
	SpeedDown();
}

void Common1::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//戦闘状態
	if (m_isBattleMode || !m_isHitSearch)
	{
		m_update = &Common1::IdleUpdate;
		return;
	}
	//減速
	SpeedDown();
	//向きの更新
	m_model->SetDir(VGet(m_collidable->GetRb()->GetVec().x, 0.0f, m_collidable->GetRb()->GetVec().y));
}

void Common1::AttackUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//アニメーション終了後
	if (m_model->IsFinishAnim())
	{
		m_update = &Common1::IdleUpdate;
		return;
	}

	//減速
	SpeedDown();
}

void Common1::HitUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
}

void Common1::StateInit()
{
	//状態が変わっていないなら早期リターン
	if (m_lastUpdate == m_update)return;
	if (m_update == &Common1::IdleUpdate)
	{
		//待機状態
		m_model->SetAnim(kIdleAnim, true);
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Common1::MoveUpdate)
	{
		//走る
		m_model->SetAnim(kRunAnim, true);
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Common1::AttackUpdate)
	{
		m_collidable->SetState(State::None);
		//攻撃
		m_model->SetAnim(kAttackAnim, false);
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