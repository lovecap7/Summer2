#include "SmallDragonStateBase.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Attack/AttackBase.h"

SmallDragonStateBase::SmallDragonStateBase(std::shared_ptr<SmallDragon> owner) :
	m_owner(owner),
	m_nextState(nullptr)
{
}

SmallDragonStateBase::~SmallDragonStateBase()
{
}

void SmallDragonStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager>& attackManager)
{
	//çUåÇÇì¸ÇÍÇÈ
	attack->Init();//èÛë‘ÇÉäÉZÉbÉg
	attackManager->Entry(attack);//ìoò^
}

void SmallDragonStateBase::ChangeState(std::shared_ptr<SmallDragonStateBase> nextState)
{
	//èÛë‘ïœâª
	m_nextState = move(nextState);
}
