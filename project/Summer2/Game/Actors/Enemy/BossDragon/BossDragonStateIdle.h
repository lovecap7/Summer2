#pragma once
#include "BossDragonStateBase.h"
class Camera;
class ActorManager;
class Input;
class BossDragon;
class BossDragonStateIdle :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateIdle>
	
{
public:
	BossDragonStateIdle(std::shared_ptr<BossDragon> owner);
	~BossDragonStateIdle();
	void Init()override;
	void Update(const Input & input, const std::unique_ptr<Camera>&camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
	//減速していく
	void SpeedDown();
	//どの攻撃をするか考える
	void ThinkAttack();
};