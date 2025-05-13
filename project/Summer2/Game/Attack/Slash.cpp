#include "Slash.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include "../Actors/Enemy/EnemyBase.h"
#include <DxLib.h>

#if _DEBUG
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"
#endif

Slash::Slash(std::shared_ptr<Collidable> coll, float damege, int keepFrame) :
	AttackBase(coll, damege, keepFrame),
	m_initDamege(damege),
	m_initKeepFrame(keepFrame)
{
}

Slash::~Slash()
{
	
}

void Slash::Init()
{
	//����������
	m_damege = m_initDamege;
	m_keepFrame = m_initKeepFrame;
	m_isDead = false;
	//ID�̏�����
	if (m_hitEnemyId.size() != 0)
	{
		m_hitEnemyId.clear();
	}
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

void Slash::Draw()
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		32,
		0xff00ff,
		0xff00ff,
		true//���G�̎��͓h��Ԃ����
	);
#endif
}

void Slash::OnHit(std::shared_ptr<Actor> actor)
{
	//�G�ɓ��������ꍇID���L�^����
	if (actor->GetActorKind() == ActorKind::Enemy)
	{
		bool isFind = false;
		//�G��ID�����łɋL�^����Ă��邩�m�F
		for (auto id : m_hitEnemyId)
		{
			if (id == std::dynamic_pointer_cast<EnemyBase>(actor)->GetID())
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			//�L�^����Ă��Ȃ���΋L�^����
			m_hitEnemyId.emplace_back(std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
			//�_���[�W��^����
			actor->GetHurtPoint()->OnHitDamage(m_damege);
#if _DEBUG
			//�U���𓖂Ă����Ƃ��
			printfDx("�U������������ ID = %d\n", std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
#endif
		}
	}

#if _DEBUG
	printf("�U������������");
#endif
}
