#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class PlayerStateAttackN2 :
    public PlayerStateBase
{
public:
    PlayerStateAttackN2(std::shared_ptr<Player>  player);
    ~PlayerStateAttackN2();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //•Ší
    std::shared_ptr<Collidable> m_rightSword;
    //UŒ‚
    std::shared_ptr<MeleeAttack> m_attackN2;//’Êí2
    //Œ•(UŒ‚)‚Ì“–‚½‚è”»’èì¬
    void CreateAttack();
    //Œ•(UŒ‚)‚ÌˆÊ’uXV
    void UpdateAttack();
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

