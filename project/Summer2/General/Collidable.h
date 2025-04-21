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
	const std::shared_ptr<ColliderBase>& GetColl() { return m_coll; }
	//座標とベクトル
	const std::shared_ptr<Rigidbody>& GetRb() { return m_rb; }
private:
	//当たり判定
	std::shared_ptr<ColliderBase> m_coll;
	//座標とベクトル
	std::shared_ptr<Rigidbody> m_rb;
};


