#pragma once

/// <summary>
/// 当たり判定の形によって処理が変わるので
/// お互いがどの形の当たり判定をしているのかを
/// 先にチェックする際に使う
/// </summary>
enum class ColliderKind
{
	Sphere,
	Capsule,
	Box,
	Polygon
};

/// <summary>
/// 当たり判定の形に関する基底クラス
/// </summary>
class ColliderBase abstract
{
public:
	ColliderBase(ColliderKind kind);
	virtual ~ColliderBase() {};

	/// <summary>
	/// 当たり判定の形を取得
	/// </summary>
	/// <returns>当たり判定の形</returns>
	ColliderKind GetCollKind()const { return m_kind; }
private:
	//当たり判定の形
	ColliderKind m_kind;
};

