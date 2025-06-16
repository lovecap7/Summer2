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

void AttackManager::Entry(std::shared_ptr<AttackBase> attack)
{
	//���łɓo�^����Ă���Ȃ�Â��U��������
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it != m_attacks.end())
	{
		//�폜
		m_attacks.erase(it);
	}

	//�U�����Z�b�g
	m_attacks.emplace_back(attack);
}

void AttackManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	if (m_attacks.empty())return;//��Ȃ牽�����Ȃ�

	//�U���̍X�V����
	for (auto& attack : m_attacks)
	{
		attack->Update();
	}

	//�������U������̃C�e���[�^���擾
	auto remIt = std::remove_if(m_attacks.begin(), m_attacks.end(), [](std::shared_ptr<AttackBase> attack) {return attack->IsDead();});
	m_attacks.erase(remIt, m_attacks.end());//�폜

	for (auto& actor : actors)
	{
		//�v���C���[�ƓG�̂�
		if (actor->GetActorKind() == ActorKind::Player ||
			actor->GetActorKind() == ActorKind::Enemy)
		{
			actor->GetHurtPoint()->Init();//���ꔻ��̏�����
		}
	}
	//�U�����������Ă��邩�`�F�b�N
	for (auto& attack : m_attacks)
	{
		//�U�����������Ă��邩���`�F�b�N����@
		for (auto& actor : actors)
		{
			//�v���C���[�ƓG�̂�
			if (actor->GetActorKind() == ActorKind::Player ||
				actor->GetActorKind() == ActorKind::Enemy)
			{
				if (actor->GetHurtPoint()->IsNoDamege())continue;//���G�̎��͓�����Ȃ�
				//�������Ă邩���`�F�b�N
				bool isHit = false;
				//�U���̃R���C�_�u��
				auto attackCollidable = attack->GetCollidable();
				//���ꔻ��
				auto actorHurtPointCollidable = actor->GetHurtPoint()->GetCollidable();

				//����
				if (attackCollidable->GetColl()->GetShape() == Shape::Sphere)
				{
					//��
					if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSS(attackCollidable, actorHurtPointCollidable);
					}
					//�J�v�Z��
					else if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCS(actorHurtPointCollidable, attackCollidable);
					}
				}
				//�J�v�Z����
				else if (attackCollidable->GetColl()->GetShape() == Shape::Capsule)
				{
					//��
					if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollCS(attackCollidable, actorHurtPointCollidable);
					}
					//�J�v�Z��
					else if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCC(attackCollidable, actorHurtPointCollidable);
					}
				}

				//�������Ă�Ȃ�
				if (isHit)
				{
					//�����������̏���
					attack->OnHit(actor);
				}
			}
		}
	}
}

void AttackManager::Draw() const
{
	if (m_attacks.empty())return;//��Ȃ牽�����Ȃ�

	//�U���̕`��
	for (auto& attack : m_attacks)
	{
		attack->Draw();
	}
}
