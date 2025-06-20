#pragma once
#include "SmallDragonStateBase.h"
class SmallDragon;
class Input;
class Camera;
class AttackManager;
class SmallDragonStateIdle :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateIdle>
{
public:
	SmallDragonStateIdle(std::shared_ptr<SmallDragon> owner);
	~SmallDragonStateIdle();
	void Init()override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
	//減速していく
	void SpeedDown();
};