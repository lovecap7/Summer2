#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateWalk :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWalk>
{
public:
    PlayerStateWalk(std::shared_ptr<Player>  player);
    ~PlayerStateWalk();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //���͂̑傫���ɍ��킹�đ��x��Ԃ�
    float InputValueSpeed(const Input& input);
};

