#pragma once
//やられ判定クラス
#include <memory>
#include "../../General/Math/MyMath.h"
class Collidable;
class Actor;
class HurtPoint
{
public:
	/// <summary>
	/// やられ判定と体力と所有者を持つクラス
	/// </summary>
	/// <param name="coll">やられ判定</param>
	/// <param name="hp">体力</param>
	HurtPoint(std::shared_ptr<Collidable> coll,int hp, std::shared_ptr<Actor> owner);
	~HurtPoint();
	//攻撃を喰らったかどうかをチェックする際に使う
	std::shared_ptr<Collidable> GetCollidable() const { return m_collidable; }
	//初期化処理
	void Init();
	//無敵
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
	//ダメージを喰らう
	void OnHitDamage(int damage);
	//のけぞる
	void OnHitKnockBack(const Vector3& knockBackVec);
	//死亡
	bool IsDead() { return m_hp <= 0; };
	//体力
	int GetHp() { return m_hp; };
	//体力に加算
	void AddHp(int add);
	//攻撃を受けたかどうか
	bool IsHit() const { return m_isHit; }
private:
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//無敵
	bool m_isNoDamage;
	//体力
	int m_hp;
	//最大体力
	int m_maxHp;
	//持ち主
	std::shared_ptr<Actor> m_owner;
	//攻撃を受けたかどうか
	bool m_isHit;
};

