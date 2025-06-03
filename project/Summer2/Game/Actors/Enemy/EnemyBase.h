#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Trigger;
class AttackBase;
class AttackManager;
class EnemyBase abstract :
    public Actor
{
public:
    EnemyBase();
    virtual ~EnemyBase() {};
    //範囲内にプレイヤーがいた場合位置を取得
    void OnHitSearch(const Vector3& playerPos);
    //索敵に成功したか
    bool IsHitSearch() const { return m_isHitSearch; };
    //プレイヤーへのベクトルを返す関数
    Vector3 GetPlayerVec() const;
    std::shared_ptr<Collidable>& GetSearchTrigger() { return m_searchTrigger; };
    //モデルクラス
    std::shared_ptr<Model> GetModel() const { return m_model; };
protected:
    //モデル
    std::shared_ptr<Model> m_model;
    //プレイヤーが近くにいるかをチェックするトリガー
    std::shared_ptr<Collidable> m_searchTrigger;
    //サーチに成功したか
    bool m_isHitSearch;
    //プレイヤーの座標
    Vector3 m_playerPos;
    //やられ判定の更新
    virtual void UpdateHurtPoint()abstract;
    //攻撃判定を出す
    void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
};

