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
	//������
	virtual void Init() abstract;
	//�X�V����
	virtual void Update() abstract;
	//�`��
	virtual void Draw() abstract;
	//����������
	virtual void OnHit(std::shared_ptr<Actor> actor) abstract;
	//�R���C�_�[
	std::shared_ptr<Collidable>& GetCollidable() { return m_collidable; };
	//����
	bool IsDead() { return m_isDead; };
	//�_���[�W
	float GetDamege() { return m_damege; };
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
	std::vector<int> m_hitEnemyId;
	//������
	Actor* m_owner;
};

