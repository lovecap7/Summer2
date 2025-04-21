#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
class Input;
class CameraBase;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input) override;
	void Draw()const override;
	void Complete() override;
private:
	//モデルハンドル
	int m_modelHandle;
private:
	//状態遷移
	using UpdateFunc_t = void(Player::*)(const Input& input);
	UpdateFunc_t m_update;
	//待機状態
	void IdleUpdate(const Input& input);
	//移動
	void MoveUpdate(const Input& input);
	//ジャンプ
	void JumpUpdate(const Input& input);
};

