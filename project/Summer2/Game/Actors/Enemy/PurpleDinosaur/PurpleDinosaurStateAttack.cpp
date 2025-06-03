#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/MeleeAttack.h"
namespace
{
	//左腕と左手のインデックス
	constexpr int kLeftArmIndex = 13;
	constexpr int kLeftHandIndex = 17;
	//左腕の当たり判定の大きさ(攻撃の大きさ)
	constexpr float kLeftArmRadius = 10.0f;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = 20;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 10;
	//アニメーション
	const char* kAnim = "CharacterArmature|Weapon";
}

PurpleDinosaurStateAttack::PurpleDinosaurStateAttack(std::shared_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃1
	owner->GetCollidable()->SetState(State::None);
	//攻撃1
	owner->GetModel()->SetAnim(kAnim, false);
	//攻撃判定の準備
	CreateAttack();
}

PurpleDinosaurStateAttack::~PurpleDinosaurStateAttack()
{
	//攻撃判定を消す
	m_attack->Delete();
}

void PurpleDinosaurStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡状態
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//攻撃を入れる
		AppearAttack(m_attack, attackManager);
	}
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
	}

	//減速
	SpeedDown();
}

void PurpleDinosaurStateAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	m_leftArm = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(leftArm.x, leftArm.y, leftArm.z), kLeftArmRadius),
		std::make_shared<Rigidbody>(Vector3(leftHand.x, leftHand.y, leftHand.z)));
	//武器の座標と当たり判定を攻撃に紐図ける
	m_attack = std::make_shared<MeleeAttack>(m_leftArm, kAttackDamage, kAttackKeepFrame, m_owner);
}

void PurpleDinosaurStateAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	//左腕を形成するカプセルの座標をセット
	m_leftArm->GetRb()->SetPos(Position3(leftArm.x, leftArm.y, leftArm.z));
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->SetEndPos(Position3(leftHand.x, leftHand.y, leftHand.z));
}

void PurpleDinosaurStateAttack::SpeedDown()
{
}
