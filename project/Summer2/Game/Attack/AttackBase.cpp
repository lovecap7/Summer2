#include "AttackBase.h"

AttackBase::AttackBase(std::shared_ptr<Collidable> coll, float damege, int keepFrame, Actor& owner):
	m_collidable(coll),
	m_damege(damege),
	m_keepFrame(keepFrame),
	m_isDead(false),
	m_owner(&owner)
{
}
