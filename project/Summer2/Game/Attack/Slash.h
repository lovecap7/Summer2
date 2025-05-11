#pragma once
#include "AttackBase.h"
class Slash :
    public AttackBase
{
public:
	Slash(std::shared_ptr<Collidable> coll, float& damege, int& keepFrame);
	~Slash();
	//XVˆ—
	void Update() override;
	//“–‚½‚Á‚½
	void OnHit() override;
private:

};

