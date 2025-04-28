#pragma once
#include "../Actor.h"
#include "../../General/Math/MyMath.h"
class TestPolygon :
    public Actor
{
public:
    TestPolygon(Position3 pos, int modelHandle);
    ~TestPolygon();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
private:
    int m_modelHandle;
};

