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
	//�󒆈ړ����x
	constexpr float kAirMoveSpeed = 1.5f;
	//�󒆈ړ��ō����x
	constexpr float kMaxAirMoveSpeed = 20.0f;
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

void PlayerStateFall::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�n�ʂɕt���Ă���Ȃ�
	if (m_player->IsGround())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto collidable = m_player->GetCollidable();
	//�󒆈ړ�
	collidable->GetRb()->AddVec(GetForwardVec(input,camera) * kAirMoveSpeed);
	//���ړ����x�ɏ��������
	float speed = collidable->GetRb()->GetMoveVec().Magnitude();
	if (speed > 0.0f)
	{
		speed = ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
		collidable->GetRb()->SetMoveVec(collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}
	//��������������
	SpeedDown();
	//�����̍X�V
	m_player->GetModel()->SetDir(collidable->GetRb()->GetVec().ToDxLibVector());
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
