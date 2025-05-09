#include "EnemyBase.h"
#include "../../EnemyManager.h"
#include "../../../General/Model.h"
EnemyBase::EnemyBase(std::unique_ptr<EnemyManager>& enemyManager):
	Actor(ActorKind::Enemy),
	m_id(enemyManager->GetEnemyID())
{
}
