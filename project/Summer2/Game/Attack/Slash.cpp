#include "Slash.h"
#include "../Actors/Actor.h"
#include "../Actors/Enemy/EnemyBase.h"
Slash::Slash(std::shared_ptr<Collidable> coll, float& damege, int& keepFrame):
	AttackBase(coll, damege, keepFrame)
{
}

Slash::~Slash()
{
}

void Slash::Update()
{
	//�����t���[�������炷
	m_keepFrame--;
	//�����t���[����0�ɂȂ��������
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void Slash::OnHit(std::shared_ptr<Actor> actor)
{
	//�G�ɓ��������ꍇID���L�^����
	if (actor->GetActorKind() == ActorKind::Enemy)
	{
		m_hitEnemyId.emplace_back(std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
	}

#if _DEBUG
	printf("�U������������");
#endif
}
