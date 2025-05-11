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
	//�����t���[�������炷
	m_keepFrame--;
	//�����t���[����0�ɂȂ��������
	if (m_keepFrame <= 0)
	{
		m_isDead = true;
	}
}

void Slash::OnHit()
{
#if _DEBUG
	printf("�U������������");
#endif
}
