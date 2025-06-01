#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class PlayerStateAttackN3 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN3>
{
public:
    PlayerStateAttackN3(std::shared_ptr<Player>  player);
    ~PlayerStateAttackN3();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //•Ší
    std::shared_ptr<Collidable> m_rightSword;
    //UŒ‚
    std::shared_ptr<MeleeAttack> m_attackN3;//’Êí3
    //Œ•(UŒ‚)‚Ì“–‚½‚è”»’èì¬
    void CreateAttack();
    //Œ•(UŒ‚)‚ÌˆÊ’uXV
    void UpdateAttack();
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

