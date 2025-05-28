#include "EnemyBase.h"
#include "../../EnemyManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/Model.h"
EnemyBase::EnemyBase(std::unique_ptr<EnemyManager>& enemyManager):
	Actor(ActorKind::Enemy),
	m_id(enemyManager->GetEnemyID())
{
}

//UŒ‚”»’è‚ğo‚·
void EnemyBase::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//UŒ‚‚ğ“ü‚ê‚é
	attackManager->SetAttack(attack);
	attack->Init();
}
