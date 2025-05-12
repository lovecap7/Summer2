#include "AttackManager.h"
#include "AttackBase.h"
#include "../../General/Collision/CollisionChecker.h"
#include "../../General/Collidable.h"
#include "../../General/Collision/ColliderBase.h"
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

	//�������U������̃C�e���[�^���擾
	auto remIt = std::remove_if(m_attacks.begin(), m_attacks.end(), [](std::shared_ptr<AttackBase> attack) {return attack->IsDead();});
	m_attacks.erase(remIt, m_attacks.end());//�폜

	//�U�����������Ă��邩���`�F�b�N����@
	for (auto& actor : actors)
	{
		//�U�����󂯂Ȃ��A�N�^�[�͓����蔻������Ȃ�
		if (actor->GetActorKind() == ActorKind::None)continue;
		if (actor->GetActorKind() == ActorKind::Field)continue;
		if (actor->GetActorKind() == ActorKind::Obstacle)continue;
		if (actor->GetActorKind() == ActorKind::Item)continue;
		if (actor->GetHurtPoint()->IsNoDamege())continue;//���G�̎��͓�����Ȃ�

		//�U�����������Ă��邩�`�F�b�N
		for (auto& attack : m_attacks)
		{
			//�������Ă邩���`�F�b�N
			bool isHit = false;

			//����
			if (attack->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
			{
				//��
				if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollSS(attack->GetCollidable(), actor->GetHurtPoint()->GetCollidable());
				}
				//�J�v�Z��
				else if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCS(actor->GetHurtPoint()->GetCollidable(), attack->GetCollidable());
				}
			}
			//�J�v�Z����
			else if (attack->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
			{
				//��
				if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollCS(attack->GetCollidable(), actor->GetHurtPoint()->GetCollidable());
				}
				//�J�v�Z��
				else if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCC(attack->GetCollidable(), actor->GetHurtPoint()->GetCollidable());
				}
			}

			//�������Ă�Ȃ�
			if (isHit)
			{
				//�����������̏���
				actor->GetHurtPoint()->OnHitDamage(attack->GetDamege());//�_���[�W��^����
				attack->OnHit(actor);//�U�����̓����������̏���
			}
		}
	}
}

void AttackManager::SetAttack(std::shared_ptr<AttackBase> attack)
{
	//�U�����Z�b�g
	m_attacks.emplace_back(attack);
}
