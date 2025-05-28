#pragma once
#include "AttackBase.h"
class MeleeAttack :
    public AttackBase
{
public:
	MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, Actor& owner);
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

