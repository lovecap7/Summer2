#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class InvisibleWall :
    public Actor
{
public:
    InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle);
    ~InvisibleWall();
    //ìoò^èàóù
    void Entry(std::shared_ptr<ActorManager> actorManager) override {};
    //ìoò^âèú
    void Exit(std::shared_ptr<ActorManager> actorManager) override {};
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
private:
    int m_modelHandle;
};

