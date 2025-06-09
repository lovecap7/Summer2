#include "BossDragon.h"

BossDragon::BossDragon(int modelHandle, Vector3 pos):
	EnemyBase(),
	m_attackCoolTime(0)
{
}

BossDragon::~BossDragon()
{
}

void BossDragon::Exit(std::shared_ptr<ActorManager> actorManager)
{
}

void BossDragon::Init()
{
}

void BossDragon::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
}

void BossDragon::Gravity(const Vector3& gravity)
{
}

void BossDragon::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void BossDragon::Complete()
{
}

void BossDragon::UpdateHurtPoint()
{
}

void BossDragon::UpdateAttackCoolTime()
{
}
