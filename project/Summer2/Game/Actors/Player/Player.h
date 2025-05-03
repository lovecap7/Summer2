#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"

class Input;
class CameraBase;
class Camera;
class Model;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input,const std::unique_ptr<Camera>& camera) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
private:
	//モデル
	std::unique_ptr<Model> m_model;
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
	//地面に付いているかどうか
	bool m_isGround;
	//ジャンプの回数
	unsigned int m_jumpNum;
	int m_nextJumpFrame;//2回目のジャンプが行えるまでの時間
	//次の攻撃の入力があるか
	bool m_isNextAttackInput;
	//強攻撃のタメ時間
	int m_chargeHighAttackFrame;
private:
	//状態遷移
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//直前の状態を覚えておく
	//待機状態
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//移動
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//ジャンプ
	void JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//落下中
	void FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//弱攻撃
	void AttackLight1Update(const Input& input, const std::unique_ptr<Camera>& camera);
	void AttackLight2Update(const Input& input, const std::unique_ptr<Camera>& camera);
	void AttackLight3Update(const Input& input, const std::unique_ptr<Camera>& camera);
	//強攻撃
	void AttackHigh1Update(const Input& input, const std::unique_ptr<Camera>& camera);
	void AttackHigh2Update(const Input& input, const std::unique_ptr<Camera>& camera);
	//回避
	void RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//状態に合わせて初期化すべきものを初期化する
	void StateInit();
private:
	//進行方向を返すベクトル
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
	//減速していく
	void SpeedDown();
};

