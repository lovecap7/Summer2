#include "Strike.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include "../Actors/Enemy/EnemyBase.h"
#include <DxLib.h>

#if _DEBUG
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"
#endif

Strike::Strike(std::shared_ptr<Collidable> coll, float damage, int keepFrame, Actor& owner):
	AttackBase(coll, damage, keepFrame, owner)
{
}

Strike::~Strike()
{
}

void Strike::Init()
{
	//����������
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDead = false;
	//ID�̏�����
	if (m_hitEnemyId.size() != 0)
	{
		m_hitEnemyId.clear();
	}
}

void Strike::Update()
{
	//�����t���[�������炷
	m_keepFrame--;
	//�����t���[����0�ɂȂ��������
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void Strike::Draw()
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		32,
		0xfff0ff,
		0xfff0ff,
		true//���G�̎��͓h��Ԃ����
	);
#endif
}

void Strike::OnHit(std::shared_ptr<Actor> actor)
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
			actor->GetHurtPoint()->OnHitDamage(m_damage);
			//�m�b�N�o�b�N
			Vector3 knockBackVec = actor->GetCollidable()->GetRb()->GetNextPos() - m_owner->GetCollidable()->GetRb()->GetNextPos();//�����x�N�g��
			knockBackVec.y = 0.0f;//Y�����͂Ȃ�
			knockBackVec = knockBackVec.Normalize() * 2.0f;//�m�b�N�o�b�N
			actor->GetHurtPoint()->OnHitKnockBack(knockBackVec);

#if _DEBUG
			//�U���𓖂Ă����Ƃ��
			printfDx("�Ō����������� ID = %d\n", std::dynamic_pointer_cast<EnemyBase>(actor)->GetID());
#endif
		}
	}
}
