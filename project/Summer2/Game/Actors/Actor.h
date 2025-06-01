#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"

enum class ActorKind
{
	None,
	Player,		//プレイヤー
	Enemy,		//敵
	Item,		//アイテム
	Obstacle,	//障害物
	Field,		//フィールド
};

class ActorManager;
class Input;
class Camera;
class Collidable;
class HurtPoint;
class AttackManager;
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
/// //newできなくなる
class Actor abstract:public std::enable_shared_from_this<Actor>
{
protected:
	//このアクターの種類
	ActorKind m_actorKind;
	//当たり判定や座標を持ったクラス
	std::shared_ptr<Collidable> m_collidable;
	//やられ判定
	std::shared_ptr<HurtPoint> m_hurtPoint;
	//消滅
	bool m_isDead;
	//アクターの識別番号
	int m_id;
public:
	Actor(ActorKind kind);
	virtual ~Actor() {};
	//(純粋仮想関数)
	/// <summary>
	/// 登録処理
	/// </summary>
	virtual void Entry(std::shared_ptr<ActorManager> actorManager);
	/// <summary>
	/// 登録解除
	/// </summary>
	/// <param name="actorManager"></param>
	virtual void Exit(std::shared_ptr<ActorManager> actorManager);
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// 位置などの更新を行う
	/// </summary>
	/// <param name="">入力オブジェクト</param>
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) abstract;
	/// <summary>
	/// 重力を受ける
	/// </summary>
	/// <param name="gravity"></param>
	virtual void Gravity(const Vector3& gravity)abstract;
	/// <summary>
	/// 衝突したときのイベント関数
	/// </summary>
	/// <param name="other"></param>
	virtual void OnHitColl(const std::shared_ptr<Collidable>& other)abstract;
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
	const std::shared_ptr<Collidable> GetCollidable() const { return m_collidable; }
	/// <summary>
	/// やられ判定
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<HurtPoint> GetHurtPoint() const { return m_hurtPoint; }
	/// <summary>
	/// 消滅フラグ
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return m_isDead; };
	/// <summary>
	/// IDを取得
	/// </summary>
	/// <returns></returns>
	int GetID() const { return m_id; };
	/// <summary>
	/// 攻撃を受けたときのリアクション
	/// </summary>
	virtual void HitReaction() abstract;
};

