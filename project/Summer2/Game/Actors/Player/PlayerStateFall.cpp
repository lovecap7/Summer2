#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"
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
	//�󒆈ړ����x
	constexpr float kAirMoveSpeed = 1.5f;
	//�󒆈ړ��ō����x
	constexpr float kMaxAirMoveSpeed = 10.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Fall";
	//����
	constexpr float kAirMoveDeceRate = 0.9f;
}

PlayerStateFall::PlayerStateFall(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//����
	m_player->GetModel()->SetAnim(kAnim, true);
	//�������Ă�̂�
	m_player->NoIsGround();
	m_player->GetCollidable()->SetState(State::Fall);
}
PlayerStateFall::~PlayerStateFall()
{
}
void PlayerStateFall::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateFall::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
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
	//�n�ʂɕt���Ă���Ȃ�
	if (m_player->IsGround())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto collidable = m_player->GetCollidable();
	//�ړ��̓��͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�󒆈ړ�
		collidable->GetRb()->AddVec(GetForwardVec(input, camera) * kAirMoveSpeed);
		//���ړ����x�ɏ��������
		float speed = collidable->GetRb()->GetMoveVec().Magnitude();
		if (speed > 0.0f)
		{
			speed = MathSub::ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
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
	m_player->GetModel()->SetDir(VGet(-dir.x, 0.0f, dir.y));
}

void PlayerStateFall::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kAirMoveDeceRate;
	vec.z *= kAirMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
