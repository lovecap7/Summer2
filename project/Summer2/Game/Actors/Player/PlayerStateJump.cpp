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
	//�W�����v��
	const Vector3 kJumpVec = { 0.0f,13.0f,0.0f };
	//�󒆈ړ����x
	constexpr float kAirMoveSpeed = 1.5f;
	//�󒆈ړ��ō����x
	constexpr float kMaxAirMoveSpeed = 20.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Jump";
	//����
	constexpr float kAirMoveDeceRate = 0.9f;
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

void PlayerStateJump::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	auto collidable = m_player->GetCollidable();
	//�������Ă���Ȃ�
	if (collidable->GetRb()->GetVec().y < 0.0f)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//�󒆈ړ�
	collidable->GetRb()->AddVec(GetForwardVec(input ,camera) * kAirMoveSpeed);
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
void PlayerStateJump::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kAirMoveDeceRate;
	vec.z *= kAirMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

