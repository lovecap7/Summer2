#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class PurpleDinosaurStateAttack :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateAttack>
{
public:
	PurpleDinosaurStateAttack(std::shared_ptr<PurpleDinosaur> owner);
	~PurpleDinosaurStateAttack();
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
    //攻撃時の前進処理
    void AttackMove();
};

