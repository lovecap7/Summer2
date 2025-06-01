#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
namespace
{
	//アニメーション
	const char* kAnim = "Player|Hit1";
	//減速率
	constexpr float kDeceRate = 0.95f;
}


PlayerStateHit::PlayerStateHit(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//ヒットリアクション
	m_player->GetModel()->SetAnim(kAnim, false);
	m_player->GetCollidable()->SetState(State::None);
	//フラグリセット
	m_player->SetIsHit(false);
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//やられリアクション中に攻撃を食らったらアニメーションを初めから
	if (m_player->IsHit())
	{
		//リプレイ
		m_player->GetModel()->ReplayAnim();
		//フラグリセット
		m_player->SetIsHit(false);
	}

	//モデルのアニメーションが終わったら
	if (m_player->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
}

void PlayerStateHit::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}
