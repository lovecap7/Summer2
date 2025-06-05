#include "BulletAttack.h"
#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>

#if _DEBUG
#include "../../General/Rigidbody.h"
#endif

BulletAttack::BulletAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, std::shared_ptr<Actor> owner):
	AttackBase(coll, damage, keepFrame, owner)
{
}

BulletAttack::~BulletAttack()
{
}

void BulletAttack::Init()
{
	//‰Šú‰»ˆ—
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDead = false;
	//ID‚Ì‰Šú‰»
	if (m_hitId.size() != 0)
	{
		m_hitId.clear();
	}
}

void BulletAttack::Update()
{
}

void BulletAttack::Draw()
{
}

void BulletAttack::OnHit(std::shared_ptr<Actor> actor)
{
}
