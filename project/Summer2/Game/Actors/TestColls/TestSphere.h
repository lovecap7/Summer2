#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class Camera;
class Collidable;
class AttackManager;
class TestSphere :
    public Actor
{
public:
    TestSphere(Position3 pos,float radius,bool isStatic);
    ~TestSphere();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
    void HitReaction() override;
private:
    Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
    float InputValueSpeed(const Input& input);
    //スティックの向きを持つベクトル
    Vector2 m_stickVec;
    bool m_isGround = false;
};

