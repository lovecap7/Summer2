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
	//�W�����v��
	const Vector3 kJumpVec = { 0.0f,13.0f,0.0f };
	//�󒆈ړ����x
	constexpr float kAirMoveSpeed = 1.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Jump";
	//����
	constexpr float kAirMoveDeceRate = 0.9f;
	//�ړ����x
	constexpr float kLowAirMoveSpeed = 2.0f;//�󒆂̏��ړ����x
	constexpr float kMediumAirMoveSpeed = 5.0f;//�󒆂̒��ړ����x
	constexpr float kHighAirMoveSpeed = 10.0f;//�󒆂̑�ړ����x
}

PlayerStateJump::PlayerStateJump(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//�W�����v
	m_player->GetModel()->SetAnim(kAnim, true);
	auto collidable = m_player->GetCollidable();
	collidable->SetState(State::Jump);
	//�n�ʂ��痣���̂�false�ɂ��Ă���
	m_player->NoIsGround();
	//�͂�^����
	collidable->GetRb()->ResetVec();
	collidable->GetRb()->SetVec(kJumpVec);
}

PlayerStateJump::~PlayerStateJump()
{
}
void PlayerStateJump::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateJump::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
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
	//�������Ă���Ȃ�
	if (collidable->GetRb()->GetVec().y < 0.0f)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//�ړ��̓��͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�󒆈ړ�
		collidable->GetRb()->AddVec(GetForwardVec(input, camera) * kAirMoveSpeed);
		//���ړ����x�ɏ��������
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
		//��������������
		SpeedDown();
	}
	//�����̍X�V
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
}
void PlayerStateJump::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kAirMoveDeceRate;
	vec.z *= kAirMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}


float PlayerStateJump::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//���x���X�e�B�b�N���͂̐[�x�ɍ��킹��
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

