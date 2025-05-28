#pragma once
#include "AttackBase.h"
class MeleeAttack :
    public AttackBase
{
public:
	MeleeAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, Actor& owner);
	~MeleeAttack() override;
	//‰Šú‰»
	void Init() override;
	//XVˆ—
	void Update() override;
	//•`‰æ
	void Draw() override;
	//“–‚½‚Á‚½
	void OnHit(std::shared_ptr<Actor> actor) override;
};

