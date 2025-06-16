#include "AttackManager.h"
#include "AttackBase.h"
#include "../../General/Collision/CollisionChecker.h"
#include "../../General/Collidable.h"
#include "../../General/Collision/ColliderBase.h"
#include "HurtPoint.h"
#include "../Actors/Actor.h"

AttackManager::AttackManager():
	m_collChecker(std::make_shared<CollisionChecker>())
{
}

AttackManager::~AttackManager()
{
}

void AttackManager::Entry(std::shared_ptr<AttackBase> attack)
{
	//すでに登録されているなら古い攻撃を消す
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it != m_attacks.end())
	{
		//削除
		m_attacks.erase(it);
	}

	//攻撃をセット
	m_attacks.emplace_back(attack);
}

void AttackManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	if (m_attacks.empty())return;//空なら何もしない

	//攻撃の更新処理
	for (auto& attack : m_attacks)
	{
		attack->Update();
	}

	//消えた攻撃判定のイテレータを取得
	auto remIt = std::remove_if(m_attacks.begin(), m_attacks.end(), [](std::shared_ptr<AttackBase> attack) {return attack->IsDead();});
	m_attacks.erase(remIt, m_attacks.end());//削除

	for (auto& actor : actors)
	{
		//プレイヤーと敵のみ
		if (actor->GetActorKind() == ActorKind::Player ||
			actor->GetActorKind() == ActorKind::Enemy)
		{
			actor->GetHurtPoint()->Init();//やられ判定の初期化
		}
	}
	//攻撃が当たっているかチェック
	for (auto& attack : m_attacks)
	{
		//攻撃が当たっているかをチェックする　
		for (auto& actor : actors)
		{
			//プレイヤーと敵のみ
			if (actor->GetActorKind() == ActorKind::Player ||
				actor->GetActorKind() == ActorKind::Enemy)
			{
				if (actor->GetHurtPoint()->IsNoDamege())continue;//無敵の時は当たらない
				//当たってるかをチェック
				bool isHit = false;
				//攻撃のコライダブル
				auto attackCollidable = attack->GetCollidable();
				//やられ判定
				auto actorHurtPointCollidable = actor->GetHurtPoint()->GetCollidable();

				//球と
				if (attackCollidable->GetColl()->GetShape() == Shape::Sphere)
				{
					//球
					if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSS(attackCollidable, actorHurtPointCollidable);
					}
					//カプセル
					else if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCS(actorHurtPointCollidable, attackCollidable);
					}
				}
				//カプセルと
				else if (attackCollidable->GetColl()->GetShape() == Shape::Capsule)
				{
					//球
					if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollCS(attackCollidable, actorHurtPointCollidable);
					}
					//カプセル
					else if (actorHurtPointCollidable->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCC(attackCollidable, actorHurtPointCollidable);
					}
				}

				//当たってるなら
				if (isHit)
				{
					//当たった時の処理
					attack->OnHit(actor);
				}
			}
		}
	}
}

void AttackManager::Draw() const
{
	if (m_attacks.empty())return;//空なら何もしない

	//攻撃の描画
	for (auto& attack : m_attacks)
	{
		attack->Draw();
	}
}
