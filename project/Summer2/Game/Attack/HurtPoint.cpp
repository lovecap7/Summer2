#include "HurtPoint.h"

HurtPoint::HurtPoint(std::shared_ptr<Collidable> coll, int hp):
	m_collidable(coll),
	m_isNoDamage(false),
	m_hp(hp),
	m_isDead(false)
{
}

HurtPoint::~HurtPoint()
{
}

void HurtPoint::OnHitDamage(int damage)
{
	//���G�Ȃ�
	if (m_isNoDamage)return;
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isDead = true;//���S
	}
}
