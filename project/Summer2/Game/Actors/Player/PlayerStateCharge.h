#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateCharge :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCharge>
{
public:
    PlayerStateCharge(std::shared_ptr<Player> player);
    ~PlayerStateCharge();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    //ƒ^ƒŠÔ
    int m_chargeFrame;
    //Œ¸‘¬
    void SpeedDown();
};



