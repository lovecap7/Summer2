#pragma once
#include <memory>
#include <vector>
class Collidable;
class Actor;
class AttackBase
{
public:
	AttackBase(std::shared_ptr<Collidable> coll, float& damege, int& keepFrame);
	virtual ~AttackBase() {};
	//コライダー
	std::shared_ptr<Collidable>& GetCollidable() { return m_collidable; };
private:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//ダメージ
	float m_damege;
	//持続フレーム
	int m_keepFrame;
	//消滅フラグ
	bool m_isDead;
};

