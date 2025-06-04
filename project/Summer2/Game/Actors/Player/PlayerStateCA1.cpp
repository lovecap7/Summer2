#include "PlayerStateCA1.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/HurtPoint.h"
namespace
{
	//�`���[�W�U���̒i�K�ʃ_���[�W
	constexpr int kCA1AnimDamage = 100.0f;
	//�`���[�W�U���̎���
	constexpr float kCA1KeepFrame = 30.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|CA";//�`���[�W�U��
	//�`���[�W�U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCA1AnimSpeed = 2.0f;
	//�����̍��{�Ƒ���̃C���f�b�N�X
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//�����̔��a
	constexpr float kLeftLegRadius = 20.0f;
	//�ړ����x
	constexpr float kAttackMoveSpeed = 5.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateCA1::PlayerStateCA1(std::shared_ptr<Player> player, const std::shared_ptr<AttackManager>& attackManager) :
	PlayerStateBase(player)
{
	m_player->GetCollidable()->SetState(State::None);
	//�`���[�W�U��1
	auto model = m_player->GetModel();
	model->SetAnim(kAnim, true, kCA1AnimSpeed);
	model->SetFixedLoopFrame(kCA1KeepFrame);//�w�胋�[�v
	//�U������̏���
	CreateAttack();
	attackManager->Entry(m_attackC);
}

PlayerStateCA1::~PlayerStateCA1()
{
	//�U�����������
	m_attackC->Delete();
}
void PlayerStateCA1::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateCA1::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//�U�����󂯂���
	if (m_player->GetHurtPoint()->IsHit())
	{
		//������
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	auto model = m_player->GetModel();
	//�A�j���[�V�������I��������
	if (model->IsFinishFixedLoop())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�A�j���[�V������������邽�тɍU������̃��Z�b�g
	if (model->IsFinishAnim())
	{
		//������
		m_attackC->Init();
	}
	//���͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�ړ�
		m_player->GetCollidable()->GetRb()->SetMoveVec(GetForwardVec(input, camera) * kAttackMoveSpeed);
	}
	else
	{
		//��������������
		SpeedDown();
	}
	//�U���̈ʒu�X�V
	UpdateAttack();
}

void PlayerStateCA1::CreateAttack()
{
	auto model = m_player->GetModel();
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//����
	m_leftLeg = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(toe, kLeftLegRadius),
		std::make_shared<Rigidbody>(root));

	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attackC = std::make_shared<MeleeAttack>(m_leftLeg, kCA1AnimDamage, kCA1KeepFrame, m_player);
}

void PlayerStateCA1::UpdateAttack()
{
	//�����̏�Ԃ��X�V������U�����X�V�����
	auto model = m_player->GetModel();
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//����
	//���W�X�V
	m_leftLeg->GetRb()->SetPos(root);
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->SetEndPos(toe);
}

void PlayerStateCA1::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
