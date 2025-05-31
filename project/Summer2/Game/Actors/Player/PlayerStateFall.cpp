#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//空中移動速度
	constexpr float kAirMoveSpeed = 1.5f;
	//空中移動最高速度
	constexpr float kMaxAirMoveSpeed = 20.0f;
	//アニメーション
	const char* kAnim = "Player|Fall";
	//減速
	constexpr float kAirMoveDeceRate = 0.9f;
}

PlayerStateFall::PlayerStateFall(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//落下
	m_player->GetModel()->SetAnim(kAnim, true);
	//落下してるので
	m_player->NoIsGround();
	m_player->GetCollidable()->SetState(State::Fall);
}
PlayerStateFall::~PlayerStateFall()
{
}

void PlayerStateFall::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//地面に付いているなら
	if (m_player->IsGround())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto collidable = m_player->GetCollidable();
	//空中移動
	collidable->GetRb()->AddVec(GetForwardVec(input,camera) * kAirMoveSpeed);
	//横移動速度に上限をつける
	float speed = collidable->GetRb()->GetMoveVec().Magnitude();
	if (speed > 0.0f)
	{
		speed = ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
		collidable->GetRb()->SetMoveVec(collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}
	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_player->GetModel()->SetDir(collidable->GetRb()->GetVec().ToDxLibVector());
}

void PlayerStateFall::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kAirMoveDeceRate;
	vec.z *= kAirMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
