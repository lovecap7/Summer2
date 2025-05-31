#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateWalk :
    public PlayerStateBase
{
public:
    PlayerStateWalk(std::shared_ptr<Player>  player);
    ~PlayerStateWalk();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //“ü—Í‚Ì‘å‚«‚³‚É‡‚í‚¹‚Ä‘¬“x‚ğ•Ô‚·
    float InputValueSpeed(const Input& input);
};

