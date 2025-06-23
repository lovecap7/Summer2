#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../Player/Player.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/BlastAttack.h"

namespace
{
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 50.0f;
	//�����̓����蔻��̔��a
	constexpr float kBlastRadius = 90.0f;
	//�����܂ł̃t���[����
	constexpr int kBlastFrame = 180;
}

Bomb::Bomb(int modelHandle, Vector3 pos) :
	ItemBase(ItemKind::Heart),
	m_blastCountFrame(kBlastFrame)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�Փ˔���
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//�͂�^����
	m_collidable->GetRb()->SetVecY(kJumpPower);
	//�R���C�_�u���̏�����
	m_collidable->Init(State::None, Priority::Low, GameTag::Item);
}

Bomb::~Bomb()
{
}

void Bomb::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[�ɓo�^
	actorManager->Entry(shared_from_this());
}

void Bomb::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//�A�N�^�[�}�l�[�W���[����
	actorManager->Exit(shared_from_this());
}

void Bomb::Init()
{
	//�R���C�_�[�Ɏ����̃|�C���^����������
	m_collidable->SetOwner(shared_from_this());
}

void Bomb::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�ړ��ʂ�������
	m_collidable->GetRb()->SetMoveVec(Vector3::Zero());
	//�����̃J�E���g�����炷
	m_blastCountFrame--;
	//�����܂ł̃t���[����0�ɂȂ����甚��
	if (m_blastCountFrame <= 0 && !m_isDelete)
	{
		//�����̓����蔻����쐬
		CreateAttack();
		//�폜�t���O�𗧂Ă�
		m_isDelete = true;
		return;
	}
}

void Bomb::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Bomb::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	
}

void Bomb::Draw() const
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

void Bomb::Complete()
{
	//���̍��W��
	m_collidable->GetRb()->SetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Bomb::CreateAttack()
{
	std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->SetRadius(kBlastRadius);
	//�����̓����蔻����쐬
	m_blastAttack = std::make_shared<BlastAttack>(m_collidable, 100.0f, 5,20.0f, shared_from_this());
}