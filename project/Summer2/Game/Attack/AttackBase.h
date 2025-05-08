#pragma once
#include <memory>
#include <vector>
class Collidable;
class Actor;
class AttackBase abstract
{
public:
	AttackBase(std::shared_ptr<Collidable> coll, float& damege, int& keepFrame);
	virtual ~AttackBase() {};
	//コライダー
	std::shared_ptr<Collidable>& GetCollidable() { return m_collidable; };
	//更新処理
	virtual void Update() abstract;
protected:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//ダメージ
	float m_damege;
	//持続フレーム
	int m_keepFrame;
	//消滅フラグ
	bool m_isDead;
	//当てたことのあるCollidableを覚えておく
	std::vector<std::weak_ptr<Collidable>> m_hitCollidables;
};

