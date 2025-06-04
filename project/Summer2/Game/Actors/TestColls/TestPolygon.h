#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class ActorManager;
class Camera;
class Collidable;
class AttackManger;
class TestPolygon :
    public Actor
{
public:
    TestPolygon(Position3 pos, int modelHandle);
    ~TestPolygon();
    //“o˜^ˆ—
    void Entry(std::shared_ptr<ActorManager> actorManager) override{};
    //“o˜^‰ğœ
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
