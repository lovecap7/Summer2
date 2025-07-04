#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class PlayerStateAttackN3 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN3>
{
public:
    PlayerStateAttackN3(std::shared_ptr<Player>  player);
    ~PlayerStateAttackN3();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
    
private:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //武器
    std::shared_ptr<Collidable> m_rightSword;
    //攻撃
    std::shared_ptr<MeleeAttack> m_attackN3;//通常3
    //剣(攻撃)の当たり判定作成
    void CreateAttack();
    //剣(攻撃)の位置更新
    void UpdateAttack();
    //減速していく
    void SpeedDown();
    //削除
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
    //攻撃時の前進処理
    void AttackMove();
};

