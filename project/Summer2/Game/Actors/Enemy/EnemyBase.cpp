#include "EnemyBase.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/Model.h"
EnemyBase::EnemyBase():
	Actor(ActorKind::Enemy)
{
}

//UŒ‚”»’è‚ğo‚·
void EnemyBase::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//UŒ‚‚ğ“ü‚ê‚é
	attack->Init();
	attackManager->Entry(attack);
}
