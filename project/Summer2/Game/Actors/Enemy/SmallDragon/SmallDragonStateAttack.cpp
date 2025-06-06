#include "SmallDragonStateAttack.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/BulletAttack.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//弾の半径の大きさ
	constexpr float kBulletRadius = 20.0f;
	//弾のダメージ
	constexpr int kBulletDamage = 100;
	//弾の持続フレーム
	constexpr int kBulletKeepFrame = 180;
	//弾の発生フレーム
	constexpr int kBulletFireFrame = 30;
	//弾のスピード
	constexpr float kBulletSpeed = 2.0f;

	//アニメーション
	const char* kAnim = "CharacterArmature|Headbutt";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.3f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 150;//2.5秒くらいの感覚で攻撃
}

SmallDragonStateAttack::SmallDragonStateAttack(std::shared_ptr<SmallDragon> owner) :
	SmallDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃
	m_owner->GetCollidable()->SetState(State::None);
	//攻撃
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//モデルの向きをプレイヤーに向ける
	m_owner->GetModel()->SetDir(m_owner->GetPlayerNomVecXZ().ToDxLibVector());
}

SmallDragonStateAttack::~SmallDragonStateAttack()
{
	//攻撃判定を消す
	//m_attack->Delete();
	//攻撃のクールタイム
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}
void SmallDragonStateAttack::Init()
{
	// 次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡状態
		//ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHit())
	{
		//やられ状態
		//ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
		return;
	}
	
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kBulletFireFrame)
	{
		//攻撃判定の準備
		CreateAttack();
		//攻撃を入れる
		AppearAttack(m_attack, attackManager);
	}
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
	}

	//減速
	SpeedDown();
}

void SmallDragonStateAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//生成位置
	Vector3 bulletPos = m_owner->GetCollidable()->GetRb()->GetPos();
	bulletPos.y += 100.0f;
	//弾の当たり判定作成
	auto coll = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kBulletRadius),
		std::make_shared<Rigidbody>(bulletPos));
	//弾の座標と当たり判定を攻撃に紐図ける
	m_attack = std::make_shared<BulletAttack>(coll, kBulletDamage, kBulletKeepFrame, m_owner);
	//弾の進行方向をスピード
	m_attack->SetDirAndSpeed(model->GetDir(), kBulletSpeed);
}

void SmallDragonStateAttack::UpdateAttack()
{
	//なし
}

void SmallDragonStateAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
