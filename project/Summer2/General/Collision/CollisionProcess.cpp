#include "CollisionProcess.h"
#include "../Collidable.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"

CollisionProcess::CollisionProcess()
{
}

CollisionProcess::~CollisionProcess()
{
}

void CollisionProcess::ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Colldable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->GetRb()->IsStatic() && otherB->GetRb()->IsStatic())return;
}

void CollisionProcess::ProcessSP(const std::shared_ptr<Colldable>& otherA, const std::shared_ptr<Colldable>& otherB)
{
}

void CollisionProcess::ProcessCC(const std::shared_ptr<Colldable>& otherA, const std::shared_ptr<Colldable>& otherB)
{
}

void CollisionProcess::ProcessCS(const std::shared_ptr<Colldable>& otherA, const std::shared_ptr<Colldable>& otherB)
{
}

void CollisionProcess::ProcessCP(const std::shared_ptr<Colldable>& otherA, const std::shared_ptr<Colldable>& otherB)
{
}
