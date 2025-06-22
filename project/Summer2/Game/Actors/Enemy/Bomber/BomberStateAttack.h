#pragma once
#include "BomberStateBase.h"

class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class BomberStateAttack :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateAttack>
{
public:
    BomberStateAttack(std::shared_ptr<Bomber> owner);
    ~BomberStateAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //¶˜r
    std::shared_ptr<Collidable> m_leftArm;
    //UŒ‚
    std::shared_ptr<MeleeAttack> m_attack;
    //UŒ‚‚Ì“–‚½‚è”»’èì¬
    void CreateAttack();
    //UŒ‚‚ÌˆÊ’uXV
    void UpdateAttack();
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
    //íœ
    void DeleteAttack(const std::shared_ptr<AttackManager>& attackManager);
    //UŒ‚‚Ì‘Oiˆ—
    void AttackMove();
};

