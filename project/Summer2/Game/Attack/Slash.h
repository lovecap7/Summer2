#pragma once
#include "AttackBase.h"
class Actor;
class Slash :
    public AttackBase
{
public:
	Slash(std::shared_ptr<Collidable> coll, float damege, int keepFrame);
	~Slash();
	//初期化
	void Init() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;
	//当たった時
	void OnHit(std::shared_ptr<Actor> actor) override;
private:
	float m_initDamege;//初期化用
	float m_initKeepFrame;//初期化用
};

