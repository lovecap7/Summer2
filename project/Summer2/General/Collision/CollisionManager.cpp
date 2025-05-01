#include "CollisionManager.h"
#include "../../Game/Actors/Actor.h"
#include "CollisionChecker.h"
#include "CollisionProcess.h"
#include "../Rigidbody.h"
#include "ColliderBase.h"
#include "../Collidable.h"

namespace
{
	//確認回数
	constexpr int kTryNum = 3;
}

CollisionManager::CollisionManager():
	m_collChecker(std::make_unique<CollisionChecker>()),
	m_collProcessor(std::make_unique<CollisionProcess>())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//補正したことで別のオブジェクトに当たる可能性があるので一定回数チャックする
	for (int i = 0;i < kTryNum;++i)
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
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessSS(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
					//カプセル
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCS(actorB->GetCollidable(), actorA->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCS(actorB->GetCollidable(), actorA->GetCollidable());
						}
					}
					//ポリゴン
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollSP(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessSP(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
				}
				//カプセルと
				else if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					//球
					if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollCS(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCS(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
					//カプセル
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCC(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCC(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
					//ポリゴン
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollCP(actorA->GetCollidable(), actorB->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCP(actorA->GetCollidable(), actorB->GetCollidable());
						}
					}
				}
				//ポリゴンと
				else if (actorA->GetCollidable()->GetColl()->GetShape() == Shape::Polygon)
				{
					//球
					if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSP(actorB->GetCollidable(), actorA->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessSP(actorB->GetCollidable(), actorA->GetCollidable());
						}
					}
					//カプセル
					else if (actorB->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCP(actorB->GetCollidable(), actorA->GetCollidable());
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCP(actorB->GetCollidable(), actorA->GetCollidable());
						}
					}
				}

				//当たってるなら
				if (isHit)
				{
					//当たった時の処理
					actorA->OnHitColl(actorB->GetCollidable());
					actorB->OnHitColl(actorA->GetCollidable());
				}
			}
		}
	}
}
