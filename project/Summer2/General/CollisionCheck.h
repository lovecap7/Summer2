#pragma once
#include "Geometry.h"

class CollisionCheck
{
public:
    CollisionCheck();
    ~CollisionCheck();
    //�����蔻��
  /// <summary>
  /// �~���m�̓����蔻��
  /// </summary>
  /// <param name="c1">�~</param>
  /// <param name="c2">�~</param>
  /// <returns>�������� true : �������ĂȂ� false</returns>
    bool IsCollCicCic(const Circle2D& c1, const Circle2D& c2);
    /// <summary>
    /// �~�Ƌ�`�̓����蔻��
    /// </summary>
    /// <param name="c1">�~</param>
    /// <param name="c2">��`</param>
    /// <returns>�������� true : �������ĂȂ� false</returns>
    bool IsCollCicRec(const Circle2D& circle, const Rect2D& rect);
    /// <summary>
    /// �~�ƃJ�v�Z���̓����蔻��
    /// </summary>
    /// <param name="c1">�~</param>
    /// <param name="c2">�J�v�Z��</param>
    /// <returns>�������� true : �������ĂȂ� false</returns>
    bool IsCollCicCap(const Circle2D& circle, const Capsule2D& capsule);
    /// <summary>
    /// ��`�Ƌ�`�̓����蔻��
    /// </summary>
    /// <param name="c1">��`</param>
    /// <param name="c2">��`</param>
    /// <returns>�������� true : �������ĂȂ� false</returns>
    bool IsCollRecRec(const Rect2D& r1, const Rect2D& r2);
    /// <summary>
    /// ��`�ƃJ�v�Z���̓����蔻��
    /// </summary>
    /// <param name="c1">��`</param>
    /// <param name="c2">�J�v�Z��</param>
    /// <returns>�������� true : �������ĂȂ� false</returns>
    bool IsCollRecCap(const Rect2D& rect, const Capsule2D& capsule);
    /// <summary>
    /// �J�v�Z���̂ƃJ�v�Z���̓����蔻��
    /// </summary>
    /// <param name="c1">�J�v�Z����</param>
    /// <param name="c2">�J�v�Z��</param>
    /// <returns>�������� true : �������ĂȂ� false</returns>
    bool IsCollCapCap(const Capsule2D& cap1, const Capsule2D& cap2);
};

