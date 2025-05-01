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
	MV1_COLL_RESULT_POLY* m_wall[kMaxHitPolygon];
	MV1_COLL_RESULT_POLY* m_floor[kMaxHitPolygon];
	/// <summary>
	/// 床ポリゴンと壁ポリゴンに分ける
	/// </summary>
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const Vector3& oldPos);
	/// <summary>
	/// 球とポリゴンの押し戻しベクトルを返す
	/// </summary>
	Vector3 OverlapVecSphereAndPoly(int hitNum, const Vector3& nextPos, MV1_COLL_RESULT_POLY* dim, float shortDis);
	/// <summary>
	/// カプセルとポリゴンの押し戻しベクトルを返す
	/// </summary>
	/// <param name="hitNum"></param>
	/// <param name="nextPos"></param>
	/// <param name="dim"></param>
	/// <param name="shortDis"></param>
	/// <returns></returns>
	Vector3 OverlapVecCapsuleAndPoly(int hitNum, const Vector3& headPos, const Vector3& legPos, MV1_COLL_RESULT_POLY* dim, float shortDis);
	/// <summary>
	/// 床の高さに合わせる
	/// </summary>
	void HitFloorY(const std::shared_ptr<Collidable>& other, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis);
	/// <summary>
	/// Y軸上にあるかチェック
	/// </summary>
	bool CheckHitYLine(const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis);
};

