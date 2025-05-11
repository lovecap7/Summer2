#include "AttackManager.h"
#include "AttackBase.h"
#include "../../General/Collision/CollisionChecker.h"
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

	//消えた攻撃判定を削除
	std::erase_if(m_attacks, [](AttackBase attack) {return attack.IsDead();});//C++20からの関数 条件に合うものを配列の中から削除

	//攻撃が当たっているかをチェックする　
	for (auto& actor : actors)
	{
		//攻撃を受けないアクターは当たり判定をしない
		if (actor->GetActorKind() == ActorKind::None)continue;
		if (actor->GetActorKind() == ActorKind::Field)continue;
		if (actor->GetActorKind() == ActorKind::Obstacle)continue;
		if (actor->GetActorKind() == ActorKind::Item)continue;

		//攻撃が当たっているかチェック
		for (auto& attack : m_attacks)
		{

		}
	}
}

void AttackManager::SetAttack(std::shared_ptr<AttackBase> attack)
{
	//攻撃をセット
	m_attacks.emplace_back(attack);
}
