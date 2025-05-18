#pragma once
#include "AttackBase.h"
class Actor;
class Strike :
    public AttackBase
{
public:
    Strike(std::shared_ptr<Collidable> coll, float damage, int keepFrame, Actor& owner);
    ~Strike();
	//‰Šú‰»
	void Init() override;
	//XVˆ—
	void Update() override;
	//•`‰æ
	void Draw() override;
	//“–‚½‚Á‚½
	void OnHit(std::shared_ptr<Actor> actor) override;
};

