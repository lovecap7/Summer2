#include "Slash.h"

Slash::Slash(std::shared_ptr<Collidable> coll, float& damege, int& keepFrame):
	AttackBase(coll, damege, keepFrame)
{
}

Slash::~Slash()
{
}

void Slash::Update()
{
	//持続フレームを減らす
	m_keepFrame--;
	//持続フレームが0になったら消滅
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void Slash::OnHit()
{
#if _DEBUG
	printf("攻撃が当たった");
#endif
}
