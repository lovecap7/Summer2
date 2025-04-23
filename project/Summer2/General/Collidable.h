#pragma once
#include <memory>
#include "Math/MyMath.h"

class ColliderBase;
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
private:
	//当たり判定
	std::shared_ptr<ColliderBase> m_coll;
	//座標とベクトル
	std::shared_ptr<Rigidbody> m_rb;
	//衝突判定を行う
	bool m_isCollide
};


