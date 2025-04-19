#include "CollisionCheck.h"

CollisionCheck::CollisionCheck()
{
}

CollisionCheck::~CollisionCheck()
{
}


//�~���m�̓����蔻��
bool CollisionCheck::IsCollCicCic(const Circle2D& c1, const Circle2D& c2)
{
	//���ꂼ��̒��S���W����̊Ԃ̃x�N�g�������߂�
	Vector2 dist = c2.pos - c1.pos;
	//2���̂̊Ԃ̑傫�������ꂼ��̔��a�̑������l�����傫���Ȃ瓖�����Ă��Ȃ�
	if (dist.Magnitude() > c1.radius + c2.radius)
	{
		return false;
	}
	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionCheck::IsCollCicRec(const Circle2D& circle, const Rect2D& rect)
{
	//���ꂼ��̒��S���W����̊Ԃ̃x�N�g�������߂�
	Vector2 dist = circle.pos - rect.pos;
	//��`�̒��S����e���_�ւ̒��������߂�
	float hypot = Vector2{ (rect.w / 2.0f),(rect.h / 2.0f) }.Magnitude();
	//Y���������ꂼ��̔��a�̑������l�����傫���A�܂���
	//X���������ꂼ��̔��a�̑������l�����傫���A�܂���
	//�x�N�g���̑傫������`�̒��S����e���_�ւ̒����Ƌ��̔��a�̑傫����
	//�������l�����傫���Ȃ瓖�����Ă��Ȃ�
	if (abs(dist.y) > circle.radius + (rect.h / 2.0f) ||
		abs(dist.x) > circle.radius + (rect.w / 2.0f) ||
		dist.Magnitude() > circle.radius + hypot)
	{
		return false;
	}

	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionCheck::IsCollCicCap(const Circle2D& circle, const Capsule2D& capsule)
{
	//������J�v�Z���ɐ����������ċ�����J�v�Z���ւ̍ŒZ���������߂�
	// �J�v�Z����posA -> A
	// �J�v�Z����posB -> B
	// ���̒��S���W(pos) -> C
	// ������J�v�Z���ɐ����������Ăł����W -> V
	Vector2 A = capsule.posA;
	Vector2 B = capsule.posB;
	Vector2 C = circle.pos;


	//�J�v�Z����A���狅�̒��S���W�ւ̃x�N�g��
	Vector2 AtoC = C - A;
	//posA����posB�ւ̒P�ʃx�N�g��
	Vector2 AtoB = B - A;
	Vector2 nomAtoB = AtoB.Normalize();
	//���ς̌v�Z
	//A������WV�ւ̋������o��
	float dotAtoV = nomAtoB.Dot(AtoC);
	//posA��posA����posB�ւ̒P�ʃx�N�g�� * dot�����č��WV���o��
	Vector2 V = A + (nomAtoB * dotAtoV);


	//V��AB�̊Ԃɂ�����W���ǂ������`�F�b�N����
	float dotAtoB = nomAtoB.Dot(AtoB);//�S��
	float rate = (dotAtoV / dotAtoB);//����

	//0��菬�����Ȃ�posA���O�A1���傫���Ȃ�posB���O�A�ǂ���ł��Ȃ��Ȃ�AB�Ԃɂ���
	if (rate > 1.0f)
	{
		//�ŒZ������posB���狅�̊Ԃ̋���()BtoC
		Vector2 BtoC = C - B;
		//���ƃJ�v�Z���̔��a�̍��v���傫����Γ������Ă��Ȃ�
		if (BtoC.Magnitude() > circle.radius + capsule.radius)
		{
			return false;
		}
	}
	else if (rate < 0.0f)
	{
		//�ŒZ������posA���狅�̊Ԃ̋���(AtoC)
		//���ƃJ�v�Z���̔��a�̍��v���傫����Γ������Ă��Ȃ�
		if (AtoC.Magnitude() > circle.radius + capsule.radius)
		{
			return false;
		}
	}
	else
	{
		//�ŒZ�����͋�����V(�����������Ăł����W)�̊Ԃ̋���(VtoC)
		Vector2 VtoC = C - V;
		//���ƃJ�v�Z���̔��a�̍��v���傫����Γ������Ă��Ȃ�
		if (VtoC.Magnitude() > circle.radius + capsule.radius)
		{
			return false;
		}
	}

	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionCheck::IsCollRecRec(const Rect2D& r1, const Rect2D& r2)
{
	//���ꂼ��̒��S���W����̊Ԃ̃x�N�g�������߂�
	Vector2 dist = r2.pos - r1.pos;
	//X�����̑傫������`�̕��̍��v���傫���Ȃ瓖�����Ă��Ȃ��A�܂���
	//Y�����̑傫������`�̍����̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (abs(dist.x) > (r1.w / 2.0f) + (r2.w / 2.0f) ||
		abs(dist.y) > (r1.h / 2.0f) + (r2.h / 2.0f))
	{
		return false;
	}
	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionCheck::IsCollRecCap(const Rect2D& rect, const Capsule2D& capsule)
{
	//�~�ƃJ�v�Z���̓����蔻��̍Ō�̕������~�Ƌ�`�̓����蔻��ɕς��邾��

	//��`����J�v�Z���ɐ����������ċ�����J�v�Z���ւ̍ŒZ���������߂�
	// �J�v�Z����posA -> A
	// �J�v�Z����posB -> B
	// ��`�̒��S���W(pos) -> R
	// ��`����J�v�Z���ɐ����������Ăł����W -> V
	Vector2 A = capsule.posA;
	Vector2 B = capsule.posB;
	Vector2 R = rect.pos;


	//�J�v�Z����A�����`�̒��S���W�ւ̃x�N�g��
	Vector2 AtoR = R - A;
	//posA����posB�ւ̒P�ʃx�N�g��
	Vector2 AtoB = B - A;
	Vector2 nomAtoB = AtoB.Normalize();
	//���ς̌v�Z
	//A������WV�ւ̋������o��
	float dotAtoV = nomAtoB.Dot(AtoR);
	//posA��posA����posB�ւ̒P�ʃx�N�g�� * dot�����č��WV���o��
	Vector2 V = A + (nomAtoB * dotAtoV);


	//V��AB�̊Ԃɂ�����W���ǂ������`�F�b�N����
	float dotAtoB = nomAtoB.Dot(AtoB);//�S��
	float rate = (dotAtoV / dotAtoB);//����

	//���ƃJ�v�Z���̔��a�̍��v���傫����Γ������Ă��Ȃ�
	//��`�̒��S����e���_�ւ̒��������߂�
	float hypot = Vector2{ (rect.w / 2.0f),(rect.h / 2.0f) }.Magnitude();
	//0��菬�����Ȃ�posA���O�A1���傫���Ȃ�posB���O�A�ǂ���ł��Ȃ��Ȃ�AB�Ԃɂ���
	if (rate > 1.0f)
	{
		//�ŒZ������posB�����`�̊Ԃ̋���()BtoR
		Vector2 BtoR = R - B;
		//Y���������ꂼ��̔��a�̑������l�����傫���A�܂���
		//X���������ꂼ��̔��a�̑������l�����傫���A�܂���
		//�x�N�g���̑傫������`�̒��S����e���_�ւ̒����Ƌ��̔��a�̑傫����
		//�������l�����傫���Ȃ瓖�����Ă��Ȃ�
		if (abs(BtoR.y) > capsule.radius + (rect.h / 2.0f) ||
			abs(BtoR.x) > capsule.radius + (rect.w / 2.0f) ||
			BtoR.Magnitude() > capsule.radius + hypot)
		{
			return false;
		}
	}
	else if (rate < 0.0f)
	{
		if (abs(AtoR.y) > capsule.radius + (rect.h / 2.0f) ||
			abs(AtoR.x) > capsule.radius + (rect.w / 2.0f) ||
			AtoR.Magnitude() > capsule.radius + hypot)
		{
			return false;
		}
	}
	else
	{
		//�ŒZ�����͋�����V(�����������Ăł����W)�̊Ԃ̋���(VtoC)
		Vector2 VtoR = R - V;
		if (abs(VtoR.y) > capsule.radius + (rect.h / 2.0f) ||
			abs(VtoR.x) > capsule.radius + (rect.w / 2.0f) ||
			VtoR.Magnitude() > capsule.radius + hypot)
		{
			return false;
		}
	}

	//�����܂ł���Γ������Ă���
	return true;
}

bool CollisionCheck::IsCollCapCap(const Capsule2D& cap1, const Capsule2D& cap2)
{
	//�J�v�Z���̊e�_���琂�������낵�A�ŒZ���������߂�
	// �ŒZ����
	// �J�v�Z��1��posA -> A
	// �J�v�Z��1��posB -> B
	// �J�v�Z��2��posA -> C
	// �J�v�Z��2��posB -> D
	// �����̍��W -> V
	// �ŒZ���� -> S 
	Vector2 A = cap1.posA;
	Vector2 B = cap1.posB;
	Vector2 C = cap2.posA;
	Vector2 D = cap2.posB;
	Vector2 V = {};
	Vector2 S = {};

	Vector2 point = {};//�����������_
	Vector2 lineA = {};//�����̕Е��̍��W
	Vector2 lineB = {};//�����̕Е��̍��W
	for (int i = 0; i < 2; ++i)
	{
		switch (i)
		{
		case 0:
			//����CD
			lineA = C;
			lineB = D;
			break;
		case 1:
			//����AB
			lineA = A;
			lineB = B;
			break;
		default:
			break;
		}
		for (int j = 0; j < 2; ++j)
		{
			if (i == 0 && j == 0)
			{
				//A��������ɉ��낷
				point = A;
			}
			else if (i == 0 && j == 1)
			{
				//B����
				point = B;
			}
			else if (i == 1 && j == 0)
			{
				//C����
				point = C;
			}
			else
			{
				//D����
				point = D;
			}
			//point�ւ̃x�N�g��
			Vector2 lineAToPoint = point - lineA;
			//���������̒P�ʃx�N�g��
			Vector2 lineAToB = lineB - lineA;
			Vector2 nomLineAToB = lineAToB.Normalize();
			//���ς̌v�Z
			//lineA������WV�ւ̋������o��
			float dotLineAtoV = nomLineAToB.Dot(lineAToPoint);
			V = lineA + (nomLineAToB * dotLineAtoV);
			//V��������ɂ�����W���ǂ������`�F�b�N����
			float dotLineAtoB = nomLineAToB.Dot(lineAToB);//�S��
			float rate = (dotLineAtoV / dotLineAtoB);//����
			//������ɐ���������Ȃ�ŒZ�������ǂ�����r����
			Vector2 candidate;//�ŒZ�����������Ă���
			if (rate > 1.0f)
			{
				//LineB���ł�point�ɋ߂�
				candidate = lineB - point;
			}
			else if (rate < 0.0f)
			{
				//LineA���ł�point�ɋ߂�
				candidate = lineA - point;
			}
			else
			{
				//V���ł�point�ɋ߂�
				candidate = V - point;
			}
			//����܂��͑O��̍ŒZ���������������Ȃ�
			if (S.Magnitude() == 0 || S.Magnitude() > candidate.Magnitude())
			{
				//���݂̍ŒZ�����ɂ���
				S = candidate;
			}
		}
	}

	//�ŒZ���������ꂼ��̔��a�̍��v���傫���Ȃ瓖�����Ă��Ȃ�
	if (S.Magnitude() > cap1.radius + cap2.radius)
	{
		return false;
	}

	//�����܂ł���Γ������Ă���
	return true;
}

