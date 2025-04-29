#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Actor;
class Rigidbody;
#if _DEBUG
class Input;
#endif
class Camera
{
public:
	Camera(Position3 firstPos, std::shared_ptr<Actor> player);
	~Camera();
	void Update();
	//カメラの座標
	Position3 GetPos() { return m_pos; }
	//カメラの向き
	Vector3 GetDir();

#if _DEBUG
	//カメラ回転
	void RotaCamera(const Input& input);
#endif
private:
	//自分の座標
	Position3 m_pos;
	//カメラの向き
	Position3 m_target;
	//プレイヤー
	std::shared_ptr<Actor> m_player;
};

