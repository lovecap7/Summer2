#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
class CapsuleCollider :
    public ColliderBase
{
public:
    CapsuleCollider(Position3 endPos,float radius);
    virtual ~CapsuleCollider() {};
    //�����̍��W��m_endPos��2�_�ō\�����ꂽ�J�v�Z��
    Position3 GetEndPos() { return m_endPos; };
    void SetEndPos(Position3 pos) { m_endPos = pos; };
    //2�_�̋��̔��a
    float GetRadius() { return m_radius; };
    void SetRadius(float radius) { m_radius = radius; };
    //�����������̂Ƌ߂����W
    Vector3 GetNearPos() { return m_hitNearPos; };
    void SetNearPos(Position3 hitNearPos) { m_hitNearPos = hitNearPos; };
private:
    //�����̍��W��m_endPos��2�_�ō\�����ꂽ�J�v�Z��
    Position3 m_endPos;
    //2�_�̋��̔��a
    float m_radius;
    //�����������̂Ƌ߂����W
    Vector3 m_hitNearPos;
};

