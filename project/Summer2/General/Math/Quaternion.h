#pragma once
#include "Vector3.h"
class Quaternion
{
public:
	Quaternion();
	virtual ~Quaternion() {};
	float x, y, z, w;
	/// <summary>
	/// �N�H�[�^�j�I�����쐬����
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="radian">���W�A��(�p�x)</param>
	/// <returns></returns>
	Quaternion Make(Vector3 axis, float radian);
	/// <summary>
	/// �N�H�[�^�j�I���̊|���Z
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	Quaternion operator*(const Quaternion& right)const;
};

