#pragma once
#include "SmallDragonStateBase.h"
class Input;
class Camera;
class ActorManager;
class Collidable;
class BulletAttack;
class SmallDragonStateAttack :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateAttack>
{
public:
	SmallDragonStateAttack(std::shared_ptr<SmallDragon> owner);
	~SmallDragonStateAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃
    std::shared_ptr<BulletAttack> m_attack;
    //攻撃の当たり判定作成
    void CreateAttack();
    //減速していく
    void SpeedDown();
};