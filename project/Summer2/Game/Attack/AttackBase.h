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
	//�R���C�_�[
	std::shared_ptr<Collidable>& GetCollidable() { return m_collidable; };
	//�X�V����
	virtual void Update() abstract;
protected:
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//�_���[�W
	float m_damege;
	//�����t���[��
	int m_keepFrame;
	//���Ńt���O
	bool m_isDead;
	//���Ă����Ƃ̂���Collidable���o���Ă���
	std::vector<std::weak_ptr<Collidable>> m_hitCollidables;
};

