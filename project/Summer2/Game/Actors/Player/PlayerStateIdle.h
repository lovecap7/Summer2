#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateIdle :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateIdle>
{
public:
    PlayerStateIdle(std::shared_ptr<Player>  player);
    ~PlayerStateIdle();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

