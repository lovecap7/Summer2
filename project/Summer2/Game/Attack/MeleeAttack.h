#pragma once
#include "AttackBase.h"
class MeleeAttack :
    public AttackBase
{
public:
	/// <summary>
	/// �ߐڍU��
	/// </summary>
	/// <param name="coll">�Փˊm�F�̂��߂̏��</param>
	/// <param name="damage">�_���[�W</param>
	/// <param name="keepFrame">����</param>
	/// <param name="owner">������</param>
	MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, std::shared_ptr<Actor> owner);
	~MeleeAttack() override;
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;
	//����������
	void OnHit(std::shared_ptr<Actor> actor) override;
};

