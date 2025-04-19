#pragma once
#include "Geometry.h"

class Collider2D
{
public:
    Collider2D();
    Collider2D(Rect2D& rect);
    Collider2D(Circle2D& circle);
    Collider2D(Capsule2D& capsule);
    ~Collider2D();

    //当たり判定のタイプ
    enum class CollType
    {
        Circle,
        Capsule,
        Rect,
    };
    CollType collType;
    //当たり判定に使う
    Rect2D& GetRect2D();
    Circle2D& GetCircle2D();
    Capsule2D& GetCapsule2D();
    bool IsHit() { return m_isHit; }
    void OnCollision();
    void Reset();
private:
    Rect2D m_rect;
    Circle2D m_circle;
    Capsule2D m_capsule;
    bool m_isHit = false;
};


