#include "CollisionProcess.h"
#include "../Collidable.h"
#include "../Rigidbody.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include <vector>

CollisionProcess::CollisionProcess():
	m_wallNum(0),
	m_floorNum(0),
{
}

CollisionProcess::~CollisionProcess()
{
}

void CollisionProcess::ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//A����B�ւ̃x�N�g��
	Vector3 aToB = otherB->GetRb()->GetNextPos() - otherA->GetRb()->GetNextPos();
	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (otherA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider> (otherB->GetColl())->GetRadius();

	//���������̂Ƃ�������Ȃ����̂Ƃŏ����𕪂���
	if (otherA->IsStatic())
	{
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else if (otherA->IsStatic())
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
	}
}

void CollisionProcess::ProcessSP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//�ǂƏ��Ƃ̃t���O���Z�b�g
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->ResetHitFlag();
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//�ǃ|���S���Ə��|���S���̐���������
	int wallNum = 0;
	int floorNum = 0;
	//�ǂƏ��|���S����ۑ�����z��
	std::vector<MV1_COLL_RESULT_POLY*> wall;
	std::vector<MV1_COLL_RESULT_POLY*> floor;

	//���������|���S���̏��
	auto hitDim = std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->GetHitDim();

	//���̍��W
	Position3 sPos = otherA->GetRb()->GetNextPos();
}

void CollisionProcess::ProcessCC(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (otherA->IsStatic() && otherB->IsStatic())return;
}

void CollisionProcess::ProcessCS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//A����B�ւ̃x�N�g��
	Vector3 aToB = otherB->GetRb()->GetNextPos() - std::dynamic_pointer_cast<CapsuleCollider>(otherA->GetColl())->GetNearPos();

	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (otherB->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider> (otherB->GetColl())->GetRadius();

	//���������̂Ƃ�������Ȃ����̂Ƃŏ����𕪂���
	if (otherA->IsStatic())
	{
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else if (otherA->IsStatic())
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
	}
}

void CollisionProcess::ProcessCP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//���݂������Ȃ��I�u�W�F�N�g�Ȃ�Փ˂��Ȃ�
	if (otherA->IsStatic() && otherB->IsStatic())return;
}

void CollisionProcess::AnalyzeWallAndFloor(const MV1_COLL_RESULT_POLY_DIM& hitDim, const Vector3& oldPos)
{
}