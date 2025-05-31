#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class MeleeAttack;
class Collidable;
class PlayerStateCA1 :
    public PlayerStateBase
{
public:
    PlayerStateCA1(std::shared_ptr<Player> player);
    ~PlayerStateCA1();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
private:
    //����
    std::shared_ptr<Collidable> m_leftLeg;
    //�U������
    std::shared_ptr<MeleeAttack> m_attackC;
    //�U��������쐬
    void CreateAttack();
    //�U���̏�ԍX�V
    void UpdateAttack();
    //����
    void SpeedDown();
};



