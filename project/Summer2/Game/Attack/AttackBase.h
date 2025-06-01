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
	//������
	virtual void Init() abstract;
	//�X�V����
	virtual void Update() abstract;
	//�`��
	virtual void Draw() abstract;
	//����������
	virtual void OnHit(std::shared_ptr<Actor> actor) abstract;
	//�R���C�_�[
	std::shared_ptr<Collidable> GetCollidable()const { return m_collidable; };
	//����
	bool IsDead() { return m_isDead; };
	//�U�����������
	void Delete() { m_isDead = true; };
	//�_���[�W
	int GetDamage() { return m_damage; };
	void SetDamage(int damage);
	//�����t���[��
	void SetKeepFrame(int keepFrame);
protected:
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//�_���[�W
	int m_damage;
	//�����t���[��
	int m_keepFrame;
	//���Ńt���O
	bool m_isDead;
	//���Ă����Ƃ̂���Collidable���o���Ă���
	std::vector<int> m_hitId;
	//������
	std::shared_ptr<Actor> m_owner;
	//�������p
	int m_initDamage;//�������p
	int m_initKeepFrame;//�������p
};

