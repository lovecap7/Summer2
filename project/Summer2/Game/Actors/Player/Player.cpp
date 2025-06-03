#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "../../../General/game.h"
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
#include "../ActorManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 20.0f; //�J�v�Z���̔��a
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_stickVec(0.0f,0.0f),
	m_isGround(false),
	m_isHit(false)
{
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�Փ˔���
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
}

Player::~Player()
{
}

void Player::Entry(std::shared_ptr<ActorManager> actorManager)
{
}

void Player::Exit(std::shared_ptr<ActorManager> actorManager)
{
}

void Player::Init()
{
	//�R���X�g���N�^�őS����������������������
	//shared_from_this()���g���ꍇ�R���X�g���N�^���_�ł�
	//���������m�肵�Ă��Ȃ��̂Ŏg�����Ƃ��ł��Ȃ�
	//�΍�Ƃ���Init���g��

	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = shared_from_this();
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//���̏�Ԃ�ҋ@��Ԃ�
	m_state->ChangeState(m_state);
	//���ꔻ��(�Փ˔���Ɠ����ɂ���)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, 100, thisPointer);
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//�X�e�B�b�N�̌���������
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);

	//��Ԃɍ��킹���X�V
	m_state->Update(input, camera, attackManager);
	//��Ԃ��ς���������`�F�b�N
	if (m_state != m_state->GetNextState())
	{
		//��Ԃ�ω�����
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//���ꔻ��̈ʒu�X�V
	UpdateHurtPoint();
}

void Player::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
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

//�U�����ꂽ�Ƃ��ɌĂяo��
void Player::HitReaction()
{
	//�q�b�g��Ԃɂ���
	m_isHit = true;
}

void Player::UpdateHurtPoint()
{
	//�ړ��ʂ��擾
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//���W�X�V
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}