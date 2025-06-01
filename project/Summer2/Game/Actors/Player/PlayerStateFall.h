#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateFall :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateFall>
{
public:
    PlayerStateFall(std::shared_ptr<Player>  player);
    ~PlayerStateFall();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

