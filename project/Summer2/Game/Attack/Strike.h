#pragma once
#include "AttackBase.h"
class Actor;
class Strike :
    public AttackBase
{
public:
    Strike(std::shared_ptr<Collidable> coll, float damage, int keepFrame, Actor& owner);
    ~Strike();
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;
	//����������
	void OnHit(std::shared_ptr<Actor> actor) override;
};

