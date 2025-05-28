#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class EnemyManager;
class AttackBase;
class EnemyBase abstract:
    public Actor
{
public:
    EnemyBase(std::unique_ptr<EnemyManager>& enemyManager);
    virtual ~EnemyBase() {};
    virtual void OnHitSearch(const Vector3& playerPos) abstract;
    std::shared_ptr<Collidable>& GetSearchTrigger() { return m_searchTrigger; };
	//IDを取得
	int GetID() const { return m_id; };
protected:
    //モデル
    std::unique_ptr<Model> m_model;
    //プレイヤーが近くにいるかをチェックするトリガー
    std::shared_ptr<Collidable> m_searchTrigger;
    //エネミーの識別番号
    const int m_id;
    //攻撃のコンポーネント
    virtual void CreateAttack() abstract;
    //状態に合わせて初期化すべきものを初期化する
    virtual void InitState() abstract;
    //やられ判定の更新
    virtual void UpdateHurtPoint()abstract;
    //攻撃判定を出す
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
};

