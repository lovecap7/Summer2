#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>

#if _DEBUG
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"
#endif

MeleeAttack::MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, std::shared_ptr<Actor> owner) :
	AttackBase(coll, damage, keepFrame, owner)
{
}

MeleeAttack::~MeleeAttack()
{

}

void MeleeAttack::Init()
{
	//����������
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDead = false;
	//ID�̏�����
	if (m_hitId.size() != 0)
	{
		m_hitId.clear();
	}
}

void MeleeAttack::Update()
{
	//�����t���[�������炷
	m_keepFrame--;
	//�����t���[����0�ɂȂ��������
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void MeleeAttack::Draw()
{
#if _DEBUG
	if(m_collidable->GetColl()->GetShape() == Shape::Capsule)
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

void MeleeAttack::OnHit(std::shared_ptr<Actor> actor)
{
	//�����Ɠ�����ނ̃A�N�^�[�Ȃ疳��
	if (m_owner->GetActorKind() == actor->GetActorKind())return;

	bool isFind = false;
	//ID�����łɋL�^����Ă��邩�m�F
	for (auto id : m_hitId)
	{
		if (id == actor->GetID())
		{
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		//�L�^����Ă��Ȃ���΋L�^����
		m_hitId.emplace_back(actor->GetID());
		//�_���[�W��^����
		actor->GetHurtPoint()->OnHitDamage(m_damage);
		//�m�b�N�o�b�N
		Vector3 knockBackVec = actor->GetCollidable()->GetRb()->GetNextPos() - m_owner->GetCollidable()->GetRb()->GetNextPos();//�����x�N�g��
		knockBackVec.y = 0.0f;//Y�����͂Ȃ�
		knockBackVec = knockBackVec.Normalize() * 2.0f;//�m�b�N�o�b�N
		actor->GetHurtPoint()->OnHitKnockBack(knockBackVec);
	}
}
