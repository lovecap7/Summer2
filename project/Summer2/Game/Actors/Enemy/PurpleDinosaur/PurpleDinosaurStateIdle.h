#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class AttackManager;
class PurpleDinosaurStateIdle :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateIdle>
{
public:
	PurpleDinosaurStateIdle(std::shared_ptr<PurpleDinosaur> owner);
	~PurpleDinosaurStateIdle();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
	//Œ¸‘¬‚µ‚Ä‚¢‚­
	void SpeedDown();
};