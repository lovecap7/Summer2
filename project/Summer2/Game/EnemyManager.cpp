#include "EnemyManager.h"
#include "../General/Collision/CollisionChecker.h"
#include "Actors/Player/Player.h"
#include "Actors/Actor.h"
#include "Actors/Enemy/EnemyBase.h"
#include "../General/Collidable.h"
#include "../General/Rigidbody.h"
#include "../General/Collision/SphereCollider.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> player):
	m_player(player),
	m_collChecker(std::make_shared<CollisionChecker>())
{
}

void EnemyManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//�͈͓��Ƀv���C���[�����邩������`�F�b�N
	for (auto& enemy : actors)
	{
		//�G�݂̂��`�F�b�N
		if (enemy->GetActorKind() != ActorKind::Enemy)continue;

		//�͈͓��Ƀv���C���[�����邩�`�F�b�N
		if (m_collChecker->CheckCollCS(m_player->GetCollidable(), std::dynamic_pointer_cast<EnemyBase>(enemy)->GetSearchTrigger()))
		{
			//�����������̏���
			std::dynamic_pointer_cast<EnemyBase>(enemy)->OnHitSearch(m_player->GetCollidable()->GetRb()->GetPos());
		}
	}
}
