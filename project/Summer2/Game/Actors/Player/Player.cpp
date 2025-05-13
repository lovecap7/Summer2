#include "Player.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/Slash.h"
#include "../../Attack/AttackManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 20.0f; //�J�v�Z���̔��a
	//�ړ����x
	constexpr float kMoveSpeed = 10.0f;//�n��̈ړ����x
	constexpr float kAirMoveSpeed = 1.5f;//�󒆈ړ����x
	constexpr float kMaxAirMoveSpeed = 20.0f;//�󒆈ړ��ō����x
	constexpr float kLightAttackMoveSpeed = 0.5f;//��U�����̈ړ����x
	constexpr float kHighAttackMoveSpeed = 13.0f;//���U�����̈ړ����x
	constexpr float kRollingMoveSpeed = 10.0f;//��𑬓x
	//�W�����v
	constexpr float kMaxGravity = -10.0f;//�����X�s�[�h���傫���Ȃ肷���Ȃ��悤��
	const Vector3 kJumpVec = { 0.0f,13.0f,0.0f };//�W�����v
	constexpr unsigned int kMaxJumpNum = 2;//�W�����v��
	constexpr int kNextJumpFrame = 5;//���̃W�����v���ł���܂ł̃t���[��
	constexpr float kChangeStateFall = -2.0f;//������Ԃɐ؂�ւ�闎���x�N�g���̑傫��
	//������
	constexpr float kMoveDeceRate = 0.8f;
	constexpr float kAirMoveDeceRate = 0.9f;
	//�L�����Z���\�t���[��
	constexpr float kCancelAttackFrame = 15.0f;

	//���U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN1AnimSpeed = 1.1f;
	constexpr float kAN2AnimSpeed = 1.1f;
	constexpr float kAN3AnimSpeed = 1.2f;
	//���U���̒i�K�ʍU���t���[��
	constexpr float kCharge1Frame = 30.0f;
	constexpr float kCharge2Fraem = 60.0f;
	constexpr float kCharge3Frame = 100.0f;
	//���U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCharge1AnimSpeed = 1.0f;
	constexpr float kCharge2AnimSpeed = 1.5f;
	constexpr float kCharge3AnimSpeed = 2.0f;

	//����
	//�E��̖�w�̃t���[��
	constexpr int kRightHandPinky1 = 55;
	constexpr int kRightHandPinky2 = 43;
	//����̒����Ɣ��a
	constexpr float kSwordHeight = 100.0f;
	constexpr float kSwordRadius = 20.0f;

	//�ʏ�U��1�̃_���[�W�Ǝ����t���[���Ɣ����t���[��
	constexpr float kAttackN1Damege = 10.0f;
	constexpr int kAttackN1KeepFrame = 20;
	constexpr int kAttackN1StartFrame = 10;
	//�ʏ�U��2�̃_���[�W�Ǝ����t���[��
	constexpr float kAttackN2Damege = 20.0f;
	constexpr int kAttackN2KeepFrame = 20;
	constexpr int kAttackN2StartFrame = 10;
	//�ʏ�U��3�̃_���[�W�Ǝ����t���[��
	constexpr float kAttackN3Damege = 30.0f;
	constexpr int kAttackN3KeepFrame = 40;
	constexpr int kAttackN3StartFrame = 10;

	//�A�j���[�V�����̖��O
	const char* kIdleAnim = "Player|Idle";//�ҋ@
	const char* kRunAnim = "Player|Run";//����
	const char* kRollingAnim = "Player|Rolling";//���
	const char* kJumpAnim = "Player|Jump";//�W�����v
	const char* kFallAnim = "Player|Fall";//������
	const char* kAttack_N1Anim = "Player|Attack_N1";//�ʏ�U��1
	const char* kAttack_N2Anim = "Player|Attack_N2";//�ʏ�U��2
	const char* kAttack_N3Anim = "Player|Attack_N3";//�ʏ�U��3
	const char* kAttack_C1Anim = "Player|Attack_C1";//�`���[�W
	const char* kAttack_C2Anim = "Player|Attack_C2";//�`���[�W�U��
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_stickVec(0.0f,0.0f),
	m_update(&Player::IdleUpdate),
	m_lastUpdate(&Player::FallUpdate),
	m_isGround(true),
	m_jumpNum(0),
	m_nextJumpFrame(kNextJumpFrame),
	m_chargeHighAttackFrame(0),
	m_attackCountFrame(0)
{
	//�Փ˔���
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));

	//���ꔻ��(�Փ˔���Ɠ����ɂ���)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, 100, *this);

	//���f��
	m_model = std::make_unique<Model>(modelHandle, firstPos.ToDxLibVector());

	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR rightHand = MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky1);
	//����̖���
	VECTOR swordDir = VNorm(VSub(MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky2),rightHand));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(rightHand, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kSwordRadius), std::make_shared<Rigidbody>(Vector3(rightHand.x, rightHand.y, rightHand.z)));


	//�U���̏���
	m_attackN1 = std::make_shared<Slash>(m_rightSword, kAttackN1Damege, kAttackN1KeepFrame);
	m_attackN2 = std::make_shared<Slash>(m_rightSword, kAttackN2Damege, kAttackN2KeepFrame);
	m_attackN3 = std::make_shared<Slash>(m_rightSword, kAttackN3Damege, kAttackN3KeepFrame);

}

Player::~Player()
{
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�X�e�B�b�N�̌���������
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//��Ԃɍ��킹�ď�����
	StateInit();
	//�X�V
	(this->*m_update)(input,camera, attackManager);
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//�Փ˔�������Ƃɂ��ꔻ��̈ʒu�X�V

	//����̈ʒu�X�V
	WeaponUpdate();
}

void Player::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= kMaxGravity)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Player::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	if (other->GetColl()->GetShape() == Shape::Polygon)
	{
		if (std::dynamic_pointer_cast<PolygonCollider>(other->GetColl())->IsFloor())
		{
			m_isGround = true;
		}
	}
}

void Player::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_isGround//�n�ʂɂ���Ɠh��Ԃ����
	);
	//���ꔻ��
	DrawCapsule3D(
		m_hurtPoint->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetRadius(),
		32,
		0x0000ff,
		0x0000ff,
		m_hurtPoint->IsNoDamege()//���G�̎��͓h��Ԃ����
	);
	printf("POS = %2f, %2f, %2f\n", m_collidable->GetRb()->GetPos().x, m_collidable->GetRb()->GetPos().y, m_collidable->GetRb()->GetPos().z);
	printf("VEC = %2f, %2f, %2f\n", m_collidable->GetRb()->GetVec().x, m_collidable->GetRb()->GetVec().y, m_collidable->GetRb()->GetVec().z);
	DrawCapsule3D(
		m_rightSword->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetRadius(),
		16,
		0xffff00,
		0xffff00,
		false
	);
#endif
	m_model->Draw();
}

void Player::Complete()
{
	m_collidable->GetRb()->SetNextPos();//���̍��W��
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Player::HitReaction()
{
	//���ꃊ�A�N�V����
}

void Player::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�������Ă��邩�`�F�b�N
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//����
		m_update = &Player::FallUpdate;
		return;
	}
	//����{�^������������
	if (input.IsTriggered("RB"))
	{
		//���
		m_update = &Player::RollingUpdate;
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTriggered("A") && m_isGround)
	{
		//�W�����v
		m_update = &Player::JumpUpdate;
		return;
	}
	//��U���{�^������������
	if (input.IsTriggered("X"))
	{
		//�U��������(�e�X�g)
		attackManager->SetAttack(m_attackN1);
		m_attackN1->Init();

		//��U��
		m_update = &Player::AttackNormal1Update;
		return;
	}
	//���U���{�^������������
	if (input.IsTriggered("Y"))
	{
		//���U��
		m_update = &Player::AttackCharge1Update;
		return;
	}
	//���͂�����Ȃ�ړ�
	if (m_stickVec.Magnitude() != 0)
	{
		//�ړ�
		m_update = &Player::MoveUpdate;
		return;
	}
	//��������������
	SpeedDown();
}

void Player::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�������Ă��邩�`�F�b�N
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//����
		m_update = &Player::FallUpdate;
		return;
	}
	//����{�^������������
	if (input.IsTriggered("RB"))
	{
		//���
		m_update = &Player::RollingUpdate;
		return;
	}
	//�W�����v
	if (input.IsTriggered("A") && m_isGround)
	{
		//�W�����v
		m_update = &Player::JumpUpdate;
		return;
	}
	//��U���{�^������������
	if (input.IsTriggered("X"))
	{
		//��U��
		m_update = &Player::AttackNormal1Update;
		return;
	}
	//���U���{�^������������
	if (input.IsTriggered("Y"))
	{
		//���U��
		m_update = &Player::AttackCharge1Update;
		return;
	}
	//���͂��Ȃ��Ȃ�ҋ@��Ԃ�
	if (m_stickVec.Magnitude() == 0)
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	else
	{
		//�ړ�
		m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * kMoveSpeed);
		//�����̍X�V
		m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
	}
}

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�������Ă���Ȃ�
	if (m_collidable->GetRb()->GetVec().y < 0.0f)
	{
		//����
		m_update = &Player::FallUpdate;
		return;
	}
	//���̃W�����v�̃N�[���^�C��
	--m_nextJumpFrame;
	if (m_nextJumpFrame <= 0 && input.IsTriggered("A"))//�㏸���ɃW�����v���������ꍇ
	{
		m_nextJumpFrame = 0;
		//������ԂɑJ�ڂ��đJ�ڐ�ŃW�����v
		m_update = &Player::FallUpdate;
		return;
	}
	//�󒆈ړ�
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	//���ړ����x�ɏ��������
	float speed = m_collidable->GetRb()->GetMoveVec().Magnitude();
	if (speed > 0.0f)
	{
		speed = ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
		m_collidable->GetRb()->SetMoveVec(m_collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}

	//��������������
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�W�����v�ł���Ȃ�
	if (input.IsTriggered("A") && (m_jumpNum < kMaxJumpNum))
	{
		//�W�����v
		m_update = &Player::JumpUpdate;
		return;
	}
	//�n�ʂɕt���Ă���Ȃ�
	if (m_isGround)
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	//�󒆈ړ�
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	//���ړ����x�ɏ��������
	float speed = m_collidable->GetRb()->GetMoveVec().Magnitude();
	if (speed > 0.0f)
	{
		speed = ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
		m_collidable->GetRb()->SetMoveVec(m_collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}
	//��������������
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::AttackNormal1Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackN1StartFrame)
	{
		//�U��������
		attackManager->SetAttack(m_attackN1);
		m_attackN1->Init();
	}
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (m_model->GetTatalAnimFrame() - kCancelAttackFrame <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			//2�i��
			m_update = &Player::AttackNormal2Update;
			return;
		}
	}

	//��������������
	SpeedDown();
}

void Player::AttackNormal2Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackN2StartFrame)
	{
		//�U��������
		attackManager->SetAttack(m_attackN2);
		m_attackN2->Init();
	}
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (m_model->GetTatalAnimFrame() - kCancelAttackFrame <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			//3�i��
			m_update = &Player::AttackNormal3Update;
			return;
		}
	}

	//��������������
	SpeedDown();
}
void Player::AttackNormal3Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackN3StartFrame)
	{
		//�U��������
		attackManager->SetAttack(m_attackN3);
		m_attackN3->Init();
	}
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}

	//��������������
	SpeedDown();
}
//�^��
void Player::AttackCharge1Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//��������������
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
	//���߂Ă鎞
	if (input.IsPressed("Y"))
	{
		//�^���U���`���[�W
		++m_chargeHighAttackFrame;
		//�ő�
		if (m_chargeHighAttackFrame >= kCharge3Frame)
		{
			m_chargeHighAttackFrame = kCharge3Frame;
		}
	}
	//�{�^���𗣂�
	else
	{
		//�^���U��
		m_update = &Player::AttackCharge2Update;
		return;
	}
}

void Player::AttackCharge2Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�����t���[�����I��������
	if (m_chargeHighAttackFrame <= 0.0f)
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	 //�����Ă�����Ɉړ�
	 m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kHighAttackMoveSpeed);
	 --m_chargeHighAttackFrame;
}
void Player::RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	//�����Ă�����Ɉړ�
	m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kRollingMoveSpeed);
}
//��Ԃɍ��킹�ď���������
void Player::StateInit()
{
	//��Ԃ��ς���Ă��Ȃ��Ȃ瑁�����^�[��
	if (m_lastUpdate == m_update)return;
	if (m_update == &Player::IdleUpdate)
	{
		//�ҋ@���
		m_model->SetAnim(kIdleAnim, true);
		//�W�����v�J�E���g���Z�b�g
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::MoveUpdate)
	{
		//����
		m_model->SetAnim(kRunAnim, true);
		//�W�����v�J�E���g���Z�b�g
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::JumpUpdate)
	{
		//�W�����v
		m_model->SetAnim(kJumpAnim, false);
		//���ł�̂�
		m_isGround = false;
		m_collidable->SetState(State::Jump);
		//�͂�^����
		m_collidable->GetRb()->ResetVec();
		m_collidable->GetRb()->SetVec(kJumpVec);
		//�N�[���^�C��
		m_nextJumpFrame = kNextJumpFrame;
	}
	else if (m_update == &Player::FallUpdate)
	{
		//����
		m_model->SetAnim(kFallAnim, true);
		//�������Ă�̂�
		m_isGround = false;
		//�W�����v�J�E���g�͗�����ԂɂȂ��Ă���J�E���g��i�߂�
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
	else if (m_update == &Player::AttackNormal1Update)
	{
		m_collidable->SetState(State::None);
		//�U��1
		m_model->SetAnim(kAttack_N1Anim, false, kAN1AnimSpeed);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//�U���t���[�����Z�b�g
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::AttackNormal2Update)
	{
		m_collidable->SetState(State::None);
		//�U��2
		m_model->SetAnim(kAttack_N2Anim, false, kAN2AnimSpeed);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//�U���t���[�����Z�b�g
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::AttackNormal3Update)
	{
		m_collidable->SetState(State::None);
		//�U��3
		m_model->SetAnim(kAttack_N3Anim, false, kAN3AnimSpeed);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//�U���t���[�����Z�b�g
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::AttackCharge1Update)
	{
		m_collidable->SetState(State::None);
		//�`���[�W�U��1
		m_model->SetAnim(kAttack_C1Anim, true);
	}
	else if (m_update == &Player::AttackCharge2Update)
	{
		m_collidable->SetState(State::None);
		//�`���[�W���Ԃɍ��킹�Ď���������
		//1�i���
		if (m_chargeHighAttackFrame <= kCharge1Frame)
		{
			//�`���[�W�U��2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge1AnimSpeed);
			m_chargeHighAttackFrame = kCharge1Frame;
		}
		//2�i���
		else if (m_chargeHighAttackFrame <= kCharge2Fraem)
		{
			//�`���[�W�U��2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge2AnimSpeed);
			m_chargeHighAttackFrame = kCharge2Fraem;
		}
		//3�i���
		else if (m_chargeHighAttackFrame <= kCharge3Frame)
		{
			//�`���[�W�U��2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge3AnimSpeed);
			m_chargeHighAttackFrame = kCharge3Frame;
		}
	}
	else if (m_update == &Player::RollingUpdate)
	{
		m_collidable->SetState(State::None);
		//���
		m_model->SetAnim(kRollingAnim, false);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
	}
	m_lastUpdate = m_update;
}

Vector3 Player::GetForwardVec(const std::unique_ptr<Camera>& camera)
{
	//���͂��Ȃ��Ȃ�0��Ԃ�
	if (m_stickVec.Magnitude() == 0)
	{
		return Vector3{ 0.0f,0.0f,0.0f };
	}
	//�J�����̌����ɂ��킹��
	//�J�����̌���
	Vector2 cameraDir{ camera->GetDir().x,camera->GetDir().z };
	cameraDir = cameraDir.Normalize();
	//���[���h���W��Z��������ɃJ�������ǂ̂��炢�����Ă���̂����v�Z
	Vector2 z = Vector2{ 0.0f, -1.0f };
	//�J�����̌���(�p�x)
	float cameraTheata = Theata(z, cameraDir);
	//��ɑ΂��ăX�e�B�b�N���ǂ̂��炢�����Ă���̂����v�Z
	float stickTheata = Theata(z, m_stickVec.Normalize());
	//�v���C���[�𒆐S�Ɏ��̍��W����]
	Matrix4x4 rotaMat = RotateYMat4x4(cameraTheata + stickTheata);
	//�x�N�g���ɂ�����(��])
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y���͖���
	return moveVec.Normalize();
}

void Player::SpeedDown()
{
	//����
	Vector3 vec = m_collidable->GetRb()->GetVec();
	if (m_isGround)
	{
		vec.x *= kMoveDeceRate;
		vec.z *= kMoveDeceRate;
	}
	else
	{
		vec.x *= kAirMoveDeceRate;
		vec.z *= kAirMoveDeceRate;
	}
	m_collidable->GetRb()->SetVec(vec);
}

void Player::WeaponUpdate()
{
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR rightHand = MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky1);
	//����̌���
	VECTOR swordDir = VNorm(VSub(MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky2),rightHand));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(rightHand, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	//����̎�������Z�b�g
	m_rightSword->GetRb()->SetPos(Position3(rightHand.x, rightHand.y, rightHand.z));
	//����̌�����Z�b�g
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void Player::HurtPointUpdate()
{
	//�ړ��ʂ��擾
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//���W�X�V
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}
