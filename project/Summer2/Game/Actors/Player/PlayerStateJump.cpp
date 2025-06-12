#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/HurtPoint.h"

namespace
{
	//ジャンプ力
	const Vector3 kJumpVec = { 0.0f,13.0f,0.0f };
	//空中移動速度
	constexpr float kAirMoveSpeed = 1.0f;
	//アニメーション
	const char* kAnim = "Player|Jump";
	//減速
	constexpr float kAirMoveDeceRate = 0.9f;
	//移動速度
	constexpr float kLowAirMoveSpeed = 2.0f;//空中の小移動速度
	constexpr float kMediumAirMoveSpeed = 5.0f;//空中の中移動速度
	constexpr float kHighAirMoveSpeed = 10.0f;//空中の大移動速度
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
void PlayerStateJump::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateJump::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//死亡
	if (m_player->GetHurtPoint()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//攻撃を受けた時
	if (m_player->GetHurtPoint()->IsHit())
	{
		//やられ状態
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	auto collidable = m_player->GetCollidable();
	//落下しているなら
	if (collidable->GetRb()->GetVec().y < 0.0f)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//空中移動
		collidable->GetRb()->AddVec(GetForwardVec(input, camera) * kAirMoveSpeed);
		//横移動速度に上限をつける
		float speed = collidable->GetRb()->GetMoveVec().Magnitude();
		if (speed > 0.0f)
		{
			speed = MathSub::ClampFloat(speed, 0.0f, InputValueSpeed(input));
			Vector3 moveVec = collidable->GetRb()->GetMoveVec();
			if (moveVec.Magnitude() > 0.0f)
			{
				moveVec = moveVec.Normalize();
			}
			collidable->GetRb()->SetMoveVec(moveVec * speed);
		}
	}
	else
	{
		//少しずつ減速する
		SpeedDown();
	}
	//向きの更新
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
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


float PlayerStateJump::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//速度をスティック入力の深度に合わせる
	if (input.IsLowPowerLeftStick())
	{
		moveSpeed = kLowAirMoveSpeed;
	}
	else if (input.IsMediumPowerLeftStick())
	{
		moveSpeed = kMediumAirMoveSpeed;
	}
	else if (input.IsHighPowerLeftStick())
	{
		moveSpeed = kHighAirMoveSpeed;
	}
	return moveSpeed;
}

