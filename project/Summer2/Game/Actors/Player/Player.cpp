#include "Player.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include <DxLib.h>

namespace
{
	constexpr float kCapsuleHeight = 50.0f; //カプセルの上端
	constexpr float kCapsuleRadius = 10.0f; //カプセルの半径
}

Player::Player(int modelHandle, Position3 firstPos) :
	m_modelHandle(modelHandle),
	m_update(&Player::IdleUpdate)
{
	//初期位置
	Vector3 endPos = firstPos;
	endPos.y += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
}

Player::~Player()
{
}

void Player::Update(const Input& input)
{
	//更新
	(this->*m_update)(input);
}

void Player::Draw() const
{
	//モデルの描画
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
