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
	//回転軸の長さをチェック
	if (axis.Magnitude() <= 0.0f)return q;

	//動かす角度の半分のsin,cos
	float halfSin, halfCos;

	//単位ベクトルに変換
	axis = axis.Normalize();
	//角度を代入
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
