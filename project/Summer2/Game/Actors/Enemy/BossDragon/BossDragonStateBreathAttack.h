#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class BulletAttack;
class Model;
class BossDragon;
class BossDragonStateBreathAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateBreathAttack>
{
public:
    BossDragonStateBreathAttack(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateBreathAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃
    std::shared_ptr<BulletAttack> m_bullet1;
    std::shared_ptr<BulletAttack> m_bullet2;
    std::shared_ptr<BulletAttack> m_bullet3;
    //攻撃の当たり判定作成
    void CreateAttack();
    //弾作成
    void CreateBullet(Vector3& bulletPos, std::shared_ptr<Model>& model, std::shared_ptr<BulletAttack>& bullet);
    //減速していく
    void SpeedDown();
};

