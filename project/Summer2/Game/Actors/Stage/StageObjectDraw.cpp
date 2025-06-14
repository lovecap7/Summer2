#include "StageObjectDraw.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Model.h"

StageObjectDraw::StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle) :
	Actor(ActorKind::None)//None�ɂ����瓖���蔻������Ȃ��̂ŕ`��p�̃N���X�ɂȂ�
{
	//���f��
	m_model = std::make_shared<Model>(modelHandle, pos);
	m_model->SetScale(scale);
	m_model->SetRot(angle);
}


StageObjectDraw::~StageObjectDraw()
{
	//�Ȃ�
}

void StageObjectDraw::Init()
{
	//�Ȃ�
}

void StageObjectDraw::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//�Ȃ�
}

void StageObjectDraw::Gravity(const Vector3& gravity)
{
	//�Ȃ�
}

void StageObjectDraw::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//�Ȃ�
}

void StageObjectDraw::Draw() const
{
	m_model->Draw();
}

void StageObjectDraw::Complete()
{
	//�Ȃ�
}
