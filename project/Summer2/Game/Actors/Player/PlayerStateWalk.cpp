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
	//�ړ����x
	constexpr float kLowMoveSpeed = 2.0f;//�n��̏��ړ����x
	constexpr float kMediumMoveSpeed = 5.0f;//�n��̒��ړ����x
	constexpr float kHighMoveSpeed = 10.0f;//�n��̑�ړ����x
	//�A�j���[�V����
	const char* kAnim = "Player|Run";
}

PlayerStateWalk::PlayerStateWalk(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//�������
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateWalk::~PlayerStateWalk()
{
}
void PlayerStateWalk::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateWalk::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	auto collidable = m_player->GetCollidable();
	//�������Ă��邩�`�F�b�N
	if (collidable->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//����{�^������������
	if (input.IsTrigger("RB"))
	{
		//���
		ChangeState(std::make_shared<PlayerStateRolling>(m_player));
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("A") && m_player->IsGround())
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
	collidable->GetRb()->SetMoveVec(GetForwardVec(input,camera) * InputValueSpeed(input));
	//�����̍X�V
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(VGet(dir.x, 0.0f, dir.y));
}


float PlayerStateWalk::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//���x���X�e�B�b�N���͂̐[�x�ɍ��킹��
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