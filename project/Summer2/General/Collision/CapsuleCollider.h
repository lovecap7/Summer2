#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
class CapsuleCollider :
    public ColliderBase
{
public:
    CapsuleCollider(Position3 endPos,float radius);
    virtual ~CapsuleCollider() {};
    //自分の座標とm_endPosの2点で構成されたカプセル
    Position3 GetEndPos() { return m_endPos; };
    void SetEndPos(Position3 pos) { m_endPos = pos; };
    //2点の球の半径
    float GetRadius() { return m_radius; };
    void SetRadius(float radius) { m_radius = radius; };
    //当たった物体と近い座標
    Vector3 GetNearPos() { return m_hitNearPos; };
    void SetNearPos(Position3 hitNearPos) { m_hitNearPos = hitNearPos; };
private:
    //自分の座標とm_endPosの2点で構成されたカプセル
    Position3 m_endPos;
    //2点の球の半径
    float m_radius;
    //当たった物体と近い座標
    Vector3 m_hitNearPos;
};

