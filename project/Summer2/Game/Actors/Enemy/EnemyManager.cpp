#include "EnemyManager.h"
#include "../../../General/Collision/CollisionChecker.h"
#include "../Player/Player.h"
#include "../Actor.h"
#include "EnemyBase.h"
#include "../../../General/Collidable.h"
#include "../../../General/Rigidbody.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> player) :
	m_player(player),
	m_collChecker(std::make_shared<CollisionChecker>())
{
}

void EnemyManager::Entry(std::shared_ptr<EnemyBase> enemy)
{
	m_enemies.emplace_back(enemy);
}

void EnemyManager::Exit(std::shared_ptr<EnemyBase> enemy)
{
	auto it = std::find(m_enemies.begin(), m_enemies.end(), enemy);
	//見つかった場合
	if (it != m_enemies.end())
	{
		//削除
		m_enemies.erase(it);
	}
}

void EnemyManager::Search()
{
	//空なら処理しない
	if (m_enemies.empty())return;

	//範囲内にプレイヤーがいるか判定をチェック
	for (auto& enemy : m_enemies)
	{
		//範囲内にプレイヤーがいるかチェック
		if (m_collChecker->CheckCollCS(m_player->GetCollidable(), std::dynamic_pointer_cast<EnemyBase>(enemy)->GetSearchTrigger()))
		{
			//当たった時の処理
			std::dynamic_pointer_cast<EnemyBase>(enemy)->OnHitSearchPlayer(m_player->GetCollidable()->GetRb()->GetPos());
		}
	}
}