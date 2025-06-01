#pragma once
#include <memory>
#include <vector>
class Collidable;
class Actor;
class AttackBase abstract
{
public:
	AttackBase(std::shared_ptr<Collidable> coll, int damege, int keepFrame, std::shared_ptr<Actor> owner);
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
	std::shared_ptr<Collidable> GetCollidable()const { return m_collidable; };
	//消滅
	bool IsDead() { return m_isDead; };
	//攻撃判定を消す
	void Delete() { m_isDead = true; };
	//ダメージ
	int GetDamage() { return m_damage; };
	void SetDamage(int damage);
	//持続フレーム
	void SetKeepFrame(int keepFrame);
protected:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//ダメージ
	int m_damage;
	//持続フレーム
	int m_keepFrame;
	//消滅フラグ
	bool m_isDead;
	//当てたことのあるCollidableを覚えておく
	std::vector<int> m_hitId;
	//持ち主
	std::shared_ptr<Actor> m_owner;
	//初期化用
	int m_initDamage;//初期化用
	int m_initKeepFrame;//初期化用
};

