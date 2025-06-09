#include "BossDragonStateBase.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Attack/AttackBase.h"

BossDragonStateBase::BossDragonStateBase(std::shared_ptr<BossDragon> owner) :
	m_owner(owner),
	m_nextState(nullptr)
{
}

BossDragonStateBase::~BossDragonStateBase()
{
}
void BossDragonStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager>& attackManager)
{
	//çUåÇÇì¸ÇÍÇÈ
	attack->Init();//èÛë‘ÇÉäÉZÉbÉg
	attackManager->Entry(attack);//ìoò^
}

void BossDragonStateBase::ChangeState(std::shared_ptr<BossDragonStateBase> nextState)
{
	//èÛë‘ïœâª
	m_nextState = move(nextState);
}
