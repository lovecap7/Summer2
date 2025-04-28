#pragma once
#include "../Actor.h"
#include "../../General/Math/MyMath.h"
class TestCapsule :
    public Actor
{
public:
    TestCapsule(Position3 startPos, Position3 endPos, float radius, bool isStatic);
    ~TestCapsule();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
    void Draw()const override;
    void Complete() override;
};

