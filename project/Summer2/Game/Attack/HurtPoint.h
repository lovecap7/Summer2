#pragma once
//���ꔻ��N���X
#include <memory>
#include "../../General/Math/MyMath.h"
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
	HurtPoint(std::shared_ptr<Collidable> coll,int hp, std::shared_ptr<Actor> owner);
	~HurtPoint();
	//�U�������������ǂ������`�F�b�N����ۂɎg��
	std::shared_ptr<Collidable> GetCollidable() const { return m_collidable; }
	//����������
	void Init();
	//���G
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
	//�_���[�W����炤
	void OnHitDamage(int damage);
	//�̂�����
	void OnHitKnockBack(const Vector3& knockBackVec);
	//���S
	bool IsDead() { return m_hp <= 0; };
	//�̗�
	int GetHp() { return m_hp; };
	//�̗͂ɉ��Z
	void AddHp(int add);
	//�U�����󂯂����ǂ���
	bool IsHit() const { return m_isHit; }
private:
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//���G
	bool m_isNoDamage;
	//�̗�
	int m_hp;
	//�ő�̗�
	int m_maxHp;
	//������
	std::shared_ptr<Actor> m_owner;
	//�U�����󂯂����ǂ���
	bool m_isHit;
};

