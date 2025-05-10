#include "AttackManager.h"
#include "AttackBase.h"
#include "../../General/Collision/CollisionChecker.h"
#include "HurtPoint.h"

AttackManager::AttackManager():
	m_collChecker(std::make_shared<CollisionChecker>())
{
}

AttackManager::~AttackManager()
{
}

void AttackManager::Update()
{
	if (m_attacks.empty())return;//空なら何もしない

	//攻撃が当たっているかをチェックする　
}

void AttackManager::SetAttack(std::shared_ptr<AttackBase> attack)
{
	//攻撃をセット
	m_attacks.emplace_back(attack);
}
