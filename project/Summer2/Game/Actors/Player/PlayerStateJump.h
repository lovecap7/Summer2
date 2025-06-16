#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateJump :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateJump>
{
public:
    PlayerStateJump(std::shared_ptr<Player>  player);
    ~PlayerStateJump();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    //“ü—Í‚É‘Î‰ž‚µ‚½ˆÚ“®—Ê
    float InputValueSpeed(const Input& input);
};

