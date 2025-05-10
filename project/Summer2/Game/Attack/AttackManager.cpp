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
	if (m_attacks.empty())return;//��Ȃ牽�����Ȃ�

	//�U�����������Ă��邩���`�F�b�N����@
}

void AttackManager::SetAttack(std::shared_ptr<AttackBase> attack)
{
	//�U�����Z�b�g
	m_attacks.emplace_back(attack);
}
