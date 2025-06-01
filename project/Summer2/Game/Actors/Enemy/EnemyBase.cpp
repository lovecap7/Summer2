#include "EnemyBase.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/Model.h"
EnemyBase::EnemyBase():
	Actor(ActorKind::Enemy)
{
}

//�U��������o��
void EnemyBase::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//�U��������
	attack->Init();
	attackManager->Entry(attack);
}
