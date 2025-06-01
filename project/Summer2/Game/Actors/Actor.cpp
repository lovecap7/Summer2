#include "Actor.h"
#include "ActorManager.h"
#include "Enemy/EnemyManager.h"
#include "../../General/Collision/CollisionManager.h"
#include "Enemy/EnemyBase.h"

Actor::Actor(ActorKind kind):
	m_actorKind(kind),
	m_isDead(false),
	m_id(0),
	m_isSetId(false)
{
}

void Actor::SetID(int id)
{
	//���łɓo�^���Ă�Ȃ�
	if (m_isSetId)return;
	m_id = id;
	m_isSetId = true;
}
