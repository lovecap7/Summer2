#pragma once
#include <cmath>

struct Vector2
{
	Vector2() :x(0.0f), y(0.0f) {};
	Vector2(float initX,float initY) :x(initX), y(initY) {};
	float x, y;
	/// <summary>
	/// ベクトルの大きさを返します
	/// </summary>
	/// <returns></returns>
	float Magnitude()const;

	/// <summary>
	/// 正規化(大きさを1にする)
	/// </summary>
	Vector2 Normalize() const;

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	float Dot(const Vector2& right)const;

	//演算子オーバーロード
	Vector2 operator+(const Vector2& right)const;
	Vector2 operator-(const Vector2& right)const;
	Vector2 operator*(const float& right)const;
	Vector2 operator/(const float& right)const;
	Vector2 operator+=(const Vector2& right);
	Vector2 operator-=(const Vector2& right);
	Vector2 operator*=(const float& right);
	Vector2 operator/=(const float& right);
	bool operator==(const Vector2& right)const;
	bool operator!=(const Vector2& right)const;
};

//座標で表すときに使う
typedef Vector2 Position2;

//円
struct Circle2D
{
	Position2 pos;//中心座標
	float radius;//半径
	Circle2D() :pos(0.0f, 0.0f), radius(0) {};
	Circle2D(const Position2& p,float r) :pos(p), radius(r) {};
	void Draw();
	void Draw(bool isFull, unsigned int color);//自分の円を描画(塗りつぶす)
};

//カプセル
struct Capsule2D
{
	Position2 posA;//端点A
	Position2 posB;//端点B
	float radius;//半径
	Capsule2D() :posA(0.0f,0.0f), posB(0.0f,0.0f), radius(0.0f) {};
	Capsule2D(const Position2& a, const Position2& b, float r) :posA(a), posB(b), radius(r) {};
	void Draw(bool isFull,unsigned int color);//カプセル描画
	Vector2 GetCenterPos();//中心座標返す
	Capsule2D RotateCapsule(const Position2& center, float angle);//ある点を中心に回る
};


struct Rect2D
{
	Position2 pos;//中心座標
	float w,h;//幅と高さ
	Rect2D() :pos(0.0f, 0.0f), w(0.0f), h(0.0f) {};
	Rect2D(const Position2& p, float initW, float initH) :pos(p), w(initW), h(initH) {};
	void SetCenter(const Position2& p) { pos = p; }
	Vector2 GetCenter() { return pos; }
	float GetLeft() { return pos.x - (w / 2); }
	float GetRight() { return pos.x + (w / 2); }
	float GetTop() { return pos.y - (h / 2); }
	float GetBottom() { return pos.y + (h / 2); }
	void Draw();//自分の矩形を描画(アウトラインのみ)
	void Draw(bool isFull, unsigned int color);//自分の矩形を描画(塗りつぶす)
	void Draw(Vector2& offset);//オフセット付き描画
};

struct Matrix3x3
{
	float mat[3][3];
};

/// <summary>
/// /単位行列を返す(3x3)
/// </summary>
/// <returns></returns>
Matrix3x3 IdentityMat3x3();

/// <summary>
/// 平行移動行列を返す(3x3)
/// </summary>
/// <param name="x">X方向の移動量</param>
/// <param name="y">Y方向の移動量</param>
/// <returns></returns>
Matrix3x3 TranslateMat3x3(float x, float y);

/// <summary>
/// 回転行列を返す(3x3)
/// </summary>
/// <param name="angle">回転角度</param>
/// <returns></returns>
Matrix3x3 RotateMat3x3(float angle);

/// <summary>
/// ２つの行列の乗算を返す(乗算の順番に注意)
/// </summary>
/// <param name="left">左辺行列</param>
/// <param name="right">右辺行列</param>
/// <returns></returns>
Matrix3x3  MultipleMat3x3(const Matrix3x3& left, const Matrix3x3& right);
Matrix3x3 operator* (const Matrix3x3& left, const Matrix3x3& right);

/// <summary>
/// ベクトルに対して行列乗算を適用し、結果のベクトルを返す
/// </summary>
/// <param name="m">行列</param>
/// <param name="v">ベクトル</param>
/// <returns></returns>
Vector2 MultipleVec2(const Matrix3x3& m, const Vector2& v);
Vector2 operator*(const Matrix3x3& m, const Vector2& v);

/// <summary>
/// とある点を中心に回転して、その回転後の座標を返します
/// </summary>
/// <param name="center">回転中心点</param>
/// <param name="angle">回転角度</param>
/// <returns></returns>
Matrix3x3 RotatePositionMatrix(const Position2& center, float angle);
