#include "Matrix4x4.h"
#include <cmath>

Matrix4x4::Matrix4x4():
    mat{}
{
}
//�P�ʍs��
Matrix4x4 IdentityMat4x4()
{
    Matrix4x4 identity = {};//�v�f�����ׂ�0�ɂ���
    identity.mat[0][0] = 1.0f;
    identity.mat[1][1] = 1.0f;
    identity.mat[2][2] = 1.0f;
    identity.mat[3][3] = 1.0f;
    return identity;
}

//���s�ړ�
Matrix4x4 TranslateMat4x4(float x, float y, float z)
{
    Matrix4x4 translate = {};
    translate = IdentityMat4x4();//�P�ʍs��ɂ���
    translate.mat[0][3] = x;
    translate.mat[1][3] = y;
    translate.mat[2][3] = z;
    return translate;
}

//X����]
Matrix4x4 RotateXMat4x4(float angle)
{
    Matrix4x4 rotaX{};
    rotaX = IdentityMat4x4();//�P�ʍs��
    rotaX.mat[1][1] = cosf(angle);
    rotaX.mat[1][2] = -sinf(angle);
    rotaX.mat[2][1] = sinf(angle);
    rotaX.mat[2][2] = cosf(angle);
    return rotaX;
}

//Y����]
Matrix4x4 RotateYMat4x4(float angle)
{
    Matrix4x4 rotaY{};
    rotaY = IdentityMat4x4();//�P�ʍs��
    rotaY.mat[0][0] = cosf(angle);
    rotaY.mat[0][2] = sinf(angle);
    rotaY.mat[2][0] = -sinf(angle);
    rotaY.mat[2][2] = cosf(angle);
    return rotaY;
}

//Z����]
Matrix4x4 RotateZMat4x4(float angle)
{
    Matrix4x4 rotaZ{};
    rotaZ = IdentityMat4x4();//�P�ʍs��
    rotaZ.mat[0][0] = cosf(angle);
    rotaZ.mat[0][1] = -sinf(angle);
    rotaZ.mat[1][0] = sinf(angle);
    rotaZ.mat[1][1] = cosf(angle);
    return rotaZ;
}

//�}�g���N�X���m�̏�Z
Matrix4x4 Matrix4x4::MultipleMat4x4(const Matrix4x4& left, const Matrix4x4& right)
{
    //����ɏ�Z���ʂ����Ă���
    Matrix4x4 mul = {};
    mul.mat[0][0] = left.mat[0][0] * right.mat[0][0] + left.mat[0][1] * right.mat[1][0] + left.mat[0][2] * right.mat[2][0] + left.mat[0][3] * right.mat[3][0];
    mul.mat[0][1] = left.mat[0][0] * right.mat[0][1] + left.mat[0][1] * right.mat[1][1] + left.mat[0][2] * right.mat[2][1] + left.mat[0][3] * right.mat[3][1];
    mul.mat[0][2] = left.mat[0][0] * right.mat[0][2] + left.mat[0][1] * right.mat[1][2] + left.mat[0][2] * right.mat[2][2] + left.mat[0][3] * right.mat[3][2];
    mul.mat[0][3] = left.mat[0][0] * right.mat[0][3] + left.mat[0][1] * right.mat[1][3] + left.mat[0][2] * right.mat[2][3] + left.mat[0][3] * right.mat[3][3];

    mul.mat[1][0] = left.mat[1][0] * right.mat[0][0] + left.mat[1][1] * right.mat[1][0] + left.mat[1][2] * right.mat[2][0] + left.mat[1][3] * right.mat[3][0];
    mul.mat[1][1] = left.mat[1][0] * right.mat[0][1] + left.mat[1][1] * right.mat[1][1] + left.mat[1][2] * right.mat[2][1] + left.mat[1][3] * right.mat[3][1];
    mul.mat[1][2] = left.mat[1][0] * right.mat[0][2] + left.mat[1][1] * right.mat[1][2] + left.mat[1][2] * right.mat[2][2] + left.mat[1][3] * right.mat[3][2];
    mul.mat[1][3] = left.mat[1][0] * right.mat[0][3] + left.mat[1][1] * right.mat[1][3] + left.mat[1][2] * right.mat[2][3] + left.mat[1][3] * right.mat[3][3];

    mul.mat[2][0] = left.mat[2][0] * right.mat[0][0] + left.mat[2][1] * right.mat[1][0] + left.mat[2][2] * right.mat[2][0] + left.mat[2][3] * right.mat[3][0];
    mul.mat[2][1] = left.mat[2][0] * right.mat[0][1] + left.mat[2][1] * right.mat[1][1] + left.mat[2][2] * right.mat[2][1] + left.mat[2][3] * right.mat[3][1];
    mul.mat[2][2] = left.mat[2][0] * right.mat[0][2] + left.mat[2][1] * right.mat[1][2] + left.mat[2][2] * right.mat[2][2] + left.mat[2][3] * right.mat[3][2];
    mul.mat[2][3] = left.mat[2][0] * right.mat[0][3] + left.mat[2][1] * right.mat[1][3] + left.mat[2][2] * right.mat[2][3] + left.mat[2][3] * right.mat[3][3];

    mul.mat[3][0] = left.mat[3][0] * right.mat[0][0] + left.mat[3][1] * right.mat[1][0] + left.mat[3][2] * right.mat[2][0] + left.mat[3][3] * right.mat[3][0];
    mul.mat[3][1] = left.mat[3][0] * right.mat[0][1] + left.mat[3][1] * right.mat[1][1] + left.mat[3][2] * right.mat[2][1] + left.mat[3][3] * right.mat[3][1];
    mul.mat[3][2] = left.mat[3][0] * right.mat[0][2] + left.mat[3][1] * right.mat[1][2] + left.mat[3][2] * right.mat[2][2] + left.mat[3][3] * right.mat[3][2];
    mul.mat[3][3] = left.mat[3][0] * right.mat[0][3] + left.mat[3][1] * right.mat[1][3] + left.mat[3][2] * right.mat[2][3] + left.mat[3][3] * right.mat[3][3];
    return mul;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& right)
{
    return MultipleMat4x4(*this, right);
}

//�x�N�g���ɍs�����Z�����x�N�g����Ԃ�
Vector3 Matrix4x4::MultipleVec3(const Matrix4x4& m, const Vector3& v)
{
    Vector3 vec = {};
    vec.x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * 1.0f;
    vec.y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * 1.0f;
    vec.z = m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * 1.0f;
    return vec;
}

Vector3 Matrix4x4::operator*(const Vector3& v)
{
    return MultipleVec3(*this, v);
}

//������W�𒆐S��Y����]�������W
Matrix4x4 RotateYPositionMatrix4x4(const Position3& center, float angle)
{
    //���S�����_�ɕ��s�ړ�������
     //���_���S�ɉ�]
     //���S�����Ƃ̍��W�ɖ߂�
    Matrix4x4 rotaMat = TranslateMat4x4(center.x, center.y, center.z) *//���̍��W
        RotateYMat4x4(angle) *//��]
        TranslateMat4x4(-center.x, -center.y, -center.z);//���_�ɖ߂�
    return rotaMat;
}


//������W�𒆐S��X����]�������W
Matrix4x4 RotateXPositionMatrix4x4(const Position3& center, float angle)
{
    //���S�����_�ɕ��s�ړ�������
     //���_���S�ɉ�]
     //���S�����Ƃ̍��W�ɖ߂�
    Matrix4x4 rotaMat = TranslateMat4x4(center.x, center.y, center.z) *//���̍��W
        RotateXMat4x4(angle) *//��]
        TranslateMat4x4(-center.x, -center.y, -center.z);//���_�ɖ߂�
    return rotaMat;
}

Matrix4x4 RotateZPositionMatrix4x4(const Position3& center, float angle)
{
    //���S�����_�ɕ��s�ړ�������
    //���_���S�ɉ�]
    //���S�����Ƃ̍��W�ɖ߂�
    Matrix4x4 rotaMat = TranslateMat4x4(center.x, center.y, center.z) *//���̍��W
        RotateZMat4x4(angle) *//��]
        TranslateMat4x4(-center.x, -center.y, -center.z);//���_�ɖ߂�
    return rotaMat;
}
//������W�𒆐S��X����]����Y����]�������W
Matrix4x4 RotateXYPositionMatrix4x4(const Position3& center, float hAngle, float vAngle)
{
    //���S�����_�ɕ��s�ړ�������
     //���_���S�ɉ�]
     //���S�����Ƃ̍��W�ɖ߂�
    Matrix4x4 rotaMat = TranslateMat4x4(-center.x, -center.y, -center.z) *//���̍��W
        RotateYMat4x4(hAngle) *//��]
        RotateXMat4x4(vAngle) *//��]
        TranslateMat4x4(center.x, center.y, center.z);//���_�ɖ߂�
    return rotaMat;
}

