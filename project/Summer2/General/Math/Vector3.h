#pragma once
#include <DxLib.h>
class Vector3
{
public:
	Vector3();
	Vector3(float initX, float initY, float initZ);
	~Vector3() {};
	float x, y, z;

	/// <summary>
	/// �x�N�g���̑傫����Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	float Magnitude()const;

	/// <summary>
	/// ���K��(�傫����1�ɂ���)
	/// </summary>
	Vector3 Normalize() const;

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	float Dot(const Vector3& right)const;

	/// <summary>
	/// �O��
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& right)const;

	/// <summary>
	/// 0�x�N�g���ɂ���
	/// </summary>
	Vector3 Zero()const;

	/// <summary>
	/// DxLib�̃x�N�g���ɕϊ����܂�
	/// </summary>
	/// <returns></returns>
	VECTOR ToDxLibVector()const;


	//���Z�q�I�[�o�[���[�h
	Vector3 operator+(const Vector3& right)const;
	Vector3 operator-(const Vector3& right)const;
	Vector3 operator*(const float& right)const;
	Vector3 operator/(const float& right)const;
	Vector3 operator+=(const Vector3& right);
	Vector3 operator-=(const Vector3& right);
	Vector3 operator*=(const float& right);
	Vector3 operator/=(const float& right);
	bool operator==(const Vector3& right)const;
	bool operator!=(const Vector3& right)const;
};
//���W�ŕ\���Ƃ��Ɏg��
typedef Vector3 Position3;
