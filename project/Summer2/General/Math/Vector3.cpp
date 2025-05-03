#include "Vector3.h"
#include <cmath>

//3D�p
Vector3::Vector3() :
    x(0.0f),
    y(0.0f),
    z(0.0f)
{
}

Vector3::Vector3(float initX, float initY, float initZ) :
    x(initX),
    y(initY),
    z(initZ)
{
}

float Vector3::Magnitude() const
{
    //�O�����̒藝�Ōv�Z
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 Vector3::Normalize() const
{
    //���K��
    Vector3 ans;
    ans = (*this) / Magnitude();
    return ans;
}

float Vector3::Dot(const Vector3& right) const
{
    //����
    float ans;
    ans = this->x * right.x + this->y * right.y + this->z * right.z;
    return ans;
}

Vector3 Vector3::Cross(const Vector3& right) const
{
    //�O��
    Vector3 ans;
    ans.x = this->y * right.z - this->z * right.y;
    ans.y = this->z * right.x - this->x * right.z;
    ans.z = this->x * right.y - this->y * right.x;
    return ans;
    //a��b�̃x�N�g������镽�ʂɐ�����a����b��
    //�E�˂����񂵂��Ƃ��ɂ˂��̐i�����������傫����
    //a��b����镽�s�l�ӌ`�̖ʐςɓ�����
}

Vector3 Vector3::Zero()const
{
    Vector3 zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    return zero;
}

DxLib::VECTOR Vector3::ToDxLibVector() const
{
    return VGet(this->x,this->y,this->z);
}

Vector3 Vector3::operator+(const Vector3& right) const
{
    Vector3 ans;
    ans.x = this->x + right.x;
    ans.y = this->y + right.y;
    ans.z = this->z + right.z;
    return ans;
}

Vector3 Vector3::operator-(const Vector3& right) const
{
    Vector3 ans;
    ans.x = this->x - right.x;
    ans.y = this->y - right.y;
    ans.z = this->z - right.z;
    return ans;
}

Vector3 Vector3::operator*(const float& right) const
{
    Vector3 ans;
    ans.x = this->x * right;
    ans.y = this->y * right;
    ans.z = this->z * right;
    return ans;
}

Vector3 Vector3::operator/(const float& right) const
{
    Vector3 ans;
    ans.x = this->x / right;
    ans.y = this->y / right;
    ans.z = this->z / right;
    return ans;
}

Vector3 Vector3::operator=(const float& right) const
{
    Vector3 ans;
    ans.x = right;
    ans.y = right;
    ans.z = right;
    return ans;
}

Vector3 Vector3::operator+=(const Vector3& right)
{
    this->x = this->x + right.x;
    this->y = this->y + right.y;
    this->z = this->z + right.z;
    return *this;
}

Vector3 Vector3::operator-=(const Vector3& right)
{
    this->x = this->x - right.x;
    this->y = this->y - right.y;
    this->z = this->z - right.z;
    return *this;
}

Vector3 Vector3::operator*=(const float& right)
{
    this->x = this->x * right;
    this->y = this->y * right;
    this->z = this->z * right;
    return *this;
}

Vector3 Vector3::operator/=(const float& right)
{
    this->x = this->x / right;
    this->y = this->y / right;
    this->z = this->z / right;
    return *this;
}

bool Vector3::operator==(const Vector3& right) const
{
    bool isFlag = (this->x == right.x && this->y == right.y && this->z == right.z);
    return isFlag;
}

bool Vector3::operator!=(const Vector3& right) const
{
    bool isFlag = (this->x != right.x || this->y != right.y || this->z != right.z);
    return isFlag;
}
