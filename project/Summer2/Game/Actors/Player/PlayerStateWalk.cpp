#include "PlayerStateWalk.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
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
	//移動速度
	constexpr float kLowMoveSpeed = 2.0f;//地上の小移動速度
	constexpr float kMediumMoveSpeed = 5.0f;//地上の中移動速度
	constexpr float kHighMoveSpeed = 10.0f;//地上の大移動速度
	//アニメーション
	const char* kAnim = "Player|Run";
}

PlayerStateWalk::PlayerStateWalk(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//歩き状態
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateWalk::~PlayerStateWalk()
{
}
void PlayerStateWalk::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateWalk::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	auto collidable = m_player->GetCollidable();
	//落下しているかチェック
	if (collidable->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_player));
		return;
	}
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("A") && m_player->IsGround())
	{
		//ジャンプ
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	//弱攻撃ボタンを押したら
	if (input.IsTrigger("X"))
	{
		//弱攻撃
		ChangeState(std::make_shared<PlayerStateAttackN1>(m_player));
		return;
	}
	//チャージボタンを押したら
	if (input.IsTrigger("Y"))
	{
		//チャージ
		ChangeState(std::make_shared<PlayerStateCharge>(m_player));
		return;
	}
	//入力がないなら待機
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//移動
	collidable->GetRb()->SetMoveVec(GetForwardVec(input,camera) * InputValueSpeed(input));
	//向きの更新
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(VGet(dir.x, 0.0f, dir.y));
}


float PlayerStateWalk::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//速度をスティック入力の深度に合わせる
	if (input.IsLowPowerLeftStick())
	{
		moveSpeed = kLowMoveSpeed;
	}
	else if (input.IsMediumPowerLeftStick())
	{
		moveSpeed = kMediumMoveSpeed;
	}
	else if (input.IsHighPowerLeftStick())
	{
		moveSpeed = kHighMoveSpeed;
	}
	return moveSpeed;
}