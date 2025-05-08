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
	//��s���̓t���[��
	constexpr float kAdvanceInput = 15.0f;
	//���U���̒i�K�ʍU���t���[��
	constexpr float kOneChargeHighAttackFrame = 30.0f;
	constexpr float kTwoChargeHighAttackFrame = 70.0f;
	constexpr float kThreeChargeHighAttackFrame = 100.0f;
	//���U���̒i�K�ʃA�j���[�V�����̑��x
	constexpr float kOneChargeHighAttackAnimSpeed = 0.3f;
	constexpr float kTwoChargeHighAttackAnimSpeed = 0.5f;
	constexpr float kThreeChargeHighAttackAnimSpeed = 1.0f;

	//�A�j���[�V�����̖��O
	const char* kIdleAnim = "Player|Idle";//�ҋ@
	const char* kWalkAnim = "Player|Walk";//����
	const char* kRunAnim = "Player|Run";//����
	const char* kRollingAnim = "Player|Rolling";//���
	const char* kJump1Anim = "Player|Jump1";//�W�����v1
	const char* kJump2Anim = "Player|Jump2";//�W�����v2
	const char* kAttack_L1Anim = "Player|Attack_L1";//��U��1
	const char* kAttack_L2Anim = "Player|Attack_L2";//��U��2
	const char* kAttack_L3Anim = "Player|Attack_L3";//��U��3
	const char* kAttack_H1Anim = "Player|Attack_H1";//���U��1
	const char* kAttack_H2Anim = "Player|Attack_H2";//���U��2
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_stickVec(0.0f,0.0f),
	m_update(&Player::IdleUpdate),
	m_lastUpdate(&Player::FallUpdate),
	m_isGround(true),
	m_jumpNum(0),
	m_nextJumpFrame(kNextJumpFrame),
	m_isNextAttackInput(false),
	m_chargeHighAttackFrame(0)
{
	//�����ʒu
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
	//���f��
	m_model = std::make_unique<Model>(modelHandle, firstPos.ToDxLibVector());
}

Player::~Player()
{
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera)
{
	//�X�e�B�b�N�̌���������
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//��Ԃɍ��킹�ď�����
	StateInit();
	//�X�V
	(this->*m_update)(input,camera);
	//�A�j���[�V�����̍X�V
	m_model->Update();
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
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_isGround
	);
	printf("POS = %2f, %2f, %2f\n", m_collidable->GetRb()->GetPos().x, m_collidable->GetRb()->GetPos().y, m_collidable->GetRb()->GetPos().z);
	printf("VEC = %2f, %2f, %2f\n", m_collidable->GetRb()->GetVec().x, m_collidable->GetRb()->GetVec().y, m_collidable->GetRb()->GetVec().z);
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

void Player::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
		//��U��
		m_update = &Player::AttackLight1Update;
		return;
	}
	//���U���{�^������������
	if (input.IsTriggered("Y"))
	{
		//���U��
		m_update = &Player::AttackHigh1Update;
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

void Player::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
		m_update = &Player::AttackLight1Update;
		return;
	}
	//���U���{�^������������
	if (input.IsTriggered("Y"))
	{
		//���U��
		m_update = &Player::AttackHigh1Update;
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

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
	if (m_nextJumpFrame <= 0)
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
		speed = ClampFloat(speed, 0.0f, kAirMoveSpeed);
		m_collidable->GetRb()->SetMoveVec(m_collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}

	//��������������
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
	//��������������
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::AttackLight1Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�U���̓��͂�����Ȃ�
		if (m_isNextAttackInput)
		{
			//2�i��
			m_update = &Player::AttackLight2Update;
			return;
		}
		else
		{
			//�ҋ@
			m_update = &Player::IdleUpdate;
			return;
		}
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (m_model->GetTatalAnimFrame() - kAdvanceInput <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			m_isNextAttackInput = true;
		}
	}

	//��������������
	SpeedDown();
}

void Player::AttackLight2Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//���f���̃A�j���[�V�������I�������
	if (m_model->IsFinishAnim())
	{
		//�U���̓��͂�����Ȃ�
		if (m_isNextAttackInput)
		{
			//3�i��
			m_update = &Player::AttackLight3Update;
			return;
		}
		else
		{
			//�ҋ@
			m_update = &Player::IdleUpdate;
			return;
		}
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (m_model->GetTatalAnimFrame() - kAdvanceInput <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			m_isNextAttackInput = true;
		}
	}

	//��������������
	SpeedDown();
}
void Player::AttackLight3Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
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
void Player::AttackHigh1Update(const Input& input, const std::unique_ptr<Camera>& camera)
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
		if (m_chargeHighAttackFrame >= kThreeChargeHighAttackFrame)
		{
			m_chargeHighAttackFrame = kThreeChargeHighAttackFrame;
		}
	}
	//�{�^���𗣂�
	else
	{
		//�^���U��
		m_update = &Player::AttackHigh2Update;
		return;
	}
}

void Player::AttackHigh2Update(const Input& input, const std::unique_ptr<Camera>& camera)
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
void Player::RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
		if (m_lastUpdate == &Player::FallUpdate)
		{
			//2��ڃW�����v
			m_model->SetAnim(kJump2Anim, false);
		}
		else
		{
			//1��ڃW�����v
			m_model->SetAnim(kJump1Anim, true);
		}
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
		//�W�����v�ȊO�ł��̏�ԂɂȂ����Ȃ�
		if (m_lastUpdate != &Player::JumpUpdate)
		{
			//����
			m_model->SetAnim(kJump1Anim, true);
		}
		//�������Ă�̂�
		m_isGround = false;
		//�W�����v�J�E���g�͗�����ԂɂȂ��Ă���J�E���g��i�߂�
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
	else if (m_update == &Player::AttackLight1Update)
	{
		m_collidable->SetState(State::None);
		//��U��1
		m_model->SetAnim(kAttack_L1Anim, false);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//��s���͂̏���
		m_isNextAttackInput = false;
	}
	else if (m_update == &Player::AttackLight2Update)
	{
		m_collidable->SetState(State::None);
		//��U��2
		m_model->SetAnim(kAttack_L2Anim, false);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//��s���͂̏���
		m_isNextAttackInput = false;
	}
	else if (m_update == &Player::AttackLight3Update)
	{
		m_collidable->SetState(State::None);
		//��U��3
		m_model->SetAnim(kAttack_L3Anim, false);
		//�����̍X�V
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//��s���͂̏���
		m_isNextAttackInput = false;
	}
	else if (m_update == &Player::AttackHigh1Update)
	{
		m_collidable->SetState(State::None);
		//���U��1
		m_model->SetAnim(kAttack_H1Anim, true);
	}
	else if (m_update == &Player::AttackHigh2Update)
	{
		m_collidable->SetState(State::None);
		//�`���[�W���Ԃɍ��킹�Ď���������
		//1�i���
		if (m_chargeHighAttackFrame <= kOneChargeHighAttackFrame)
		{
			//���U��2
			m_model->SetAnim(kAttack_H2Anim, true, kOneChargeHighAttackAnimSpeed);
			m_chargeHighAttackFrame = kOneChargeHighAttackFrame;
		}
		//2�i���
		else if (m_chargeHighAttackFrame <= kTwoChargeHighAttackFrame)
		{
			//���U��2
			m_model->SetAnim(kAttack_H2Anim, true, kTwoChargeHighAttackAnimSpeed);
			m_chargeHighAttackFrame = kTwoChargeHighAttackFrame;
		}
		//3�i���
		else if (m_chargeHighAttackFrame <= kThreeChargeHighAttackFrame)
		{
			//���U��2
			m_model->SetAnim(kAttack_H2Anim, true, kThreeChargeHighAttackAnimSpeed);
			m_chargeHighAttackFrame = kThreeChargeHighAttackFrame;
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
