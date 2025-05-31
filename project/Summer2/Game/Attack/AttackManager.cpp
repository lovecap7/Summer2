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

	//攻撃が当たっているかをチェックする　
	for (auto& actor : actors)
	{
		//攻撃を受けないアクターは当たり判定をしない
		if (actor->GetActorKind() == ActorKind::None)continue;
		if (actor->GetActorKind() == ActorKind::Field)continue;
		if (actor->GetActorKind() == ActorKind::Obstacle)continue;
		if (actor->GetActorKind() == ActorKind::Item)continue;
		if (actor->GetHurtPoint()->IsNoDamege())continue;//無敵の時は当たらない

		//攻撃が当たっているかチェック
		for (auto& attack : m_attacks)
		{
			//当たってるかをチェック
			bool isHit = false;

			//球と
			if (attack->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
			{
				//球
				if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollSS(attack->GetCollidable(), actor->GetHurtPoint()->GetCollidable());
				}
				//カプセル
				else if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCS(actor->GetHurtPoint()->GetCollidable(), attack->GetCollidable());
				}
			}
			//カプセルと
			else if (attack->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
			{
				//球
				if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Sphere)
				{
					isHit = m_collChecker->CheckCollCS(attack->GetCollidable(), actor->GetHurtPoint()->GetCollidable());
				}
				//カプセル
				else if (actor->GetHurtPoint()->GetCollidable()->GetColl()->GetShape() == Shape::Capsule)
				{
					isHit = m_collChecker->CheckCollCC(attack->GetCollidable(), actor->GetHurtPoint()->GetCollidable());
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

void AttackManager::Draw() const
{
	//攻撃の描画
	for (auto& attack : m_attacks)
	{
		attack->Draw();
	}
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
