#pragma once
#include "EnemyBase.h"
class Input;
class Camera;
class EnemyManager;
class MeleeAttack;
class Common1 :
	public EnemyBase, public std::enable_shared_from_this<Common1>
{
public:
    Common1(int modelHandle,Vector3 pos);
    ~Common1();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager) override;
	//重力
	void Gravity(const Vector3& gravity)override;
	//衝突イベント
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//描画
	void Draw()const override;
	//更新処理の確定
	void Complete() override;
	//プレイヤーとの距離を索敵
	void OnHitSearch(const Vector3& playerPos)override;
	//ヒットリアクション
	void HitReaction() override;
private:
	//状態遷移
	using UpdateFunc_t = void(Common1::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//直前の状態を覚えておく
	//待機状態
	void UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera);
	//移動
	void UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera);
	//攻撃
	void UpdateAttack(const Input& input, const std::unique_ptr<Camera>& camera);
	//喰らい
	void UpdateHit(const Input& input, const std::unique_ptr<Camera>& camera);
	//死亡
	void UpdateDead(const Input& input, const std::unique_ptr<Camera>& camera);
private:
	//状態に合わせて初期化すべきものを初期化する
	void InitState()override;
	//減速
	void SpeedDown();
	//戦闘に関する更新処理
	void BattleUpdate();
	//左腕の当たり判定作成
	void CreateLeftArm();
	//左腕の位置更新
	void UpdateLeftArm();
	//攻撃のコンポーネント
	void CreateAttack() override;
	//やられ判定の更新
	void UpdateHurtPoint()override;
private:
	//戦闘状態
	bool m_isBattleMode;
	//サーチ
	bool m_isHitSearch;
	//攻撃のクールタイム
	int m_attackCoolTime;
	//攻撃
	std::shared_ptr<MeleeAttack> m_punch;
	//左腕
	std::shared_ptr<Collidable> m_leftArm;
};

