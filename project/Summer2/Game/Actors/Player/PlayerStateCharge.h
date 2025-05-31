#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateCharge :
    public PlayerStateBase
{
public:
    PlayerStateCharge(std::shared_ptr<Player> player);
    ~PlayerStateCharge();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //ƒ^ƒŠÔ
    int m_chargeFrame;
    //Œ¸‘¬
    void SpeedDown();
};



