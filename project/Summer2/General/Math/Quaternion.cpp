#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion():
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{
}

Quaternion Quaternion::Make(Vector3 axis, float radian)
{
	Quaternion q;
	//��]���̒������`�F�b�N
	if (axis.Magnitude() <= 0.0f)return q;

	//�������p�x�̔�����sin,cos
	float halfSin, halfCos;

	//�P�ʃx�N�g���ɕϊ�
	axis = axis.Normalize();
	//�p�x����
	halfSin = sinf(radian * 0.5f);
	halfCos = cosf(radian * 0.5f);

	q.x = axis.x * halfSin;
	q.y = axis.y * halfSin;
	q.z = axis.z * halfSin;
	q.w = halfCos;

	return q;
}

Quaternion Quaternion::operator*(const Quaternion& right) const
{
	Quaternion q;
	q.x = right.w * this->x + 

	return Quaternion();
}
