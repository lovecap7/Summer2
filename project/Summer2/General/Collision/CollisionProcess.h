#pragma once
#include <memory>
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
};

