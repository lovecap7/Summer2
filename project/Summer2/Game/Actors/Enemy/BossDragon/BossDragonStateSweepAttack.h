#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class BossDragon;
class BossDragonStateSweepAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateSweepAttack>
{
public:
    BossDragonStateSweepAttack(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateSweepAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //左腕
    std::shared_ptr<Collidable> m_leftArm;
    //攻撃
    std::shared_ptr<MeleeAttack> m_attack;
    //攻撃の当たり判定作成
    void CreateAttack();
    //攻撃の位置更新
    void UpdateAttack();
    //減速していく
    void SpeedDown();
    //削除
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};

