#pragma once
#include <list>
#include <memory>
class AttackBase;
class CollisionChecker;
class AttackManager
{
public:
	AttackManager();
	~AttackManager();
	void Update();
	void SetAttack(std::shared_ptr<AttackBase> attack);
private:
	//�U��
	std::list<std::shared_ptr<AttackBase>> m_attacks;
	//�����蔻��̃`�F�b�N������N���X
	std::shared_ptr<CollisionChecker> m_collChecker;
};

