#include "Quaternion.h"
#include <cmath>
#include <cassert>
Quaternion::Quaternion():
	w(1.0f),
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Quaternion::Quaternion(float w,float x, float y, float z):
	w(w),
	x(x),
	y(y),
	z(z)
{
}
//クォータニオンの回転の合成
Quaternion Quaternion::operator*(const Quaternion& q)const
{
	//クォータニオンの掛け算
	Quaternion rQ;
	rQ.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;//実部
	rQ.x = this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y;//虚部
	rQ.y = this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z;//虚部
	rQ.z = this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x;//虚部
	return rQ;
}
//ベクトル回転させる
Vector3 Quaternion::operator*(const Vector3& v)const
{
	Quaternion qPos, qInv;
	Vector3 vPos;
	//三次元座標をクォータニオンに変換
	qPos.w = 1.0f;
	qPos.x = v.x;
	qPos.y = v.y;
	qPos.z = v.z;

	//回転クォータニオンのインバースを作成
	qInv = this->InverseQ();
	//回転クォータニオンの作成
	qPos = *this * qPos * qInv;
	//三次元座標に戻す
	vPos.x = qPos.x;
	vPos.y = qPos.y;
	vPos.z = qPos.z;

	return vPos;
}
Quaternion Quaternion::operator*(const float& s)const
{
	Quaternion rQ;
	rQ.w = this->w * s;
	rQ.x = this->x * s;
	rQ.y = this->y * s;
	rQ.z = this->z * s;
	return rQ;
}
Quaternion Quaternion::operator+(const Quaternion& q) const
{
	Quaternion rQ;
	rQ.w = this->w + q.w;
	rQ.x = this->x + q.x;
	rQ.y = this->y + q.y;
	rQ.z = this->z + q.z;
	return rQ;
}
bool Quaternion::operator==(const Quaternion& q) const
{
	return (this->w == q.w && this->x == q.x && this->y == q.y && this->z == q.z);
}
bool Quaternion::operator!=(const Quaternion& q) const
{
	return !(*this == q);
}
//二つのクォータニオンの間の角度を求める際に使う
float Quaternion::DotQ(const Quaternion& q)const
{
	float result = this->w * q.w + this->x * q.x + this->y * q.y + this->z * q.z;
	return result;
}

//正規化クォータニオン(値の誤差をなくす)
Quaternion Quaternion::NormQ()const
{
	//単位クォータニオン
	Quaternion rQ = IdentityQ();
	//自分の大きさを出す(絶対値)
	float length = this->Magnitude();
	assert(length > 0.0f && "正規化に失敗しました");
	//自分に自分のインバースをかけて正規化
	//rQ = *this * this->InverseQ();
	rQ.w = this->w / length;
	rQ.x = this->x / length;
	rQ.y = this->y / length;
	rQ.z = this->z / length;
	return rQ;
}
//回転しないクォータニオン(単位クォータニオン)
Quaternion Quaternion::IdentityQ()
{
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

//共役(逆)クォータニオン
Quaternion Quaternion::InverseQ()const
{
	return  Quaternion(this->w, -this->x, -this->y, -this->z);
}

float Quaternion::Magnitude() const
{
	float magnitude = sqrt((this->w * this->w) + (this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	return magnitude;
}

Quaternion Quaternion::Euler(float xRad, float yRad, float zRad)
{
	//単位クォータニオン
	Quaternion rQ = IdentityQ();
	//各要素の回転クォータニオンを作成
	Vector3 right = { 1.0f,0.0f,0.0f };
	Quaternion xQ = AngleAxis(xRad, right);//ロール
	Vector3 up = { 0.0f,1.0f,0.0f };
	Quaternion yQ = AngleAxis(yRad, up);//ヨー
	Vector3 forward = { 0.0f,0.0f,-1.0f };
	Quaternion zQ = AngleAxis(zRad, forward);//ピッチ
	//計算
	rQ = zQ * yQ * xQ;
	return rQ.NormQ();
}

Matrix4x4 Quaternion::GetMatrix() const
{
	Matrix4x4 rM = Matrix4x4::IdentityMat4x4();
	Quaternion q = *this;
	if (q.Magnitude() > 0.0f)
	{
		q = q.NormQ();
	}
	//クォータニオンを行列に変換
	rM.mat[0][0] = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
	rM.mat[0][1] = 2.0f * ((q.x * q.y) - (q.z * q.w));
	rM.mat[0][2] = 2.0f * ((q.x * q.z) + (q.y * q.w));
	rM.mat[1][0] = 2.0f * ((q.x * q.y) + (q.z * q.w));
	rM.mat[1][1] = -(q.x * q.x) + (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
	rM.mat[1][2] = 2.0f * ((q.y * q.z) - (q.x * q.w));
	rM.mat[2][0] = 2.0f * ((q.x * q.z) - (q.y * q.w));
	rM.mat[2][1] = 2.0f * ((q.y * q.z) + (q.x * q.w));
	rM.mat[2][2] = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
	return rM;
}

Quaternion Quaternion::AngleAxis(const float& rad, const Vector3& axis)
{
	assert(axis.Magnitude() > 0.0f && "軸がありません");
	Quaternion rQ;
	//正規化
	Vector3 nAxis = axis.Normalize();
	rQ.w = cos(rad / 2.0f);//実部
	rQ.x = nAxis.x * sin(rad / 2.0f);//虚部
	rQ.y = nAxis.y * sin(rad / 2.0f);//虚部
	rQ.z = nAxis.z * sin(rad / 2.0f);//虚部
	return rQ;
}

//二つの向き・回転の間を一定角速度で滑らかに保管する
//回転や方向を直線ではなく球面上の最短弧(大円)で補完する
Quaternion Quaternion::Slerp(const Quaternion& sRota, const Quaternion& eRota, float t)
{
	//範囲外の時の処理
	if (t > 1.0f)t = 1.0f;
	if (t < 0.0f)t = 0.0f;

	//単位クォータニオン
	Quaternion rQ = IdentityQ();
	//正規化できない場合
	if (sRota.Magnitude() <= 0.0f || 
		eRota.Magnitude() <= 0.0f)
	{
		//回転しないクォータニオンを返す
		return rQ;
	}
	//開始と終了のクォータニオンを正規化
	Quaternion sRotaNom = sRota.NormQ();
	Quaternion eRotaNom = eRota.NormQ();
	//開始と終了のクォータニオンが同じなら
	if (sRotaNom == eRotaNom)
	{
		//回転しないクォータニオンを返す
		return rQ;
	}

	//内積から角度を求める
	float dot = sRotaNom.DotQ(eRotaNom);
	//負の時符号反転
	if (dot < 0.0f)
	{
		dot *= -1.0f;
	}
	float theta = acos(dot);
	
	//球面線形補間の公式より
	float s1 = sin((1.0f - t) * theta) / sin(theta);
	float s2 = sin(t * theta) / sin(theta);
	rQ = (sRotaNom * s1) + (eRotaNom * s2);
	return rQ;
}

Quaternion Quaternion::LookAt(const Vector3& targetDir, const Vector3& up)
{
	//単位クォータニオン
	Quaternion rQ = IdentityQ();
	assert(targetDir.Magnitude() > 0.0f && "方向がありません");
	Vector3 forwardDir = targetDir.Normalize();
	//回転しないなら
	if (forwardDir == Vector3::Forward())
	{
		//回転しないクォータニオンを返す
		return rQ;
	}

	//内積から角度を求める
	float dot = Vector3::Forward().Dot(targetDir);
	//負の時符号反転
	if (dot < 0.0f)
	{
		dot *= -1.0f;
	}
	float theta = acos(dot);

	//回転の軸を求める
	Vector3 axis = { 0.0f,1.0f,0.0f };
	//真後ろを向くクォータニオン以外なら
	if ((forwardDir * -1.0f) != Vector3::Forward())
	{
		//軸を外積で求める
		axis = Vector3::Forward().Cross(targetDir);
	}
	//クォータニオン作成
	rQ = AngleAxis(theta, axis);
	return rQ;
}
