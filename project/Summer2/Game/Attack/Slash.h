#pragma once
#include "AttackBase.h"
class Actor;
class Slash :
    public AttackBase
{
public:
	Slash(std::shared_ptr<Collidable> coll, float& damege, int& keepFrame);
	~Slash();
	//XVˆ—
	void Update() override;
	//“–‚½‚Á‚½
	void OnHit(std::shared_ptr<Actor> actor) override;
private:

};

