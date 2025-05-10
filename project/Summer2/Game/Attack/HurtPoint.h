#pragma once
//やられ判定クラス
#include <memory>
class Collidable;
class HurtPoint
{
public:
	HurtPoint(std::shared_ptr<Collidable> coll,int hp);
	~HurtPoint();
	//攻撃を喰らったかどうかをチェックする際に使う
	const std::shared_ptr<Collidable>& GetCollidable() const { return m_collidable; }

	//無敵
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
	//ダメージを喰らう
	void OnHitDamage(int damage);
	//死亡
	bool IsDead() { return m_isDead; };
private:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//無敵
	bool m_isNoDamage;
	//体力
	int m_hp;
	//死亡
	bool m_isDead;
};

