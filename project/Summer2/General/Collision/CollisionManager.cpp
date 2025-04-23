#include "CollisionManager.h"
#include "../../Game/Actors/Actor.h"
#include "CollisionChecker.h"
#include "../Rigidbody.h"
#include "ColliderBase.h"
#include "../Collidable.h"

CollisionManager::CollisionManager():
	m_collChecker(std::make_unique<CollisionChecker>())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//当たり判定をチェック
	for (auto& actorA : actors)
	{
		//当たり判定を行わないなら飛ばす
		if (!actorA->GetCollidable()->IsCollide())continue;
		for (auto& actorB : actors)
		{
			//当たり判定を行わないなら飛ばす
			if (!actorB->GetCollidable()->IsCollide())continue;
			//自分とは当たり判定をしない
			if (actorA == actorB)continue;

			//当たってるかをチェック
			bool isHit = false;

			//球と
			if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
			{
				//球
				if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollSS(actorA->GetCollidable(), actorB->GetCollidable());
				}
				//カプセル
				else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCS(actorB->GetCollidable(), actorA->GetCollidable());
				}
				//ポリゴン
				else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
				{
					isHit = m_collChecker->CheckCollSP(actorA->GetCollidable(), actorB->GetCollidable());
				}
			}
			//カプセルと
			else if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
			{
				//球
				if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollCS(actorA->GetCollidable(), actorB->GetCollidable());
				}
				//カプセル
				else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCC(actorA->GetCollidable(), actorB->GetCollidable());
				}
				//ポリゴン
				else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
				{
					isHit = m_collChecker->CheckCollCP(actorA->GetCollidable(), actorB->GetCollidable());
				}
			}
			//ポリゴンと
			else if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
			{
				//球
				if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollSP(actorB->GetCollidable(), actorA->GetCollidable());
				}
				//カプセル
				else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCP(actorB->GetCollidable(), actorA->GetCollidable());
				}
			}

			//当たってるなら
			if (isHit)
			{
				//当たった時の処理

			}
		}
	}
}
