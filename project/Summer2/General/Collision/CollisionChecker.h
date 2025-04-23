#pragma once
#include <memory>
class Collidable;
class CollisionChecker
{
public:
    CollisionChecker();
    virtual ~CollisionChecker() {};

    /// <summary>
    /// 球と球の当たり判定
    /// </summary>
    /// <param name="actorA">球</param>
    /// <param name="actorB">球</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollSS(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB);

    /// <summary>
    /// カプセルと球の当たり判定(引数に注意してください)
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">球</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollCS(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB);

    /// <summary>
    /// カプセルとカプセルの当たり判定
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">カプセル</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollCC(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB);

    /// <summary>
    /// 球とポリゴンの当たり判定
    /// </summary>
    /// <param name="actorA">球</param>
    /// <param name="actorB">ポリゴン</param>
     /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollSP(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB);

    /// <summary>
    /// カプセルとポリゴンの当たり判定
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">ポリゴン</param>
    /// <returns></returns>
    bool CheckCollCP(const std::shared_ptr<Collidable>& actorA, const std::shared_ptr<Collidable>& actorB);
};

