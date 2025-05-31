#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class Camera;
class Collidable;
class AttackManager;
class TestCapsule :
    public Actor
{
public:
    TestCapsule(Position3 startPos, Position3 endPos, float radius, bool isStatic);
    ~TestCapsule();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
    void HitReaction() override;
};

