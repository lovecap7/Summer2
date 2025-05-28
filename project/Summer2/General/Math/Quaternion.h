#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
class Quaternion
{
public:
	//変数
	float x, y, z, w;
	//コンストラクタ
	Quaternion();
	Quaternion(float w,float x, float y, float z);

	//演算子オーバーロード
	//クォータニオン同士
	Quaternion operator *(const Quaternion& q)const;
	//クォータニオンとベクター
	Vector3 operator *(const Vector3& v)const;
	//クォータニオンとスカラー
	Quaternion operator *(const float& s)const;
	//クオータニオン同士の足し算
	Quaternion operator +(const Quaternion& q)const;
	//クォータニオン同士の比較
	bool operator ==(const Quaternion& q)const;
	bool operator !=(const Quaternion& q)const;
	//内積
	float DotQ(const Quaternion& q)const;
	//クォータニオンを正規化
	Quaternion NormQ()const;
	//単位クォータニオン
	static Quaternion IdentityQ();
	//共役(逆)クォータニオン
	Quaternion InverseQ()const;
	//大きさ
	float Magnitude()const;

	/// <summary>
	/// オイラー角からクォータニオンを作成
	/// </summary>
	/// <param name="x">ロール</param>
	/// <param name="y">ヨー</param>
	/// <param name="z">ピッチ</param>
	/// <returns>回転クオータニオン</returns>
	static Quaternion Euler(float x, float y, float z);
	/// <summary>
	/// マトリクスに変換
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>マトリクス</returns>
	Matrix4x4 GetMatrix() const;
	/// <summary>
	/// 回転クォータニオン
	/// </summary>
	/// <param name="rad">ラジアン</param>
	/// <param name="axis">軸</param>
	/// <returns>回転クォータニオン</returns>
	static Quaternion AngleAxis(const float& rad, const Vector3& axis);
	/// <summary>
	/// 球面線形補間（Spherical Linear Interpolation）
	/// </summary>
	/// <param name="sRota">開始の回転クオータニオン</param>
	/// <param name="eRota">終了の回転クオータニオン</param>
	/// <param name="t">補間係数(0~1)</param>
	/// <returns>補間後クオータニオン</returns>
	static Quaternion Slerp(const Quaternion& sRota, const Quaternion& eRota, float t);
	/// <summary>
	/// 向きたい方向を向く回転クォータニオン
	/// </summary>
	/// <param name="targetDir">向きたい方向</param>
	/// <param name="up">地面が傾いているならその法線を入れる</param>
	/// <returns>回転クォータニオン</returns>
	static Quaternion LookAt(const Vector3& targetDir, const Vector3& up = Vector3::Up());
};

