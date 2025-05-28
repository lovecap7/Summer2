#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include "../Actors/Enemy/EnemyBase.h"
#include <DxLib.h>

#if _DEBUG
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"
#endif

MeleeAttack::MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, Actor& owner) :
	AttackBase(coll, damage, keepFrame, owner)
{
}

MeleeAttack::~MeleeAttack()
{

}

void MeleeAttack::Init()
{
	//初期化処理
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDead = false;
	//IDの初期化
	if (m_hitEnemyId.size() != 0)
	{
		m_hitEnemyId.clear();
	}
}

void MeleeAttack::Update()
{
	//持続フレームを減らす
	m_keepFrame--;
	//持続フレームが0になったら消滅
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void MeleeAttack::Draw()
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		32,
		0xff00ff,
		0xff00ff,
		true//無敵の時は塗りつぶされる
	);
#endif
}

void MeleeAttack::OnHit(std::shared_ptr<Actor> actor)
{
	//敵に当たった場合IDを記録する
	if (actor->GetActorKind() == ActorKind::Enemy)
	{
		bool isFind = false;
		//敵のIDがすでに記録されているか確認
		for (auto id : m_hitEnemyId)
		{
			if (id == std::dynamic_pointer_cast<EnemyBase>(actor)->GetID())
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			//記録されていなければ記録する
			m_hitEnemyId.emplace_back(std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
			//ダメージを与える
			actor->GetHurtPoint()->OnHitDamage(m_damage);
			//ノックバック
			Vector3 knockBackVec = actor->GetCollidable()->GetRb()->GetNextPos() - m_owner->GetCollidable()->GetRb()->GetNextPos();//離れるベクトル
			knockBackVec.y = 0.0f;//Y成分はなし
			knockBackVec = knockBackVec.Normalize() * 2.0f;//ノックバック
			actor->GetHurtPoint()->OnHitKnockBack(knockBackVec);

#if _DEBUG
			//攻撃を当てたことを報告
			printfDx("斬撃が当たった ID = %d\n", std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
#endif
		}
	}
}
