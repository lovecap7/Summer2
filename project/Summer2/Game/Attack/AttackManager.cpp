#include "AttackManager.h"
#include "AttackBase.h"
#include "../../General/Collision/CollisionChecker.h"
#include "HurtPoint.h"
#include "../Actors/Actor.h"

AttackManager::AttackManager():
	m_collChecker(std::make_shared<CollisionChecker>())
{
}

AttackManager::~AttackManager()
{
}

void AttackManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	if (m_attacks.empty())return;//��Ȃ牽�����Ȃ�

	//�������U��������폜
	std::erase_if(m_attacks, [](AttackBase attack) {return attack.IsDead();});//C++20����̊֐� �����ɍ������̂�z��̒�����폜

	//�U�����������Ă��邩���`�F�b�N����@
	for (auto& actor : actors)
	{
		//�U�����󂯂Ȃ��A�N�^�[�͓����蔻������Ȃ�
		if (actor->GetActorKind() == ActorKind::None)continue;
		if (actor->GetActorKind() == ActorKind::Field)continue;
		if (actor->GetActorKind() == ActorKind::Obstacle)continue;
		if (actor->GetActorKind() == ActorKind::Item)continue;

		//�U�����������Ă��邩�`�F�b�N
		for (auto& attack : m_attacks)
		{

		}
	}
}

void AttackManager::SetAttack(std::shared_ptr<AttackBase> attack)
{
	//�U�����Z�b�g
	m_attacks.emplace_back(attack);
}
