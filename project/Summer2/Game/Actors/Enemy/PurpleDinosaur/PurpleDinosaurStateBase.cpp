#include "PurpleDinosaurStateBase.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Attack/AttackBase.h"

PurpleDinosaurStateBase::PurpleDinosaurStateBase(std::shared_ptr<PurpleDinosaur> owner):
	m_owner(owner),
	m_nextState(nullptr)
{
}

PurpleDinosaurStateBase::~PurpleDinosaurStateBase()
{
}
void PurpleDinosaurStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager>& attackManager)
{
	//çUåÇÇì¸ÇÍÇÈ
	attack->Init();//èÛë‘ÇÉäÉZÉbÉg
	attackManager->Entry(attack);//ìoò^
}

void PurpleDinosaurStateBase::ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState)
{
	//èÛë‘ïœâª
	m_nextState = move(nextState);
}
