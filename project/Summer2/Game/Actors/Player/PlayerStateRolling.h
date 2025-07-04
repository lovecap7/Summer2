#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateRolling :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateRolling>
{
public:
    PlayerStateRolling(std::shared_ptr<Player> player);
    ~PlayerStateRolling();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    
};

