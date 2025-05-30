#pragma once
#include <memory>

class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateBase
{
protected:
	Player* m_player;
public:
	PlayerStateBase(Player* player);
	~PlayerStateBase();
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) abstract;
};

