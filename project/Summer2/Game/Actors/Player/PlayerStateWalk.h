#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateWalk :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWalk>
{
public:
    PlayerStateWalk(std::shared_ptr<Player>  player);
    ~PlayerStateWalk();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //入力の大きさに合わせて速度を返す
    float InputValueSpeed(const Input& input);
};

