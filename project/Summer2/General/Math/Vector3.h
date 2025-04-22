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
	/// ベクトルの大きさを返します
	/// </summary>
	/// <returns></returns>
	float Magnitude()const;

	/// <summary>
	/// 正規化(大きさを1にする)
	/// </summary>
	Vector3 Normalize() const;

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	float Dot(const Vector3& right)const;

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	Vector3 Cross(const Vector3& right)const;

	/// <summary>
	/// 0ベクトルにする
	/// </summary>
	Vector3 Zero()const;

	/// <summary>
	/// DxLibのベクトルに変換します
	/// </summary>
	/// <returns></returns>
	VECTOR ToDxLibVector()const;


	//演算子オーバーロード
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
//座標で表すときに使う
typedef Vector3 Position3;
