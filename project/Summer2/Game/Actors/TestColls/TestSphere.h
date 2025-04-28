#pragma once
#include "../Actor.h"
#include "../../General/Math/MyMath.h"
class TestSphere :
    public Actor
{
public:
    TestSphere(Position3 pos,float radius,bool isStatic);
    ~TestSphere();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
};

