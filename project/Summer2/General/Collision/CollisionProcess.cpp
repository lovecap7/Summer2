#include "CollisionProcess.h"
#include "../Collidable.h"
#include "../Rigidbody.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include <vector>

CollisionProcess::CollisionProcess():
	m_wallNum(0),
	m_floorNum(0),
{
}

CollisionProcess::~CollisionProcess()
{
}

void CollisionProcess::ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//AからBへのベクトル
	Vector3 aToB = otherB->GetRb()->GetNextPos() - otherA->GetRb()->GetNextPos();
	//最短距離
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (otherA->GetColl())->GetRadius() + std::dynamic_pointer_cast<SphereCollider> (otherB->GetColl())->GetRadius();

	//動かす物体とそうじゃない物体とで処理を分ける
	if (otherA->IsStatic())
	{
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else if (otherA->IsStatic())
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
	}
}

void CollisionProcess::ProcessSP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//壁と床とのフラグリセット
	std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->ResetHitFlag();
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//壁ポリゴンと床ポリゴンの数を初期化
	int wallNum = 0;
	int floorNum = 0;
	//壁と床ポリゴンを保存する配列
	std::vector<MV1_COLL_RESULT_POLY*> wall;
	std::vector<MV1_COLL_RESULT_POLY*> floor;

	//当たったポリゴンの情報
	auto hitDim = std::dynamic_pointer_cast<PolygonCollider>(otherB->GetColl())->GetHitDim();

	//球の座標
	Position3 sPos = otherA->GetRb()->GetNextPos();
}

void CollisionProcess::ProcessCC(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;
}

void CollisionProcess::ProcessCS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;

	//AからBへのベクトル
	Vector3 aToB = otherB->GetRb()->GetNextPos() - std::dynamic_pointer_cast<CapsuleCollider>(otherA->GetColl())->GetNearPos();

	//最短距離
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (otherB->GetColl())->GetRadius() + std::dynamic_pointer_cast<CapsuleCollider> (otherB->GetColl())->GetRadius();

	//動かす物体とそうじゃない物体とで処理を分ける
	if (otherA->IsStatic())
	{
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else if (otherA->IsStatic())
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis);
	}
	else
	{
		otherA->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
		otherB->GetRb()->SetVec(aToB.Normalize() * shortDis / 2.0f);
	}
}

void CollisionProcess::ProcessCP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB)
{
	//お互い動かないオブジェクトなら衝突しない
	if (otherA->IsStatic() && otherB->IsStatic())return;
}

void CollisionProcess::AnalyzeWallAndFloor(const MV1_COLL_RESULT_POLY_DIM& hitDim, const Vector3& oldPos)
{
}