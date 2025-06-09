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
	//�U��������
	attack->Init();//��Ԃ����Z�b�g
	attackManager->Entry(attack);//�o�^
}

void BossDragonStateBase::ChangeState(std::shared_ptr<BossDragonStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}
