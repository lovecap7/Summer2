#pragma once
#include "Vector3.h"
class Quaternion
{
public:
	Quaternion();
	virtual ~Quaternion() {};
	float x, y, z, w;
	/// <summary>
	/// クォータニオンを作成する
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="radian">ラジアン(角度)</param>
	/// <returns></returns>
	Quaternion Make(Vector3 axis, float radian);
	/// <summary>
	/// クォータニオンの掛け算
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	Quaternion operator*(const Quaternion& right)const;
};

