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
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //����
    std::shared_ptr<Collidable> m_rightSword;
    //�U��
    std::shared_ptr<MeleeAttack> m_attackN2;//�ʏ�2
    //��(�U��)�̓����蔻��쐬
    void CreateAttack();
    //��(�U��)�̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
};

