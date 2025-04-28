#pragma once
#include <memory>

enum class ActorKind
{
	None,
	Player,		//プレイヤー
	Enemy,		//敵
	Item,		//アイテム
	Obstacle,	//障害物
	Field,		//フィールド
};

class Input;
class Camera;
class Collidable;
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
class Actor abstract//newできなくなる
{
protected:
	//このアクターの種類
	ActorKind m_actorKind;
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
public:
	Actor(ActorKind kind);
	virtual ~Actor() {};
	//(純粋仮想関数)
	/// <summary>
	/// 位置などの更新を行う
	/// </summary>
	/// <param name="">入力オブジェクト</param>
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera) abstract;
	/// <summary>
	/// 対象のアクターを表示
	/// </summary>
	virtual void Draw()const abstract;
	/// <summary>
	/// Updateの処理によって起こった衝突処理などの処理の確定
	/// </summary>
	virtual void Complete() abstract;
	/// <summary>
	/// アクターの種類
	/// </summary>
	/// <returns></returns>
	ActorKind GetActorKind() { return m_actorKind; };
	/// <summary>
	/// 当たり判定と衝突処理に使う
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Collidable>& GetCollidable() const { return m_collidable; }
};

