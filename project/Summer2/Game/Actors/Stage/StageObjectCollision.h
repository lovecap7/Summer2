#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class UIManager;
/// <summary>
/// ステージのオブジェクトを描画する用のクラス
/// </summary>
class StageObjectCollision :
    public Actor
{
public:
    StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);//ポリゴンのコリジョン
    StageObjectCollision(VECTOR pos, float radius);//球のコリジョン
    StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius);//カプセルのコリジョン
    ~StageObjectCollision();
    //登録処理
    void Entry(std::shared_ptr<ActorManager> actorManager, std::shared_ptr<UIManager> uiManager) override {};
    //登録解除
    void Exit(std::shared_ptr<ActorManager> actorManager, std::shared_ptr<UIManager> uiManager) override {};
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<AttackManager> attackManager, std::shared_ptr<UIManager> uiManager) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
private:
    int m_collisionHandle;
};


