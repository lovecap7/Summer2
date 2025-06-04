#include "HurtPoint.h"
#include "../Actors/Actor.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"

HurtPoint::HurtPoint(std::shared_ptr<Collidable> coll, int hp, std::shared_ptr<Actor> owner):
	m_collidable(coll),
	m_isNoDamage(false),
	m_hp(hp),
	m_owner(owner),
	m_isHit(false)
{
}

HurtPoint::~HurtPoint()
{

}

void HurtPoint::Init()
{
	m_isHit = false; //UŒ‚‚ğó‚¯‚Ä‚¢‚È‚¢ó‘Ô‚É‚·‚é
}

void HurtPoint::OnHitDamage(int damage)
{
	//–³“G‚È‚ç
	if (m_isNoDamage)return;
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
	else
	{
		m_isHit = true; //UŒ‚‚ğó‚¯‚½
	}
}

void HurtPoint::OnHitKnockBack(const Vector3& knockBackVec)
{
	m_owner->GetCollidable()->GetRb()->SetMoveVec(knockBackVec);
}
