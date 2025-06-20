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
	//���������ꍇ
	if (it != m_enemies.end())
	{
		//�폜
		m_enemies.erase(it);
	}
}

void EnemyManager::Search()
{
	//��Ȃ珈�����Ȃ�
	if (m_enemies.empty())return;

	//�͈͓��Ƀv���C���[�����邩������`�F�b�N
	for (auto& enemy : m_enemies)
	{
		//�͈͓��Ƀv���C���[�����邩�`�F�b�N
		if (m_collChecker->CheckCollCS(m_player->GetCollidable(), std::dynamic_pointer_cast<EnemyBase>(enemy)->GetSearchTrigger()))
		{
			//�����������̏���
			std::dynamic_pointer_cast<EnemyBase>(enemy)->OnHitSearchPlayer(m_player->GetCollidable()->GetRb()->GetPos());
		}
	}
}