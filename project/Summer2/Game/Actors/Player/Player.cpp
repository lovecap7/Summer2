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
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/AttackManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 20.0f; //�J�v�Z���̔��a
	//�ړ����x
	constexpr float kLowMoveSpeed = 2.0f;//�n��̏��ړ����x
	constexpr float kMediumMoveSpeed = 5.0f;//�n��̒��ړ����x
	constexpr float kHighMoveSpeed = 10.0f;//�n��̑�ړ����x
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
	//�U���I���O�ɃL�����Z���\�t���[��
	constexpr float kAttackCancelFrame = 20.0f;
	//����
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒����Ɣ��a
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//�����̍��{�Ƒ���̃C���f�b�N�X
	constexpr int kRootIndex = 60;
	constexpr int kToeIndex = 64;
	//�����̔��a
	constexpr float kLeftLegRadius = 20.0f;

	//�ʏ�U��1�̃_���[�W�Ǝ����t���[���Ɣ����t���[��
	constexpr int kAttackN1Damege = 100.0f;
	constexpr int kAttackN1KeepFrame = 6;
	constexpr int kAttackN1StartFrame = 15;
	//�ʏ�U��2�̃_���[�W�Ǝ����t���[��
	constexpr int kAttackN2Damege = 200.0f;
	constexpr int kAttackN2KeepFrame = 10;
	constexpr int kAttackN2StartFrame = 15;
	//�ʏ�U��3�̃_���[�W�Ǝ����t���[��
	constexpr int kAttackN3Damege = 300.0f;
	constexpr int kAttackN3KeepFrame = 13;
	constexpr int kAttackN3StartFrame = 15;
	//��U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kAN1AnimSpeed = 1.3f;
	constexpr float kAN2AnimSpeed = 1.3f;
	constexpr float kAN3AnimSpeed = 1.3f;
	//���U���̒i�K�ʍU���t���[��
	constexpr float kCharge1KeepFrame = 30.0f;
	constexpr float kCharge2KeepFrame = 60.0f;
	constexpr float kCharge3KeepFrame = 100.0f;
	//���U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kCharge1AnimSpeed = 2.0f;
	constexpr float kCharge2AnimSpeed = 2.5f;
	constexpr float kCharge3AnimSpeed = 3.0f;
	//���U���̒i�K�ʃ_���[�W
	constexpr int kCharge1AnimDamage = 10.0f;
	constexpr int kCharge2AnimDamage = 12.0f;
	constexpr int kCharge3AnimDamage = 15.0f;
	//������[�V�����̑��x
	constexpr float kRollingAnimSpeed = 1.2f;

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
	m_update(&Player::UpdateIdle),
	m_lastUpdate(&Player::UpdateFall),
	m_isGround(true),
	m_jumpNum(0),
	m_nextJumpFrame(kNextJumpFrame),
	m_attackCountFrame(0),
	m_chargeFrame(0)
{
	//���f��
	m_model = std::make_unique<Model>(modelHandle, firstPos.ToDxLibVector());
	//�Փ˔���
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
	//���ꔻ��(�Փ˔���Ɠ����ɂ���)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, 100, *this);
	//�U���̔���̏���
	CreateAttack();
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
	InitState();
	//��Ԃɍ��킹���X�V
	(this->*m_update)(input,camera, attackManager);
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//�퓬�Ɋւ���X�V����
	BattleUpdate();
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
	//�n�ʂɓ����������̏���
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
	//��
	DrawCapsule3D(
		m_rightSword->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetRadius(),
		16,
		0xffff00,
		0xffff00,
		false
	);
	//����
	unsigned int color = 0xff0000;
	//2�i���
	if (m_chargeFrame > kCharge1KeepFrame && m_chargeFrame <= kCharge2KeepFrame)
	{
		color = 0xffff00;
	}
	//3�i���
	else if (m_chargeFrame > kCharge2KeepFrame && m_chargeFrame <= kCharge3KeepFrame)
	{
		color = 0x00ffff;
	}
	DrawCapsule3D(
		m_leftLeg->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->GetRadius(),
		16,
		color,
		color,
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

void Player::UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�������Ă��邩�`�F�b�N
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//����
		m_update = &Player::UpdateFall;
		return;
	}
	//����{�^������������
	if (input.IsTrigger("RB"))
	{
		//���
		m_update = &Player::UpdateRolling;
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("A") && m_isGround)
	{
		//�W�����v
		m_update = &Player::UpdateJump;
		return;
	}
	//��U���{�^������������
	if (input.IsTrigger("X"))
	{
		//�U��������(�e�X�g)
		attackManager->SetAttack(m_attackN1);
		m_attackN1->Init();

		//��U��
		m_update = &Player::UpdateAttackNormal1;
		return;
	}
	//���U���{�^������������
	if (input.IsTrigger("Y"))
	{
		//���U��
		m_update = &Player::UpdateAttackCharge1;
		return;
	}
	//���͂�����Ȃ�ړ�
	if (m_stickVec.Magnitude() != 0)
	{
		//�ړ�
		m_update = &Player::UpdateMove;
		return;
	}
	//��������������
	SpeedDown();
}

void Player::UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�������Ă��邩�`�F�b�N
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//����
		m_update = &Player::UpdateFall;
		return;
	}
	//����{�^������������
	if (input.IsTrigger("RB"))
	{
		//���
		m_update = &Player::UpdateRolling;
		return;
	}
	//�W�����v
	if (input.IsTrigger("A") && m_isGround)
	{
		//�W�����v
		m_update = &Player::UpdateJump;
		return;
	}
	//��U���{�^������������
	if (input.IsTrigger("X"))
	{
		//��U��
		m_update = &Player::UpdateAttackNormal1;
		return;
	}
	//���U���{�^������������
	if (input.IsTrigger("Y"))
	{
		//���U��
		m_update = &Player::UpdateAttackCharge1;
		return;
	}
	//���͂��Ȃ��Ȃ�ҋ@��Ԃ�
	if (m_stickVec.Magnitude() == 0)
	{
		//�ҋ@
		m_update = &Player::UpdateIdle;
		return;
	}
	else
	{
		//�ړ�
		m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * InputValueSpeed(input));
		//�����̍X�V
		m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
	}
}

void Player::UpdateJump(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�������Ă���Ȃ�
	if (m_collidable->GetRb()->GetVec().y < 0.0f)
	{
		//����
		m_update = &Player::UpdateFall;
		return;
	}
	//���̃W�����v�̃N�[���^�C��
	--m_nextJumpFrame;
	if (m_nextJumpFrame <= 0 && input.IsTrigger("A"))//�㏸���ɃW�����v���������ꍇ
	{
		m_nextJumpFrame = 0;
		//������ԂɑJ�ڂ��đJ�ڐ�ŃW�����v
		m_update = &Player::UpdateFall;
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

void Player::UpdateFall(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�W�����v�ł���Ȃ�
	if (input.IsTrigger("A") && (m_jumpNum < kMaxJumpNum))
	{
		//�W�����v
		m_update = &Player::UpdateJump;
		return;
	}
	//�n�ʂɕt���Ă���Ȃ�
	if (m_isGround)
	{
		//�ҋ@
		m_update = &Player::UpdateIdle;
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

void Player::UpdateAttackNormal1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackN1StartFrame)
	{
		//�U��������
		AppearAttack(m_attackN1, attackManager);
	}
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�U�����������
		m_attackN1->Delete();
		//�ҋ@
		m_update = &Player::UpdateIdle;
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (m_model->GetTotalAnimFrame() - kAttackCancelFrame <= m_model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("RB"))
		{
			//�U�����������
			m_attackN1->Delete();
			//���
			m_update = &Player::UpdateRolling;
			return;
		}
		//2�i��
		if (input.IsTrigger("X"))
		{
			//�U�����������
			m_attackN1->Delete();
			//2�i��
			m_update = &Player::UpdateAttackNormal2;
			return;
		}
		//�^���U��
		if (input.IsPress("Y"))
		{
			//�U�����������
			m_attackN1->Delete();
			//���U��
			m_update = &Player::UpdateAttackCharge1;
			return;
		}
	}

	//��������������
	SpeedDown();
}

void Player::UpdateAttackNormal2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackN2StartFrame)
	{
		//�U��������
		AppearAttack(m_attackN2, attackManager);
	}
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�U�����������
		m_attackN2->Delete();
		//�ҋ@
		m_update = &Player::UpdateIdle;
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (m_model->GetTotalAnimFrame() - kAttackCancelFrame <= m_model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("RB"))
		{
			//�U�����������
			m_attackN2->Delete();
			//���
			m_update = &Player::UpdateRolling;
			return;
		}
		//3�i��
		if (input.IsTrigger("X"))
		{
			//�U�����������
			m_attackN2->Delete();
			//3�i��
			m_update = &Player::UpdateAttackNormal3;
			return;
		}
		//�^���U��
		if (input.IsPress("Y"))
		{
			//�U�����������
			m_attackN2->Delete();
			//���U��
			m_update = &Player::UpdateAttackCharge1;
			return;
		}
	}

	//��������������
	SpeedDown();
}
void Player::UpdateAttackNormal3(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == kAttackN3StartFrame)
	{
		AppearAttack(m_attackN3,attackManager);
	}
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�U�����������
		m_attackN3->Delete();
		//�ҋ@
		m_update = &Player::UpdateIdle;
		return;
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��ŋ��U���̓��͂�����Ȃ�
	if (m_model->GetTotalAnimFrame() - kAttackCancelFrame <= m_model->GetNowAnimFrame())
	{
		if (input.IsTrigger("RB"))
		{
			//�U�����������
			m_attackN3->Delete();
			//���
			m_update = &Player::UpdateRolling;
			return;
		}
		if (input.IsPress("Y"))
		{
			//�U�����������
			m_attackN3->Delete();
			//���U��
			m_update = &Player::UpdateAttackCharge1;
			return;
		}
	}
	//��������������
	SpeedDown();
}
//�^��
void Player::UpdateAttackCharge1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//����{�^������������
	if (input.IsTrigger("RB"))
	{
		//���
		m_update = &Player::UpdateRolling;
		return;
	}
	//��������������
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
	//���߂Ă鎞
	if (input.IsPress("Y"))
	{
		//�^���U���`���[�W
		++m_chargeFrame;
		//�ő�
		if (m_chargeFrame >= kCharge3KeepFrame)
		{
			m_chargeFrame = kCharge3KeepFrame;
		}
	}
	//�{�^���𗣂�
	else
	{
		//�U������
		AppearAttack(m_attackC, attackManager);
		//�^���U��
		m_update = &Player::UpdateAttackCharge2;
		return;
	}
}

void Player::UpdateAttackCharge2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//�A�j���[�V�������I��������
	if (m_model->IsFinishFixedLoop())
	{
		//�U�����������
		m_attackC->Delete();
		//�ҋ@
		m_update = &Player::UpdateIdle;
		return;
	}
	//�A�j���[�V������������邽�тɍU������̃��Z�b�g
	if (m_model->IsFinishAnim())
	{
		//������
		m_attackC->Init();
	}
	 //�����Ă�����Ɉړ�
	 m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kHighAttackMoveSpeed);
}
void Player::UpdateRolling(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�ҋ@
		m_update = &Player::UpdateIdle;
		return;
	}
	//�����Ă�����Ɉړ�
	m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kRollingMoveSpeed);
}

//��Ԃɍ��킹�ď���������
void Player::InitState()
{
	//��Ԃ��ς���Ă��Ȃ��Ȃ瑁�����^�[��
	if (m_lastUpdate == m_update)return;
	if (m_update == &Player::UpdateIdle)
	{
		//�ҋ@���
		m_model->SetAnim(kIdleAnim, true);
		//�W�����v�J�E���g���Z�b�g
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::UpdateMove)
	{
		//����
		m_model->SetAnim(kRunAnim, true);
		//�W�����v�J�E���g���Z�b�g
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::UpdateJump)
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
	else if (m_update == &Player::UpdateFall)
	{
		//����
		m_model->SetAnim(kFallAnim, true);
		//�������Ă�̂�
		m_isGround = false;
		//�W�����v�J�E���g�͗�����ԂɂȂ��Ă���J�E���g��i�߂�
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
	else if (m_update == &Player::UpdateAttackNormal1)
	{
		m_collidable->SetState(State::None);
		//�U��1
		m_model->SetAnim(kAttack_N1Anim, false, kAN1AnimSpeed);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//�U���t���[�����Z�b�g
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::UpdateAttackNormal2)
	{
		m_collidable->SetState(State::None);
		//�U��2
		m_model->SetAnim(kAttack_N2Anim, false, kAN2AnimSpeed);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//�U���t���[�����Z�b�g
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::UpdateAttackNormal3)
	{
		m_collidable->SetState(State::None);
		//�U��3
		m_model->SetAnim(kAttack_N3Anim, false, kAN3AnimSpeed);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//�U���t���[�����Z�b�g
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::UpdateAttackCharge1)
	{
		m_collidable->SetState(State::None);
		//�`���[�W�U��1
		m_model->SetAnim(kAttack_C1Anim, true);
	}
	else if (m_update == &Player::UpdateAttackCharge2)
	{
		m_collidable->SetState(State::None);
		//�`���[�W���Ԃɍ��킹�Ď���������
		//1�i���
		if (m_chargeFrame <= kCharge1KeepFrame)
		{
			//�`���[�W�U��2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge1AnimSpeed);
			m_model->SetFixedLoopFrame(kCharge1KeepFrame);//�w�胋�[�v
			m_attackC->SetDamage(kCharge1AnimDamage);//�_���[�W
		}
		//2�i���
		else if (m_chargeFrame <= kCharge2KeepFrame)
		{
			//�`���[�W�U��2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge2AnimSpeed);
			m_model->SetFixedLoopFrame(kCharge2KeepFrame);//�w�胋�[�v
			m_attackC->SetDamage(kCharge2AnimDamage);//�_���[�W
		}
		//3�i���
		else if (m_chargeFrame <= kCharge3KeepFrame)
		{
			//�`���[�W�U��2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge3AnimSpeed);
			m_model->SetFixedLoopFrame(kCharge3KeepFrame);//�w�胋�[�v
			m_attackC->SetDamage(kCharge3AnimDamage);//�_���[�W
		}
		m_chargeFrame = 0;//������
	}
	else if (m_update == &Player::UpdateRolling)
	{
		m_collidable->SetState(State::None);
		//���
		m_model->SetAnim(kRollingAnim, false, kRollingAnimSpeed);
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
	Matrix4x4 rotaMat = Matrix4x4::RotateYMat4x4(cameraTheata + stickTheata);
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

float Player::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//���x���X�e�B�b�N���͂̐[�x�ɍ��킹��
	if (input.IsLowPowerLeftStick())moveSpeed = kLowMoveSpeed;
	if (input.IsMediumPowerLeftStick())moveSpeed = kMediumMoveSpeed;
	if (input.IsHighPowerLeftStick())moveSpeed = kHighMoveSpeed;
	return moveSpeed;
}

void Player::BattleUpdate()
{
	//����̈ʒu�X�V
	UpdateRightSword();
	//�����̈ʒu�X�V
	UpdateLeftLeg();
	//���ꔻ��̈ʒu�X�V
	UpdateHurtPoint();
}

void Player::CreateRightSword()
{
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kRightSwordRadius), std::make_shared<Rigidbody>(Vector3(ringFinger.x, ringFinger.y, ringFinger.z)));
}

void Player::UpdateRightSword()
{
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//����̒���
	swordDir = VAdd(ringFinger, swordDir);//������̍��W�ɉ��Z���Č���̍��W���o��
	//����̎�������Z�b�g
	m_rightSword->GetRb()->SetPos(Position3(ringFinger.x, ringFinger.y, ringFinger.z));
	//����̌�����Z�b�g
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void Player::CreateLeftLeg()
{
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(m_model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(m_model->GetModelHandle(), kToeIndex);//����
	m_leftLeg = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(toe.x, toe.y, toe.z), kLeftLegRadius), 
		std::make_shared<Rigidbody>(Vector3(root.x, root.y, root.z)));
}

void Player::UpdateLeftLeg()
{
	//����
	//�t�����Ƒ���
	VECTOR root = MV1GetFramePosition(m_model->GetModelHandle(), kRootIndex);//�t����
	VECTOR toe = MV1GetFramePosition(m_model->GetModelHandle(), kToeIndex);//����
	//�t�������Z�b�g
	m_leftLeg->GetRb()->SetPos(Position3(root.x, root.y, root.z));
	//������Z�b�g
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->SetEndPos(Position3(toe.x, toe.y, toe.z));
}

void Player::CreateAttack()
{
	//�U���ɕK�v�Ȕ���̍쐬
	//��
	CreateRightSword();
	//����
	CreateLeftLeg();
	//�U���̏���
	m_attackN1 = std::make_shared<MeleeAttack>(m_rightSword, kAttackN1Damege, kAttackN1KeepFrame, *this);
	m_attackN2 = std::make_shared<MeleeAttack>(m_rightSword, kAttackN2Damege, kAttackN2KeepFrame, *this);
	m_attackN3 = std::make_shared<MeleeAttack>(m_rightSword, kAttackN3Damege, kAttackN3KeepFrame, *this);
	m_attackC = std::make_shared<MeleeAttack>(m_leftLeg, kCharge1AnimDamage, kCharge1KeepFrame, *this);
}

void Player::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//�U��������
	attack->Init();
	attackManager->SetAttack(attack);
}

void Player::UpdateHurtPoint()
{
	//�ړ��ʂ��擾
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//���W�X�V
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}