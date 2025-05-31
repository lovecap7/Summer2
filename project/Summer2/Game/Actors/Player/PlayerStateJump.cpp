#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
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
	//ジャンプ力
	const Vector3 kJumpVec = { 0.0f,13.0f,0.0f };
	//空中移動速度
	constexpr float kAirMoveSpeed = 1.5f;
	//空中移動最高速度
	constexpr float kMaxAirMoveSpeed = 20.0f;
	//アニメーション
	const char* kAnim = "Player|Jump";
	//減速
	constexpr float kAirMoveDeceRate = 0.9f;
}

PlayerStateJump::PlayerStateJump(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//ジャンプ
	m_player->GetModel()->SetAnim(kAnim, true);
	auto collidable = m_player->GetCollidable();
	collidable->SetState(State::Jump);
	//地面から離れるのでfalseにしておく
	m_player->NoIsGround();
	//力を与える
	collidable->GetRb()->ResetVec();
	collidable->GetRb()->SetVec(kJumpVec);
}

PlayerStateJump::~PlayerStateJump()
{
}

void PlayerStateJump::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	auto collidable = m_player->GetCollidable();
	//落下しているなら
	if (collidable->GetRb()->GetVec().y < 0.0f)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//空中移動
	collidable->GetRb()->AddVec(GetForwardVec(input ,camera) * kAirMoveSpeed);
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
void PlayerStateJump::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kAirMoveDeceRate;
	vec.z *= kAirMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

