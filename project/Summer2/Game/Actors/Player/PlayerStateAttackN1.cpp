#include "PlayerStateAttackN1.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAttackN2.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
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
namespace
{
	//�ʏ�U��1�̃_���[�W�Ǝ����t���[���Ɣ����t���[��
	constexpr int kAN1Damege = 100.0f;
	constexpr int kAN1KeepFrame = 6;
	constexpr int kAN1StartFrame = 15;
	//�A�j���[�V����
	const char* kAnim = "Player|Attack_N1";
	//��U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN1AnimSpeed = 1.3f;
	//�U���I���O�ɃL�����Z���\�t���[��
	constexpr float kAttackCancelFrame = 20.0f;
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒����Ɣ��a
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateAttackN1::PlayerStateAttackN1(std::shared_ptr<Player> player):
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	//�ʏ�U��1
	m_player->GetCollidable()->SetState(State::None);
	//�U��1
	m_player->GetModel()->SetAnim(kAnim, false, kAN1AnimSpeed);
	//�U������̏���
	CreateAttack();
}

PlayerStateAttackN1::~PlayerStateAttackN1()
{
	//�U�����������
	m_attackN1->Delete();
}

void PlayerStateAttackN1::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateAttackN1::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAN1StartFrame)
	{
		//�U��������
		AppearAttack(m_attackN1, attackManager);
	}
	auto model = m_player->GetModel();
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("RB"))
		{
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//2�i��
		if (input.IsTrigger("X"))
		{
			ChangeState(std::make_shared<PlayerStateAttackN2>(m_player));
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�`���[�W
			ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//�U���̈ʒu�X�V
	UpdateAttack();
	//��������������
	SpeedDown();
}

void PlayerStateAttackN1::CreateAttack()
{
	auto model = m_player->GetModel();
	//����̈ʒu���o��
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kRightSwordRadius), 
		std::make_shared<Rigidbody>(Vector3(ringFinger.x, ringFinger.y, ringFinger.z)));

	//����̍��W�Ɠ����蔻����U���ɕR�}����
	m_attackN1 = std::make_shared<MeleeAttack>(m_rightSword, kAN1Damege, kAN1KeepFrame, m_player);
}

void PlayerStateAttackN1::UpdateAttack()
{
	//����̏�Ԃ��X�V������U�����X�V�����

	auto model = m_player->GetModel();
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	//����̎�������Z�b�g
	m_rightSword->GetRb()->SetPos(Position3(ringFinger.x, ringFinger.y, ringFinger.z));
	//����̌�����Z�b�g
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void PlayerStateAttackN1::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
