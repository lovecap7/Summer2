#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class Input;
class CameraBase;
class Camera;
class Model;
class AttackBase;
class MeleeAttack;
class AttackManager;
class PlayerStateBase;
class Player :
	public Actor, std::enable_shared_from_this<Player>
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//更新処理
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
	//重力
	void Gravity(const Vector3& gravity)override;
	//衝突イベント関数
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//ヒットリアクション
	void HitReaction() override;
	//地面に付いているか
	bool IsGround() { return m_isGround; };
	void NoIsGround() { m_isGround = false; };//地面に付いていないと判断したときに呼ぶ
	//入力中の方向キー
	Vector2 GetStickVec() { return m_stickVec; };
	//モデルクラス
	std::shared_ptr<Model> GetModel() const{ return m_model; };
private:
	//プレイヤーの状態
	std::shared_ptr<PlayerStateBase> m_state;
	//モデル
	std::shared_ptr<Model> m_model;
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
	//地面に付いているかどうか
	bool m_isGround;
private:
	//やられ判定の更新
	void UpdateHurtPoint();
};

