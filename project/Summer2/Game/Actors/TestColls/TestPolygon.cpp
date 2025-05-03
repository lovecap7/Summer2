#include "TestPolygon.h"
#include "../../General/Collision/PolygonCollider.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include <DxLib.h>

TestPolygon::TestPolygon(Position3 pos, int modelHandle):
	Actor(ActorKind::Obstacle),
	m_modelHandle(modelHandle)
{
	//èâä˙à íu
	m_collidable = std::make_shared<Collidable>(std::make_shared<PolygonCollider>(modelHandle), std::make_shared<Rigidbody>(pos));

	DxLib::MV1SetScale(m_modelHandle, VGet(20.0f, 20.0f, 20.0f));
	DxLib::MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, -90.0f / 180.0f * DX_PI_F, 0.0f));
}

TestPolygon::~TestPolygon()
{
}

void TestPolygon::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	DxLib::MV1SetPosition(m_modelHandle, m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void TestPolygon::Gravity(const Vector3& gravity)
{
}

void TestPolygon::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void TestPolygon::Draw() const
{
	DxLib::MV1DrawModel(m_modelHandle);
}

void TestPolygon::Complete()
{
	
}
