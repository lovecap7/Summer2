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
	//攻撃
	std::list<std::shared_ptr<AttackBase>> m_attacks;
	//当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
};

