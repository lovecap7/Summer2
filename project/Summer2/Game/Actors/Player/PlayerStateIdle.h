#pragma once
#include "PlayerStateBase.h"
class Player;
class PlayerStateIdle :
    public PlayerStateBase
{
public:
    PlayerStateIdle(Player* player);
    ~PlayerStateIdle();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
};

