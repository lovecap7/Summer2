#include "TestCapsule.h"
#include "../../General/Math/MyMath.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include <DxLib.h>

TestCapsule::TestCapsule(Position3 startPos, Position3 endPos, float radius, bool isStatic):
	Actor(ActorKind::Obstacle)
{
	//初期位置
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, radius), std::make_shared<Rigidbody>(startPos));
	m_collidable->SetIsStatic(isStatic);
}

TestCapsule::~TestCapsule()
{
}

void TestCapsule::Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	m_collidable->GetRb()->ResetVec();
}

void TestCapsule::Draw() const
{
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_collidable->IsStatic()
	);
}

void TestCapsule::Complete()
{
	//位置更新
	m_collidable->GetRb()->SetNextPos();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetNextEndPos(m_collidable->GetRb()->GetVec());
}
