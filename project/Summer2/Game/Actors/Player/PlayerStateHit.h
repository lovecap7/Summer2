#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateHit :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateHit>
{
public:
    PlayerStateHit(std::shared_ptr<Player> player);
    ~PlayerStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
};

