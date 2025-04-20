#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
#include <DxLib.h>

//���f���̃|���S���������蔻��
class PolygonCollider :
    public ColliderBase
{
public:
	PolygonCollider(int modelHandle);
	MV1_COLL_RESULT_POLY_DIM GetHitDim() { return m_hitDim; }
    //�ǂƏ��̋߂����W
	Vector3 GetNearWallHitPos() { return m_nearWallHitPos; }
	Vector3 GetNearFloorHitPos() { return m_nearFloorHitPos; }
private:
    //�����蔻������郂�f��
    int m_modelHandle;
    //���������|���S���̏��
    MV1_COLL_RESULT_POLY_DIM m_hitDim;
    //���������ۂ̍ł��߂��ǃ|���S���̍��W
    Vector3 m_nearWallHitPos;
    //���������ۂ̍ł��߂����|���S���̍��W
    Vector3 m_nearFloorHitPos;
    //���ɓ��������Ȃ�true
    bool m_isFloor;
    //�ǂɓ��������Ȃ�true
    bool m_isWall;
};

