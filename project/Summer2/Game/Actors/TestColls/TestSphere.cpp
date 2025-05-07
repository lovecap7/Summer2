#include "TestSphere.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include <DxLib.h>

TestSphere::TestSphere(Position3 pos, float radius, bool isStatic):
	Actor(ActorKind::Obstacle)
{
	//初期位置
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(radius), std::make_shared<Rigidbody>(pos));
	m_collidable->SetIsStatic(isStatic);
}

TestSphere::~TestSphere()
{
}

void TestSphere::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	m_collidable->GetRb()->ResetVec();
}

void TestSphere::Gravity(const Vector3& gravity)
{
}

void TestSphere::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void TestSphere::Draw() const
{
	DrawSphere3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0x00ff00,
		0x00ff00,
		m_collidable->IsStatic()
	);
}

void TestSphere::Complete()
{
	//位置更新
	m_collidable->GetRb()->SetNextPos();
}
