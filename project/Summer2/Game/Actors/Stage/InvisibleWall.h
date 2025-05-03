#pragma once
#include "../Actor.h"
#include "../../General/Math/MyMath.h"
#include <DxLib.h>
class InvisibleWall :
    public Actor
{
public:
    InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle);
    ~InvisibleWall();
    void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
private:
    int m_modelHandle;
};

