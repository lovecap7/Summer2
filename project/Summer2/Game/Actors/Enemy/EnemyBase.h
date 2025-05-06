#pragma once
#include "../Actor.h"
#include <memory>
#include "../../General/Model.h"
class Trigger;
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
};

