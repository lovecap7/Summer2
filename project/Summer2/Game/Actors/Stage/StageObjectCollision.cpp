#include "StageObjectCollision.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"

StageObjectCollision::StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle) :
	Actor(ActorKind::Object),
	m_collisionHandle(modelHandle)
{
	//�ʒu�Ƒ傫���Ɖ�]�̃Z�b�g
	//�����ʒu
	m_collidable = std::make_shared<Collidable>(std::make_shared<PolygonCollider>(modelHandle), std::make_shared<Rigidbody>(pos));
	DxLib::MV1SetScale(m_collisionHandle, scale);
	DxLib::MV1SetRotationXYZ(m_collisionHandle, angle);
}

StageObjectCollision::StageObjectCollision(VECTOR pos, float radius):
	Actor(ActorKind::Object),
	m_collisionHandle(-1)
{
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(radius),std::make_shared<Rigidbody>(pos));
}

StageObjectCollision::StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius):
	Actor(ActorKind::Object),
	m_collisionHandle(-1)
{
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(pos2, radius), std::make_shared<Rigidbody>(pos1));
}


StageObjectCollision::~StageObjectCollision()
{
	//�Ȃ�
}

void StageObjectCollision::Init()
{
	//�Ȃ�
}

void StageObjectCollision::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<AttackManager> attackManager, std::shared_ptr<UIManager> uiManager)
{
	//�Ȃ�
}

void StageObjectCollision::Gravity(const Vector3& gravity)
{
	//�Ȃ�
}

void StageObjectCollision::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//�Ȃ�
}

void StageObjectCollision::Draw() const
{
	//�Ȃ�
}

void StageObjectCollision::Complete()
{
	//�Ȃ�
}
