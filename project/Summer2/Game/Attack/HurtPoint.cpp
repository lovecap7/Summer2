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
	m_isHit = false; //�U�����󂯂Ă��Ȃ���Ԃɂ���
}

void HurtPoint::OnHitDamage(int damage)
{
	//���G�Ȃ�
	if (m_isNoDamage)return;
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
	else
	{
		m_isHit = true; //�U�����󂯂�
	}
}

void HurtPoint::OnHitKnockBack(const Vector3& knockBackVec)
{
	m_owner->GetCollidable()->GetRb()->SetMoveVec(knockBackVec);
}
