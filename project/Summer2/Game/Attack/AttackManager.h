#pragma once
#include <vector>
#include <memory>
class AttackBase;
class CollisionChecker;
class Actor;
class AttackManager
{
public:
	AttackManager();
	~AttackManager();
	void Update(std::vector<std::shared_ptr<Actor>> actors);
	void Draw()const;
	void SetAttack(std::shared_ptr<AttackBase> attack);
private:
	//攻撃
	std::vector<std::shared_ptr<AttackBase>> m_attacks;
	//当たり判定のチェックをするクラス
	std::shared_ptr<CollisionChecker> m_collChecker;
};

