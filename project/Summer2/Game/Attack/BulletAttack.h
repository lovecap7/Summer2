#pragma once
#include "AttackBase.h"
class BulletAttack :
    public AttackBase
{
public:
	/// <summary>
	/// 遠距離攻撃
	/// </summary>
	/// <param name="coll">衝突確認のための情報</param>
	/// <param name="damage">ダメージ</param>
	/// <param name="keepFrame">持続</param>
	/// <param name="owner">持ち主</param>
	BulletAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame, std::shared_ptr<Actor> owner);
	~BulletAttack() override;
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;
	//当たった時
	void OnHit(std::shared_ptr<Actor> actor) override;
};

