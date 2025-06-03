#include "EnemyBase.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/Model.h"
#include "../../../General/Collidable.h"
#include "../../../General/Rigidbody.h"
EnemyBase::EnemyBase():
	Actor(ActorKind::Enemy),
	m_isHitSearch(false),
	m_playerPos()
{
}

void EnemyBase::OnHitSearch(const Vector3& playerPos)
{
	//�T�[�`
	m_isHitSearch = true;
	//�v���C���[�̈ʒu���擾
	m_playerPos = playerPos;
}

//�U��������o��
void EnemyBase::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//�U��������
	attack->Init();
	attackManager->Entry(attack);
}

Vector3 EnemyBase::GetPlayerVec() const
{
	Vector3 playerVec = m_playerPos - m_collidable->GetRb()->GetPos();
	return playerVec;
}
