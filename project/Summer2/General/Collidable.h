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
	//�����蔻��
	const std::shared_ptr<ColliderBase>& GetColl() const { return m_coll; }
	//���W�ƃx�N�g��
	const std::shared_ptr<Rigidbody>& GetRb()const { return m_rb; }
	//�����蔻����s�����ǂ���
	bool IsCollide() const { return m_isCollide; };
	void SetIsCollide(bool isCollide) { m_isCollide = isCollide; }
private:
	//�����蔻��
	std::shared_ptr<ColliderBase> m_coll;
	//���W�ƃx�N�g��
	std::shared_ptr<Rigidbody> m_rb;
	//�Փ˔�����s��
	bool m_isCollide
};


