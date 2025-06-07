#pragma once
#include "SmallDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class SmallDragon;
class SmallDragonStateDeath :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateDeath>
{
public:
	SmallDragonStateDeath(std::shared_ptr<SmallDragon> owner);
	~SmallDragonStateDeath();
	void Init() override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};

