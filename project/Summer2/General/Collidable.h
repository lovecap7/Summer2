#pragma once
#include <memory>
#include "Math/MyMath.h"

//��Ԃɍ��킹�ē����蔻����s�����Ɏg��
enum class State
{
	None,
	Jump,
	Fall,
	Dead
};

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
	//�����Ȃ����ǂ���
	bool IsStatic() const { return m_isStatic; };
	void SetIsStatic(bool isStatic) { m_isStatic = isStatic; }
	//���
	State GetState() { return m_state; };
	void SetState(State state) { m_state = state; };
private:
	//�����蔻��
	std::shared_ptr<ColliderBase> m_coll;
	//���W�ƃx�N�g��
	std::shared_ptr<Rigidbody> m_rb;
	//�Փ˔�����s��
	bool m_isCollide;
	//�����Ȃ�
	bool m_isStatic;
	//���
	State m_state;
};

//���W�ŕ\���Ƃ��Ɏg��
typedef Collidable Trigger;


