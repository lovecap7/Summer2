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
				actor->GetHurtPoint()->OnHitDamage(attack->GetDamege());//ダメージを与える
				attack->OnHit(actor);//攻撃側の当たった時の処理
			}
		}
	}
}

void AttackManager::SetAttack(std::shared_ptr<AttackBase> attack)
{
	//攻撃をセット
	m_attacks.emplace_back(attack);
}
