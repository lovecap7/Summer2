#include "Geometry.h"
#include <DxLib.h>
#include <cassert>

namespace
{
    //カプセルの矩形部分が少しずれているので修正
    constexpr float radiusOffset = 1.0f;
}

float Vector2::Magnitude() const
{
    //三平方の定理で計算
    return sqrtf(this->x * this->x + this->y * this->y);
}

Vector2 Vector2::Normalize()const
{
    //正規化した値を返す
    Vector2 ans = *this / Magnitude();
   return ans;
}

float Vector2::Dot(const Vector2& right) const
{
    float ans;
    ans = this->x * right.x + this->y * right.y;
    return ans;
}

Vector2 Vector2::operator+(const Vector2& right) const
{
    Vector2 ans;
    ans.x = this->x + right.x;
    ans.y = this->y + right.y;
    return ans;
}

Vector2 Vector2::operator-(const Vector2& right) const
{
    Vector2 ans;
    ans.x = this->x - right.x;
    ans.y = this->y - right.y;
    return ans;
}

Vector2 Vector2::operator*(const float& right) const
{
    Vector2 ans;
    ans.x = this->x * right;
    ans.y = this->y * right;
    return ans;
}

Vector2 Vector2::operator/(const float& right) const
{
    Vector2 ans;
    ans.x = this->x / right;
    ans.y = this->y / right;
    return ans;
}

Vector2 Vector2::operator+=(const Vector2& right)
{
    this->x += right.x;
    this->y += right.y;
    return *this;
}

Vector2 Vector2::operator-=(const Vector2& right)
{
    this->x -= right.x;
    this->y -= right.y;
    return *this;
}

Vector2 Vector2::operator*=(const float& right)
{
    this->x *= right;
    this->y *= right;
    return *this;
}

Vector2 Vector2::operator/=(const float& right)
{
    //0を以外なら計算する
    assert(right != 0.0f);
    this->x /= right;
    this->y /= right;
    return *this;
}

bool Vector2::operator==(const Vector2& right) const
{
    bool isX = this->x == right.x;
    bool isY = this->y == right.y;
    return isX && isY;
}

bool Vector2::operator!=(const Vector2& right) const
{
    bool isX = this->x != right.x;
    bool isY = this->y != right.y;
    return isX || isY;
}

void Circle2D::Draw()
{
    DrawCircle(pos.x, pos.y, radius, 0xffffff, false);
}

void Circle2D::Draw(bool isFull, unsigned int color)
{
    DrawCircle(pos.x, pos.y, radius, color, isFull);
}

void Capsule2D::Draw(bool isFull,unsigned int color)
{
    //球の部分
    DrawCircle(posA.x, posA.y, radius, color, isFull);
    DrawCircle(posB.x, posB.y, radius, color, isFull);
    //四角形部分
    Vector2 vec = posB - posA;//ベクトル
    //atan2
    // x軸の正の向きと、点(x, y) （ただし、(0, 0)ではない）まで伸ばした半直線（レイ）との
    // 間の、ユークリッド平面上における角度として定義される。
    // 値はラジアンで返ってくる。
    auto angle = atan2(vec.y, vec.x) -DX_PI_F / 2.0f;//直角な角度の取得
    auto w = cos(angle) * (radius + radiusOffset);//底辺/傾き * 傾き = 底辺
    auto h = sin(angle) * (radius + radiusOffset);//高さ/傾き * 傾き = 高さ
    DrawQuadrangle(
        posA.x - w, posA.y - h, //左上
        posB.x - w, posB.y - h, //右上
        posB.x + w, posB.y + h, //右下
        posA.x + w, posA.y + h, //左下
        color, isFull) ;
}

Vector2 Capsule2D::GetCenterPos()
{
    Vector2 center = {};
    center.x = (posA.x + posB.x) / 2;
    center.y = (posA.y + posB.y) / 2;
    return center;
}

Capsule2D Capsule2D::RotateCapsule(const Position2& center, float angle)
{
    //回転後の座標を出す
    auto mat = RotatePositionMatrix(center, angle);
    //それぞれの座標にかける
    this->posA = mat * this->posA;
    this->posB = mat * this->posB;
    return *this;
}

void Rect2D::Draw()
{
    DrawBox(GetLeft(), GetTop(), GetRight(), GetBottom(), 0xffffff, false);
}

void Rect2D::Draw(bool isFull, unsigned int color)
{
    DrawBox(GetLeft(), GetTop(), GetRight(), GetBottom(), color, isFull);
}

void Rect2D::Draw(Vector2& offset)
{
    DrawBox(GetLeft() + offset.x, GetTop() + offset.y, GetRight() + offset.x, GetBottom() + offset.y, 0xffffff, false);
}

//単位行列
Matrix3x3 IdentityMat3x3()
{
    Matrix3x3 identity = {};//要素をすべて0にする
    identity.mat[0][0] = 1.0f;
    identity.mat[1][1] = 1.0f;
    identity.mat[2][2] = 1.0f;
    return identity;
}

//平行移動行列
Matrix3x3 TranslateMat3x3(float x, float y)
{
    Matrix3x3 translate = {};
    translate = IdentityMat3x3();//単位行列にする
    translate.mat[0][2] = x;
    translate.mat[1][2] = y;
    return translate;
}

//回転行列
Matrix3x3 RotateMat3x3(float angle)
{
    Matrix3x3 rotate = {};
    rotate = IdentityMat3x3();//単位行列にする
    //[cos][-sin]
    //[sin][cos]
    rotate.mat[0][0] = cos(angle);
    rotate.mat[0][1] = -sin(angle);
    rotate.mat[1][0] = sin(angle);
    rotate.mat[1][1] = cos(angle);
    return rotate;
}

//２つの行列の乗算を返す
Matrix3x3 MultipleMat3x3(const Matrix3x3& left, const Matrix3x3& right)
{
    //これに乗算結果を入れていく
    Matrix3x3 mul = {};
    for (int k = 0;k <= 2;++k)
    {
        for (int j = 0;j <= 2;++j)
        {
            for (int i = 0;i <= 2;++i)
            {
                mul.mat[k][j] += left.mat[k][i] * right.mat[i][j];
            }
        }
    }
    //mul.mat[0][0] = 
    //    left.mat[0][0] * right.mat[0][0] + 
    //    left.mat[0][1] * right.mat[1][0] + 
    //    left.mat[0][2] * right.mat[2][0];
    //mul.mat[0][1] =
    //    left.mat[0][0] * right.mat[0][1] +
    //    left.mat[0][1] * right.mat[1][1] +
    //    left.mat[0][2] * right.mat[2][1];
    //...
    return mul;
}

Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right)
{
    return MultipleMat3x3(left, right);
}

//ベクトルに対して行列乗算を適用し、結果のベクトルを返す
Vector2 MultipleVec2(const Matrix3x3& m, const Vector2& v)
{
    Vector2 vec = {};
    vec.x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * 1.0f;
    vec.y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * 1.0f;
    return vec;
}

Vector2 operator*(const Matrix3x3& m, const Vector2& v)
{
    return MultipleVec2(m, v);
}


///とある点を中心に回転して、その回転後の座標を返します
Matrix3x3 RotatePositionMatrix(const Position2& center, float angle)
{
    //中心を原点に平行移動させる
    //原点中心に回転
    //中心をもとの座標に戻す
    Matrix3x3 rotaMat = TranslateMat3x3(center.x,center.y) *//元の座標
                        RotateMat3x3(angle)*//回転
                        TranslateMat3x3(-center.x, -center.y);//原点に戻す
    return rotaMat;
}
