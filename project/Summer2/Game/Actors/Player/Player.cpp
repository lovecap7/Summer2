#include "Player.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include <DxLib.h>

namespace
{
	constexpr float kCapsuleHeight = 50.0f; //�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 10.0f; //�J�v�Z���̔��a
}

Player::Player(int modelHandle, Position3 firstPos) :
	m_modelHandle(modelHandle),
	m_update(&Player::IdleUpdate)
{
	//�����ʒu
	Vector3 endPos = firstPos;
	endPos.y += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
}

Player::~Player()
{
}

void Player::Update(const Input& input)
{
	//�X�V
	(this->*m_update)(input);
}

void Player::Draw() const
{
	//���f���̕`��
	MV1DrawModel(m_modelHandle);
}

void Player::Complete()
{
}

void Player::IdleUpdate(const Input& input)
{
}

void Player::MoveUpdate(const Input& input)
{
}

void Player::JumpUpdate(const Input& input)
{
}
