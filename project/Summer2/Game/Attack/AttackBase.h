#pragma once
#include <memory>
#include <vector>
class Collidable;
class Actor;
class AttackBase abstract
{
public:
	AttackBase(std::shared_ptr<Collidable> coll, float damege, int keepFrame, Actor& owner);
	virtual ~AttackBase() {};
	//初期化
	virtual void Init() abstract;
	//更新処理
	virtual void Update() abstract;
	//描画
	virtual void Draw() abstract;
	//当たった時
	virtual void OnHit(std::shared_ptr<Actor> actor) abstract;
	//コライダー
	std::shared_ptr<Collidable>& GetCollidable() { return m_collidable; };
	//消滅
	bool IsDead() { return m_isDead; };
	//攻撃判定を消す
	void Delete() { m_isDead = true; };
	//ダメージ
	float GetDamage() { return m_damage; };
	void SetDamage(float damage);
	//持続フレーム
	void SetKeepFrame(int keepFrame);
protected:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//ダメージ
	float m_damage;
	//持続フレーム
	int m_keepFrame;
	//消滅フラグ
	bool m_isDead;
	//当てたことのあるCollidableを覚えておく
	std::vector<int> m_hitEnemyId;
	//持ち主
	Actor* m_owner;
	//初期化用
	float m_initDamage;//初期化用
	float m_initKeepFrame;//初期化用
};

