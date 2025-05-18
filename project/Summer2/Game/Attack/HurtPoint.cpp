#include "HurtPoint.h"
#include "../Actors/Actor.h"
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
	//–³“G‚È‚ç
	if (m_isNoDamage)return;
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isDead = true;//Ž€–S
	}
	else
	{
		m_owner->HitReaction();//‚â‚ç‚êƒŠƒAƒNƒVƒ‡ƒ“
	}
	printfDx("‚í‚¢‚ÌHP = %d\n", m_hp);
}

void HurtPoint::OnHitKnockBack(const Vector3& knockBackVec)
{
	m_owner->GetCollidable()->GetRb()->SetMoveVec(knockBackVec);
}
