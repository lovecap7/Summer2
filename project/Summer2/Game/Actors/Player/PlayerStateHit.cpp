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
	//�A�j���[�V����
	const char* kAnim = "Player|Hit1";
	//������
	constexpr float kDeceRate = 0.95f;
}


PlayerStateHit::PlayerStateHit(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//�q�b�g���A�N�V����
	m_player->GetModel()->SetAnim(kAnim, false);
	m_player->GetCollidable()->SetState(State::None);
	//�t���O���Z�b�g
	m_player->SetIsHit(false);
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//���ꃊ�A�N�V�������ɍU����H�������A�j���[�V���������߂���
	if (m_player->IsHit())
	{
		//���v���C
		m_player->GetModel()->ReplayAnim();
		//�t���O���Z�b�g
		m_player->SetIsHit(false);
	}

	//���f���̃A�j���[�V�������I�������
	if (m_player->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
}

void PlayerStateHit::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}
