#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "BomberStateDeath.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|HitReact";//小喰らい
}

BomberStateHit::BomberStateHit(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner)
{
	//やられ
	m_owner->GetModel()->SetAnim(kAnim, false);
}

BomberStateHit::~BomberStateHit()
{
}

void BomberStateHit::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}

	//やられリアクション中に攻撃を食らったらアニメーションを初めから
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//リプレイ
		m_owner->GetModel()->ReplayAnim();
	}

	//モデルのアニメーションが終わったら
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}

	//減速
	SpeedDown();
}

void BomberStateHit::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
