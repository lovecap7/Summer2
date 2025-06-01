#include "Actor.h"
#include "ActorManager.h"
#include "Enemy/EnemyManager.h"
#include "../../General/Collision/CollisionManager.h"
#include "Enemy/EnemyBase.h"

Actor::Actor(ActorKind kind):
	m_actorKind(kind),
	m_isDead(false)
{
}

void Actor::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//“G‚Ìê‡“o˜^
	if (m_actorKind == ActorKind::Enemy)
	{
		actorManager->GetEnemyManager()->Entry(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
	}

}

void Actor::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//“G‚Ìê‡“o˜^‰ðœ
	if (m_actorKind == ActorKind::Enemy)
	{
		actorManager->GetEnemyManager()->Exit(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
	}
}
