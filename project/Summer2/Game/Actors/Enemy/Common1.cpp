#include "Common1.h"
#include <memory>
#include "../../General/Model.h"
#include "../../General/Input.h"
#include "../../Game/Camera/Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/SphereCollider.h"

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,100.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 20.0f; //�J�v�Z���̔��a
	//�g���K�[�̔��a
	constexpr float kSearchTriggerRadius = 100.0f;
	//�v���C���[��ǂ������鋗��
	constexpr float kRunDistance = 50.0f;
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 10.0f;
	//�v���C���[���U������
	constexpr float kAttackDistance = 10.0f;
	//�d��
	constexpr float kMaxGravity = -10.0f;//�����X�s�[�h���傫���Ȃ肷���Ȃ��悤��
	//����
	constexpr float kMoveDeceRate = 0.8f;

	//�A�j���[�V�����̖��O
	const char* kIdleAnim = "CharacterArmature|Idle";//�ҋ@
	const char* kWalkAnim = "CharacterArmature|Walk";//����
	const char* kRunAnim = "CharacterArmature|Run";//����
	const char* kAttackAnim = "CharacterArmature|Weapon";//�U��
	const char* kSmallHitAnim = "CharacterArmature|HitReact";//����炢
	const char* kBigHitAnim = "CharacterArmature|Death";//���炢
}

Common1::Common1(int modelHandle, Vector3 pos):
	m_update(&Common1::IdleUpdate),
	m_lastUpdate(&Common1::AttackUpdate)
{
	//���f���̏�����
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//���f���̑傫������
	m_model->SetScale(VGet(0.5f, 0.5f, 0.5f));
	//�����ʒu
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(pos));
	//�g���K�[
	m_searchTrigger = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kSearchTriggerRadius), std::make_shared<Rigidbody>(pos));
}

Common1::~Common1()
{
}

void Common1::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//��Ԃɍ��킹�ď�����
	StateInit();
	//�X�V
	(this->*m_update)(input, camera);
	//�A�j���[�V�����̍X�V
	m_model->Update();
}

void Common1::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= kMaxGravity)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Common1::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void Common1::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void Common1::Complete()
{
	//�R���C�_�[
	m_collidable->GetRb()->SetNextPos();//���̍��W��
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//�v���C���[��T���g���K�[
	m_searchTrigger->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Common1::HitSearch(const Vector3& playerPos)
{
	//�������`�F�b�N
	Vector3 dist = playerPos - m_searchTrigger->GetRb()->GetPos();

	//�����Ȃ�
	if (dist.Magnitude() > kRunDistance)
	{
		//�U��������Ȃ����
		if (m_update != &Common1::AttackUpdate)
		{
			//�ǂ�������
			m_collidable->GetRb()->SetMoveVec(dist.Normalize() * kChaseSpeed);
			//��ԕω�
			if (m_update != &Common1::MoveUpdate)
			{
				m_update = &Common1::MoveUpdate;
			}
		}
	}
	//�U���͈͓̔��Ȃ�
	else if (dist.Magnitude() > kAttackDistance)
	{
		//��ԕω�
		if (m_update != &Common1::AttackUpdate)
		{
			//�����̍X�V
			m_model->SetDir(VGet(dist.Normalize().x, 0.0f, dist.Normalize().z));
			m_update = &Common1::AttackUpdate;
		}
	}
	else
	{
		//�U��������Ȃ����
		if (m_update != &Common1::AttackUpdate)
		{
			//��ԕω�
			if (m_update != &Common1::IdleUpdate)
			{
				m_update = &Common1::IdleUpdate;
			}
		}
	}
}

void Common1::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//����
	SpeedDown();
}

void Common1::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//����
	SpeedDown();
	//�����̍X�V
	m_model->SetDir(VGet(m_collidable->GetRb()->GetVec().x, 0.0f, m_collidable->GetRb()->GetVec().y));
}

void Common1::AttackUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//�A�j���[�V�����I����
	if (m_model->IsFinishAnim())
	{
		m_update = &Common1::IdleUpdate;
		return;
	}

	//����
	SpeedDown();
}

void Common1::HitUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
}

void Common1::StateInit()
{
	//��Ԃ��ς���Ă��Ȃ��Ȃ瑁�����^�[��
	if (m_lastUpdate == m_update)return;
	if (m_update == &Common1::IdleUpdate)
	{
		//�ҋ@���
		m_model->SetAnim(kIdleAnim, true);
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Common1::MoveUpdate)
	{
		//����
		m_model->SetAnim(kRunAnim, true);
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Common1::AttackUpdate)
	{
		m_collidable->SetState(State::None);
		//�U��
		m_model->SetAnim(kAttackAnim, false);
	}
	m_lastUpdate = m_update;
}

void Common1::SpeedDown()
{
	//����
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	m_collidable->GetRb()->SetVec(vec);
}