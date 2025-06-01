#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class AttackBase;
class AttackManager;
class EnemyBase abstract:
    public Actor
{
public:
    EnemyBase();
    virtual ~EnemyBase() {};
    virtual void OnHitSearch(const Vector3& playerPos) abstract;
    std::shared_ptr<Collidable>& GetSearchTrigger() { return m_searchTrigger; };
protected:
    //モデル
    std::unique_ptr<Model> m_model;
    //プレイヤーが近くにいるかをチェックするトリガー
    std::shared_ptr<Collidable> m_searchTrigger;
    //攻撃のコンポーネント
    virtual void CreateAttack() abstract;
    //状態に合わせて初期化すべきものを初期化する
    virtual void InitState() abstract;
    //やられ判定の更新
    virtual void UpdateHurtPoint()abstract;
    //攻撃判定を出す
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
};

