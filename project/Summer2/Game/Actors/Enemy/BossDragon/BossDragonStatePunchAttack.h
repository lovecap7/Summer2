#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class BossDragon;
class BossDragonStatePunchAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStatePunchAttack>
{
public:
    BossDragonStatePunchAttack(std::shared_ptr<BossDragon> owner);
    ~BossDragonStatePunchAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //�U��
    std::shared_ptr<MeleeAttack> m_attack;
    //�U���̓����蔻��쐬
    void CreateAttack();
    //�U���̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
};