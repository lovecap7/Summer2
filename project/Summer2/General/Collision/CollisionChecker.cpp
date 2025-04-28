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


bool CollisionChecker::CheckCollSS(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//A�̃R���C�_�[�̏��
	Vector3 posA = actorA->GetRb()->GetNextPos();
	float radiusA = std::dynamic_pointer_cast<SphereCollider>(actorA->GetColl())->GetRadius();
	//B�̃R���C�_�[�̏��
	Vector3 posB = actorB->GetRb()->GetNextPos();
	float radiusB = std::dynamic_pointer_cast<SphereCollider>(actorB->GetColl())->GetRadius();

	Vector3 aToB = posB - posA;

	//�Q���̊Ԃ̒��������a�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (aToB.Magnitude() > radiusA + radiusB)
	{
		return false;
	}

	//�����܂ŗ����瓖�����Ă���
	return true;
}

bool CollisionChecker::CheckCollCS(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//������J�v�Z���ɐ����������ċ��ƃJ�v�Z���̍ŒZ���������߂�

	//�J�v�Z���̂��ꂼ��̍��W
	Vector3 cPosA = actorA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetEndPos() + actorA->GetRb()->GetVec();
	//���̍��W
	Vector3 sPos = actorB->GetRb()->GetNextPos();
	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider>(actorB->GetColl())->GetRadius();

	//�J�v�Z���̍��WA���狅�ւ̃x�N�g��
	Vector3 AtoS = sPos - cPosA;
	//�J�v�Z���̍��WA����B�ւ̒P�ʃx�N�g��
	Vector3 AtoB = cPosB - cPosA;
	Vector3 nomAtoB = AtoB.Normalize();

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
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(cPosB);

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
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(cPosA);

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
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(verPos);

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

bool CollisionChecker::CheckCollCC(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	// �ǂ��炩�̃J�v�Z���̈�̍��W����Е��̃J�v�Z���ɐ�����������
	//�@���݂��̃J�v�Z���ɍł��߂����W�����ꂼ��o��

	//�J�v�Z���̂��ꂼ��̍��W
	//�J�v�Z��A
	Vector3 cPosA = actorA->GetRb()->GetNextPos();
	Vector3 cPosB = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetNextEndPos(actorA->GetRb()->GetVec());
	//�J�v�Z��B
	Vector3 cPosC = actorB->GetRb()->GetNextPos();
	Vector3 cPosD = std::dynamic_pointer_cast<CapsuleCollider>(actorB->GetColl())->GetNextEndPos(actorB->GetRb()->GetVec());
	//�ŒZ����
	float shortDis = std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider>(actorB->GetColl())->GetRadius();
	//���̍ŒZ����
	Vector3 nowShortDis = {};

	for (int i = 0; i < 2; ++i)
	{
		//�����̂��ꂼ��̍��W
		Vector3 lineA;
		Vector3 lineB;
		//�ŏ��ɃJ�v�Z��B�ɑ΂��ăJ�v�Z��A�̂��ꂼ��̓_����ŒZ�̍��W���o��
		if (i <= 0)
		{
			//����CD
			lineA = cPosC;
			lineB = cPosD;
		}
		//���ɂɃJ�v�Z��A�ɑ΂��ăJ�v�Z��B�̂��ꂼ��̓_����ŒZ�̍��W���o��
		else
		{
			//����AB
			lineA = cPosA;
			lineB = cPosB;
		}

		for (int j = 0; j < 2; ++j)
		{
			//�m�F������W
			Vector3 checkPoint;
			//A��������ɉ��낷
			if (i <= 0 && j <= 0)
			{
				checkPoint = cPosA;
			}
			//B��������ɉ��낷
			else if (i <= 0 && j >= 1)
			{
				checkPoint = cPosB;
			}
			//C��������ɉ��낷
			else if (i >= 1 && j <= 0)
			{
				checkPoint = cPosC;
			}
			//D��������ɉ��낷
			else
			{
				checkPoint = cPosD;
			}

			//���Ƃ͋��ƃJ�v�Z���̎��Ɠ���
			//A����m�F������W�ւ̃x�N�g��
			Vector3 lineAtoP = checkPoint - lineA;
			//����AB�̒P�ʃx�N�g��
			Vector3 lineAB = lineB - lineA;
			Vector3 nomLineAB = lineAB.Normalize();

			//���ꂼ��̃x�N�g��������ς������ċ����琂�����������ʒu�����߂�
			float dotVer = nomLineAB.Dot(lineAtoP);
			Vector3 verPos = lineA + (nomLineAB * dotVer);//���������������W

			//���������������W������AB�Ԃ̊Ԃɂ��邩�O�ɂ��邩���`�F�b�N����
			float dotAB = nomLineAB.Dot(lineAB);//�S��
			float rate = dotVer / dotAB;//����

			//�ŒZ�����������Ă���
			Vector3 shortCandidate;
			//�Փ˔���Ɏg���̂ōŒZ���W������
			Vector3 shortPos;
			if (rate >= 1.0f)
			{
				//���WB�ɋ߂�
				shortCandidate = lineB - checkPoint;
				shortPos = lineB;
			}
			else if (rate <= 0.0f)
			{
				//���WA�ɋ߂�
				shortCandidate = lineA - checkPoint;
				shortPos = lineA;
			}
			else
			{
				//���������������W�ɋ߂�
				shortCandidate = verPos - checkPoint;
				shortPos = verPos;
			}

			//����܂��͑O��̍ŒZ������菬�����Ȃ猻�݂̍ŒZ�����Ƃ���
			if (nowShortDis.Magnitude() <= 0 || nowShortDis.Magnitude() > shortCandidate.Magnitude())
			{
				nowShortDis = shortCandidate;

				//�ŒZ���W���L�^
				if (i == 0)
				{
					//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
					std::dynamic_pointer_cast<CapsuleCollider>(actorB->GetColl())->SetNearPos(shortPos);
				}
				else
				{
					//�Փ˔���Ŏg���̂ň�ԋ߂����W���o���Ă���
					std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->SetNearPos(shortPos);
				}
			}
		}
	}

	//���݂̍ŒZ�������ό`�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (nowShortDis.Magnitude() > shortDis)
	{
		return false;
	}

	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionChecker::CheckCollSP(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//�������Ă�|���S���̐�
	auto hitDim = MV1CollCheck_Sphere(
		std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->GetModelHandle(),
		-1,
		actorA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(actorA->GetColl())->GetRadius());
	//����������Ă��Ȃ��Ȃ�false
	if (hitDim.HitNum <= 0)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//�����蔻��Ɏg���̂ŕۑ�
	std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->SetHitDim(hitDim);

	return true;
}

bool CollisionChecker::CheckCollCP(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB)
{
	//�������Ă�|���S���̐�
	auto hitDim = MV1CollCheck_Capsule(
		std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->GetModelHandle(),
		-1,
		actorA->GetRb()->GetNextPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetNextEndPos(actorA->GetRb()->GetVec()).ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(actorA->GetColl())->GetRadius(),
		-1);
	//�������Ă��Ȃ��Ȃ�false
	if (!hitDim.Dim->HitFlag)
	{
		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
		return false;
	}

	//�����蔻��Ɏg���̂ŕۑ�
	std::dynamic_pointer_cast<PolygonCollider>(actorB->GetColl())->SetHitDim(hitDim);

	return true;
}
