#include "HurtPoint.h"
#include "../Actors/Actor.h"
#include "../../General/Math/MyMath.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"

HurtPoint::HurtPoint(std::shared_ptr<Collidable> coll, int hp, Actor& owner):
	m_collidable(coll),
	m_isNoDamage(false),
	m_hp(hp),
	m_isDead(false),
	m_owner(&owner)
{
}

HurtPoint::~HurtPoint()
{

}

void HurtPoint::OnHitDamage(int damage)
{
	//無敵なら
	if (m_isNoDamage)return;
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isDead = true;//死亡
	}
	else
	{
		m_owner->HitReaction();//やられリアクション
	}
}

void HurtPoint::OnHitKnockBack(const std::shared_ptr<Collidable>& other)
{
	Vector3 knockBackVec = m_owner->GetCollidable()->GetRb()->GetNextPos() - other->GetRb()->GetNextPos();//自分へのベクトル
	knockBackVec.y = 0.0f;//Y成分はなし
	knockBackVec = knockBackVec.Normalize() * 10.0f;//ノックバック
	m_owner->GetCollidable()->GetRb()->SetVec(knockBackVec);
}
