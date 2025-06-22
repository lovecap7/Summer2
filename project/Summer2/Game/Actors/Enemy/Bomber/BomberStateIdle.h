#pragma once
#include "BomberStateBase.h"

class Input;
class Camera;
class AttackManager;
class BomberStateIdle :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateIdle>
{
public:
	BomberStateIdle(std::shared_ptr<Bomber> owner);
	~BomberStateIdle();
	void Init()override;
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};