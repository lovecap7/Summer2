#include "InvisibleWall.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"

InvisibleWall::InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle):
	Actor(ActorKind::Obstacle),
	m_modelHandle(modelHandle)
{
	//初期位置
	m_collidable = std::make_shared<Collidable>(std::make_shared<PolygonCollider>(modelHandle), std::make_shared<Rigidbody>(pos));

	DxLib::MV1SetScale(m_modelHandle, scale);
	DxLib::MV1SetRotationXYZ(m_modelHandle, angle);
}

InvisibleWall::~InvisibleWall()
{
}

void InvisibleWall::Init()
{
}

void InvisibleWall::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	DxLib::MV1SetPosition(m_modelHandle, m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void InvisibleWall::Gravity(const Vector3& gravity)
{
}

void InvisibleWall::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void InvisibleWall::Draw() const
{
#if _DEBUG
	//デバッグの時だけ可視化する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DxLib::MV1DrawModel(m_modelHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif
}

void InvisibleWall::Complete()
{
}

void InvisibleWall::HitReaction()
{

}
