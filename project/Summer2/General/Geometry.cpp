#include "Geometry.h"
#include <DxLib.h>
#include <cassert>

namespace
{
    //�J�v�Z���̋�`��������������Ă���̂ŏC��
    constexpr float radiusOffset = 1.0f;
}

float Vector2::Magnitude() const
{
    //�O�����̒藝�Ōv�Z
    return sqrtf(this->x * this->x + this->y * this->y);
}

Vector2 Vector2::Normalize()const
{
    //���K�������l��Ԃ�
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
    //0���ȊO�Ȃ�v�Z����
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
    //���̕���
    DrawCircle(posA.x, posA.y, radius, color, isFull);
    DrawCircle(posB.x, posB.y, radius, color, isFull);
    //�l�p�`����
    Vector2 vec = posB - posA;//�x�N�g��
    //atan2
    // x���̐��̌����ƁA�_(x, y) �i�������A(0, 0)�ł͂Ȃ��j�܂ŐL�΂����������i���C�j�Ƃ�
    // �Ԃ́A���[�N���b�h���ʏ�ɂ�����p�x�Ƃ��Ē�`�����B
    // �l�̓��W�A���ŕԂ��Ă���B
    auto angle = atan2(vec.y, vec.x) -DX_PI_F / 2.0f;//���p�Ȋp�x�̎擾
    auto w = cos(angle) * (radius + radiusOffset);//���/�X�� * �X�� = ���
    auto h = sin(angle) * (radius + radiusOffset);//����/�X�� * �X�� = ����
    DrawQuadrangle(
        posA.x - w, posA.y - h, //����
        posB.x - w, posB.y - h, //�E��
        posB.x + w, posB.y + h, //�E��
        posA.x + w, posA.y + h, //����
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
    //��]��̍��W���o��
    auto mat = RotatePositionMatrix(center, angle);
    //���ꂼ��̍��W�ɂ�����
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

//�P�ʍs��
Matrix3x3 IdentityMat3x3()
{
    Matrix3x3 identity = {};//�v�f�����ׂ�0�ɂ���
    identity.mat[0][0] = 1.0f;
    identity.mat[1][1] = 1.0f;
    identity.mat[2][2] = 1.0f;
    return identity;
}

//���s�ړ��s��
Matrix3x3 TranslateMat3x3(float x, float y)
{
    Matrix3x3 translate = {};
    translate = IdentityMat3x3();//�P�ʍs��ɂ���
    translate.mat[0][2] = x;
    translate.mat[1][2] = y;
    return translate;
}

//��]�s��
Matrix3x3 RotateMat3x3(float angle)
{
    Matrix3x3 rotate = {};
    rotate = IdentityMat3x3();//�P�ʍs��ɂ���
    //[cos][-sin]
    //[sin][cos]
    rotate.mat[0][0] = cos(angle);
    rotate.mat[0][1] = -sin(angle);
    rotate.mat[1][0] = sin(angle);
    rotate.mat[1][1] = cos(angle);
    return rotate;
}

//�Q�̍s��̏�Z��Ԃ�
Matrix3x3 MultipleMat3x3(const Matrix3x3& left, const Matrix3x3& right)
{
    //����ɏ�Z���ʂ����Ă���
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

//�x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
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


///�Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂�
Matrix3x3 RotatePositionMatrix(const Position2& center, float angle)
{
    //���S�����_�ɕ��s�ړ�������
    //���_���S�ɉ�]
    //���S�����Ƃ̍��W�ɖ߂�
    Matrix3x3 rotaMat = TranslateMat3x3(center.x,center.y) *//���̍��W
                        RotateMat3x3(angle)*//��]
                        TranslateMat3x3(-center.x, -center.y);//���_�ɖ߂�
    return rotaMat;
}
