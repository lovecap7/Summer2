#include "PlayerStateRolling.h"
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
#include "../../Attack/HurtPoint.h"

namespace
{
	//��𑬓x
	constexpr float kRollingMoveSpeed = 10.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Rolling";
	//������[�V�����̑��x
	constexpr float kAnimSpeed = 1.2f;
}

PlayerStateRolling::PlayerStateRolling(std::shared_ptr<Player> player) :
	PlayerStateBase(player)
{
	//������
	m_player->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	m_player->GetCollidable()->SetState(State::None);
	//�����̍X�V
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(VGet(-dir.x, 0.0f, dir.y));
	//���G��
	m_player->GetHurtPoint()->SetIsNoDamege(true);
}

PlayerStateRolling::~PlayerStateRolling()
{
	//���G����
	m_player->GetHurtPoint()->SetIsNoDamege(false);
}
void PlayerStateRolling::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateRolling::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	auto collidable = m_player->GetCollidable();
	//���f���̃A�j���[�V�������I�������
	if (m_player->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�����Ă�����Ɉړ�
	collidable->GetRb()->SetMoveVec(m_player->GetModel()->GetDir() * kRollingMoveSpeed);
}
