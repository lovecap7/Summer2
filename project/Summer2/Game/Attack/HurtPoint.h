#pragma once
//���ꔻ��N���X
#include <memory>
class Collidable;
class Actor;
class HurtPoint
{
public:
	/// <summary>
	/// ���ꔻ��Ƒ̗͂Ə��L�҂����N���X
	/// </summary>
	/// <param name="coll">���ꔻ��</param>
	/// <param name="hp">�̗�</param>
	HurtPoint(std::shared_ptr<Collidable> coll,int hp, Actor& owner);
	~HurtPoint();
	//�U�������������ǂ������`�F�b�N����ۂɎg��
	const std::shared_ptr<Collidable>& GetCollidable() const { return m_collidable; }

	//���G
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
	//�_���[�W����炤
	void OnHitDamage(int damage);
	//�̂�����
	void OnHitKnockBack(const std::shared_ptr<Collidable>& other);
	//���S
	bool IsDead() { return m_isDead; };
private:
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//���G
	bool m_isNoDamage;
	//�̗�
	int m_hp;
	//���S
	bool m_isDead;
	//������
	Actor* m_owner;
};

