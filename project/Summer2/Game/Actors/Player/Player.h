#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class Input;
class CameraBase;
class Camera;
class Model;
class Slash;
class Strike;
class AttackBase;
class AttackManager;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
	void HitReaction() override;
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
	//武器
	std::shared_ptr<Collidable> m_rightSword;
	//左足
	std::shared_ptr<Collidable> m_leftLeg;
	//攻撃のフレームを数える
	int m_attackCountFrame;
	//タメ時間
	int m_chargeFrame;
private:
	//状態遷移
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//直前の状態を覚えておく
	//待機状態
	void UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//移動
	void UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//ジャンプ
	void UpdateJump(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//落下中
	void UpdateFall(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//弱攻撃
	void UpdateAttackNormal1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void UpdateAttackNormal2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void UpdateAttackNormal3(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//強攻撃
	void UpdateAttackCharge1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	void UpdateAttackCharge2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//回避
	void UpdateRolling(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager);
	//状態に合わせて初期化すべきものを初期化する
	void InitState();
	//攻撃判定を出す
	void AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager);
private:
	//進行方向を返すベクトル
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
	//減速していく
	void SpeedDown();
	//やられ判定の更新
	void UpdateHurtPoint();
	//剣のコンポーネント
	void CreateRightSword();
	//剣の位置更新
	void UpdateRightSword();
	//左足のコンポーネント
	void CreateLeftLeg();
	//左足の位置更新
	void UpdateLeftLeg();
	//攻撃のコンポーネント
	void CreateAttack();
private:
	//攻撃
	std::shared_ptr<Slash> m_attackN1;//通常1
	std::shared_ptr<Slash> m_attackN2;//通常2
	std::shared_ptr<Slash> m_attackN3;//通常3
	std::shared_ptr<Strike> m_attackC;//ため攻撃
};

