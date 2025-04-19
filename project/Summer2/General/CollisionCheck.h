#pragma once
#include "Geometry.h"

class CollisionCheck
{
public:
    CollisionCheck();
    ~CollisionCheck();
    //当たり判定
  /// <summary>
  /// 円同士の当たり判定
  /// </summary>
  /// <param name="c1">円</param>
  /// <param name="c2">円</param>
  /// <returns>当たった true : 当たってない false</returns>
    bool IsCollCicCic(const Circle2D& c1, const Circle2D& c2);
    /// <summary>
    /// 円と矩形の当たり判定
    /// </summary>
    /// <param name="c1">円</param>
    /// <param name="c2">矩形</param>
    /// <returns>当たった true : 当たってない false</returns>
    bool IsCollCicRec(const Circle2D& circle, const Rect2D& rect);
    /// <summary>
    /// 円とカプセルの当たり判定
    /// </summary>
    /// <param name="c1">円</param>
    /// <param name="c2">カプセル</param>
    /// <returns>当たった true : 当たってない false</returns>
    bool IsCollCicCap(const Circle2D& circle, const Capsule2D& capsule);
    /// <summary>
    /// 矩形と矩形の当たり判定
    /// </summary>
    /// <param name="c1">矩形</param>
    /// <param name="c2">矩形</param>
    /// <returns>当たった true : 当たってない false</returns>
    bool IsCollRecRec(const Rect2D& r1, const Rect2D& r2);
    /// <summary>
    /// 矩形とカプセルの当たり判定
    /// </summary>
    /// <param name="c1">矩形</param>
    /// <param name="c2">カプセル</param>
    /// <returns>当たった true : 当たってない false</returns>
    bool IsCollRecCap(const Rect2D& rect, const Capsule2D& capsule);
    /// <summary>
    /// カプセルのとカプセルの当たり判定
    /// </summary>
    /// <param name="c1">カプセルの</param>
    /// <param name="c2">カプセル</param>
    /// <returns>当たった true : 当たってない false</returns>
    bool IsCollCapCap(const Capsule2D& cap1, const Capsule2D& cap2);
};

