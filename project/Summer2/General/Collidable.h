#pragma once
#include <memory>
#include "Math/MyMath.h"

//状態に合わせて当たり判定を行う時に使う
enum class State
{
	None,
	Jump,
	Fall,
	Dead
};

class ColliderBase;
class Actor;
class Rigidbody;
class Collidable
{
public:
	Collidable(std::shared_ptr<ColliderBase> coll, std::shared_ptr<Rigidbody> rb);
	virtual ~Collidable() {};
	//当たり判定
	const std::shared_ptr<ColliderBase>& GetColl() const { return m_coll; }
	//座標とベクトル
	const std::shared_ptr<Rigidbody>& GetRb()const { return m_rb; }
	//当たり判定を行うかどうか
	bool IsCollide() const { return m_isCollide; };
	void SetIsCollide(bool isCollide) { m_isCollide = isCollide; }
	//動かないかどうか
	bool IsStatic() const { return m_isStatic; };
	void SetIsStatic(bool isStatic) { m_isStatic = isStatic; }
	//状態
	State GetState() { return m_state; };
	void SetState(State state) { m_state = state; };
	//持ち主
	std::shared_ptr<Actor> GetOwner() { return m_owner; };
	void SetOwner(std::shared_ptr<Actor> owner) { m_owner = owner; };
private:
	//当たり判定
	std::shared_ptr<ColliderBase> m_coll;
	//座標とベクトル
	std::shared_ptr<Rigidbody> m_rb;
	//衝突判定を行う
	bool m_isCollide;
	//動かない
	bool m_isStatic;
	//状態
	State m_state;
	//持ち主
	std::shared_ptr<Actor> m_owner;
};



