#pragma once
#include <cmath>

struct Vector2
{
	Vector2() :x(0.0f), y(0.0f) {};
	Vector2(float initX,float initY) :x(initX), y(initY) {};
	float x, y;
	/// <summary>
	/// �x�N�g���̑傫����Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	float Magnitude()const;

	/// <summary>
	/// ���K��(�傫����1�ɂ���)
	/// </summary>
	Vector2 Normalize() const;

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	float Dot(const Vector2& right)const;

	//���Z�q�I�[�o�[���[�h
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

//���W�ŕ\���Ƃ��Ɏg��
typedef Vector2 Position2;

//�~
struct Circle2D
{
	Position2 pos;//���S���W
	float radius;//���a
	Circle2D() :pos(0.0f, 0.0f), radius(0) {};
	Circle2D(const Position2& p,float r) :pos(p), radius(r) {};
	void Draw();
	void Draw(bool isFull, unsigned int color);//�����̉~��`��(�h��Ԃ�)
};

//�J�v�Z��
struct Capsule2D
{
	Position2 posA;//�[�_A
	Position2 posB;//�[�_B
	float radius;//���a
	Capsule2D() :posA(0.0f,0.0f), posB(0.0f,0.0f), radius(0.0f) {};
	Capsule2D(const Position2& a, const Position2& b, float r) :posA(a), posB(b), radius(r) {};
	void Draw(bool isFull,unsigned int color);//�J�v�Z���`��
	Vector2 GetCenterPos();//���S���W�Ԃ�
	Capsule2D RotateCapsule(const Position2& center, float angle);//����_�𒆐S�ɉ��
};


struct Rect2D
{
	Position2 pos;//���S���W
	float w,h;//���ƍ���
	Rect2D() :pos(0.0f, 0.0f), w(0.0f), h(0.0f) {};
	Rect2D(const Position2& p, float initW, float initH) :pos(p), w(initW), h(initH) {};
	void SetCenter(const Position2& p) { pos = p; }
	Vector2 GetCenter() { return pos; }
	float GetLeft() { return pos.x - (w / 2); }
	float GetRight() { return pos.x + (w / 2); }
	float GetTop() { return pos.y - (h / 2); }
	float GetBottom() { return pos.y + (h / 2); }
	void Draw();//�����̋�`��`��(�A�E�g���C���̂�)
	void Draw(bool isFull, unsigned int color);//�����̋�`��`��(�h��Ԃ�)
	void Draw(Vector2& offset);//�I�t�Z�b�g�t���`��
};

struct Matrix3x3
{
	float mat[3][3];
};

/// <summary>
/// /�P�ʍs���Ԃ�(3x3)
/// </summary>
/// <returns></returns>
Matrix3x3 IdentityMat3x3();

/// <summary>
/// ���s�ړ��s���Ԃ�(3x3)
/// </summary>
/// <param name="x">X�����̈ړ���</param>
/// <param name="y">Y�����̈ړ���</param>
/// <returns></returns>
Matrix3x3 TranslateMat3x3(float x, float y);

/// <summary>
/// ��]�s���Ԃ�(3x3)
/// </summary>
/// <param name="angle">��]�p�x</param>
/// <returns></returns>
Matrix3x3 RotateMat3x3(float angle);

/// <summary>
/// �Q�̍s��̏�Z��Ԃ�(��Z�̏��Ԃɒ���)
/// </summary>
/// <param name="left">���Ӎs��</param>
/// <param name="right">�E�Ӎs��</param>
/// <returns></returns>
Matrix3x3  MultipleMat3x3(const Matrix3x3& left, const Matrix3x3& right);
Matrix3x3 operator* (const Matrix3x3& left, const Matrix3x3& right);

/// <summary>
/// �x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
/// </summary>
/// <param name="m">�s��</param>
/// <param name="v">�x�N�g��</param>
/// <returns></returns>
Vector2 MultipleVec2(const Matrix3x3& m, const Vector2& v);
Vector2 operator*(const Matrix3x3& m, const Vector2& v);

/// <summary>
/// �Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂�
/// </summary>
/// <param name="center">��]���S�_</param>
/// <param name="angle">��]�p�x</param>
/// <returns></returns>
Matrix3x3 RotatePositionMatrix(const Position2& center, float angle);
