#pragma once
#include "EnemyBase.h"
class Input;
class Camera;
class EnemyManager;
class Common1 :
    public EnemyBase
{
public:
    Common1(std::unique_ptr<EnemyManager>& enemyManager,int modelHandle,Vector3 pos);
    ~Common1();
	void Update(const Input& input, const std::unique_ptr<Camera>& camera) override;
	void Gravity(const Vector3& gravity)override;
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	void Draw()const override;
	void Complete() override;
	void OnHitSearch(const Vector3& playerPos)override;
private:
	//状態遷移
	using UpdateFunc_t = void(Common1::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	UpdateFunc_t m_lastUpdate;//直前の状態を覚えておく
	//待機状態
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//移動
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//攻撃
	void AttackUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//喰らい
	void HitUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//状態に合わせて初期化すべきものを初期化する
	void StateInit();
	//減速
	void SpeedDown();
private:
	//戦闘状態
	bool m_isBattleMode;
	//サーチ
	bool m_isHitSearch;
	//攻撃のクールタイム
	int m_attackCoolTime;
};

