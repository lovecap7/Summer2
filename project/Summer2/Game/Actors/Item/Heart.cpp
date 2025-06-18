#include "Heart.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"

namespace
{
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 50.0f;
	//��]��
	constexpr float kRotaAngle = 1.0f;
}

Heart::Heart(int modelHandle, Vector3 pos):
	ItemBase(ItemKind::Heart)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�Փ˔���
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//�͂�^����
	m_collidable->GetRb()->SetVecY(kJumpPower);

}

Heart::~Heart()
{
}

void Heart::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
}

void Heart::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
}

void Heart::Init()
{
	//�R���C�_�[�Ɏ����̃|�C���^����������
	m_collidable->SetOwner(shared_from_this());
}

void Heart::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<AttackManager> attackManager)
{
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
}

void Heart::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Heart::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//�v���C���[�ɓ����������̏���
	if (other->GetOwner() != nullptr)
	{
		if (other->GetOwner()->GetActorKind() == ActorKind::Player)
		{
			//�폜
			m_isDelete = true;
		}
	}
}

void Heart::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawSphere3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void Heart::Complete()
{
	//���̍��W��
	m_collidable->GetRb()->SetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}
