#pragma once
#include <memory>
#include <DxLib.h>
#include "../Math/MyMath.h"
namespace
{
	//ポリゴンの当たり判定の配列の最大数
	constexpr int kMaxHitPolygon = 2048;
}
class Collidable;
class CollisionProcess
{
public:
	CollisionProcess();
	~CollisionProcess();
	/// <summary>
	/// 球と球の衝突処理
	/// </summary>
	/// <param name="otherA">球</param>
	/// <param name="otherB">球</param>
	void ProcessSS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// 球とポリゴンの衝突処理
	/// </summary>
	/// <param name="otherA">球</param>
	/// <param name="otherB">ポリゴン</param>
	void ProcessSP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// カプセルとカプセルの衝突処理
	/// </summary>
	/// <param name="otherA">カプセル</param>
	/// <param name="otherB">カプセル</param>
	void ProcessCC(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// カプセルと球
	/// </summary>
	/// <param name="otherA">カプセル</param>
	/// <param name="otherB">球</param>
	void ProcessCS(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
	/// <summary>
	/// カプセルとポリゴンの衝突処理
	/// </summary>
	/// <param name="otherA">カプセル</param>
	/// <param name="otherB">ポリゴン</param>
	void ProcessCP(const std::shared_ptr<Collidable>& otherA, const std::shared_ptr<Collidable>& otherB);
private:
	int	m_wallNum;			// 壁ポリゴンと判断されたポリゴンの数
	int	m_floorNum;			// 床ポリゴンと判断されたポリゴンの数
	MV1_COLL_RESULT_POLY_DIM* m_wall[kMaxHitPolygon];
	MV1_COLL_RESULT_POLY_DIM* m_floor[kMaxHitPolygon];
	/// <summary>
	/// 床ポリゴンと壁ポリゴンに分ける
	/// </summary>
	void AnalyzeWallAndFloor(const MV1_COLL_RESULT_POLY_DIM& hitDim, const Vector3& oldPos);
};

