#include "CollisionChecker.h"
#include "../Math/MyMath.h"
#include <DxLib.h>
#include "../Collidable.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "../Rigidbody.h"

CollisionChecker::CollisionChecker()
{
}


bool CollisionChecker::CheckCollSS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//A�̃R���C�_�[�̏��
	Vector3 posA = collA->GetRb()->GetNextPos();
	float radiusA = std::dynamic_pointer_cast<SphereCollider>(collA->GetColl())->GetRadius();
	//B�̃R���C�_�[�̏��
	Vector3 posB = collB->GetRb()->GetNextPos();
	float radiusB = std::dynamic_pointer_cast<SphereCollider>(collB->GetColl())->GetRadius();

	Vector3 aToB = posB - posA;

	//�Q���̊Ԃ̒��������a�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (aToB.Magnitude() > radiusA + radiusB)
	{
		return false;
	}

	//�����܂ŗ����瓖�����Ă���
	return true;
}

bool CollisionChecker::CheckCollCS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//������J�v�Z���ɐ����������ċ��ƃJ�v�Z���̍ŒZ���������߂�

	//�J�v�Z���̂��ꂼ��̍��W
	Vector3 cPosA = collA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetEndPos() + collA->GetRb()->GetVec();
	//���̍��W
	Vector3 sPos = collB->GetRb()->GetNextPos();
	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider>(collB->GetColl())->GetRadius();

	//�J�v�Z���̍��WA���狅�ւ̃x�N�g��
	Vector3 AtoS = sPos - cPosA;
	//�J�v�Z���̍��WA����B�ւ̒P�ʃx�N�g��
	Vector3 AtoB = cPosB - cPosA;
	Vector3 nomAtoB;
	if (AtoB.Magnitude() > 0.0f)
	{
		nomAtoB = AtoB.Normalize();
	}

	//���ꂼ��̃x�N�g��������ς������ċ����琂�����������ʒu�����߂�
	float dotVer = nomAtoB.Dot(AtoS);
	Vector3 verPos = cPosA + (nomAtoB * dotVer);//���������������W

	//���������������W���J�v�Z����AB�Ԃ̊Ԃɂ��邩�O�ɂ��邩���`�F�b�N����
	float dotAB = nomAtoB.Dot(AtoB);//�S��
	float rate = dotVer / dotAB;//����

	//1���傫���Ȃ�posB�����Ɉ�ԋ߂����W
	if (rate >= 1.0f)
	{
		//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosB);

		//posB�Ƌ��̊ԋ������瓖�����Ă��邩���`�F�b�N
		Vector3 BtoS = sPos - cPosB;
		//�ŒZ�������傫���Ȃ瓖�����Ă��Ȃ�
		if (BtoS.Magnitude() > shortDis)
		{
			return false;
		}
	}
	//0��菬�����Ȃ�posA�����Ɉ�ԋ߂����W
	else if (rate <= 0.0f)
	{
		//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosA);

		//posA�Ƌ��̊Ԃ̋������瓖�����Ă��邩�`�F�b�N
		//�ŒZ�������傫���Ȃ瓖�����Ă��Ȃ�
		if (AtoS.Magnitude() > shortDis)
		{
			return false;
		}
	}
	//0~1�̊ԂȂ琂�������������W�����Ɉ�ԋ߂����W
	else
	{
		//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(verPos);

		//���������������W�Ƌ��̊Ԃ̋������瓖�����Ă��邩�`�F�b�N
		Vector3 VtoS = sPos - verPos;
		//�ŒZ�������傫���Ȃ瓖�����Ă��Ȃ�
		if (VtoS.Magnitude() > shortDis)
		{
			return false;
		}
	}

	//�����܂ł���Γ������Ă���
	return true;
}

//�Փ˔���̂��߂ɍŋߐړ_�̌v�Z�����Ă�
bool CollisionChecker::CheckCollCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	// �ǂ��炩�̃J�v�Z���̈�̍��W����Е��̃J�v�Z���ɐ�����������
	//�@���݂��̃J�v�Z���ɍł��߂����W�����ꂼ��o��

	//�J�v�Z���̂��ꂼ��̍��W
	//�J�v�Z��A
	Vector3 capAStartPos = collA->GetRb()->GetNextPos();
	Vector3 capAEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec());
	float capARadius = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius();
	//�J�v�Z��B
	Vector3 capBStartPos = collB->GetRb()->GetNextPos();
	Vector3 capBEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetNextEndPos(collB->GetRb()->GetVec());
	float capBRadius = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetRadius();
	
	//���s���ǂ����m�F����
	auto capADir = capAEndPos - capAStartPos;//����1
	auto capBDir = capBStartPos - capBEndPos;//����2
	//���K�����ē��Ȃ��畽�s���`�F�b�N
	auto cross = capADir.Cross(capBDir);
	//���s�Ȃ�
	if (cross.SqMagnitude() <= 0.0f)
	{
		return ParallelCC(collA, collB);
	}

	//�ŒZ����
	float shortDis = capARadius + capBRadius;
	//���̍ŒZ����
	float nowShortDis = 10000.0f;//�n�_���m���ŒZ�ɂ��Ă���

	for (int i = 0; i < 2; ++i)
	{
		//�����̂��ꂼ��̍��W
		Vector3 lineStart;
		Vector3 lineEnd;
		//�ŏ��ɃJ�v�Z��B�ɑ΂��ăJ�v�Z��A�̂��ꂼ��̓_����ŒZ�̍��W���o��
		if (i <= 0)
		{
			//����CD
			lineStart = capBStartPos;
			lineEnd = capBEndPos;
		}
		//���ɂɃJ�v�Z��A�ɑ΂��ăJ�v�Z��B�̂��ꂼ��̓_����ŒZ�̍��W���o��
		else
		{
			//����AB
			lineStart = capAStartPos;
			lineEnd = capAEndPos;
		}

		for (int j = 0; j < 2; ++j)
		{
			//�m�F������W
			Vector3 checkPoint;
			//A��������ɉ��낷
			if (i == 0 && j == 0)checkPoint = capAStartPos;
			//B��������ɉ��낷
			if (i == 0 && j == 1)checkPoint = capAEndPos;
			//C��������ɉ��낷
			if (i == 1 && j == 0)checkPoint = capBStartPos;
			//D��������ɉ��낷
			if (i == 1 && j == 1)checkPoint = capBEndPos;

			//���Ƃ͋��ƃJ�v�Z���̎��Ɠ���
			//A����m�F������W�ւ̃x�N�g��
			Vector3 lineAtoP = checkPoint - lineStart;
			//����AB�̒P�ʃx�N�g��
			Vector3 lineAB = lineEnd - lineStart;
			Vector3 nomLineAB = lineAB.Normalize();

			//���ꂼ��̃x�N�g��������ς������ċ����琂�����������ʒu�����߂�
			float dotVer = nomLineAB.Dot(lineAtoP);
			Vector3 verPos = lineStart + (nomLineAB * dotVer);//���������������W

			//���������������W������AB�Ԃ̊Ԃɂ��邩�O�ɂ��邩���`�F�b�N����
			float rate = dotVer / lineAB.Magnitude();//����

			//�ŒZ�����������Ă���
			float shortCandidate;
			//�Փ˔���Ɏg���̂ōŒZ���W������
			Vector3 shortPos;
			if (rate >= 1.0f)
			{
				//���WB�ɋ߂�
				shortCandidate = (lineEnd - checkPoint).Magnitude();
				shortPos = lineEnd;
			}
			else if (rate <= 0.0f)
			{
				//���WA�ɋ߂�
				shortCandidate = (lineStart - checkPoint).Magnitude();
				shortPos = lineStart;
			}
			else
			{
				//���������������W�ɋ߂�
				shortCandidate = (verPos - checkPoint).Magnitude();
				shortPos = verPos;
			}

			//����܂��͑O��̍ŒZ������菬�����Ȃ猻�݂̍ŒZ�����Ƃ���
			if (nowShortDis > shortCandidate)
			{
				nowShortDis = shortCandidate;

				//�ŒZ���W���L�^
				if (i == 0)
				{
					//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
					std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(shortPos);
				}
				else
				{
					//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
					std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(shortPos);
				}
			}
		}
	}

	//���݂̍ŒZ�������ό`�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (nowShortDis > shortDis)
	{
		return false;
	}


	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionChecker::CheckCollCCVerDxLib(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//�J�v�Z���̂��ꂼ��̍��W
	//�J�v�Z��A
	Vector3 capAStartPos = collA->GetRb()->GetNextPos();
	Vector3 capAEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec());
	float capARadius = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius();
	//�J�v�Z��B
	Vector3 capBStartPos = collB->GetRb()->GetNextPos();
	Vector3 capBEndPos = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetNextEndPos(collB->GetRb()->GetVec());
	float capBRadius = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetRadius();

	return HitCheck_Capsule_Capsule(capAStartPos.ToDxLibVector(), capAEndPos.ToDxLibVector(), capARadius,
		capBStartPos.ToDxLibVector(), capBEndPos.ToDxLibVector(), capBRadius);
}

bool CollisionChecker::CheckCollSP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//�������Ă�|���S���̐�
	auto hitDim = MV1CollCheck_Sphere(
		std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->GetModelHandle(),
		-1,
		collA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(collA->GetColl())->GetRadius());
	//����������Ă��Ȃ��Ȃ�false
	if (hitDim.HitNum <= 0)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//�����蔻��Ɏg���̂ŕۑ�
	std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::CheckCollCP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//�������Ă�|���S���̐�
	auto hitDim = MV1CollCheck_Capsule(
		std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->GetModelHandle(),
		-1,
		collA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec()).ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius(),
		-1);

	//�������Ă��Ȃ��Ȃ�false
	if (hitDim.HitNum <= 0)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//�����蔻��Ɏg���̂ŕۑ�
	std::dynamic_pointer_cast<PolygonCollider>(collB->GetColl())->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::ParallelCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB)
{
	//�J�v�Z��A
	Vector3 cPosA = collA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetNextEndPos(collA->GetRb()->GetVec());
	//�J�v�Z��B
	Vector3 cPosC = collB->GetRb()->GetNextPos();
	Vector3 cPosD = std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetNextEndPos(collB->GetRb()->GetVec());
	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->GetRadius();
	
	//�e�������`�F�b�N
	Vector3 ac = cPosC - cPosA;
	Vector3 ad = cPosD - cPosA;
	Vector3 bc = cPosC - cPosB;
	Vector3 bd = cPosD - cPosB;
	//�ŒZ�������o��
	float dis = ac.Magnitude();
	//��x����Ă���
	std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosA);
	std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosC);
	//�Z���Ȃ�
	if (dis > ad.Magnitude())
	{
		dis = ad.Magnitude();
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosA);
		std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosD);
	}
	if (dis > bc.Magnitude())
	{
		dis = bc.Magnitude();
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosB);
		std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosC);
	}
	if (dis > bd.Magnitude())
	{
		dis = bd.Magnitude();
		std::dynamic_pointer_cast<CapsuleCollider>(collA->GetColl())->SetNearPos(cPosB);
		std::dynamic_pointer_cast<CapsuleCollider>(collB->GetColl())->SetNearPos(cPosD);
	}

	//�ŒZ�������傫���Ȃ瓖�����Ă��Ȃ�
	if (dis > shortDis)
	{
		return false;
	}

	//�����܂ŗ����瓖�����Ă�
	return true;
}