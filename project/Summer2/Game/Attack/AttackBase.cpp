#include "AttackBase.h"

AttackBase::AttackBase(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner):
	m_collidable(coll),
	m_damage(damage),
	m_keepFrame(keepFrame),
	m_knockBackPower(knockBackPower),
	m_isDelete(false),
	m_owner(owner),
	m_initDamage(damage),
	m_initKeepFrame(keepFrame)
{
}

void AttackBase::SetDamage(int damage)
{
	m_initDamage = damage;
	m_damage = damage;
}

void AttackBase::SetKeepFrame(int keepFrame)
{
	m_initKeepFrame = keepFrame;
	m_keepFrame = keepFrame;
}
