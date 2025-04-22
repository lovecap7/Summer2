#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"

class Input;
class CameraBase;
class Camera;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	void Update(const Input& input,const std::unique_ptr<Camera>& camera) override;
	void Draw()const override;
	void Complete() override;
private:
	//モデルハンドル
	int m_modelHandle;
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
private:
	//状態遷移
	using UpdateFunc_t = void(Player::*)(const Input& input, const std::unique_ptr<Camera>& camera);
	UpdateFunc_t m_update;
	//待機状態
	void IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//移動
	void MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
	//ジャンプ
	void JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera);
private:
	//進行方向を返すベクトル
	Vector3 GetForwardVec(const std::unique_ptr<Camera>& camera);
};

