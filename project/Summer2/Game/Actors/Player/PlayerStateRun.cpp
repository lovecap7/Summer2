#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
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
	//�ړ����x
	constexpr float kMoveSpeed = 15.0f;//�n��̈ړ����x
	//�A�j���[�V����
	const char* kAnim = "Player|Dash";
}


PlayerStateRun::PlayerStateRun(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//������
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}


PlayerStateRun::~PlayerStateRun()
{
}

void PlayerStateRun::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateRun::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//���S
	if (m_player->GetHurtPoint()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//�U�����󂯂���
	if (m_player->GetHurtPoint()->IsHit())
	{
		//������
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	auto collidable = m_player->GetCollidable();
	//�������Ă��邩�`�F�b�N
	if (collidable->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//����{�^������������
	if (input.IsTrigger("A"))
	{
		//���
		ChangeState(std::make_shared<PlayerStateRolling>(m_player));
		return;
	}
	//�Q�[�W������Ƃ��g����
	if (input.IsTrigger("RB"))
	{
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player, attackManager));
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("B") && m_player->IsGround())
	{
		//�W�����v
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	//��U���{�^������������
	if (input.IsTrigger("X"))
	{
		//��U��
		ChangeState(std::make_shared<PlayerStateAttackN1>(m_player));
		return;
	}
	//�`���[�W�{�^������������
	if (input.IsTrigger("Y"))
	{
		//�`���[�W
		ChangeState(std::make_shared<PlayerStateCharge>(m_player));
		return;
	}
	//���͂��Ȃ��Ȃ�ҋ@
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�ړ�
	collidable->GetRb()->SetMoveVec(GetForwardVec(input, camera) * kMoveSpeed);
	//�����̍X�V
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(VGet(-dir.x, 0.0f, dir.y));
}
