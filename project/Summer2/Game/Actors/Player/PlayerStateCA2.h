#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class PlayerStateCA2 :
    public PlayerStateBase
{
public:
    PlayerStateCA2(std::shared_ptr<Player> player);
    ~PlayerStateCA2();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //¶‘«
    std::shared_ptr<Collidable> m_leftLeg;
    //UŒ‚”»’è
    std::shared_ptr<MeleeAttack> m_attackC;
    //UŒ‚”»’è‚ğì¬
    void CreateAttack();
    //UŒ‚‚Ìó‘ÔXV
    void UpdateAttack();
    //Œ¸‘¬
    void SpeedDown();
};



