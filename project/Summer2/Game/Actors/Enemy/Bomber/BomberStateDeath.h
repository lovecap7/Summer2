#pragma once
#include "BomberStateBase.h"
class Input;
class Camera;
class AttackManager;
class Bomber;
class BomberStateDeath :
	public BomberStateBase, public std::enable_shared_from_this<BomberStateDeath>
{
public:
	BomberStateDeath(std::shared_ptr<Bomber> owner);
	~BomberStateDeath();
	void Init()override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};


